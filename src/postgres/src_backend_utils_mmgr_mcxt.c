/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - ErrorContext
 * - MemoryContextReset
 * - MemoryContextDeleteChildren
 * - MemoryContextDelete
 * - TopMemoryContext
 * - CurrentMemoryContext
 * - MemoryContextSetParent
 * - MemoryContextCallResetCallbacks
 * - MemoryContextResetOnly
 * - repalloc
 * - MemoryContextStats
 * - MemoryContextStatsDetail
 * - MemoryContextStatsInternal
 * - MemoryContextStatsPrint
 * - pfree
 * - pstrdup
 * - MemoryContextStrdup
 * - MemoryContextAlloc
 * - palloc
 * - MemoryContextAllocZeroAligned
 * - MemoryContextAllocZero
 * - palloc0
 * - MemoryContextCreate
 * - MemoryContextInit
 * - MemoryContextAllowInCriticalSection
 * - CurrentMemoryContext
 * - MemoryContextDelete
 * - MemoryContextDeleteTopLevel
 * - palloc0
 *--------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 *
 * mcxt.c
 *	  POSTGRES memory context management code.
 *
 * This module handles context management operations that are independent
 * of the particular kind of context being operated on.  It calls
 * context-type-specific operations via the function pointers in a
 * context's MemoryContextMethods struct.
 *
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/mmgr/mcxt.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "utils/memdebug.h"
#include "utils/memutils.h"


/*****************************************************************************
 *	  GLOBAL MEMORY															 *
 *****************************************************************************/

/*
 * CurrentMemoryContext
 *		Default memory context for allocations.
 */
__thread MemoryContext CurrentMemoryContext = NULL;


/*
 * Standard top-level contexts. For a description of the purpose of each
 * of these contexts, refer to src/backend/utils/mmgr/README
 */
__thread MemoryContext TopMemoryContext = NULL;

__thread MemoryContext ErrorContext = NULL;







/* This is a transient link to the active portal's memory context: */


static void MemoryContextCallResetCallbacks(MemoryContext context);
static void MemoryContextStatsInternal(MemoryContext context, int level,
									   bool print, int max_children,
									   MemoryContextCounters *totals);
static void MemoryContextStatsPrint(MemoryContext context, void *passthru,
									const char *stats_string);

/*
 * You should not do memory allocations within a critical section, because
 * an out-of-memory error will be escalated to a PANIC. To enforce that
 * rule, the allocation functions Assert that.
 */
#define AssertNotInCriticalSection(context) \
	Assert(CritSectionCount == 0 || (context)->allowInCritSection)

/*****************************************************************************
 *	  EXPORTED ROUTINES														 *
 *****************************************************************************/


/*
 * MemoryContextInit
 *		Start up the memory-context subsystem.
 *
 * This must be called before creating contexts or allocating memory in
 * contexts.  TopMemoryContext and ErrorContext are initialized here;
 * other contexts must be created afterwards.
 *
 * In normal multi-backend operation, this is called once during
 * postmaster startup, and not at all by individual backend startup
 * (since the backends inherit an already-initialized context subsystem
 * by virtue of being forked off the postmaster).  But in an EXEC_BACKEND
 * build, each process must do this for itself.
 *
 * In a standalone backend this must be called during backend startup.
 */
void
MemoryContextInit(void)
{
	AssertState(TopMemoryContext == NULL);

	/*
	 * First, initialize TopMemoryContext, which is the parent of all others.
	 */
	TopMemoryContext = AllocSetContextCreate((MemoryContext) NULL,
											 "TopMemoryContext",
											 ALLOCSET_DEFAULT_SIZES);

	/*
	 * Not having any other place to point CurrentMemoryContext, make it point
	 * to TopMemoryContext.  Caller should change this soon!
	 */
	CurrentMemoryContext = TopMemoryContext;

	/*
	 * Initialize ErrorContext as an AllocSetContext with slow growth rate ---
	 * we don't really expect much to be allocated in it. More to the point,
	 * require it to contain at least 8K at all times. This is the only case
	 * where retained memory in a context is *essential* --- we want to be
	 * sure ErrorContext still has some memory even if we've run out
	 * elsewhere! Also, allow allocations in ErrorContext within a critical
	 * section. Otherwise a PANIC will cause an assertion failure in the error
	 * reporting code, before printing out the real cause of the failure.
	 *
	 * This should be the last step in this function, as elog.c assumes memory
	 * management works once ErrorContext is non-null.
	 */
	ErrorContext = AllocSetContextCreate(TopMemoryContext,
										 "ErrorContext",
										 8 * 1024,
										 8 * 1024,
										 8 * 1024);
	MemoryContextAllowInCriticalSection(ErrorContext, true);
}

/*
 * MemoryContextReset
 *		Release all space allocated within a context and delete all its
 *		descendant contexts (but not the named context itself).
 */
void
MemoryContextReset(MemoryContext context)
{
	AssertArg(MemoryContextIsValid(context));

	/* save a function call in common case where there are no children */
	if (context->firstchild != NULL)
		MemoryContextDeleteChildren(context);

	/* save a function call if no pallocs since startup or last reset */
	if (!context->isReset)
		MemoryContextResetOnly(context);
}

/*
 * MemoryContextResetOnly
 *		Release all space allocated within a context.
 *		Nothing is done to the context's descendant contexts.
 */
void
MemoryContextResetOnly(MemoryContext context)
{
	AssertArg(MemoryContextIsValid(context));

	/* Nothing to do if no pallocs since startup or last reset */
	if (!context->isReset)
	{
		MemoryContextCallResetCallbacks(context);

		/*
		 * If context->ident points into the context's memory, it will become
		 * a dangling pointer.  We could prevent that by setting it to NULL
		 * here, but that would break valid coding patterns that keep the
		 * ident elsewhere, e.g. in a parent context.  Another idea is to use
		 * MemoryContextContains(), but we don't require ident strings to be
		 * in separately-palloc'd chunks, so that risks false positives.  So
		 * for now we assume the programmer got it right.
		 */

		context->methods->reset(context);
		context->isReset = true;
		VALGRIND_DESTROY_MEMPOOL(context);
		VALGRIND_CREATE_MEMPOOL(context, 0, false);
	}
}

/*
 * MemoryContextResetChildren
 *		Release all space allocated within a context's descendants,
 *		but don't delete the contexts themselves.  The named context
 *		itself is not touched.
 */


/*
 * MemoryContextDelete
 *		Delete a context and its descendants, and release all space
 *		allocated therein.
 *
 * The type-specific delete routine removes all storage for the context,
 * but we have to recurse to handle the children.
 * We must also delink the context from its parent, if it has one.
 */
void
MemoryContextDelete(MemoryContext context)
{
	AssertArg(MemoryContextIsValid(context));
	/* We had better not be deleting TopMemoryContext ... */
	Assert(context != TopMemoryContext);
	/* And not CurrentMemoryContext, either */
	Assert(context != CurrentMemoryContext);

	/* save a function call in common case where there are no children */
	if (context->firstchild != NULL)
		MemoryContextDeleteChildren(context);

	/*
	 * It's not entirely clear whether 'tis better to do this before or after
	 * delinking the context; but an error in a callback will likely result in
	 * leaking the whole context (if it's not a root context) if we do it
	 * after, so let's do it before.
	 */
	MemoryContextCallResetCallbacks(context);

	/*
	 * We delink the context from its parent before deleting it, so that if
	 * there's an error we won't have deleted/busted contexts still attached
	 * to the context tree.  Better a leak than a crash.
	 */
	MemoryContextSetParent(context, NULL);

	/*
	 * Also reset the context's ident pointer, in case it points into the
	 * context.  This would only matter if someone tries to get stats on the
	 * (already unlinked) context, which is unlikely, but let's be safe.
	 */
	context->ident = NULL;

	context->methods->delete_context(context);

	VALGRIND_DESTROY_MEMPOOL(context);
}

/*
 * MemoryContextDeleteTopLevel
 *		Delete the top-level memory context, its descendants and
 *		release all space allocated therein.
 *
 * After this is called, you would have to re-initialize the top-level
 * memory context by calling MemoryContextInit() again. Only to be used
 * in special circumstances (e.g. Postgres-based libraries that need all
 * memory to be freed after running through Postgres code).
 */
void
MemoryContextDeleteTopLevel(void)
{
	MemoryContext context = TopMemoryContext;
	AssertArg(MemoryContextIsValid(context));

	/*
	 * After this, no memory contexts are valid anymore, so ensure that
	 * the current context is the top-level context.
	 */
	Assert(TopMemoryContext == CurrentMemoryContext);

	/* save a function call in common case where there are no children */
	if (context->firstchild != NULL)
		MemoryContextDeleteChildren(context);

	/*
	 * It's not entirely clear whether 'tis better to do this before or after
	 * delinking the context; but an error in a callback will likely result in
	 * leaking the whole context (if it's not a root context) if we do it
	 * after, so let's do it before.
	 */
	MemoryContextCallResetCallbacks(context);

	/*
	 * Also reset the context's ident pointer, in case it points into the
	 * context.  This would only matter if someone tries to get stats on the
	 * (already unlinked) context, which is unlikely, but let's be safe.
	 */
	context->ident = NULL;

	/* Clean up the aset.c freelist, to leave no unused context behind */
	AllocSetDeleteFreeList(context);

	context->methods->delete_context(context);

	VALGRIND_DESTROY_MEMPOOL(context);

	/* Without this, Valgrind will complain */
	free(context);

	/* Reset pointers */
	TopMemoryContext = NULL;
	CurrentMemoryContext = NULL;
	ErrorContext = NULL;
}

/*
 * MemoryContextDeleteChildren
 *		Delete all the descendants of the named context and release all
 *		space allocated therein.  The named context itself is not touched.
 */
void
MemoryContextDeleteChildren(MemoryContext context)
{
	AssertArg(MemoryContextIsValid(context));

	/*
	 * MemoryContextDelete will delink the child from me, so just iterate as
	 * long as there is a child.
	 */
	while (context->firstchild != NULL)
		MemoryContextDelete(context->firstchild);
}

/*
 * MemoryContextRegisterResetCallback
 *		Register a function to be called before next context reset/delete.
 *		Such callbacks will be called in reverse order of registration.
 *
 * The caller is responsible for allocating a MemoryContextCallback struct
 * to hold the info about this callback request, and for filling in the
 * "func" and "arg" fields in the struct to show what function to call with
 * what argument.  Typically the callback struct should be allocated within
 * the specified context, since that means it will automatically be freed
 * when no longer needed.
 *
 * There is no API for deregistering a callback once registered.  If you
 * want it to not do anything anymore, adjust the state pointed to by its
 * "arg" to indicate that.
 */


/*
 * MemoryContextCallResetCallbacks
 *		Internal function to call all registered callbacks for context.
 */
static void
MemoryContextCallResetCallbacks(MemoryContext context)
{
	MemoryContextCallback *cb;

	/*
	 * We pop each callback from the list before calling.  That way, if an
	 * error occurs inside the callback, we won't try to call it a second time
	 * in the likely event that we reset or delete the context later.
	 */
	while ((cb = context->reset_cbs) != NULL)
	{
		context->reset_cbs = cb->next;
		cb->func(cb->arg);
	}
}

/*
 * MemoryContextSetIdentifier
 *		Set the identifier string for a memory context.
 *
 * An identifier can be provided to help distinguish among different contexts
 * of the same kind in memory context stats dumps.  The identifier string
 * must live at least as long as the context it is for; typically it is
 * allocated inside that context, so that it automatically goes away on
 * context deletion.  Pass id = NULL to forget any old identifier.
 */


/*
 * MemoryContextSetParent
 *		Change a context to belong to a new parent (or no parent).
 *
 * We provide this as an API function because it is sometimes useful to
 * change a context's lifespan after creation.  For example, a context
 * might be created underneath a transient context, filled with data,
 * and then reparented underneath CacheMemoryContext to make it long-lived.
 * In this way no special effort is needed to get rid of the context in case
 * a failure occurs before its contents are completely set up.
 *
 * Callers often assume that this function cannot fail, so don't put any
 * elog(ERROR) calls in it.
 *
 * A possible caller error is to reparent a context under itself, creating
 * a loop in the context graph.  We assert here that context != new_parent,
 * but checking for multi-level loops seems more trouble than it's worth.
 */
void
MemoryContextSetParent(MemoryContext context, MemoryContext new_parent)
{
	AssertArg(MemoryContextIsValid(context));
	AssertArg(context != new_parent);

	/* Fast path if it's got correct parent already */
	if (new_parent == context->parent)
		return;

	/* Delink from existing parent, if any */
	if (context->parent)
	{
		MemoryContext parent = context->parent;

		if (context->prevchild != NULL)
			context->prevchild->nextchild = context->nextchild;
		else
		{
			Assert(parent->firstchild == context);
			parent->firstchild = context->nextchild;
		}

		if (context->nextchild != NULL)
			context->nextchild->prevchild = context->prevchild;
	}

	/* And relink */
	if (new_parent)
	{
		AssertArg(MemoryContextIsValid(new_parent));
		context->parent = new_parent;
		context->prevchild = NULL;
		context->nextchild = new_parent->firstchild;
		if (new_parent->firstchild != NULL)
			new_parent->firstchild->prevchild = context;
		new_parent->firstchild = context;
	}
	else
	{
		context->parent = NULL;
		context->prevchild = NULL;
		context->nextchild = NULL;
	}
}

/*
 * MemoryContextAllowInCriticalSection
 *		Allow/disallow allocations in this memory context within a critical
 *		section.
 *
 * Normally, memory allocations are not allowed within a critical section,
 * because a failure would lead to PANIC.  There are a few exceptions to
 * that, like allocations related to debugging code that is not supposed to
 * be enabled in production.  This function can be used to exempt specific
 * memory contexts from the assertion in palloc().
 */
void
MemoryContextAllowInCriticalSection(MemoryContext context, bool allow)
{
	AssertArg(MemoryContextIsValid(context));

	context->allowInCritSection = allow;
}

/*
 * GetMemoryChunkSpace
 *		Given a currently-allocated chunk, determine the total space
 *		it occupies (including all memory-allocation overhead).
 *
 * This is useful for measuring the total space occupied by a set of
 * allocated chunks.
 */


/*
 * MemoryContextGetParent
 *		Get the parent context (if any) of the specified context
 */


/*
 * MemoryContextIsEmpty
 *		Is a memory context empty of any allocated space?
 */


/*
 * Find the memory allocated to blocks for this memory context. If recurse is
 * true, also include children.
 */


/*
 * MemoryContextStats
 *		Print statistics about the named context and all its descendants.
 *
 * This is just a debugging utility, so it's not very fancy.  However, we do
 * make some effort to summarize when the output would otherwise be very long.
 * The statistics are sent to stderr.
 */
void
MemoryContextStats(MemoryContext context)
{
	/* A hard-wired limit on the number of children is usually good enough */
	MemoryContextStatsDetail(context, 100);
}

/*
 * MemoryContextStatsDetail
 *
 * Entry point for use if you want to vary the number of child contexts shown.
 */
void
MemoryContextStatsDetail(MemoryContext context, int max_children)
{
	MemoryContextCounters grand_totals;

	memset(&grand_totals, 0, sizeof(grand_totals));

	MemoryContextStatsInternal(context, 0, true, max_children, &grand_totals);

	fprintf(stderr,
			"Grand total: %zu bytes in %zd blocks; %zu free (%zd chunks); %zu used\n",
			grand_totals.totalspace, grand_totals.nblocks,
			grand_totals.freespace, grand_totals.freechunks,
			grand_totals.totalspace - grand_totals.freespace);
}

/*
 * MemoryContextStatsInternal
 *		One recursion level for MemoryContextStats
 *
 * Print this context if print is true, but in any case accumulate counts into
 * *totals (if given).
 */
static void
MemoryContextStatsInternal(MemoryContext context, int level,
						   bool print, int max_children,
						   MemoryContextCounters *totals)
{
	MemoryContextCounters local_totals;
	MemoryContext child;
	int			ichild;

	AssertArg(MemoryContextIsValid(context));

	/* Examine the context itself */
	context->methods->stats(context,
							print ? MemoryContextStatsPrint : NULL,
							(void *) &level,
							totals);

	/*
	 * Examine children.  If there are more than max_children of them, we do
	 * not print the rest explicitly, but just summarize them.
	 */
	memset(&local_totals, 0, sizeof(local_totals));

	for (child = context->firstchild, ichild = 0;
		 child != NULL;
		 child = child->nextchild, ichild++)
	{
		if (ichild < max_children)
			MemoryContextStatsInternal(child, level + 1,
									   print, max_children,
									   totals);
		else
			MemoryContextStatsInternal(child, level + 1,
									   false, max_children,
									   &local_totals);
	}

	/* Deal with excess children */
	if (ichild > max_children)
	{
		if (print)
		{
			int			i;

			for (i = 0; i <= level; i++)
				fprintf(stderr, "  ");
			fprintf(stderr,
					"%d more child contexts containing %zu total in %zd blocks; %zu free (%zd chunks); %zu used\n",
					ichild - max_children,
					local_totals.totalspace,
					local_totals.nblocks,
					local_totals.freespace,
					local_totals.freechunks,
					local_totals.totalspace - local_totals.freespace);
		}

		if (totals)
		{
			totals->nblocks += local_totals.nblocks;
			totals->freechunks += local_totals.freechunks;
			totals->totalspace += local_totals.totalspace;
			totals->freespace += local_totals.freespace;
		}
	}
}

/*
 * MemoryContextStatsPrint
 *		Print callback used by MemoryContextStatsInternal
 *
 * For now, the passthru pointer just points to "int level"; later we might
 * make that more complicated.
 */
static void
MemoryContextStatsPrint(MemoryContext context, void *passthru,
						const char *stats_string)
{
	int			level = *(int *) passthru;
	const char *name = context->name;
	const char *ident = context->ident;
	int			i;

	/*
	 * It seems preferable to label dynahash contexts with just the hash table
	 * name.  Those are already unique enough, so the "dynahash" part isn't
	 * very helpful, and this way is more consistent with pre-v11 practice.
	 */
	if (ident && strcmp(name, "dynahash") == 0)
	{
		name = ident;
		ident = NULL;
	}

	for (i = 0; i < level; i++)
		fprintf(stderr, "  ");
	fprintf(stderr, "%s: %s", name, stats_string);
	if (ident)
	{
		/*
		 * Some contexts may have very long identifiers (e.g., SQL queries).
		 * Arbitrarily truncate at 100 bytes, but be careful not to break
		 * multibyte characters.  Also, replace ASCII control characters, such
		 * as newlines, with spaces.
		 */
		int			idlen = strlen(ident);
		bool		truncated = false;

		if (idlen > 100)
		{
			idlen = pg_mbcliplen(ident, idlen, 100);
			truncated = true;
		}
		fprintf(stderr, ": ");
		while (idlen-- > 0)
		{
			unsigned char c = *ident++;

			if (c < ' ')
				c = ' ';
			fputc(c, stderr);
		}
		if (truncated)
			fprintf(stderr, "...");
	}
	fputc('\n', stderr);
}

/*
 * MemoryContextCheck
 *		Check all chunks in the named context.
 *
 * This is just a debugging utility, so it's not fancy.
 */
#ifdef MEMORY_CONTEXT_CHECKING

#endif

/*
 * MemoryContextContains
 *		Detect whether an allocated chunk of memory belongs to a given
 *		context or not.
 *
 * Caution: this test is reliable as long as 'pointer' does point to
 * a chunk of memory allocated from *some* context.  If 'pointer' points
 * at memory obtained in some other way, there is a small chance of a
 * false-positive result, since the bits right before it might look like
 * a valid chunk header by chance.
 */


/*
 * MemoryContextCreate
 *		Context-type-independent part of context creation.
 *
 * This is only intended to be called by context-type-specific
 * context creation routines, not by the unwashed masses.
 *
 * The memory context creation procedure goes like this:
 *	1.  Context-type-specific routine makes some initial space allocation,
 *		including enough space for the context header.  If it fails,
 *		it can ereport() with no damage done.
 *	2.	Context-type-specific routine sets up all type-specific fields of
 *		the header (those beyond MemoryContextData proper), as well as any
 *		other management fields it needs to have a fully valid context.
 *		Usually, failure in this step is impossible, but if it's possible
 *		the initial space allocation should be freed before ereport'ing.
 *	3.	Context-type-specific routine calls MemoryContextCreate() to fill in
 *		the generic header fields and link the context into the context tree.
 *	4.  We return to the context-type-specific routine, which finishes
 *		up type-specific initialization.  This routine can now do things
 *		that might fail (like allocate more memory), so long as it's
 *		sure the node is left in a state that delete will handle.
 *
 * node: the as-yet-uninitialized common part of the context header node.
 * tag: NodeTag code identifying the memory context type.
 * methods: context-type-specific methods (usually statically allocated).
 * parent: parent context, or NULL if this will be a top-level context.
 * name: name of context (must be statically allocated).
 *
 * Context routines generally assume that MemoryContextCreate can't fail,
 * so this can contain Assert but not elog/ereport.
 */
void
MemoryContextCreate(MemoryContext node,
					NodeTag tag,
					const MemoryContextMethods *methods,
					MemoryContext parent,
					const char *name)
{
	/* Creating new memory contexts is not allowed in a critical section */
	Assert(CritSectionCount == 0);

	/* Initialize all standard fields of memory context header */
	node->type = tag;
	node->isReset = true;
	node->methods = methods;
	node->parent = parent;
	node->firstchild = NULL;
	node->mem_allocated = 0;
	node->prevchild = NULL;
	node->name = name;
	node->ident = NULL;
	node->reset_cbs = NULL;

	/* OK to link node into context tree */
	if (parent)
	{
		node->nextchild = parent->firstchild;
		if (parent->firstchild != NULL)
			parent->firstchild->prevchild = node;
		parent->firstchild = node;
		/* inherit allowInCritSection flag from parent */
		node->allowInCritSection = parent->allowInCritSection;
	}
	else
	{
		node->nextchild = NULL;
		node->allowInCritSection = false;
	}

	VALGRIND_CREATE_MEMPOOL(node, 0, false);
}

/*
 * MemoryContextAlloc
 *		Allocate space within the specified context.
 *
 * This could be turned into a macro, but we'd have to import
 * nodes/memnodes.h into postgres.h which seems a bad idea.
 */
void *
MemoryContextAlloc(MemoryContext context, Size size)
{
	void	   *ret;

	AssertArg(MemoryContextIsValid(context));
	AssertNotInCriticalSection(context);

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	context->isReset = false;

	ret = context->methods->alloc(context, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);

		/*
		 * Here, and elsewhere in this module, we show the target context's
		 * "name" but not its "ident" (if any) in user-visible error messages.
		 * The "ident" string might contain security-sensitive data, such as
		 * values in SQL commands.
		 */
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_ALLOC(context, ret, size);

	return ret;
}

/*
 * MemoryContextAllocZero
 *		Like MemoryContextAlloc, but clears allocated memory
 *
 *	We could just call MemoryContextAlloc then clear the memory, but this
 *	is a very common combination, so we provide the combined operation.
 */
void *
MemoryContextAllocZero(MemoryContext context, Size size)
{
	void	   *ret;

	AssertArg(MemoryContextIsValid(context));
	AssertNotInCriticalSection(context);

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	context->isReset = false;

	ret = context->methods->alloc(context, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_ALLOC(context, ret, size);

	MemSetAligned(ret, 0, size);

	return ret;
}

/*
 * MemoryContextAllocZeroAligned
 *		MemoryContextAllocZero where length is suitable for MemSetLoop
 *
 *	This might seem overly specialized, but it's not because newNode()
 *	is so often called with compile-time-constant sizes.
 */
void *
MemoryContextAllocZeroAligned(MemoryContext context, Size size)
{
	void	   *ret;

	AssertArg(MemoryContextIsValid(context));
	AssertNotInCriticalSection(context);

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	context->isReset = false;

	ret = context->methods->alloc(context, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_ALLOC(context, ret, size);

	MemSetLoop(ret, 0, size);

	return ret;
}

/*
 * MemoryContextAllocExtended
 *		Allocate space within the specified context using the given flags.
 */


void *
palloc(Size size)
{
	/* duplicates MemoryContextAlloc to avoid increased overhead */
	void	   *ret;
	MemoryContext context = CurrentMemoryContext;

	AssertArg(MemoryContextIsValid(context));
	AssertNotInCriticalSection(context);

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	context->isReset = false;

	ret = context->methods->alloc(context, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_ALLOC(context, ret, size);

	return ret;
}

void *
palloc0(Size size)
{
	/* duplicates MemoryContextAllocZero to avoid increased overhead */
	void	   *ret;
	MemoryContext context = CurrentMemoryContext;

	AssertArg(MemoryContextIsValid(context));
	AssertNotInCriticalSection(context);

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	context->isReset = false;

	ret = context->methods->alloc(context, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_ALLOC(context, ret, size);

	MemSetAligned(ret, 0, size);

	return ret;
}



/*
 * pfree
 *		Release an allocated chunk.
 */
void
pfree(void *pointer)
{
	MemoryContext context = GetMemoryChunkContext(pointer);

	context->methods->free_p(context, pointer);
	VALGRIND_MEMPOOL_FREE(context, pointer);
}

/*
 * repalloc
 *		Adjust the size of a previously allocated chunk.
 */
void *
repalloc(void *pointer, Size size)
{
	MemoryContext context = GetMemoryChunkContext(pointer);
	void	   *ret;

	if (!AllocSizeIsValid(size))
		elog(ERROR, "invalid memory alloc request size %zu", size);

	AssertNotInCriticalSection(context);

	/* isReset must be false already */
	Assert(!context->isReset);

	ret = context->methods->realloc(context, pointer, size);
	if (unlikely(ret == NULL))
	{
		MemoryContextStats(TopMemoryContext);
		ereport(ERROR,
				(errcode(ERRCODE_OUT_OF_MEMORY),
				 errmsg("out of memory"),
				 errdetail("Failed on request of size %zu in memory context \"%s\".",
						   size, context->name)));
	}

	VALGRIND_MEMPOOL_CHANGE(context, pointer, ret, size);

	return ret;
}

/*
 * MemoryContextAllocHuge
 *		Allocate (possibly-expansive) space within the specified context.
 *
 * See considerations in comment at MaxAllocHugeSize.
 */


/*
 * repalloc_huge
 *		Adjust the size of a previously allocated chunk, permitting a large
 *		value.  The previous allocation need not have been "huge".
 */


/*
 * MemoryContextStrdup
 *		Like strdup(), but allocate from the specified context
 */
char *
MemoryContextStrdup(MemoryContext context, const char *string)
{
	char	   *nstr;
	Size		len = strlen(string) + 1;

	nstr = (char *) MemoryContextAlloc(context, len);

	memcpy(nstr, string, len);

	return nstr;
}

char *
pstrdup(const char *in)
{
	return MemoryContextStrdup(CurrentMemoryContext, in);
}

/*
 * pnstrdup
 *		Like pstrdup(), but append null byte to a
 *		not-necessarily-null-terminated input string.
 */


/*
 * Make copy of string with all trailing newline characters removed.
 */


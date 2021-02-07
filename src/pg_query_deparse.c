#include "pg_query.h"
#include "pg_query_internal.h"
#include "pg_query_readfuncs.h"

#include "lib/stringinfo.h"
#include "nodes/nodes.h"
#include "nodes/parsenodes.h"
#include "nodes/pg_list.h"
#include "utils/builtins.h"
#include "catalog/index.h"
#include "catalog/pg_attribute.h"
#include "catalog/pg_class.h"
#include "catalog/pg_trigger.h"
#include "utils/datetime.h"
#include "utils/timestamp.h"
#include "utils/xml.h"
#include "limits.h"

typedef enum DeparseNodeContext {
	DEPARSE_NODE_CONTEXT_NONE,
	// Parent node type (and sometimes field)
	DEPARSE_NODE_CONTEXT_SELECT,
	DEPARSE_NODE_CONTEXT_INSERT_RELATION,
	DEPARSE_NODE_CONTEXT_INSERT_TARGET_LIST,
	DEPARSE_NODE_CONTEXT_INSERT_ON_CONFLICT,
	DEPARSE_NODE_CONTEXT_UPDATE,
	DEPARSE_NODE_CONTEXT_RETURNING,
	DEPARSE_NODE_CONTEXT_A_EXPR,
	DEPARSE_NODE_CONTEXT_EXPLAIN,
	DEPARSE_NODE_CONTEXT_CREATE_FUNCTION,
	DEPARSE_NODE_CONTEXT_COPY,
	DEPARSE_NODE_CONTEXT_XMLATTRIBUTES,
	DEPARSE_NODE_CONTEXT_XMLNAMESPACES,
	DEPARSE_NODE_CONTEXT_CREATE_TYPE,
	DEPARSE_NODE_CONTEXT_ALTER_TYPE,
	// Identifier vs constant context
	DEPARSE_NODE_CONTEXT_IDENTIFIER,
	DEPARSE_NODE_CONTEXT_CONSTANT
} DeparseNodeContext;

static void
removeTrailingSpace(StringInfo str)
{
	if (str->len >= 1 && str->data[str->len - 1] == ' ') {
		str->len -= 1;
		str->data[str->len] = '\0';
	}
}

/*
 * Append a SQL string literal representing "val" to buf.
 *
 * Copied here from postgres_fdw/deparse.c to avoid adding
 * many additional dependencies.
 */
static void
deparseStringLiteral(StringInfo buf, const char *val)
{
	const char *valptr;

	/*
	 * Rather than making assumptions about the remote server's value of
	 * standard_conforming_strings, always use E'foo' syntax if there are any
	 * backslashes.  This will fail on remote servers before 8.1, but those
	 * are long out of support.
	 */
	if (strchr(val, '\\') != NULL)
		appendStringInfoChar(buf, ESCAPE_STRING_SYNTAX);
	appendStringInfoChar(buf, '\'');
	for (valptr = val; *valptr; valptr++)
	{
		char		ch = *valptr;

		if (SQL_STR_DOUBLE(ch, true))
			appendStringInfoChar(buf, ch);
		appendStringInfoChar(buf, ch);
	}
	appendStringInfoChar(buf, '\'');
}

static void deparseSelectStmt(StringInfo str, SelectStmt *stmt);
static void deparseIntoClause(StringInfo str, IntoClause *into_clause);
static void deparseRangeVar(StringInfo str, RangeVar *range_var, DeparseNodeContext context);
static void deparseResTarget(StringInfo str, ResTarget *res_target, DeparseNodeContext context);
static void deparseRawStmt(StringInfo str, RawStmt *raw_stmt);
static void deparseAlias(StringInfo str, Alias *alias);
static void deparseWindowDef(StringInfo str, WindowDef* window_def);
static void deparseColumnRef(StringInfo str, ColumnRef* column_ref);
static void deparseSubLink(StringInfo str, SubLink* sub_link);
static void deparseAExpr(StringInfo str, A_Expr* a_expr, DeparseNodeContext context);
static void deparseBoolExpr(StringInfo str, BoolExpr *bool_expr);
static void deparseAStar(StringInfo str, A_Star* a_star);
static void deparseCollateClause(StringInfo str, CollateClause* collate_clause);
static void deparseSortBy(StringInfo str, SortBy* sort_by);
static void deparseParamRef(StringInfo str, ParamRef* param_ref);
static void deparseSQLValueFunction(StringInfo str, SQLValueFunction* sql_value_function);
static void deparseWithClause(StringInfo str, WithClause *with_clause);
static void deparseJoinExpr(StringInfo str, JoinExpr *join_expr);
static void deparseCommonTableExpr(StringInfo str, CommonTableExpr *cte);
static void deparseRangeSubselect(StringInfo str, RangeSubselect *range_subselect);
static void deparseRangeFunction(StringInfo str, RangeFunction *range_func);
static void deparseAArrayExpr(StringInfo str, A_ArrayExpr * array_expr);
static void deparseRowExpr(StringInfo str, RowExpr *row_expr);
static void deparseTypeCast(StringInfo str, TypeCast *type_cast);
static void deparseTypeName(StringInfo str, TypeName *type_name);
static void deparseNullTest(StringInfo str, NullTest *null_test);
static void deparseCaseExpr(StringInfo str, CaseExpr *case_expr);
static void deparseCaseWhen(StringInfo str, CaseWhen *case_when);
static void deparseAIndirection(StringInfo str, A_Indirection *a_indirection);
static void deparseAIndices(StringInfo str, A_Indices *a_indices);
static void deparseCoalesceExpr(StringInfo str, CoalesceExpr *coalesce_expr);
static void deparseBooleanTest(StringInfo str, BooleanTest *boolean_test);
static void deparseColumnDef(StringInfo str, ColumnDef *column_def);
static void deparseInsertStmt(StringInfo str, InsertStmt *insert_stmt);
static void deparseOnConflictClause(StringInfo str, OnConflictClause *on_conflict_clause);
static void deparseIndexElem(StringInfo str, IndexElem* index_elem);
static void deparseUpdateStmt(StringInfo str, UpdateStmt *update_stmt);
static void deparseDeleteStmt(StringInfo str, DeleteStmt *delete_stmt);
static void deparseLockingClause(StringInfo str, LockingClause *locking_clause);
static void deparseSetToDefault(StringInfo str, SetToDefault *set_to_default);
static void deparseCreateCastStmt(StringInfo str, CreateCastStmt *create_cast_stmt);
static void deparseCreateDomainStmt(StringInfo str, CreateDomainStmt *create_domain_stmt);
static void deparseFunctionParameter(StringInfo str, FunctionParameter *function_parameter);
static void deparseRoleSpec(StringInfo str, RoleSpec *role_spec);
static void deparseViewStmt(StringInfo str, ViewStmt *view_stmt);
static void deparseObjectWithArgs(StringInfo str, ObjectWithArgs *object_with_args, bool parens_for_empty_list);
static void deparseVariableSetStmt(StringInfo str, VariableSetStmt* variable_set_stmt);

static void deparseValue(StringInfo str, Value *value, DeparseNodeContext context);
static void deparseNode(StringInfo str, Node *node, DeparseNodeContext context);

// "any_name" in gram.y
static void deparseAnyName(StringInfo str, List *parts)
{
	ListCell *lc = NULL;

	foreach(lc, parts)
	{
		Assert(IsA(lfirst(lc), String));
		appendStringInfoString(str, quote_identifier(strVal(lfirst(lc))));
		if (lnext(parts, lc))
			appendStringInfoChar(str, '.');
	}
}

// "expr_list" in gram.y
static void deparseExprList(StringInfo str, List *exprs)
{
	ListCell *lc;

	foreach(lc, exprs)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(exprs, lc))
			appendStringInfoString(str, ", ");
	}
}

// "opt_indirection" in gram.y
static void deparseOptIndirection(StringInfo str, List *indirection)
{
	ListCell *lc = NULL;

	if (list_length(indirection) > 0)
	{
		foreach(lc, indirection)
		{
			if (IsA(lfirst(lc), String))
			{
				appendStringInfoChar(str, '.');
				appendStringInfoString(str, quote_identifier(strVal(lfirst(lc))));
			}
			else if (IsA(lfirst(lc), A_Star))
			{
				appendStringInfoString(str, ".*");
			}
			else if (IsA(lfirst(lc), A_Indices))
			{
				deparseAIndices(str, castNode(A_Indices, lfirst(lc)));
			}
			else
			{
				// No other nodes should appear here
				Assert(false);
			}
		}
	}
}

// "role_list" in gram.y
static void deparseRoleList(StringInfo str, List *roles)
{
	ListCell *lc;

	foreach(lc, roles)
	{
		RoleSpec *role_spec = castNode(RoleSpec, lfirst(lc));
		deparseRoleSpec(str, role_spec);
		if (lnext(roles, lc))
			appendStringInfoString(str, ", ");
	}
}

// "SeqOptList" in gram.y
static void deparseSeqOptList(StringInfo str, List *options)
{
	ListCell *lc;
	Assert(list_length(options) > 0);
	foreach (lc, options)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}
}

// "OptSeqOptList" in gram.y
static void deparseOptSeqOptList(StringInfo str, List *options)
{
	if (list_length(options) > 0)
		deparseSeqOptList(str, options);
}

// "OptParenthesizedSeqOptList" in gram.y
static void deparseOptParenthesizedSeqOptList(StringInfo str, List *options)
{
	if (list_length(options) > 0)
	{
		appendStringInfoChar(str, '(');
		deparseSeqOptList(str, options);
		appendStringInfoChar(str, ')');
	}
}

// "opt_drop_behavior" in gram.y
static void deparseOptDropBehavior(StringInfo str, DropBehavior behavior)
{
	switch (behavior)
	{
		case DROP_RESTRICT:
			// Default
			break;
		case DROP_CASCADE:
			appendStringInfoString(str, "CASCADE ");
			break;
	}
}

// "opt_definition" in gram.y
static void deparseOptDefinition(StringInfo str, List *options)
{
	ListCell *lc = NULL;

	if (list_length(options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach (lc, options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
}

static void deparseSelectStmt(StringInfo str, SelectStmt *stmt)
{
	const ListCell *lc = NULL;
	const ListCell *lc2 = NULL;

	if (stmt->withClause)
	{
		deparseWithClause(str, stmt->withClause);
		appendStringInfoChar(str, ' ');
	}

	switch (stmt->op) {
		case SETOP_NONE:
			if (list_length(stmt->valuesLists) > 0)
			{
				const ListCell *lc;
				appendStringInfoString(str, "VALUES ");

				foreach(lc, stmt->valuesLists)
				{
					appendStringInfoChar(str, '(');
					foreach(lc2, lfirst(lc))
					{
						deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_SELECT);
						if (lnext(lfirst(lc), lc2))
							appendStringInfoString(str, ", ");
					}
					appendStringInfoChar(str, ')');
					if (lnext(stmt->valuesLists, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ' ');
				break;
			}

			// TODO: Check if there is ever a case where we need to restrict this
			//if (list_length(stmt->fromClause) > 0 || list_length(stmt->targetList) > 0)
			appendStringInfoString(str, "SELECT ");

			if (list_length(stmt->targetList) > 0)
			{
				if (stmt->distinctClause != NULL)
				{
					appendStringInfoString(str, "DISTINCT ");

					if (list_length(stmt->distinctClause) > 0 && linitial(stmt->distinctClause) != NULL)
					{
						appendStringInfoString(str, "ON (");
						foreach(lc, stmt->distinctClause)
						{
							deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
							if (lnext(stmt->distinctClause, lc))
								appendStringInfoString(str, ", ");
						}
						appendStringInfoString(str, ") ");
					}
				}

				foreach(lc, stmt->targetList)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
					if (lnext(stmt->targetList, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ' ');

				if (stmt->intoClause != NULL)
				{
					appendStringInfoString(str, "INTO ");
					deparseIntoClause(str, stmt->intoClause);
					appendStringInfoChar(str, ' ');
				}
			}

			if (list_length(stmt->fromClause) > 0)
			{
				appendStringInfoString(str, "FROM ");

				foreach(lc, stmt->fromClause)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
					if (lnext(stmt->fromClause, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ' ');
			}

			if (stmt->whereClause != NULL)
			{
				appendStringInfoString(str, "WHERE ");
				deparseNode(str, stmt->whereClause, DEPARSE_NODE_CONTEXT_SELECT);
				appendStringInfoChar(str, ' ');
			}

			if (list_length(stmt->groupClause) > 0)
			{
				appendStringInfoString(str, "GROUP BY ");

				foreach(lc, stmt->groupClause)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
					if (lnext(stmt->groupClause, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ' ');
			}

			if (stmt->havingClause != NULL)
			{
				appendStringInfoString(str, "HAVING ");
				deparseNode(str, stmt->havingClause, DEPARSE_NODE_CONTEXT_SELECT);
				appendStringInfoChar(str, ' ');
			}

			if (stmt->windowClause != NULL)
			{
				appendStringInfoString(str, "WINDOW ");
				foreach(lc, stmt->windowClause)
				{
					WindowDef *window_def = castNode(WindowDef, lfirst(lc));
					Assert(window_def->name != NULL);
					appendStringInfoString(str, window_def->name);
					appendStringInfoString(str, " AS ");
					deparseWindowDef(str, window_def);
					if (lnext(stmt->windowClause, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ' ');
			}
			break;
		case SETOP_UNION:
		case SETOP_INTERSECT:
		case SETOP_EXCEPT:
			{
				bool need_larg_parens = list_length(stmt->larg->sortClause) > 0 || stmt->larg->op != SETOP_NONE;
				bool need_rarg_parens = list_length(stmt->rarg->sortClause) > 0 || stmt->rarg->op != SETOP_NONE || stmt->rarg->withClause != NULL;
				if (need_larg_parens)
					appendStringInfoChar(str, '(');
				deparseSelectStmt(str, stmt->larg);
				if (need_larg_parens)
					appendStringInfoChar(str, ')');
				switch (stmt->op)
				{
					case SETOP_UNION:
						appendStringInfoString(str, " UNION ");
						break;
					case SETOP_INTERSECT:
						appendStringInfoString(str, " INTERSECT ");
						break;
					case SETOP_EXCEPT:
						appendStringInfoString(str, " EXCEPT ");
						break;
					default:
						Assert(false);
				}
				if (stmt->all)
					appendStringInfoString(str, "ALL ");
				if (need_rarg_parens)
					appendStringInfoChar(str, '(');
				deparseSelectStmt(str, stmt->rarg);
				if (need_rarg_parens)
					appendStringInfoChar(str, ')');
				appendStringInfoChar(str, ' ');
			}
			break;
	}

	if (list_length(stmt->sortClause) > 0)
	{
		appendStringInfoString(str, "ORDER BY ");

		foreach(lc, stmt->sortClause)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
			if (lnext(stmt->sortClause, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}

	if (stmt->limitCount != NULL)
	{
		appendStringInfoString(str, "LIMIT ");
		deparseNode(str, stmt->limitCount, DEPARSE_NODE_CONTEXT_SELECT);
		appendStringInfoChar(str, ' ');
	}

	if (stmt->limitOffset != NULL)
	{
		appendStringInfoString(str, "OFFSET ");
		deparseNode(str, stmt->limitOffset, DEPARSE_NODE_CONTEXT_SELECT);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(stmt->lockingClause) > 0)
	{
		foreach(lc, stmt->lockingClause)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_SELECT);
			if (lnext(stmt->lockingClause, lc))
				appendStringInfoString(str, " ");
		}
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseIntoClause(StringInfo str, IntoClause *into_clause)
{
	ListCell *lc;

	deparseRangeVar(str, into_clause->rel, DEPARSE_NODE_CONTEXT_NONE); /* target relation name */

	if (list_length(into_clause->colNames) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach (lc, into_clause->colNames)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(into_clause->colNames, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
	appendStringInfoChar(str, ' ');

	if (into_clause->accessMethod != NULL)
	{
		appendStringInfoString(str, "USING ");
		appendStringInfoString(str, quote_identifier(into_clause->accessMethod));
		appendStringInfoChar(str, ' ');
	}

	if (list_length(into_clause->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach(lc, into_clause->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(into_clause->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	switch (into_clause->onCommit)
	{
		case ONCOMMIT_NOOP:
			// No clause
			break;
		case ONCOMMIT_PRESERVE_ROWS:
			appendStringInfoString(str, "ON COMMIT PRESERVE ROWS ");
			break;
		case ONCOMMIT_DELETE_ROWS:
			appendStringInfoString(str, "ON COMMIT DELETE ROWS ");
			break;
		case ONCOMMIT_DROP:
			appendStringInfoString(str, "ON COMMIT DROP ");
			break;
	}

	if (into_clause->tableSpaceName != NULL)
	{
		appendStringInfoString(str, "TABLESPACE ");
		appendStringInfoString(str, quote_identifier(into_clause->tableSpaceName));
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseRangeVar(StringInfo str, RangeVar *range_var, DeparseNodeContext context)
{
	if (!range_var->inh && context != DEPARSE_NODE_CONTEXT_CREATE_TYPE && context != DEPARSE_NODE_CONTEXT_ALTER_TYPE)
		appendStringInfoString(str, "ONLY ");

	if (range_var->schemaname != NULL)
	{
		appendStringInfoString(str, quote_identifier(range_var->schemaname));
		appendStringInfoChar(str, '.');
	}

	Assert(range_var->relname != NULL);
	appendStringInfoString(str, quote_identifier(range_var->relname));
	appendStringInfoChar(str, ' ');

	if (range_var->alias != NULL)
	{
		if (context == DEPARSE_NODE_CONTEXT_INSERT_RELATION)
			appendStringInfoString(str, "AS ");
		deparseAlias(str, range_var->alias);
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseResTarget(StringInfo str, ResTarget *res_target, DeparseNodeContext context)
{
	ListCell *lc;

	/*
	 * In a SELECT target list, 'name' is the column label from an
	 * 'AS ColumnLabel' clause, or NULL if there was none, and 'val' is the
	 * value expression itself.  The 'indirection' field is not used.
	 */
	if (context == DEPARSE_NODE_CONTEXT_SELECT)
	{
		Assert(res_target->val != NULL);
		deparseNode(str, res_target->val, context);
		if (res_target->name != NULL) {
			appendStringInfoString(str, " AS ");
			appendStringInfoString(str, quote_identifier(res_target->name));
		}
	}
	/*
	 * INSERT uses ResTarget in its target-column-names list.  Here, 'name' is
	 * the name of the destination column, 'indirection' stores any subscripts
	 * attached to the destination, and 'val' is not used.
	 */
	else if (context == DEPARSE_NODE_CONTEXT_INSERT_TARGET_LIST)
	{
		Assert(res_target->name != NULL);
		appendStringInfoString(str, quote_identifier(res_target->name));
		deparseOptIndirection(str, res_target->indirection);
	}
	/*
	 * In an UPDATE target list, 'name' is the name of the destination column,
	 * 'indirection' stores any subscripts attached to the destination, and
	 * 'val' is the expression to assign.
	 */
	// The UPDATE target list is handled in deparseSetTargetList
	else if (context == DEPARSE_NODE_CONTEXT_RETURNING)
	{
		Assert(res_target->val != NULL);
		deparseNode(str, res_target->val, context);
		if (res_target->name != NULL) {
			appendStringInfoString(str, " AS ");
			appendStringInfoString(str, quote_identifier(res_target->name));
		}
	}
	else if (context == DEPARSE_NODE_CONTEXT_XMLATTRIBUTES)
	{
		Assert(res_target->val != NULL);
		deparseNode(str, res_target->val, context);
		if (res_target->name != NULL) {
			appendStringInfoString(str, " AS ");
			appendStringInfoString(str, quote_identifier(res_target->name));
		}
	}
	else if (context == DEPARSE_NODE_CONTEXT_XMLNAMESPACES)
	{
		Assert(res_target->val != NULL);
		if (res_target->name == NULL)
		{
			appendStringInfoString(str, "DEFAULT ");
		}
		deparseNode(str, res_target->val, context);
		if (res_target->name != NULL)
		{
			appendStringInfoString(str, " AS ");
			appendStringInfoString(str, quote_identifier(res_target->name));
		}
	}
	else
	{
		elog(ERROR, "Can't deparse ResTarget in context %d", context);
	}
}

static void deparseSetTargetList(StringInfo str, List *target_list)
{
	ListCell *lc;
	ListCell *lc2;
	int skip_next_n_elems = 0;

	Assert(list_length(target_list) > 0);

	foreach(lc, target_list)
	{
		if (skip_next_n_elems > 0)
		{
			skip_next_n_elems--;
			continue;
		}
		if (foreach_current_index(lc) != 0)
			appendStringInfoString(str, ", ");
		ResTarget *res_target = castNode(ResTarget, lfirst(lc));
		if (IsA(res_target->val, MultiAssignRef))
		{
			MultiAssignRef *r = castNode(MultiAssignRef, res_target->val);
			appendStringInfoString(str, "(");
			for_each_cell(lc2, target_list, lc)
			{
				ResTarget *res_target2 = castNode(ResTarget, lfirst(lc2));
				appendStringInfoString(str, quote_identifier(res_target2->name));
				deparseOptIndirection(str, res_target2->indirection);
				if (foreach_current_index(lc2) == r->ncolumns - 1) // Last element in this multi-assign
					break;
				else if (lnext(target_list, lc2))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, ") = ");
			deparseNode(str, r->source, DEPARSE_NODE_CONTEXT_NONE);
			skip_next_n_elems = r->ncolumns - 1;
		}
		else
		{
			Assert(res_target->name != NULL);
			Assert(res_target->val != NULL);
			appendStringInfoString(str, quote_identifier(res_target->name));
			deparseOptIndirection(str, res_target->indirection);
			appendStringInfoString(str, " = ");
			deparseNode(str, res_target->val, DEPARSE_NODE_CONTEXT_NONE);
		}
	}
}

static void deparseRawStmt(StringInfo str, RawStmt *raw_stmt)
{
	deparseNode(str, raw_stmt->stmt, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseAlias(StringInfo str, Alias *alias)
{
	appendStringInfoString(str, quote_identifier(alias->aliasname));

	if (list_length(alias->colnames) > 0)
	{
		const ListCell *lc = NULL;
		appendStringInfoChar(str, '(');
		foreach(lc, alias->colnames)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(alias->colnames, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
}

static void deparseAConst(StringInfo str, A_Const *a_const)
{
	deparseValue(str, &a_const->val, DEPARSE_NODE_CONTEXT_CONSTANT);
}

static void deparseFuncCall(StringInfo str, FuncCall *func_call)
{
	const ListCell *lc = NULL;
	const ListCell *lc2 = NULL;

	Assert(list_length(func_call->funcname) > 0);

	if (list_length(func_call->funcname) == 2 &&
		strcmp(strVal(linitial(func_call->funcname)), "pg_catalog") == 0 &&
		strcmp(strVal(lsecond(func_call->funcname)), "overlay") == 0 &&
		list_length(func_call->args) == 4)
	{
		/*
		 * Note that this is a bit odd, but "OVERLAY" is a keyword on its own merit, and only accepts the
         * keyword parameter style when its called as a keyword, not as a regular function (i.e. pg_catalog.overlay)
		 */
		appendStringInfoString(str, "OVERLAY(");
		deparseNode(str, linitial(func_call->args), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, " PLACING ");
		deparseNode(str, lsecond(func_call->args), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, " FROM ");
		deparseNode(str, lthird(func_call->args), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, " FOR ");
		deparseNode(str, lfourth(func_call->args), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ')');
		return;
	}

	foreach(lc, func_call->funcname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(func_call->funcname, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, '(');

	if (func_call->agg_distinct)
		appendStringInfoString(str, "DISTINCT ");

	if (func_call->agg_star)
	{
		appendStringInfoChar(str, '*');
	}
	else if (list_length(func_call->args) > 0)
	{
		foreach(lc2, func_call->args)
		{
			if (func_call->func_variadic && !lnext(func_call->args, lc2))
				appendStringInfoString(str, "VARIADIC ");
			deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(func_call->args, lc2))
				appendStringInfoString(str, ", ");
		}
	}

	if (func_call->agg_order != NULL && !func_call->agg_within_group)
	{
		appendStringInfoString(str, " ORDER BY ");
		foreach(lc, func_call->agg_order)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(func_call->agg_order, lc))
				appendStringInfoString(str, ", ");
		}
	}

	appendStringInfoString(str, ") ");

	if (func_call->agg_order != NULL && func_call->agg_within_group)
	{
		appendStringInfoString(str, "WITHIN GROUP (ORDER BY ");
		foreach(lc, func_call->agg_order)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(func_call->agg_order, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (func_call->agg_filter)
	{
		appendStringInfoString(str, "FILTER (WHERE ");
		deparseNode(str, func_call->agg_filter, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (func_call->over)
	{
		appendStringInfoString(str, "OVER ");
		if (func_call->over->name)
			appendStringInfoString(str, func_call->over->name);
		else
			deparseWindowDef(str, func_call->over);
	}

	removeTrailingSpace(str);
}

static void deparseWindowDef(StringInfo str, WindowDef* window_def)
{
	ListCell *lc;

	// The parent node is responsible for outputting window_def->name

	appendStringInfoChar(str, '(');

	if (window_def->refname != NULL)
	{
		appendStringInfoString(str, quote_identifier(window_def->refname));
		appendStringInfoChar(str, ' ');
	}

	if (list_length(window_def->partitionClause) > 0)
	{
		appendStringInfoString(str, "PARTITION BY ");
		deparseExprList(str, window_def->partitionClause);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(window_def->orderClause) > 0)
	{
		appendStringInfoString(str, "ORDER BY ");
		foreach(lc, window_def->orderClause)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(window_def->orderClause, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}

	if (window_def->frameOptions & FRAMEOPTION_NONDEFAULT)
	{
		if (window_def->frameOptions & FRAMEOPTION_RANGE)
			appendStringInfoString(str, "RANGE ");
		else if (window_def->frameOptions & FRAMEOPTION_ROWS)
			appendStringInfoString(str, "ROWS ");
		else if (window_def->frameOptions & FRAMEOPTION_GROUPS)
			appendStringInfoString(str, "GROUPS ");
	
		if (window_def->frameOptions & FRAMEOPTION_BETWEEN)
			appendStringInfoString(str, "BETWEEN ");

		// frame_start
		if (window_def->frameOptions & FRAMEOPTION_START_UNBOUNDED_PRECEDING)
		{
			appendStringInfoString(str, "UNBOUNDED PRECEDING ");
		}
		else if (window_def->frameOptions & FRAMEOPTION_START_UNBOUNDED_FOLLOWING)
		{
			Assert(false); // disallowed
		}
		else if (window_def->frameOptions & FRAMEOPTION_START_CURRENT_ROW)
		{
			appendStringInfoString(str, "CURRENT ROW ");
		}
		else if (window_def->frameOptions & FRAMEOPTION_START_OFFSET_PRECEDING)
		{
			Assert(window_def->startOffset != NULL);
			deparseNode(str, window_def->startOffset, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " PRECEDING ");
		}
		else if (window_def->frameOptions & FRAMEOPTION_START_OFFSET_FOLLOWING)
		{
			Assert(window_def->startOffset != NULL);
			deparseNode(str, window_def->startOffset, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " FOLLOWING ");
		}

		if (window_def->frameOptions & FRAMEOPTION_BETWEEN)
		{
			appendStringInfoString(str, "AND ");

			// frame_end
			if (window_def->frameOptions & FRAMEOPTION_END_UNBOUNDED_PRECEDING)
			{
				Assert(false); // disallowed
			}
			else if (window_def->frameOptions & FRAMEOPTION_END_UNBOUNDED_FOLLOWING)
			{
				appendStringInfoString(str, "UNBOUNDED FOLLOWING ");
			}
			else if (window_def->frameOptions & FRAMEOPTION_END_CURRENT_ROW)
			{
				appendStringInfoString(str, "CURRENT ROW ");
			}
			else if (window_def->frameOptions & FRAMEOPTION_END_OFFSET_PRECEDING)
			{
				Assert(window_def->endOffset != NULL);
				deparseNode(str, window_def->endOffset, DEPARSE_NODE_CONTEXT_NONE);
				appendStringInfoString(str, " PRECEDING ");
			}
			else if (window_def->frameOptions & FRAMEOPTION_END_OFFSET_FOLLOWING)
			{
				Assert(window_def->endOffset != NULL);
				deparseNode(str, window_def->endOffset, DEPARSE_NODE_CONTEXT_NONE);
				appendStringInfoString(str, " FOLLOWING ");
			}
		}

		if (window_def->frameOptions & FRAMEOPTION_EXCLUDE_CURRENT_ROW)
			appendStringInfoString(str, "EXCLUDE CURRENT ROW ");
		else if (window_def->frameOptions & FRAMEOPTION_EXCLUDE_GROUP)
			appendStringInfoString(str, "EXCLUDE GROUP ");
		else if (window_def->frameOptions & FRAMEOPTION_EXCLUDE_TIES)
			appendStringInfoString(str, "EXCLUDE TIES ");
	}

	removeTrailingSpace(str);
	appendStringInfoChar(str, ')');
}

static void deparseColumnRef(StringInfo str, ColumnRef* column_ref)
{
	ListCell *lc;
	foreach(lc, column_ref->fields) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(column_ref->fields, lc))
			appendStringInfoString(str, ".");
	}
}

static void deparseSubLink(StringInfo str, SubLink* sub_link)
{
	switch (sub_link->subLinkType) {
		case EXISTS_SUBLINK:
			appendStringInfoString(str, "EXISTS (");
			deparseNode(str, sub_link->subselect, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case ALL_SUBLINK:
			Assert(list_length(sub_link->operName) == 1);
			Assert(IsA(linitial(sub_link->operName), String));
			deparseNode(str, sub_link->testexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			appendStringInfoString(str, strVal(linitial(sub_link->operName)));
			appendStringInfoString(str, " ALL (");
			deparseNode(str, sub_link->subselect, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case ANY_SUBLINK:
			deparseNode(str, sub_link->testexpr, DEPARSE_NODE_CONTEXT_NONE);
			if (list_length(sub_link->operName) > 0)
			{
				Assert(list_length(sub_link->operName) == 1);
				Assert(IsA(linitial(sub_link->operName), String));
				appendStringInfoChar(str, ' ');
				appendStringInfoString(str, strVal(linitial(sub_link->operName)));
				appendStringInfoString(str, " ANY ");
			}
			else
			{
				appendStringInfoString(str, " IN ");
			}
			appendStringInfoChar(str, '(');
			deparseNode(str, sub_link->subselect, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case ROWCOMPARE_SUBLINK:
			// FIXME
	        // format('(%s)', deparse_item(node['subselect']))
			Assert(false);
			return;
		case EXPR_SUBLINK:
			appendStringInfoString(str, "(");
			deparseNode(str, sub_link->subselect, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case MULTIEXPR_SUBLINK:
			// FIXME
	        // format('(%s)', deparse_item(node['subselect']))
			Assert(false);
			return;
		case ARRAY_SUBLINK:
			appendStringInfoString(str, "ARRAY(");
			deparseNode(str, sub_link->subselect, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case CTE_SUBLINK: /* for SubPlans only */
			// FIXME
	        // format('(%s)', deparse_item(node['subselect']))
			Assert(false);
			return;
	}
}

static void deparseAExpr(StringInfo str, A_Expr* a_expr, DeparseNodeContext context)
{
	ListCell *lc;
	char *name;

	// TODO: Review which op kinds should add these parens
	bool need_lexpr_parens = a_expr->lexpr != NULL && (IsA(a_expr->lexpr, BoolExpr) || IsA(a_expr->lexpr, NullTest));
	bool need_rexpr_parens = a_expr->rexpr != NULL && (IsA(a_expr->rexpr, BoolExpr) || IsA(a_expr->rexpr, NullTest));

	switch (a_expr->kind) {
		case AEXPR_OP: /* normal operator */
			// TODO: Handle schema-qualified operators
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));

			bool need_outer_parens = context == DEPARSE_NODE_CONTEXT_A_EXPR;

			if (need_outer_parens)
				appendStringInfoChar(str, '(');
			if (need_lexpr_parens)
				appendStringInfoChar(str, '(');
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_A_EXPR);
			if (need_lexpr_parens)
				appendStringInfoChar(str, ')');
			appendStringInfoChar(str, ' ');
			appendStringInfoString(str, strVal(linitial(a_expr->name)));
			appendStringInfoChar(str, ' ');
			if (need_rexpr_parens)
				appendStringInfoChar(str, '(');
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_A_EXPR);
			if (need_rexpr_parens)
				appendStringInfoChar(str, ')');

			if (need_outer_parens)
				appendStringInfoChar(str, ')');

			return;
		case AEXPR_OP_ANY: /* scalar op ANY (array) */
			// TODO: Handle schema-qualified operators
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			appendStringInfoString(str, strVal(linitial(a_expr->name)));
			appendStringInfoString(str, " ANY(");
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case AEXPR_OP_ALL: /* scalar op ALL (array) */
			// TODO: Handle schema-qualified operators
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			appendStringInfoString(str, strVal(linitial(a_expr->name)));
			appendStringInfoString(str, " ALL(");
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case AEXPR_DISTINCT: /* IS DISTINCT FROM - name must be "=" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(strcmp(strVal(linitial(a_expr->name)), "=") == 0);

			if (need_lexpr_parens)
				appendStringInfoChar(str, '(');
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_A_EXPR);
			if (need_lexpr_parens)
				appendStringInfoChar(str, ')');
			appendStringInfoString(str, " IS DISTINCT FROM ");
			if (need_rexpr_parens)
				appendStringInfoChar(str, '(');
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_A_EXPR);
			if (need_rexpr_parens)
				appendStringInfoChar(str, ')');
			return;
		case AEXPR_NOT_DISTINCT: /* IS NOT DISTINCT FROM - name must be "=" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(strcmp(strVal(linitial(a_expr->name)), "=") == 0);

			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " IS NOT DISTINCT FROM ");
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			return;
		case AEXPR_NULLIF: /* NULLIF - name must be "=" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(strcmp(strVal(linitial(a_expr->name)), "=") == 0);

			appendStringInfoString(str, "NULLIF(");
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, ", ");
			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ')');
			return;
		case AEXPR_OF: /* IS [NOT] OF - name must be "=" or "<>" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(IsA(a_expr->rexpr, List));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			name = ((Value *) linitial(a_expr->name))->val.str;
			if (strcmp(name, "=") == 0) {
				appendStringInfoString(str, "IS OF ");
			} else if (strcmp(name, "<>") == 0) {
				appendStringInfoString(str, "IS NOT OF ");
			} else {
				Assert(false);
			}
			appendStringInfoChar(str, '(');
			foreach(lc, castNode(List, a_expr->rexpr)) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(castNode(List, a_expr->rexpr), lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			return;
		case AEXPR_IN: /* [NOT] IN - name must be "=" or "<>" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(IsA(a_expr->rexpr, List));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			name = ((Value *) linitial(a_expr->name))->val.str;
			if (strcmp(name, "=") == 0) {
				appendStringInfoString(str, "IN ");
			} else if (strcmp(name, "<>") == 0) {
				appendStringInfoString(str, "NOT IN ");
			} else {
				Assert(false);
			}
			appendStringInfoChar(str, '(');
			foreach(lc, castNode(List, a_expr->rexpr)) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(castNode(List, a_expr->rexpr), lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			return;
		case AEXPR_LIKE: /* [NOT] LIKE - name must be "~~" or "!~~" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');

			name = ((Value *) linitial(a_expr->name))->val.str;
			if (strcmp(name, "~~") == 0) {
				appendStringInfoString(str, "LIKE ");
			} else if (strcmp(name, "!~~") == 0) {
				appendStringInfoString(str, "NOT LIKE ");
			} else {
				Assert(false);
			}

			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			return;
		case AEXPR_ILIKE: /* [NOT] ILIKE - name must be "~~*" or "!~~*" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');

			name = ((Value *) linitial(a_expr->name))->val.str;
			if (strcmp(name, "~~*") == 0) {
				appendStringInfoString(str, "ILIKE ");
			} else if (strcmp(name, "!~~*") == 0) {
				appendStringInfoString(str, "NOT ILIKE ");
			} else {
				Assert(false);
			}

			deparseNode(str, a_expr->rexpr, DEPARSE_NODE_CONTEXT_NONE);
			return;
		case AEXPR_SIMILAR: /* [NOT] SIMILAR - name must be "~" or "!~" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');

			name = ((Value *) linitial(a_expr->name))->val.str;
			if (strcmp(name, "~") == 0) {
				appendStringInfoString(str, "SIMILAR TO ");
			} else if (strcmp(name, "!~") == 0) {
				appendStringInfoString(str, "NOT SIMILAR TO ");
			} else {
				Assert(false);
			}

			FuncCall *n = castNode(FuncCall, a_expr->rexpr);
			Assert(list_length(n->funcname) == 2);
			Assert(strcmp(strVal(linitial(n->funcname)), "pg_catalog") == 0);
			Assert(strcmp(strVal(lsecond(n->funcname)), "similar_to_escape") == 0);
			Assert(list_length(n->args) == 1 || list_length(n->args) == 2);

			deparseNode(str, linitial(n->args), DEPARSE_NODE_CONTEXT_NONE);
			if (list_length(n->args) == 2)
			{
				appendStringInfoString(str, " ESCAPE ");
				deparseNode(str, lsecond(n->args), DEPARSE_NODE_CONTEXT_NONE);
			}

			return;
		case AEXPR_BETWEEN: /* name must be "BETWEEN" */
		case AEXPR_NOT_BETWEEN: /* name must be "NOT BETWEEN" */
		case AEXPR_BETWEEN_SYM: /* name must be "BETWEEN SYMMETRIC" */
		case AEXPR_NOT_BETWEEN_SYM: /* name must be "NOT BETWEEN SYMMETRIC" */
			Assert(list_length(a_expr->name) == 1);
			Assert(IsA(linitial(a_expr->name), String));
			Assert(IsA(a_expr->rexpr, List));

			deparseNode(str, a_expr->lexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
			appendStringInfoString(str, strVal(linitial(a_expr->name)));
			appendStringInfoChar(str, ' ');

			foreach(lc, castNode(List, a_expr->rexpr)) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(castNode(List, a_expr->rexpr), lc))
					appendStringInfoString(str, " AND ");
			}
			return;
		case AEXPR_PAREN: /* nameless dummy node for parentheses */
			// TODO
			Assert(false);
			return;
	}
}

static void deparseBoolExpr(StringInfo str, BoolExpr *bool_expr)
{
	const ListCell *lc = NULL;
	switch (bool_expr->boolop)
	{
		case AND_EXPR:
			foreach(lc, bool_expr->args)
			{
				// Put parantheses around AND + OR nodes that are inside
				bool need_parens = IsA(lfirst(lc), BoolExpr) && (castNode(BoolExpr, lfirst(lc))->boolop == AND_EXPR || castNode(BoolExpr, lfirst(lc))->boolop == OR_EXPR);

				if (need_parens)
					appendStringInfoChar(str, '(');

				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);

				if (need_parens)
					appendStringInfoChar(str, ')');

				if (lnext(bool_expr->args, lc))
					appendStringInfoString(str, " AND ");
			}
			return;
		case OR_EXPR:
			foreach(lc, bool_expr->args)
			{
				// Put parantheses around AND + OR nodes that are inside
				bool need_parens = IsA(lfirst(lc), BoolExpr) && (castNode(BoolExpr, lfirst(lc))->boolop == AND_EXPR || castNode(BoolExpr, lfirst(lc))->boolop == OR_EXPR);

				if (need_parens)
					appendStringInfoChar(str, '(');

				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);

				if (need_parens)
					appendStringInfoChar(str, ')');

				if (lnext(bool_expr->args, lc))
					appendStringInfoString(str, " OR ");
			}
			return;
		case NOT_EXPR:
			Assert(list_length(bool_expr->args) == 1);
			bool need_parens = IsA(linitial(bool_expr->args), BoolExpr) && (castNode(BoolExpr, linitial(bool_expr->args))->boolop == AND_EXPR || castNode(BoolExpr, linitial(bool_expr->args))->boolop == OR_EXPR);
			appendStringInfoString(str, "NOT ");
			if (need_parens)
				appendStringInfoChar(str, '(');
			deparseNode(str, linitial(bool_expr->args), DEPARSE_NODE_CONTEXT_NONE);
			if (need_parens)
				appendStringInfoChar(str, ')');
			return;
	}
}

static void deparseAStar(StringInfo str, A_Star *a_star)
{
	appendStringInfoChar(str, '*');
}

static void deparseCollateClause(StringInfo str, CollateClause* collate_clause)
{
	ListCell *lc;
	if (collate_clause->arg != NULL)
	{
		deparseNode(str, collate_clause->arg, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}
	appendStringInfoString(str, "COLLATE ");
	foreach(lc, collate_clause->collname) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(collate_clause->collname, lc))
			appendStringInfoString(str, ".");
	}
}

static void deparseSortBy(StringInfo str, SortBy* sort_by)
{
	ListCell *lc;

	deparseNode(str, sort_by->node, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	switch (sort_by->sortby_dir)
	{
		case SORTBY_DEFAULT:
			break;
		case SORTBY_ASC:
			appendStringInfoString(str, "ASC ");
			break;
		case SORTBY_DESC:
			appendStringInfoString(str, "DESC ");
			break;
		case SORTBY_USING:
			appendStringInfoString(str, "USING ");
			foreach(lc, sort_by->useOp) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(sort_by->useOp, lc))
					appendStringInfoString(str, ".");
				else
					appendStringInfoChar(str, ' ');
			}
			break;
	}

	switch (sort_by->sortby_nulls)
	{
		case SORTBY_NULLS_DEFAULT:
			break;
		case SORTBY_NULLS_FIRST:
			appendStringInfoString(str, "NULLS FIRST ");
			break;
		case SORTBY_NULLS_LAST:
			appendStringInfoString(str, "NULLS LAST ");
			break;
	}

	removeTrailingSpace(str);
}

static void deparseParamRef(StringInfo str, ParamRef* param_ref)
{
	if (param_ref->number == 0) {
		appendStringInfoChar(str, '?');
	} else {
		appendStringInfo(str, "$%d", param_ref->number);
	}
}

static void deparseSQLValueFunction(StringInfo str, SQLValueFunction* sql_value_function)
{
	switch (sql_value_function->op)
	{
		case SVFOP_CURRENT_DATE:
			appendStringInfoString(str, "current_date");
			break;
		case SVFOP_CURRENT_TIME:
			appendStringInfoString(str, "current_time");
			break;
		case SVFOP_CURRENT_TIME_N:
			appendStringInfoString(str, "current_time"); // with precision
			break;
		case SVFOP_CURRENT_TIMESTAMP:
			appendStringInfoString(str, "current_timestamp");
			break;
		case SVFOP_CURRENT_TIMESTAMP_N:
			appendStringInfoString(str, "current_timestamp"); // with precision
			break;
		case SVFOP_LOCALTIME:
			appendStringInfoString(str, "localtime");
			break;
		case SVFOP_LOCALTIME_N:
			appendStringInfoString(str, "localtime"); // with precision
			break;
		case SVFOP_LOCALTIMESTAMP:
			appendStringInfoString(str, "localtimestamp");
			break;
		case SVFOP_LOCALTIMESTAMP_N:
			appendStringInfoString(str, "localtimestamp"); // with precision
			break;
		case SVFOP_CURRENT_ROLE:
			appendStringInfoString(str, "current_role");
			break;
		case SVFOP_CURRENT_USER:
			appendStringInfoString(str, "current_user");
			break;
		case SVFOP_USER:
			appendStringInfoString(str, "user");
			break;
		case SVFOP_SESSION_USER:
			appendStringInfoString(str, "session_user");
			break;
		case SVFOP_CURRENT_CATALOG:
			appendStringInfoString(str, "current_catalog");
			break;
		case SVFOP_CURRENT_SCHEMA:
			appendStringInfoString(str, "current_schema");
			break;
	}

	if (sql_value_function->typmod != -1)
	{
		appendStringInfo(str, "(%d)", sql_value_function->typmod);
	}
}

static void deparseWithClause(StringInfo str, WithClause *with_clause)
{
	ListCell *lc;

	appendStringInfoString(str, "WITH ");
	if (with_clause->recursive)
		appendStringInfoString(str, "RECURSIVE ");
	
	foreach(lc, with_clause->ctes) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(with_clause->ctes, lc))
			appendStringInfoString(str, ", ");
	}

	removeTrailingSpace(str);
}

static void deparseJoinExpr(StringInfo str, JoinExpr *join_expr)
{
	ListCell *lc;

	deparseNode(str, join_expr->larg, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	switch (join_expr->jointype)
	{
		case JOIN_INNER: /* matching tuple pairs only */
			if (join_expr->isNatural)
				appendStringInfoString(str, "NATURAL ");
			else if (join_expr->quals == NULL && list_length(join_expr->usingClause) == 0)
				appendStringInfoString(str, "CROSS ");
			break;
		case JOIN_LEFT: /* pairs + unmatched LHS tuples */
			appendStringInfoString(str, "LEFT ");
			break;
		case JOIN_FULL: /* pairs + unmatched LHS + unmatched RHS */
			appendStringInfoString(str, "FULL ");
			break;
		case JOIN_RIGHT: /* pairs + unmatched RHS tuples */
			appendStringInfoString(str, "RIGHT ");
			break;
		case JOIN_SEMI:
		case JOIN_ANTI:
		case JOIN_UNIQUE_OUTER:
		case JOIN_UNIQUE_INNER:
			// Only used by the planner/executor, not seen in parser output
			Assert(false);
			break;
	}
	
	appendStringInfoString(str, "JOIN ");

	deparseNode(str, join_expr->rarg, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (join_expr->quals != NULL)
	{
		appendStringInfoString(str, "ON ");
		deparseNode(str, join_expr->quals, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(join_expr->usingClause) > 0)
	{
		appendStringInfoString(str, "USING (");
		foreach(lc, join_expr->usingClause) {
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(join_expr->usingClause, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}

	removeTrailingSpace(str);
}

static void deparseCommonTableExpr(StringInfo str, CommonTableExpr *cte)
{
	ListCell *lc;

	appendStringInfoString(str, cte->ctename);
	if (list_length(cte->aliascolnames) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, cte->aliascolnames) {
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(cte->aliascolnames, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
	appendStringInfoChar(str, ' ');

	appendStringInfoString(str, "AS ");
	switch (cte->ctematerialized) {
		case CTEMaterializeDefault: /* no option specified */
			break;
		case CTEMaterializeAlways:
			appendStringInfoString(str, "MATERIALIZED ");
			break;
		case CTEMaterializeNever:
			appendStringInfoString(str, "NOT MATERIALIZED ");
			break;
	}

	appendStringInfoChar(str, '(');
	deparseNode(str, cte->ctequery, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ')');
}

static void deparseRangeSubselect(StringInfo str, RangeSubselect *range_subselect)
{
	if (range_subselect->lateral)
		appendStringInfoString(str, "LATERAL ");

	appendStringInfoChar(str, '(');
	deparseNode(str, range_subselect->subquery, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ')');

	if (range_subselect->alias != NULL)
	{
		appendStringInfoChar(str, ' ');
		deparseAlias(str, range_subselect->alias);
	}
}

static void deparseRangeFunction(StringInfo str, RangeFunction *range_func)
{
	ListCell *lc;
	ListCell *lc2;

	if (range_func->lateral)
		appendStringInfoString(str, "LATERAL ");

	if (range_func->is_rowsfrom)
	{
		appendStringInfoString(str, "ROWS FROM ");
		appendStringInfoChar(str, '(');
		foreach(lc, range_func->functions)
		{
			List *lfunc = castNode(List, lfirst(lc));
			Assert(list_length(lfunc) == 2);
			deparseFuncCall(str, castNode(FuncCall, linitial(lfunc)));
			List *coldeflist = castNode(List, lsecond(lfunc));
			if (list_length(coldeflist) > 0)
			{
				appendStringInfoString(str, "AS (");
				foreach(lc2, coldeflist)
				{
					deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(coldeflist, lc2))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoChar(str, ')');
			}
			if (lnext(range_func->functions, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
	else
	{
		Assert(list_length(linitial(range_func->functions)) == 2);
		deparseNode(str, linitial(linitial(range_func->functions)), DEPARSE_NODE_CONTEXT_NONE);
	}
	appendStringInfoChar(str, ' ');

	if (range_func->ordinality)
		appendStringInfoString(str, "WITH ORDINALITY ");

	if (range_func->alias != NULL)
	{
		deparseAlias(str, range_func->alias);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(range_func->coldeflist) > 0)
	{
		if (range_func->alias == NULL)
			appendStringInfoString(str, "AS ");
		appendStringInfoChar(str, '(');
		foreach(lc, range_func->coldeflist)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(range_func->coldeflist, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}

	removeTrailingSpace(str);
}

static void deparseAArrayExpr(StringInfo str, A_ArrayExpr *array_expr)
{
	ListCell *lc;

	appendStringInfoString(str, "ARRAY[");
	deparseExprList(str, array_expr->elements);
	appendStringInfoChar(str, ']');
}

static void deparseRowExpr(StringInfo str, RowExpr *row_expr)
{
	ListCell *lc;

	switch (row_expr->row_format)
	{
		case COERCE_EXPLICIT_CALL:
			appendStringInfoString(str, "ROW");
			break;
		case COERCE_EXPLICIT_CAST:
			// TODO
			Assert(false);
			break;
		case COERCE_IMPLICIT_CAST:
			// No prefix
			break;
	}

	appendStringInfoString(str, "(");
	foreach(lc, row_expr->args) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(row_expr->args, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseTypeCast(StringInfo str, TypeCast *type_cast)
{
	bool need_parens = IsA(type_cast->arg, A_Expr);

	Assert(type_cast->typeName != NULL);

	/*
	 * Handle "bool" or "false" in the statement, which is represented as a typecast
     * (other boolean casts should be represented as a cast, i.e. don't need special handling)
	 */
	if (IsA(type_cast->arg, A_Const) &&
		list_length(type_cast->typeName->names) == 2 &&
		strcmp(strVal(linitial(type_cast->typeName->names)), "pg_catalog") == 0 &&
		strcmp(strVal(lsecond(type_cast->typeName->names)), "bool") == 0 &&
		nodeTag(&castNode(A_Const, type_cast->arg)->val) == T_String) {
		char *const_val = castNode(A_Const, type_cast->arg)->val.val.str;
		if (strcmp(const_val, "t") == 0)
		{
			appendStringInfoString(str, "true");
			return;
		}
		if (strcmp(const_val, "f") == 0)
		{
			appendStringInfoString(str, "false");
			return;
		}
	}

	if (need_parens)
		appendStringInfoChar(str, '(');
	deparseNode(str, type_cast->arg, DEPARSE_NODE_CONTEXT_NONE);
	if (need_parens)
		appendStringInfoChar(str, ')');

	appendStringInfoString(str, "::");
	deparseTypeName(str, type_cast->typeName);
}

static void deparseTypeName(StringInfo str, TypeName *type_name)
{
	ListCell *lc;
	bool skip_typmods = false;

	if (type_name->setof)
		appendStringInfoString(str, "SETOF ");

	if (list_length(type_name->names) == 2 && strcmp(strVal(linitial(type_name->names)), "pg_catalog") == 0)
	{
		const char *name = strVal(lsecond(type_name->names));
		if (strcmp(name, "bpchar") == 0)
		{
			appendStringInfoString(str, "char");
		}
		else if (strcmp(name, "varchar") == 0)
		{
			appendStringInfoString(str, "varchar");
		}
		else if (strcmp(name, "numeric") == 0)
		{
			appendStringInfoString(str, "numeric");
		}
		else if (strcmp(name, "bool") == 0)
		{
			appendStringInfoString(str, "boolean");
		}
		else if (strcmp(name, "int2") == 0)
		{
			appendStringInfoString(str, "smallint");
		}
		else if (strcmp(name, "int4") == 0)
		{
			appendStringInfoString(str, "int");
		}
		else if (strcmp(name, "int8") == 0)
		{
			appendStringInfoString(str, "bigint");
		}
		else if (strcmp(name, "real") == 0 || strcmp(name, "float4") == 0)
		{
			appendStringInfoString(str, "real");
		}
		else if (strcmp(name, "float8") == 0)
		{
			appendStringInfoString(str, "double precision");
		}
		else if (strcmp(name, "time") == 0)
		{
			appendStringInfoString(str, "time");
		}
		else if (strcmp(name, "timetz") == 0)
		{
			appendStringInfoString(str, "time ");
			if (list_length(type_name->typmods) > 0)
			{
				appendStringInfoChar(str, '(');
				foreach(lc, type_name->typmods) {
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(type_name->typmods, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ") ");
			}
			appendStringInfoString(str, "with time zone");
			skip_typmods = true;
		}
		else if (strcmp(name, "timestamp") == 0)
		{
			appendStringInfoString(str, "timestamp");
		}
		else if (strcmp(name, "timestamptz") == 0)
		{
			appendStringInfoString(str, "timestamp ");
			if (list_length(type_name->typmods) > 0)
			{
				appendStringInfoChar(str, '(');
				foreach(lc, type_name->typmods) {
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(type_name->typmods, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ") ");
			}
			appendStringInfoString(str, "with time zone");
			skip_typmods = true;
		}
		else if (strcmp(name, "interval") == 0 && list_length(type_name->typmods) == 0)
		{
			appendStringInfoString(str, "interval");
		}
		else if (strcmp(name, "interval") == 0 && list_length(type_name->typmods) >= 1)
		{
			Assert(IsA(linitial(type_name->typmods), A_Const));
			Assert(IsA(&castNode(A_Const, linitial(type_name->typmods))->val, Integer));

			int fields = intVal(&castNode(A_Const, linitial(type_name->typmods))->val);

			appendStringInfoString(str, "interval");

			// This logic is based on intervaltypmodout in timestamp.c
			switch (fields)
			{
				case INTERVAL_MASK(YEAR):
					appendStringInfoString(str, " year");
					break;
				case INTERVAL_MASK(MONTH):
					appendStringInfoString(str, " month");
					break;
				case INTERVAL_MASK(DAY):
					appendStringInfoString(str, " day");
					break;
				case INTERVAL_MASK(HOUR):
					appendStringInfoString(str, " hour");
					break;
				case INTERVAL_MASK(MINUTE):
					appendStringInfoString(str, " minute");
					break;
				case INTERVAL_MASK(SECOND):
					appendStringInfoString(str, " second");
					break;
				case INTERVAL_MASK(YEAR) | INTERVAL_MASK(MONTH):
					appendStringInfoString(str, " year to month");
					break;
				case INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR):
					appendStringInfoString(str, " day to hour");
					break;
				case INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE):
					appendStringInfoString(str, " day to minute");
					break;
				case INTERVAL_MASK(DAY) | INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE) | INTERVAL_MASK(SECOND):
					appendStringInfoString(str, " day to second");
					break;
				case INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE):
					appendStringInfoString(str, " hour to minute");
					break;
				case INTERVAL_MASK(HOUR) | INTERVAL_MASK(MINUTE) | INTERVAL_MASK(SECOND):
					appendStringInfoString(str, " hour to second");
					break;
				case INTERVAL_MASK(MINUTE) | INTERVAL_MASK(SECOND):
					appendStringInfoString(str, " minute to second");
					break;
				case INTERVAL_FULL_RANGE:
					// Nothing
					break;
				default:
					Assert(false);
					break;
			}

			if (list_length(type_name->typmods) == 2)
			{
				int precision = intVal(&castNode(A_Const, lsecond(type_name->typmods))->val);
				if (precision != INTERVAL_FULL_PRECISION)
					appendStringInfo(str, "(%d)", precision);
			}
			
			skip_typmods = true;
		}
		else
		{
			appendStringInfoString(str, "pg_catalog.");
			appendStringInfoString(str, name);
		}
	}
	else
	{
		foreach(lc, type_name->names) {
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(type_name->names, lc))
				appendStringInfoString(str, ".");
		}
	}

	if (list_length(type_name->typmods) > 0 && !skip_typmods)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, type_name->typmods) {
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(type_name->typmods, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}

	foreach(lc, type_name->arrayBounds) {
		appendStringInfoChar(str, '[');
		if (!IsA(lfirst(lc), Integer) || intVal(lfirst(lc)) != -1)
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ']');
	}
}

static void deparseNullTest(StringInfo str, NullTest *null_test)
{
	// TODO: Add support for row type inputs
	Assert(null_test->argisrow == false);

	deparseNode(str, (Node *) null_test->arg, DEPARSE_NODE_CONTEXT_NONE);
	switch (null_test->nulltesttype)
	{
		case IS_NULL:
			appendStringInfoString(str, " IS NULL");
			break;
		case IS_NOT_NULL:
			appendStringInfoString(str, " IS NOT NULL");
			break;
	}
}

static void deparseCaseExpr(StringInfo str, CaseExpr *case_expr)
{
	ListCell *lc;

	appendStringInfoString(str, "CASE ");

	if (case_expr->arg != NULL)
	{
		deparseNode(str, (Node *) case_expr->arg, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, case_expr->args) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (case_expr->defresult != NULL)
	{
		appendStringInfoString(str, "ELSE ");
		deparseNode(str, (Node *) case_expr->defresult, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "END");
}

static void deparseCaseWhen(StringInfo str, CaseWhen *case_when)
{
	appendStringInfoString(str, "WHEN ");
	deparseNode(str, (Node *) case_when->expr, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoString(str, " THEN ");
	deparseNode(str, (Node *) case_when->result, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseAIndirection(StringInfo str, A_Indirection *a_indirection)
{
	ListCell *lc;
	bool need_parens =
		IsA(a_indirection->arg, FuncCall) ||
		IsA(a_indirection->arg, A_Expr) ||
		IsA(a_indirection->arg, TypeCast) ||
		IsA(a_indirection->arg, RowExpr) ||
		(IsA(a_indirection->arg, ColumnRef) && !IsA(linitial(a_indirection->indirection), A_Indices));

	if (need_parens)
		appendStringInfoChar(str, '(');

	deparseNode(str, a_indirection->arg, DEPARSE_NODE_CONTEXT_NONE);

	if (need_parens)
		appendStringInfoChar(str, ')');

	foreach(lc, a_indirection->indirection)
	{
		if (!IsA(lfirst(lc), A_Indices))
			appendStringInfoChar(str, '.');
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
	}
}

static void deparseAIndices(StringInfo str, A_Indices *a_indices)
{
	appendStringInfoChar(str, '[');
	if (a_indices->lidx != NULL)
		deparseNode(str, a_indices->lidx, DEPARSE_NODE_CONTEXT_NONE);
	if (a_indices->lidx != NULL && a_indices->uidx != NULL)
		appendStringInfoChar(str, ':');
	if (a_indices->uidx != NULL)
		deparseNode(str, a_indices->uidx, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ']');
}

static void deparseCoalesceExpr(StringInfo str, CoalesceExpr *coalesce_expr)
{
	appendStringInfoString(str, "COALESCE(");
	deparseExprList(str, coalesce_expr->args);
	appendStringInfoChar(str, ')');
}

static void deparseMinMaxExpr(StringInfo str, MinMaxExpr *min_max_expr)
{
	switch (min_max_expr->op)
	{
		case IS_GREATEST:
			appendStringInfoString(str, "GREATEST(");
			break;
		case IS_LEAST:
			appendStringInfoString(str, "LEAST(");
			break;
	}
	deparseExprList(str, min_max_expr->args);
	appendStringInfoChar(str, ')');
}

static void deparseBooleanTest(StringInfo str, BooleanTest *boolean_test)
{
	deparseNode(str, (Node *) boolean_test->arg, DEPARSE_NODE_CONTEXT_NONE);
	switch (boolean_test->booltesttype)
	{
		case IS_TRUE:
			appendStringInfoString(str, " IS TRUE");
			break;
		case IS_NOT_TRUE:
			appendStringInfoString(str, " IS NOT TRUE");
			break;
		case IS_FALSE:
			appendStringInfoString(str, " IS FALSE");
			break;
		case IS_NOT_FALSE:
			appendStringInfoString(str, " IS NOT FALSE");
			break;
		case IS_UNKNOWN:
			appendStringInfoString(str, " IS UNKNOWN");
			break;
		case IS_NOT_UNKNOWN:
			appendStringInfoString(str, " IS NOT UNKNOWN");
			break;
		default:
			Assert(false);
	}
}

static void deparseColumnDef(StringInfo str, ColumnDef *column_def)
{
	ListCell *lc;

	if (column_def->colname != NULL)
	{
		appendStringInfoString(str, column_def->colname);
		appendStringInfoChar(str, ' ');
	}

	if (column_def->typeName != NULL)
	{
		deparseTypeName(str, column_def->typeName);
		appendStringInfoChar(str, ' ');
	}

	if (column_def->raw_default != NULL)
	{
		appendStringInfoString(str, "USING ");
		deparseNode(str, column_def->raw_default, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, column_def->constraints) {
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(column_def->constraints, lc))
			appendStringInfoChar(str, ' ');
	}

	if (column_def->collClause != NULL)
	{
		deparseCollateClause(str, column_def->collClause);
	}

	removeTrailingSpace(str);

	// TODO
	/*
      if node['collClause']
        output << 'COLLATE'
        output += node['collClause']['CollateClause']['collname'].map(&method(:deparse_item))
      end
	  */
}

static void deparseInsertStmt(StringInfo str, InsertStmt *insert_stmt)
{
	ListCell *lc;
	ListCell *lc2;

	if (insert_stmt->withClause != NULL)
	{
		deparseWithClause(str, insert_stmt->withClause);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "INSERT INTO ");
	deparseRangeVar(str, insert_stmt->relation, DEPARSE_NODE_CONTEXT_INSERT_RELATION);
	appendStringInfoChar(str, ' ');

	if (list_length(insert_stmt->cols) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, insert_stmt->cols)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_INSERT_TARGET_LIST);
			if (lnext(insert_stmt->cols, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (insert_stmt->selectStmt != NULL)
	{
		deparseNode(str, insert_stmt->selectStmt, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}
	else
	{
		appendStringInfoString(str, "DEFAULT VALUES ");
	}

	if (insert_stmt->onConflictClause != NULL)
	{
		deparseOnConflictClause(str, insert_stmt->onConflictClause);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(insert_stmt->returningList) > 0)
	{
		appendStringInfoString(str, "RETURNING ");
		foreach(lc2, insert_stmt->returningList)
		{
			deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_RETURNING);
			if (lnext(insert_stmt->returningList, lc2))
				appendStringInfoString(str, ", ");
		}
	}

	removeTrailingSpace(str);
}

static void deparseInferClause(StringInfo str, InferClause *infer_clause)
{
	ListCell *lc;

	if (list_length(infer_clause->indexElems) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, infer_clause->indexElems)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(infer_clause->indexElems, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (infer_clause->conname != NULL)
	{
		appendStringInfoString(str, "ON CONSTRAINT ");
		appendStringInfoString(str, quote_identifier(infer_clause->conname));
		appendStringInfoChar(str, ' ');
	}

	// TODO: Do we also need to look at on_conflict_clause->whereClause?
	if (infer_clause->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, infer_clause->whereClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseOnConflictClause(StringInfo str, OnConflictClause *on_conflict_clause)
{
	ListCell *lc;

	appendStringInfoString(str, "ON CONFLICT ");

	if (on_conflict_clause->infer != NULL)
	{
		deparseInferClause(str, on_conflict_clause->infer);
		appendStringInfoChar(str, ' ');
	}

	switch (on_conflict_clause->action)
	{
		case ONCONFLICT_NONE:
			Assert(false);
			break;
		case ONCONFLICT_NOTHING:
			appendStringInfoString(str, "DO NOTHING ");
			break;
		case ONCONFLICT_UPDATE:
			appendStringInfoString(str, "DO UPDATE ");
			break;
	}

	if (list_length(on_conflict_clause->targetList) > 0)
	{
		appendStringInfoString(str, "SET ");
		deparseSetTargetList(str, on_conflict_clause->targetList);
		appendStringInfoChar(str, ' ');
	}

	if (on_conflict_clause->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, on_conflict_clause->whereClause, DEPARSE_NODE_CONTEXT_NONE);
	}

	removeTrailingSpace(str);
}

static void deparseIndexElem(StringInfo str, IndexElem* index_elem)
{
	ListCell *lc;

	if (index_elem->name != NULL)
	{
		appendStringInfoString(str, quote_identifier(index_elem->name));
		appendStringInfoChar(str, ' ');
	}
	else if (index_elem->expr != NULL)
	{
		appendStringInfoChar(str, '(');
		deparseNode(str, index_elem->expr, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (list_length(index_elem->collation) > 0)
	{
		appendStringInfoString(str, "COLLATE ");
		foreach(lc, index_elem->collation)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(index_elem->collation, lc))
				appendStringInfoChar(str, '.');
		}
		appendStringInfoChar(str, ' ');
	}

	if (list_length(index_elem->opclass) > 0)
	{
		foreach(lc, index_elem->opclass)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(index_elem->opclass, lc))
				appendStringInfoChar(str, '.');
		}

		if (list_length(index_elem->opclassopts) > 0)
		{
			appendStringInfoChar(str, '(');
			foreach(lc, index_elem->opclassopts)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(index_elem->opclassopts, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
		}
		appendStringInfoChar(str, ' ');
	}

	switch (index_elem->ordering)
	{
		case SORTBY_DEFAULT:
			// Default
			break;
		case SORTBY_ASC:
			appendStringInfoString(str, "ASC ");
			break;
		case SORTBY_DESC:
			appendStringInfoString(str, "DESC ");
			break;
		case SORTBY_USING:
			// Not allowed in CREATE INDEX
			Assert(false);
			break;
	}

	switch (index_elem->nulls_ordering)
	{
		case SORTBY_NULLS_DEFAULT:
			// Default
			break;
		case SORTBY_NULLS_FIRST:
			appendStringInfoString(str, "NULLS FIRST ");
			break;
		case SORTBY_NULLS_LAST:
			appendStringInfoString(str, "NULLS LAST ");
			break;
	}

	removeTrailingSpace(str);
}

static void deparseUpdateStmt(StringInfo str, UpdateStmt *update_stmt)
{
	ListCell* lc;
	ListCell* lc2;
	ListCell* lc3;

	if (update_stmt->withClause != NULL)
	{
		deparseWithClause(str, update_stmt->withClause);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "UPDATE ");
	deparseRangeVar(str, update_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (list_length(update_stmt->targetList) > 0)
	{
		appendStringInfoString(str, "SET ");
		deparseSetTargetList(str, update_stmt->targetList);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(update_stmt->fromClause) > 0)
	{
		appendStringInfoString(str, "FROM ");

		foreach(lc2, update_stmt->fromClause)
		{
			deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_UPDATE);
			if (lnext(update_stmt->fromClause, lc2))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}

	if (update_stmt->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, update_stmt->whereClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(update_stmt->returningList) > 0)
	{
		appendStringInfoString(str, "RETURNING ");
		foreach(lc3, update_stmt->returningList)
		{
			deparseNode(str, lfirst(lc3), DEPARSE_NODE_CONTEXT_RETURNING);
			if (lnext(update_stmt->returningList, lc3))
				appendStringInfoString(str, ", ");
		}
	}

	removeTrailingSpace(str);
}

static void deparseDeleteStmt(StringInfo str, DeleteStmt *delete_stmt)
{
	ListCell* lc;
	ListCell* lc2;

	if (delete_stmt->withClause != NULL)
	{
		deparseWithClause(str, delete_stmt->withClause);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "DELETE FROM ");
	deparseRangeVar(str, delete_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (delete_stmt->usingClause != NULL)
	{
		appendStringInfoString(str, "USING ");
		foreach(lc, delete_stmt->usingClause)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(delete_stmt->usingClause, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}

	if (delete_stmt->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, delete_stmt->whereClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (list_length(delete_stmt->returningList) > 0)
	{
		appendStringInfoString(str, "RETURNING ");
		foreach(lc2, delete_stmt->returningList)
		{
			deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_RETURNING);
			if (lnext(delete_stmt->returningList, lc2))
				appendStringInfoString(str, ", ");
		}
	}

	removeTrailingSpace(str);
}

static void deparseLockingClause(StringInfo str, LockingClause *locking_clause)
{
	ListCell *lc;

	switch (locking_clause->strength)
	{
		case LCS_NONE:
			/* no such clause - only used in PlanRowMark */
			Assert(false);
			break;
		case LCS_FORKEYSHARE:
			appendStringInfoString(str, "FOR KEY SHARE ");
			break;
		case LCS_FORSHARE:
			appendStringInfoString(str, "FOR SHARE ");
			break;
		case LCS_FORNOKEYUPDATE:
			appendStringInfoString(str, "FOR NO KEY UPDATE ");
			break;
		case LCS_FORUPDATE:
			appendStringInfoString(str, "FOR UPDATE ");
			break;
	}

	if (list_length(locking_clause->lockedRels) > 0)
	{
		appendStringInfoString(str, "OF ");
		foreach(lc, locking_clause->lockedRels)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(locking_clause->lockedRels, lc))
				appendStringInfoString(str, ", ");
		}
	}

	// TODO: LockWaitPolicy waitPolicy;	/* NOWAIT and SKIP LOCKED */

	removeTrailingSpace(str);
}

static void deparseSetToDefault(StringInfo str, SetToDefault *set_to_default)
{
	appendStringInfoString(str, "DEFAULT");
}

static void deparseCreateCastStmt(StringInfo str, CreateCastStmt *create_cast_stmt)
{
	ListCell *lc;
	ListCell *lc2;

	appendStringInfoString(str, "CREATE CAST (");
	deparseTypeName(str, create_cast_stmt->sourcetype);
	appendStringInfoString(str, " AS ");
	deparseTypeName(str, create_cast_stmt->targettype);
	appendStringInfoString(str, ") ");

	if (create_cast_stmt->func != NULL)
	{
		appendStringInfoString(str, "WITH FUNCTION ");
		foreach(lc, create_cast_stmt->func->objname)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(create_cast_stmt->func->objname, lc))
				appendStringInfoString(str, ".");
		}
		appendStringInfoChar(str, '(');
		foreach(lc2, create_cast_stmt->func->objargs)
		{
			deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(create_cast_stmt->func->objargs, lc2))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}
	else if (create_cast_stmt->inout)
	{
		appendStringInfoString(str, "WITH INOUT ");
	}
	else
	{
		appendStringInfoString(str, "WITHOUT FUNCTION ");
	}

	switch (create_cast_stmt->context)
	{
		case COERCION_IMPLICIT:
			appendStringInfoString(str, "AS IMPLICIT");
			break;
		case COERCION_ASSIGNMENT:
			appendStringInfoString(str, "AS ASSIGNMENT");
			break;
		case COERCION_EXPLICIT:
			// Default
			break;
	}
}

static void deparseCreateOpClassStmt(StringInfo str, CreateOpClassStmt *create_op_class_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "CREATE OPERATOR CLASS ");

	foreach(lc, create_op_class_stmt->opclassname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_op_class_stmt->opclassname, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, ' ');

	if (create_op_class_stmt->isDefault)
		appendStringInfoString(str, "DEFAULT ");

	appendStringInfoString(str, "FOR TYPE ");
	deparseTypeName(str, create_op_class_stmt->datatype);
	appendStringInfoChar(str, ' ');

	appendStringInfoString(str, "USING ");
	appendStringInfoString(str, quote_identifier(create_op_class_stmt->amname));
	appendStringInfoChar(str, ' ');

	if (create_op_class_stmt->opfamilyname != NULL)
	{
		appendStringInfoString(str, "FAMILY ");
		foreach(lc, create_op_class_stmt->opfamilyname)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(create_op_class_stmt->opfamilyname, lc))
				appendStringInfoChar(str, '.');
		}
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "AS ");
	foreach(lc, create_op_class_stmt->items)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_op_class_stmt->items, lc))
			appendStringInfoString(str, ", ");
	}
}

static void deparseCreateOpClassItem(StringInfo str, CreateOpClassItem *create_op_class_item)
{
	ListCell *lc = NULL;

	switch (create_op_class_item->itemtype)
	{
		case OPCLASS_ITEM_OPERATOR:
			appendStringInfoString(str, "OPERATOR ");
			appendStringInfo(str, "%d ", create_op_class_item->number);

			deparseObjectWithArgs(str, create_op_class_item->name, false);
			appendStringInfoChar(str, ' ');

			if (create_op_class_item->order_family != NULL)
			{
				appendStringInfoString(str, "FOR ORDER BY ");
				foreach(lc, create_op_class_item->order_family)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
					if (lnext(create_op_class_item->order_family, lc))
						appendStringInfoChar(str, '.');
				}
			}
			removeTrailingSpace(str);
			break;
		case OPCLASS_ITEM_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			appendStringInfo(str, "%d ", create_op_class_item->number);
			if (create_op_class_item->class_args != NULL)
			{
				appendStringInfoChar(str, '(');
				foreach(lc, create_op_class_item->class_args)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(create_op_class_item->class_args, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ") ");
			}
			deparseObjectWithArgs(str, create_op_class_item->name, false);
			break;
		case OPCLASS_ITEM_STORAGETYPE:
			appendStringInfoString(str, "STORAGE ");
			deparseTypeName(str, create_op_class_item->storedtype);
			break;
		default:
			Assert(false);
	}
}

static void deparseTableLikeClause(StringInfo str, TableLikeClause *table_like_clause)
{
	appendStringInfoString(str, "LIKE ");
	deparseRangeVar(str, table_like_clause->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (table_like_clause->options == CREATE_TABLE_LIKE_ALL)
		appendStringInfoString(str, "INCLUDING ALL ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_COMMENTS)
		appendStringInfoString(str, "INCLUDING COMMENTS ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_CONSTRAINTS)
		appendStringInfoString(str, "INCLUDING CONSTRAINTS ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_DEFAULTS)
		appendStringInfoString(str, "INCLUDING DEFAULTS ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_IDENTITY)
		appendStringInfoString(str, "INCLUDING IDENTITY ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_GENERATED)
		appendStringInfoString(str, "INCLUDING GENERATED ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_INDEXES)
		appendStringInfoString(str, "INCLUDING INDEXES ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_STATISTICS)
		appendStringInfoString(str, "INCLUDING STATISTICS ");
	if (table_like_clause->options & CREATE_TABLE_LIKE_STORAGE)
		appendStringInfoString(str, "INCLUDING STORAGE ");

	removeTrailingSpace(str);
}

static void deparseCreateDomainStmt(StringInfo str, CreateDomainStmt *create_domain_stmt)
{
	ListCell *lc;

	Assert(create_domain_stmt->typeName != NULL);

	appendStringInfoString(str, "CREATE DOMAIN ");
	foreach(lc, create_domain_stmt->domainname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_domain_stmt->domainname, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoString(str, " AS ");

	deparseTypeName(str, create_domain_stmt->typeName);
	appendStringInfoChar(str, ' ');

	if (create_domain_stmt->collClause != NULL)
	{
		deparseCollateClause(str, create_domain_stmt->collClause);
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, create_domain_stmt->constraints)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseConstraint(StringInfo str, Constraint *constraint)
{
	ListCell *lc;
	bool skip_raw_expr = false;

	if (constraint->conname != NULL)
	{
		appendStringInfoString(str, "CONSTRAINT ");
		appendStringInfoString(str, constraint->conname);
		appendStringInfoChar(str, ' ');
	}

	switch (constraint->contype) {
		case CONSTR_NULL:
			appendStringInfoString(str, "NULL ");
			break;
		case CONSTR_NOTNULL:
			appendStringInfoString(str, "NOT NULL ");
			break;
		case CONSTR_DEFAULT:
			appendStringInfoString(str, "DEFAULT ");
			break;
		case CONSTR_IDENTITY:
			appendStringInfoString(str, "GENERATED ");
			switch (constraint->generated_when)
			{
				case ATTRIBUTE_IDENTITY_ALWAYS:
					appendStringInfoString(str, "ALWAYS ");
					break;
				case ATTRIBUTE_IDENTITY_BY_DEFAULT:
					appendStringInfoString(str, "BY DEFAULT ");
					break;
				default:
					Assert(false);
			}
			appendStringInfoString(str, "AS IDENTITY ");
			deparseOptParenthesizedSeqOptList(str, constraint->options);
			break;
		case CONSTR_GENERATED:
			Assert(constraint->generated_when == ATTRIBUTE_IDENTITY_ALWAYS);
			appendStringInfoString(str, "GENERATED ALWAYS AS (");
			deparseNode(str, constraint->raw_expr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, ") STORED ");
			skip_raw_expr = true;
			break;
		case CONSTR_CHECK:
			appendStringInfoString(str, "CHECK ");
			break;
		case CONSTR_PRIMARY:
			appendStringInfoString(str, "PRIMARY KEY ");
			break;
		case CONSTR_UNIQUE:
			appendStringInfoString(str, "UNIQUE ");
			break;
		case CONSTR_EXCLUSION:
			appendStringInfoString(str, "EXCLUDE ");
			if (strcmp(constraint->access_method, DEFAULT_INDEX_TYPE) != 0)
			{
				appendStringInfoString(str, "USING ");
				appendStringInfoString(str, quote_identifier(constraint->access_method));
				appendStringInfoChar(str, ' ');
			}
			appendStringInfoChar(str, '(');
			foreach(lc, constraint->exclusions)
			{
				List *exclusion = castNode(List, lfirst(lc));
				Assert(list_length(exclusion) == 2);
				deparseNode(str, linitial(exclusion), DEPARSE_NODE_CONTEXT_NONE);
				appendStringInfoString(str, " WITH ");
				deparseNode(str, lsecond(exclusion), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(constraint->exclusions, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, ") ");
			if (constraint->where_clause != NULL)
			{
				appendStringInfoString(str, "WHERE (");
				deparseNode(str, constraint->where_clause, DEPARSE_NODE_CONTEXT_NONE);
				appendStringInfoString(str, ") ");
			}
			break;
		case CONSTR_FOREIGN:
			if (list_length(constraint->fk_attrs) > 0)
				appendStringInfoString(str, "FOREIGN KEY ");
			break;
		case CONSTR_ATTR_DEFERRABLE:
		case CONSTR_ATTR_NOT_DEFERRABLE:
		case CONSTR_ATTR_DEFERRED:
		case CONSTR_ATTR_IMMEDIATE:
			// TODO: attributes for previous constraint node
			Assert(false);
			break;
	}

	if (constraint->raw_expr != NULL && !skip_raw_expr)
	{
		bool needs_parens = IsA(constraint->raw_expr, BoolExpr) || (IsA(constraint->raw_expr, A_Expr) && castNode(A_Expr, constraint->raw_expr)->kind == AEXPR_OP);

		if (needs_parens)
			appendStringInfoChar(str, '(');
		
		deparseNode(str, constraint->raw_expr, DEPARSE_NODE_CONTEXT_NONE);

		if (needs_parens)
			appendStringInfoChar(str, ')');
		
		appendStringInfoChar(str, ' ');
	}

	if (list_length(constraint->keys) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, constraint->keys)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(constraint->keys, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (list_length(constraint->fk_attrs) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, constraint->fk_attrs)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(constraint->fk_attrs, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (constraint->pktable != NULL)
	{
		appendStringInfoString(str, "REFERENCES ");
		deparseRangeVar(str, constraint->pktable, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
		if (list_length(constraint->pk_attrs) > 0)
		{
			appendStringInfoChar(str, '(');
			foreach(lc, constraint->pk_attrs)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(constraint->pk_attrs, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, ") ");
		}
	}

	switch (constraint->fk_matchtype)
	{
		case FKCONSTR_MATCH_SIMPLE:
			// Default
			break;
		case FKCONSTR_MATCH_FULL:
			appendStringInfoString(str, "MATCH FULL ");
			break;
		case FKCONSTR_MATCH_PARTIAL:
			// Not implemented in Postgres
			Assert(false);
			break;
		default:
			// Not specified
			break;
	}

	switch (constraint->fk_upd_action)
	{
		case FKCONSTR_ACTION_NOACTION:
			// Default
			break;
		case FKCONSTR_ACTION_RESTRICT:
			appendStringInfoString(str, "ON UPDATE RESTRICT ");
			break;
		case FKCONSTR_ACTION_CASCADE:
			appendStringInfoString(str, "ON UPDATE CASCADE ");
			break;
		case FKCONSTR_ACTION_SETNULL:
			appendStringInfoString(str, "ON UPDATE SET NULL ");
			break;
		case FKCONSTR_ACTION_SETDEFAULT:
			appendStringInfoString(str, "ON UPDATE SET DEFAULT ");
			break;
		default:
			// Not specified
			break;
	}

	switch (constraint->fk_del_action)
	{
		case FKCONSTR_ACTION_NOACTION:
			// Default
			break;
		case FKCONSTR_ACTION_RESTRICT:
			appendStringInfoString(str, "ON DELETE RESTRICT ");
			break;
		case FKCONSTR_ACTION_CASCADE:
			appendStringInfoString(str, "ON DELETE CASCADE ");
			break;
		case FKCONSTR_ACTION_SETNULL:
			appendStringInfoString(str, "ON DELETE SET NULL ");
			break;
		case FKCONSTR_ACTION_SETDEFAULT:
			appendStringInfoString(str, "ON DELETE SET DEFAULT ");
			break;
		default:
			// Not specified
			break;
	}

	if (list_length(constraint->including) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, constraint->including)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(constraint->including, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (constraint->indexname != NULL)
		appendStringInfo(str, "USING INDEX %s ", quote_identifier(constraint->indexname));

	if (constraint->indexspace != NULL)
		appendStringInfo(str, "USING INDEX TABLESPACE %s ", quote_identifier(constraint->indexspace));

	if (constraint->deferrable)
		appendStringInfoString(str, "DEFERRABLE ");

	if (constraint->initdeferred)
		appendStringInfoString(str, "INITIALLY DEFERRED ");

	if (constraint->is_no_inherit)
		appendStringInfoString(str, "NO INHERIT ");

	if (constraint->skip_validation)
		appendStringInfoString(str, "NOT VALID ");
	
	removeTrailingSpace(str);
}

static void deparseCreateFunctionStmt(StringInfo str, CreateFunctionStmt *create_function_stmt)
{
	ListCell *lc;
	bool tableFunc = false;

	appendStringInfoString(str, "CREATE ");
	if (create_function_stmt->replace)
		appendStringInfoString(str, "OR REPLACE ");
	appendStringInfoString(str, "FUNCTION ");

	foreach(lc, create_function_stmt->funcname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_function_stmt->funcname, lc))
			appendStringInfoChar(str, '.');
	}

	appendStringInfoChar(str, '(');
	foreach(lc, create_function_stmt->parameters)
	{
		FunctionParameter *function_parameter = castNode(FunctionParameter, lfirst(lc));
		if (function_parameter->mode != FUNC_PARAM_TABLE)
		{
			deparseFunctionParameter(str, function_parameter);
			if (lnext(create_function_stmt->parameters, lc) && castNode(FunctionParameter, lfirst(lnext(create_function_stmt->parameters, lc)))->mode != FUNC_PARAM_TABLE)
				appendStringInfoString(str, ", ");
		}
		else
		{
			tableFunc = true;
		}
	}
	appendStringInfoString(str, ") ");

	if (tableFunc)
	{
		appendStringInfoString(str, "RETURNS TABLE (");
		foreach(lc, create_function_stmt->parameters)
		{
			FunctionParameter *function_parameter = castNode(FunctionParameter, lfirst(lc));
			if (function_parameter->mode == FUNC_PARAM_TABLE)
			{
				deparseFunctionParameter(str, function_parameter);
				if (lnext(create_function_stmt->parameters, lc))
					appendStringInfoString(str, ", ");
			}
		}
		appendStringInfoString(str, ") ");
	}
	else if (create_function_stmt->returnType != NULL)
	{
		appendStringInfoString(str, "RETURNS ");
		deparseTypeName(str, create_function_stmt->returnType);
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, create_function_stmt->options)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_CREATE_FUNCTION);
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseFunctionParameter(StringInfo str, FunctionParameter *function_parameter)
{
	switch (function_parameter->mode)
	{
		case FUNC_PARAM_IN: /* input only */
			// Default
			break;
		case FUNC_PARAM_OUT: /* output only */
			appendStringInfoString(str, "OUT ");
			break;
		case FUNC_PARAM_INOUT: /* both */
			appendStringInfoString(str, "INOUT ");
			break;
		case FUNC_PARAM_VARIADIC: /* variadic (always input) */
			appendStringInfoString(str, "VARIADIC ");
			break;
		case FUNC_PARAM_TABLE: /* table function output column */
			// No special annotation, the caller is expected to correctly put
			// this into the RETURNS part of the CREATE FUNCTION statement
			break;
		default:
			Assert(false);
			break;
	}

	if (function_parameter->name != NULL)
	{
		appendStringInfoString(str, function_parameter->name);
		appendStringInfoChar(str, ' ');
	}

	deparseTypeName(str, function_parameter->argType);
	appendStringInfoChar(str, ' ');

	if (function_parameter->defexpr != NULL)
	{
		appendStringInfoString(str, "= ");
		deparseNode(str, function_parameter->defexpr, DEPARSE_NODE_CONTEXT_NONE);
	}

	removeTrailingSpace(str);
}

static void deparseDefElem(StringInfo str, DefElem *def_elem, DeparseNodeContext context)
{
	ListCell *lc;
	if (strcmp(def_elem->defname, "as") == 0)
	{
		appendStringInfoString(str, "AS ");
		if (IsA(def_elem->arg, List)) {
			appendStringInfoString(str, "$$");
			foreach(lc, castNode(List, def_elem->arg))
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(castNode(List, def_elem->arg), lc))
					appendStringInfoChar(str, ' ');
			}
			appendStringInfoString(str, "$$");
		}
		else
		{
			deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
		}
	}
	else if (strcmp(def_elem->defname, "language") == 0)
	{
		appendStringInfoString(str, "language ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "volatility") == 0)
	{
		Assert(IsA(def_elem->arg, String));
		char *volatility = pstrdup(strVal(def_elem->arg));
		for (unsigned char *p = (unsigned char *) volatility; *p; p++)
			*p = pg_toupper(*p);
		appendStringInfoString(str, volatility);
	}
	else if (strcmp(def_elem->defname, "strict") == 0)
	{
		Assert(IsA(def_elem->arg, Integer));
		if (intVal(def_elem->arg) == 1)
			appendStringInfoString(str, "RETURNS NULL ON NULL INPUT");
		else
			appendStringInfoString(str, "CALLED ON NULL INPUT");
	}
	else if (strcmp(def_elem->defname, "transaction_isolation") == 0)
	{
		appendStringInfoString(str, "ISOLATION LEVEL ");
		char *iso_level = pstrdup(strVal(&castNode(A_Const, def_elem->arg)->val));
		for (unsigned char *p = (unsigned char *) iso_level; *p; p++)
			*p = pg_toupper(*p);
		appendStringInfoString(str, iso_level);
	}
	else if (strcmp(def_elem->defname, "transaction_read_only") == 0 && intVal(&castNode(A_Const, def_elem->arg)->val) == 1)
	{
		appendStringInfoString(str, "READ ONLY");
	}
	else if (strcmp(def_elem->defname, "transaction_read_only") == 0 && intVal(&castNode(A_Const, def_elem->arg)->val) == 0)
	{
		appendStringInfoString(str, "READ WRITE");
	}
	else if (strcmp(def_elem->defname, "transaction_deferrable") == 0 && intVal(&castNode(A_Const, def_elem->arg)->val) == 1)
	{
		appendStringInfoString(str, "DEFERRABLE");
	}
	else if (strcmp(def_elem->defname, "transaction_deferrable") == 0 && intVal(&castNode(A_Const, def_elem->arg)->val) == 0)
	{
		appendStringInfoString(str, "NOT DEFERRABLE");
	}
	else if (strcmp(def_elem->defname, "cache") == 0)
	{
		appendStringInfoString(str, "CACHE ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "cycle") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "CYCLE");
	}
	else if (strcmp(def_elem->defname, "cycle") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NO CYCLE");
	}
	else if (strcmp(def_elem->defname, "increment") == 0)
	{
		appendStringInfoString(str, "INCREMENT ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "maxvalue") == 0 && def_elem->arg != NULL)
	{
		appendStringInfoString(str, "MAXVALUE ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "maxvalue") == 0 && def_elem->arg == NULL)
	{
		appendStringInfoString(str, "NO MAXVALUE ");
	}
	else if (strcmp(def_elem->defname, "minvalue") == 0 && def_elem->arg != NULL)
	{
		appendStringInfoString(str, "MINVALUE ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "minvalue") == 0 && def_elem->arg == NULL)
	{
		appendStringInfoString(str, "NO MINVALUE ");
	}
	else if (strcmp(def_elem->defname, "owned_by") == 0)
	{
		appendStringInfoString(str, "OWNED BY ");
		foreach(lc, castNode(List, def_elem->arg))
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(castNode(List, def_elem->arg), lc))
				appendStringInfoChar(str, '.');
		}
	}
	else if (strcmp(def_elem->defname, "sequence_name") == 0)
	{
		appendStringInfoString(str, "SEQUENCE NAME ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "start") == 0)
	{
		appendStringInfoString(str, "START ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "restart") == 0 && def_elem->arg == NULL)
	{
		appendStringInfoString(str, "RESTART ");
	}
	else if (strcmp(def_elem->defname, "restart") == 0 && def_elem->arg != NULL)
	{
		appendStringInfoString(str, "RESTART ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "handler") == 0 && def_elem->arg != NULL)
	{
		appendStringInfoString(str, "HANDLER ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "handler") == 0 && def_elem->arg == NULL)
	{
		appendStringInfoString(str, "NO HANDLER ");
	}
	else if (strcmp(def_elem->defname, "validator") == 0 && def_elem->arg != NULL)
	{
		appendStringInfoString(str, "VALIDATOR ");
		deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (strcmp(def_elem->defname, "validator") == 0 && def_elem->arg == NULL)
	{
		appendStringInfoString(str, "NO VALIDATOR ");
	}
	else if (strcmp(def_elem->defname, "superuser") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "SUPERUSER");
	}
	else if (strcmp(def_elem->defname, "superuser") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOSUPERUSER");
	}
	else if (strcmp(def_elem->defname, "createrole") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "CREATEROLE");
	}
	else if (strcmp(def_elem->defname, "createrole") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOCREATEROLE");
	}
	else if (strcmp(def_elem->defname, "replication") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "REPLICATION");
	}
	else if (strcmp(def_elem->defname, "replication") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOREPLICATION");
	}
	else if (strcmp(def_elem->defname, "createdb") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "CREATEDB");
	}
	else if (strcmp(def_elem->defname, "createdb") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOCREATEDB");
	}
	else if (strcmp(def_elem->defname, "canlogin") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "LOGIN");
	}
	else if (strcmp(def_elem->defname, "canlogin") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOLOGIN");
	}
	else if (strcmp(def_elem->defname, "bypassrls") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "BYPASSRLS");
	}
	else if (strcmp(def_elem->defname, "bypassrls") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOBYPASSRLS");
	}
	else if (strcmp(def_elem->defname, "inherit") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "INHERIT");
	}
	else if (strcmp(def_elem->defname, "inherit") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOINHERIT");
	}
	else if (strcmp(def_elem->defname, "window") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "WINDOW");
	}
	else if (strcmp(def_elem->defname, "leakproof") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "LEAKPROOF");
	}
	else if (strcmp(def_elem->defname, "isreplication") == 0 && intVal(def_elem->arg) == 1)
	{
		appendStringInfoString(str, "REPLICATION");
	}
	else if (strcmp(def_elem->defname, "isreplication") == 0 && intVal(def_elem->arg) == 0)
	{
		appendStringInfoString(str, "NOREPLICATION");
	}
	else if (strcmp(def_elem->defname, "set") == 0 && IsA(def_elem->arg, VariableSetStmt))
	{
		deparseVariableSetStmt(str, castNode(VariableSetStmt, def_elem->arg));
	}
	else if (context == DEPARSE_NODE_CONTEXT_EXPLAIN || context == DEPARSE_NODE_CONTEXT_CREATE_FUNCTION || context == DEPARSE_NODE_CONTEXT_COPY)
	{
		Assert(def_elem->defname != NULL);
		char *defname = pstrdup(def_elem->defname);
		for (unsigned char *p = (unsigned char *) defname; *p; p++)
			*p = pg_toupper(*p);
		appendStringInfoString(str, defname);
		if (def_elem->arg != NULL)
		{
			appendStringInfoChar(str, ' ');
			deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_IDENTIFIER);
		}
	}
	else
	{
		if (def_elem->defnamespace != NULL)
		{
			appendStringInfoString(str, quote_identifier(def_elem->defnamespace));
			appendStringInfoChar(str, '.');
		}
		if (def_elem->defname != NULL)
			appendStringInfoString(str, quote_identifier(def_elem->defname));
		if (def_elem->defname != NULL && def_elem->arg != NULL)
			appendStringInfoChar(str, '=');
		if (def_elem->arg != NULL)
			deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_CONSTANT);
	}
}

static void deparseCreateSchemaStmt(StringInfo str, CreateSchemaStmt *create_schema_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE SCHEMA ");

	if (create_schema_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");

	if (create_schema_stmt->schemaname)
	{
		appendStringInfoString(str, quote_identifier(create_schema_stmt->schemaname));
		appendStringInfoChar(str, ' ');
	}
		
	if (create_schema_stmt->authrole != NULL)
	{
		appendStringInfoString(str, "AUTHORIZATION ");
		deparseRoleSpec(str, create_schema_stmt->authrole);
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, create_schema_stmt->schemaElts)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_schema_stmt->schemaElts, lc))
			appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseRoleSpec(StringInfo str, RoleSpec *role_spec)
{
	switch (role_spec->roletype)
	{
		case ROLESPEC_CSTRING:
			Assert(role_spec->rolename != NULL);
			appendStringInfoString(str, quote_identifier(role_spec->rolename));
			break;
		case ROLESPEC_CURRENT_USER:
			appendStringInfoString(str, "CURRENT_USER");
			break;
		case ROLESPEC_SESSION_USER:
			appendStringInfoString(str, "SESSION_USER");
			break;
		case ROLESPEC_PUBLIC:
			appendStringInfoString(str, "public");
			break;
	}
}

static void deparsePartitionSpec(StringInfo str, PartitionSpec *partition_spec)
{
	ListCell *lc;

	appendStringInfoString(str, "PARTITION BY ");
	appendStringInfoString(str, partition_spec->strategy);

	appendStringInfoChar(str, '(');
	foreach(lc, partition_spec->partParams)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(partition_spec->partParams, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparsePartitionElem(StringInfo str, PartitionElem *partition_elem)
{
	ListCell *lc;

	if (partition_elem->name != NULL)
	{
		appendStringInfoString(str, quote_identifier(partition_elem->name));
		appendStringInfoChar(str, ' ');
	}
	else if (partition_elem->expr != NULL)
	{
		appendStringInfoChar(str, '(');
		deparseNode(str, partition_elem->expr, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (list_length(partition_elem->collation) > 0)
	{
		appendStringInfoString(str, "COLLATE ");
		foreach(lc, partition_elem->collation)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(partition_elem->collation, lc))
				appendStringInfoChar(str, '.');
		}
		appendStringInfoChar(str, ' ');
	}

	foreach(lc, partition_elem->opclass)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(partition_elem->opclass, lc))
			appendStringInfoChar(str, '.');
	}

	removeTrailingSpace(str);
}

static void deparsePartitionBoundSpec(StringInfo str, PartitionBoundSpec *partition_bound_spec)
{
	ListCell *lc;

	if (partition_bound_spec->is_default)
	{
		appendStringInfoString(str, "DEFAULT");
		return;
	}

	appendStringInfoString(str, "FOR VALUES ");

	switch (partition_bound_spec->strategy)
	{
		case PARTITION_STRATEGY_HASH:
			appendStringInfo(str, "WITH (MODULUS %d, REMAINDER %d)", partition_bound_spec->modulus, partition_bound_spec->remainder);
			break;
		case PARTITION_STRATEGY_LIST:
			appendStringInfoString(str, "IN (");
			foreach(lc, partition_bound_spec->listdatums)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(partition_bound_spec->listdatums, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			break;
		case PARTITION_STRATEGY_RANGE:
			appendStringInfoString(str, "FROM (");
			foreach(lc, partition_bound_spec->lowerdatums)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(partition_bound_spec->lowerdatums, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, ") TO (");
			foreach(lc, partition_bound_spec->upperdatums)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(partition_bound_spec->upperdatums, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			break;
		default:
			Assert(false);
			break;
	}
}

static void deparsePartitionCmd(StringInfo str, PartitionCmd *partition_cmd)
{
	deparseRangeVar(str, partition_cmd->name, DEPARSE_NODE_CONTEXT_NONE);

	if (partition_cmd->bound != NULL)
	{
		appendStringInfoChar(str, ' ');
		deparsePartitionBoundSpec(str, partition_cmd->bound);
	}
}

static void deparseCreateStmt(StringInfo str, CreateStmt *create_stmt, bool is_foreign_table)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	if (is_foreign_table)
		appendStringInfoString(str, "FOREIGN ");

	switch (create_stmt->relation->relpersistence)
	{
		case RELPERSISTENCE_PERMANENT:
			// Default
			break;
		case RELPERSISTENCE_UNLOGGED:
			appendStringInfoString(str, "UNLOGGED ");
			break;
		case RELPERSISTENCE_TEMP:
			appendStringInfoString(str, "TEMPORARY ");
			break;
		default:
			Assert(false);
			break;
	}

	appendStringInfoString(str, "TABLE ");

	if (create_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");

	deparseRangeVar(str, create_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (create_stmt->ofTypename != NULL)
	{
		appendStringInfoString(str, "OF ");
		deparseTypeName(str, create_stmt->ofTypename);
		appendStringInfoChar(str, ' ');
	}

	if ((create_stmt->partbound == NULL && create_stmt->ofTypename == NULL) || list_length(create_stmt->tableElts) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, create_stmt->tableElts)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_stmt->tableElts, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (create_stmt->partbound != NULL)
	{
		Assert(list_length(create_stmt->inhRelations) == 1);
		appendStringInfoString(str, "PARTITION OF ");
		deparseNode(str, linitial(create_stmt->inhRelations), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		appendStringInfoChar(str, ' ');
		deparsePartitionBoundSpec(str, create_stmt->partbound);
		appendStringInfoChar(str, ' ');
	}
	else if (list_length(create_stmt->inhRelations) > 0)
	{
		appendStringInfoString(str, "INHERITS (");
		foreach(lc, create_stmt->inhRelations)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_stmt->inhRelations, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (create_stmt->partspec != NULL)
	{
		deparsePartitionSpec(str, create_stmt->partspec);
		appendStringInfoChar(str, ' ');
	}

	if (create_stmt->accessMethod != NULL)
	{
		appendStringInfoString(str, "USING ");
		appendStringInfoString(str, create_stmt->accessMethod);
	}

	if (list_length(create_stmt->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach(lc, create_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	switch (create_stmt->oncommit)
	{
		case ONCOMMIT_NOOP:
			// No ON COMMIT clause
			break;
		case ONCOMMIT_PRESERVE_ROWS:
			appendStringInfoString(str, "ON COMMIT PRESERVE ROWS ");
			break;
		case ONCOMMIT_DELETE_ROWS:
			appendStringInfoString(str, "ON COMMIT DELETE ROWS ");
			break;
		case ONCOMMIT_DROP:
			appendStringInfoString(str, "ON COMMIT DROP ");
			break;
	}

	if (create_stmt->tablespacename != NULL)
	{
		appendStringInfoString(str, "TABLESPACE ");
		appendStringInfoString(str, quote_identifier(create_stmt->tablespacename));
	}

	// TODO: constraints (list of Constraint nodes)

	removeTrailingSpace(str);
}

static void deparseSecLabelStmt(StringInfo str, SecLabelStmt *sec_label_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "SECURITY LABEL ");

	if (sec_label_stmt->provider != NULL)
	{
		appendStringInfoString(str, "FOR ");
		appendStringInfoString(str, quote_identifier(sec_label_stmt->provider));
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "ON ");

	switch (sec_label_stmt->objtype)
	{
		case OBJECT_COLUMN:
			appendStringInfoString(str, "COLUMN ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_FOREIGN_TABLE:
			appendStringInfoString(str, "FOREIGN TABLE ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_SEQUENCE:
			appendStringInfoString(str, "SEQUENCE ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_VIEW:
			appendStringInfoString(str, "VIEW ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_MATVIEW:
			appendStringInfoString(str, "MATERIALIZED VIEW ");
			deparseAnyName(str, castNode(List, sec_label_stmt->object));
			break;
		case OBJECT_DATABASE:
			appendStringInfoString(str, "DATABASE ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_EVENT_TRIGGER:
			appendStringInfoString(str, "EVENT TRIGGER ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_LANGUAGE:
			appendStringInfoString(str, "LANGUAGE ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_PUBLICATION:
			appendStringInfoString(str, "PUBLICATION ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_ROLE:
			appendStringInfoString(str, "ROLE ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_SCHEMA:
			appendStringInfoString(str, "SCHEMA ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_SUBSCRIPTION:
			appendStringInfoString(str, "SUBSCRIPTION ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_TABLESPACE:
			appendStringInfoString(str, "TABLESPACE ");
			appendStringInfoString(str, quote_identifier(strVal(sec_label_stmt->object)));
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			deparseTypeName(str, castNode(TypeName, sec_label_stmt->object));
			break;
		case OBJECT_DOMAIN:
			appendStringInfoString(str, "DOMAIN ");
			deparseTypeName(str, castNode(TypeName, sec_label_stmt->object));
			break;
		case OBJECT_AGGREGATE:
			appendStringInfoString(str, "AGGREGATE ");
			deparseObjectWithArgs(str, castNode(ObjectWithArgs, sec_label_stmt->object), false);
			break;
		case OBJECT_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			deparseObjectWithArgs(str, castNode(ObjectWithArgs, sec_label_stmt->object), false);
			break;
		case OBJECT_LARGEOBJECT:
			appendStringInfoString(str, "LARGE OBJECT ");
			deparseValue(str, (Value *) sec_label_stmt->object, DEPARSE_NODE_CONTEXT_CONSTANT);
			break;
		case OBJECT_PROCEDURE:
			appendStringInfoString(str, "PROCEDURE ");
			deparseObjectWithArgs(str, castNode(ObjectWithArgs, sec_label_stmt->object), false);
			break;
		case OBJECT_ROUTINE:
			appendStringInfoString(str, "ROUTINE ");
			deparseObjectWithArgs(str, castNode(ObjectWithArgs, sec_label_stmt->object), false);
			break;
		default:
			// Not supported in the parser
			Assert(false);
			break;
	}

	appendStringInfoString(str, " IS ");

	if (sec_label_stmt->label != NULL)
		deparseStringLiteral(str, sec_label_stmt->label);
	else
		appendStringInfoString(str, "NULL");
}

static void deparseCreateForeignTableStmt(StringInfo str, CreateForeignTableStmt* create_foreign_table_stmt)
{
	ListCell *lc;

	deparseCreateStmt(str, &create_foreign_table_stmt->base, true);

	appendStringInfoString(str, " SERVER ");
	appendStringInfoString(str, quote_identifier(create_foreign_table_stmt->servername));

	if (list_length(create_foreign_table_stmt->options) > 0)
	{
		appendStringInfoString(str, "OPTIONS (");
		foreach(lc, create_foreign_table_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_foreign_table_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}
}

static void deparseCreateTableAsStmt(StringInfo str, CreateTableAsStmt *create_table_as_stmt)
{
	ListCell *lc;
	appendStringInfoString(str, "CREATE ");

	switch (create_table_as_stmt->into->rel->relpersistence)
	{
		case RELPERSISTENCE_PERMANENT:
			// Default
			break;
		case RELPERSISTENCE_UNLOGGED:
			appendStringInfoString(str, "UNLOGGED ");
			break;
		case RELPERSISTENCE_TEMP:
			appendStringInfoString(str, "TEMPORARY ");
			break;
		default:
			Assert(false);
			break;
	}

	switch (create_table_as_stmt->relkind)
	{
		case OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			break;
		case OBJECT_MATVIEW:
			appendStringInfoString(str, "MATERIALIZED VIEW ");
			break;
		default:
			// Not supported here
			Assert(false);
			break;
	}

	deparseIntoClause(str, create_table_as_stmt->into);
	appendStringInfoChar(str, ' ');

	appendStringInfoString(str, "AS ");
	deparseNode(str, create_table_as_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (create_table_as_stmt->into->skipData)
		appendStringInfoString(str, "WITH NO DATA ");

	removeTrailingSpace(str);
}

static void deparseViewStmt(StringInfo str, ViewStmt *view_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	if (view_stmt->replace)
		appendStringInfoString(str, "OR REPLACE ");

	switch (view_stmt->view->relpersistence)
	{
		case RELPERSISTENCE_PERMANENT:
			// Default
			break;
		case RELPERSISTENCE_UNLOGGED:
			appendStringInfoString(str, "UNLOGGED ");
			break;
		case RELPERSISTENCE_TEMP:
			appendStringInfoString(str, "TEMPORARY ");
			break;
		default:
			Assert(false);
			break;
	}

	appendStringInfoString(str, "VIEW ");
	deparseRangeVar(str, view_stmt->view, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (list_length(view_stmt->aliases) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, view_stmt->aliases)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(view_stmt->aliases, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (list_length(view_stmt->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach(lc, view_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(view_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	appendStringInfoString(str, "AS ");
	deparseNode(str, view_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	switch (view_stmt->withCheckOption)
	{
		case NO_CHECK_OPTION:
			// Default
			break;
		case LOCAL_CHECK_OPTION:
			appendStringInfoString(str, "WITH LOCAL CHECK OPTION ");
			break;
		case CASCADED_CHECK_OPTION:
			appendStringInfoString(str, "WITH CHECK OPTION ");
			break;
	}

	removeTrailingSpace(str);
}

static void deparseDropStmt(StringInfo str, DropStmt *drop_stmt)
{
	ListCell *lc;
	ListCell *lc2;

	appendStringInfoString(str, "DROP ");

	switch (drop_stmt->removeType)
	{
		case OBJECT_ACCESS_METHOD:
			appendStringInfoString(str, "ACCESS METHOD ");
			break;
		case OBJECT_AGGREGATE:
			appendStringInfoString(str, "AGGREGATE ");
			break;
		case OBJECT_AMOP:
			// TODO
			Assert(false);
			break;
		case OBJECT_AMPROC:
			// TODO
			Assert(false);
			break;
		case OBJECT_ATTRIBUTE:
			// TODO
			Assert(false);
			break;
		case OBJECT_CAST:
			appendStringInfoString(str, "CAST ");
			break;
		case OBJECT_COLUMN:
			// TODO
			Assert(false);
			break;
		case OBJECT_COLLATION:
			appendStringInfoString(str, "COLLATION ");
			break;
		case OBJECT_CONVERSION:
			appendStringInfoString(str, "CONVERSION ");
			break;
		case OBJECT_DATABASE:
			// TODO
			Assert(false);
			break;
		case OBJECT_DEFAULT:
			// TODO
			Assert(false);
			break;
		case OBJECT_DEFACL:
			// TODO
			Assert(false);
			break;
		case OBJECT_DOMAIN:
			appendStringInfoString(str, "DOMAIN ");
			break;
		case OBJECT_DOMCONSTRAINT:
			// TODO
			Assert(false);
			break;
		case OBJECT_EVENT_TRIGGER:
			appendStringInfoString(str, "EVENT TRIGGER ");
			break;
		case OBJECT_EXTENSION:
			appendStringInfoString(str, "EXTENSION ");
			break;
		case OBJECT_FDW:
			appendStringInfoString(str, "FOREIGN DATA WRAPPER ");
			break;
		case OBJECT_FOREIGN_SERVER:
			appendStringInfoString(str, "SERVER ");
			break;
		case OBJECT_FOREIGN_TABLE:
			appendStringInfoString(str, "FOREIGN TABLE ");
			break;
		case OBJECT_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			break;
		case OBJECT_INDEX:
			appendStringInfoString(str, "INDEX ");
			break;
		case OBJECT_LANGUAGE:
			// TODO
			Assert(false);
			break;
		case OBJECT_LARGEOBJECT:
			// TODO
			Assert(false);
			break;
		case OBJECT_MATVIEW:
			appendStringInfoString(str, "MATERIALIZED VIEW ");
			break;
		case OBJECT_OPCLASS:
			appendStringInfoString(str, "OPERATOR CLASS ");
			break;
		case OBJECT_OPERATOR:
			appendStringInfoString(str, "OPERATOR ");
			break;
		case OBJECT_OPFAMILY:
			appendStringInfoString(str, "OPERATOR FAMILY ");
			break;
		case OBJECT_POLICY:
			appendStringInfoString(str, "POLICY ");
			break;
		case OBJECT_PROCEDURE:
			// TODO
			Assert(false);
			break;
		case OBJECT_PUBLICATION:
			appendStringInfoString(str, "PUBLICATION ");
			break;
		case OBJECT_PUBLICATION_REL:
			// TODO
			Assert(false);
			break;
		case OBJECT_ROLE:
			// TODO
			Assert(false);
			break;
		case OBJECT_ROUTINE:
			// TODO
			Assert(false);
			break;
		case OBJECT_RULE:
			appendStringInfoString(str, "RULE ");
			break;
		case OBJECT_SCHEMA:
			appendStringInfoString(str, "SCHEMA ");
			break;
		case OBJECT_SEQUENCE:
			appendStringInfoString(str, "SEQUENCE ");
			break;
		case OBJECT_SUBSCRIPTION:
			// TODO
			Assert(false);
			break;
		case OBJECT_STATISTIC_EXT:
			appendStringInfoString(str, "STATISTICS ");
			break;
		case OBJECT_TABCONSTRAINT:
			// TODO
			Assert(false);
			break;
		case OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			break;
		case OBJECT_TABLESPACE:
			// TODO
			Assert(false);
			break;
		case OBJECT_TRANSFORM:
			appendStringInfoString(str, "TRANSFORM ");
			break;
		case OBJECT_TRIGGER:
			appendStringInfoString(str, "TRIGGER ");
			break;
		case OBJECT_TSCONFIGURATION:
			appendStringInfoString(str, "TEXT SEARCH CONFIGURATION ");
			break;
		case OBJECT_TSDICTIONARY:
			appendStringInfoString(str, "TEXT SEARCH DICTIONARY ");
			break;
		case OBJECT_TSPARSER:
			appendStringInfoString(str, "TEXT SEARCH PARSER ");
			break;
		case OBJECT_TSTEMPLATE:
			appendStringInfoString(str, "TEXT SEARCH TEMPLATE ");
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			break;
		case OBJECT_USER_MAPPING:
			// TODO
			Assert(false);
			break;
		case OBJECT_VIEW:
			appendStringInfoString(str, "VIEW ");
			break;
	}

	if (drop_stmt->concurrent)
		appendStringInfoString(str, "CONCURRENTLY ");

	if (drop_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	switch (drop_stmt->removeType)
	{
		case OBJECT_CAST:
			Assert(list_length(drop_stmt->objects) == 1);
			Assert(list_length(linitial(drop_stmt->objects)) == 2);
			appendStringInfoChar(str, '(');
			deparseNode(str, linitial(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " AS ");
			deparseNode(str, lsecond(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoChar(str, ')');
			break;
		case OBJECT_OPFAMILY:
		case OBJECT_OPCLASS:
			Assert(list_length(drop_stmt->objects) == 1);
			Assert(list_length(linitial(drop_stmt->objects)) >= 2);
			deparseNode(str, llast(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " USING ");
			foreach(lc, linitial(drop_stmt->objects))
			{
				// Ignore last element
				if (lnext(linitial(drop_stmt->objects), lc))
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
					if (foreach_current_index(lc) < list_length(linitial(drop_stmt->objects)) - 2)
						appendStringInfoChar(str, '.');
				}
			}
			break;
		case OBJECT_TRIGGER:
		case OBJECT_RULE:
		case OBJECT_POLICY:
			Assert(list_length(drop_stmt->objects) == 1);
			Assert(list_length(linitial(drop_stmt->objects)) >= 2);
			deparseNode(str, llast(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " ON ");
			foreach(lc, linitial(drop_stmt->objects))
			{
				// Ignore last element
				if (lnext(linitial(drop_stmt->objects), lc))
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
					if (foreach_current_index(lc) < list_length(linitial(drop_stmt->objects)) - 2)
						appendStringInfoChar(str, '.');
				}
			}
			break;
		case OBJECT_TRANSFORM:
			Assert(list_length(drop_stmt->objects) == 1);
			Assert(list_length(linitial(drop_stmt->objects)) == 2);
			appendStringInfoString(str, "FOR ");
			deparseNode(str, linitial(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " LANGUAGE ");
			deparseNode(str, lsecond(linitial(drop_stmt->objects)), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoChar(str, ' ');
			break;
		default:
			foreach(lc, drop_stmt->objects)
			{
				if (IsA(lfirst(lc), List))
				{
					foreach(lc2, lfirst(lc))
					{
						deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_IDENTIFIER);
						if (lnext(lfirst(lc), lc2))
							appendStringInfoChar(str, '.');
					}
				}
				else
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
				}
				if (lnext(drop_stmt->objects, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ' ');
	}

	deparseOptDropBehavior(str, drop_stmt->behavior);

	removeTrailingSpace(str);
}

static void deparseGroupingSet(StringInfo str, GroupingSet *grouping_set)
{
	ListCell *lc = NULL;

	switch(grouping_set->kind)
	{
		case GROUPING_SET_EMPTY:
			// We just render empty parens in this case
			break;
		case GROUPING_SET_SIMPLE:
			// Not present in raw parse trees
			Assert(false);
			break;
		case GROUPING_SET_ROLLUP:
			appendStringInfoString(str, "ROLLUP ");
			break;
		case GROUPING_SET_CUBE:
			appendStringInfoString(str, "CUBE ");
			break;
		case GROUPING_SET_SETS:
			appendStringInfoString(str, "GROUPING SETS ");
			break;
	}

	appendStringInfoChar(str, '(');
	foreach(lc, grouping_set->content)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(grouping_set->content, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseObjectWithArgs(StringInfo str, ObjectWithArgs *object_with_args, bool parens_for_empty_list)
{
	ListCell *lc;

	foreach(lc, object_with_args->objname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(object_with_args->objname, lc))
			appendStringInfoChar(str, '.');
	}

	if (!object_with_args->args_unspecified && (parens_for_empty_list || list_length(object_with_args->objargs) > 0))
	{
		appendStringInfoChar(str, '(');
		foreach(lc, object_with_args->objargs)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(object_with_args->objargs, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
}

static void deparseDropTableSpaceStmt(StringInfo str, DropTableSpaceStmt *drop_table_space_stmt)
{
	appendStringInfoString(str, "DROP TABLESPACE ");

	if (drop_table_space_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	appendStringInfoString(str, drop_table_space_stmt->tablespacename);
}

static void deparseAlterTableStmt(StringInfo str, AlterTableStmt *alter_table_stmt)
{
	ListCell *lc;
	DeparseNodeContext context = DEPARSE_NODE_CONTEXT_NONE;

	appendStringInfoString(str, "ALTER ");

	switch (alter_table_stmt->relkind)
	{
		case OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			break;
		case OBJECT_FOREIGN_TABLE:
			appendStringInfoString(str, "FOREIGN TABLE ");
			break;
		case OBJECT_INDEX:
			appendStringInfoString(str, "INDEX ");
			break;
		case OBJECT_SEQUENCE:
			appendStringInfoString(str, "SEQUENCE ");
			break;
		case OBJECT_VIEW:
			appendStringInfoString(str, "VIEW ");
			break;
		case OBJECT_MATVIEW:
			appendStringInfoString(str, "MATERIALIZED VIEW ");
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			context = DEPARSE_NODE_CONTEXT_ALTER_TYPE;
			break;
		default:
			Assert(false);
			break;
	}

	deparseRangeVar(str, alter_table_stmt->relation, context);
	appendStringInfoChar(str, ' ');

	foreach(lc, alter_table_stmt->cmds)
	{
		deparseNode(str, lfirst(lc), context);
		if (lnext(alter_table_stmt->cmds, lc))
			appendStringInfoString(str, ", ");
	}
}

static void deparseAlterTableCmd(StringInfo str, AlterTableCmd *alter_table_cmd, DeparseNodeContext context)
{
	ListCell *lc = NULL;
	const char *options = NULL;

	switch (alter_table_cmd->subtype)
	{
		case AT_AddColumn: /* add column */
			if (context == DEPARSE_NODE_CONTEXT_ALTER_TYPE)
				appendStringInfoString(str, "ADD ATTRIBUTE ");
			else
				appendStringInfoString(str, "ADD COLUMN ");
			break;
		case AT_AddColumnRecurse: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AddColumnToView: /* implicitly via CREATE OR REPLACE VIEW */
			// TODO
			Assert(false);
			break;
		case AT_ColumnDefault: /* alter column default */
			appendStringInfoString(str, "ALTER COLUMN ");
			if (alter_table_cmd->def != NULL)
				options = "SET DEFAULT";
			else
				options = "DROP DEFAULT";
			break;
		case AT_CookedColumnDefault: /* add a pre-cooked column default */
			// TODO
			Assert(false);
			break;
		case AT_DropNotNull: /* alter column drop not null */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "DROP NOT NULL";
			break;
		case AT_SetNotNull: /* alter column set not null */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "SET NOT NULL";
			break;
		case AT_DropExpression: /* alter column drop expression */
			// TODO
			Assert(false);
			break;
		case AT_CheckNotNull: /* check column is already marked not null */
			// TODO
			Assert(false);
			break;
		case AT_SetStatistics: /* alter column set statistics */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "SET STATISTICS";
			break;
		case AT_SetOptions: /* alter column set ( options ) */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "SET";
			break;
		case AT_ResetOptions: /* alter column reset ( options ) */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "RESET";
			break;
		case AT_SetStorage: /* alter column set storage */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "SET STORAGE";
			break;
		case AT_DropColumn: /* drop column */
			appendStringInfoString(str, "DROP ");
			break;
		case AT_DropColumnRecurse: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AddIndex: /* add index */
			appendStringInfoString(str, "ADD INDEX ");
			break;
		case AT_ReAddIndex: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AddConstraint: /* add constraint */
			appendStringInfoString(str, "ADD ");
			break;
		case AT_AddConstraintRecurse: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_ReAddConstraint: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_ReAddDomainConstraint: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AlterConstraint: /* alter constraint */
			appendStringInfoString(str, "ALTER CONSTRAINT ");
			break;
		case AT_ValidateConstraint: /* validate constraint */
			appendStringInfoString(str, "VALIDATE CONSTRAINT ");
			break;
		case AT_ValidateConstraintRecurse: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AddIndexConstraint: /* add constraint using existing index */
			// TODO
			Assert(false);
			break;
		case AT_DropConstraint: /* drop constraint */
			appendStringInfoString(str, "DROP CONSTRAINT ");
			break;
		case AT_DropConstraintRecurse: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_ReAddComment: /* internal to commands/tablecmds.c */
			Assert(false);
			break;
		case AT_AlterColumnType: /* alter column type */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "TYPE";
			break;
		case AT_AlterColumnGenericOptions: /* alter column OPTIONS (...) */
			appendStringInfoString(str, "ALTER COLUMN ");
			options = "OPTIONS";
			break;
		case AT_ChangeOwner: /* change owner */
			appendStringInfoString(str, "OWNER TO ");
			deparseRoleSpec(str, alter_table_cmd->newowner);
			break;
		case AT_ClusterOn: /* CLUSTER ON */
			appendStringInfoString(str, "CLUSTER ON ");
			break;
		case AT_DropCluster: /* SET WITHOUT CLUSTER */
			appendStringInfoString(str, "SET WITHOUT CLUSTER ");
			break;
		case AT_SetLogged: /* SET LOGGED */
			// TODO
			Assert(false);
			break;
		case AT_SetUnLogged: /* SET UNLOGGED */
			// TODO
			Assert(false);
			break;
		case AT_DropOids: /* SET WITHOUT OIDS */
			// TODO
			Assert(false);
			break;
		case AT_SetTableSpace: /* SET TABLESPACE */
			// TODO
			Assert(false);
			break;
		case AT_SetRelOptions: /* SET (...) -- AM specific parameters */
			appendStringInfoString(str, "SET ");
			break;
		case AT_ResetRelOptions: /* RESET (...) -- AM specific parameters */
			appendStringInfoString(str, "RESET ");
			break;
		case AT_ReplaceRelOptions: /* replace reloption list in its entirety */
			// TODO
			Assert(false);
			break;
		case AT_EnableTrig: /* ENABLE TRIGGER name */
			appendStringInfoString(str, "ENABLE TRIGGER ");
			break;
		case AT_EnableAlwaysTrig: /* ENABLE ALWAYS TRIGGER name */
			appendStringInfoString(str, "ENABLE ALWAYS TRIGGER ");
			break;
		case AT_EnableReplicaTrig: /* ENABLE REPLICA TRIGGER name */
			appendStringInfoString(str, "ENABLE REPLICA TRIGGER ");
			break;
		case AT_DisableTrig: /* DISABLE TRIGGER name */
			appendStringInfoString(str, "DISABLE TRIGGER ");
			break;
		case AT_EnableTrigAll: /* ENABLE TRIGGER ALL */
			appendStringInfoString(str, "ENABLE TRIGGER ");
			break;
		case AT_DisableTrigAll: /* DISABLE TRIGGER ALL */
			appendStringInfoString(str, "DISABLE TRIGGER ALL ");
			break;
		case AT_EnableTrigUser: /* ENABLE TRIGGER USER */
			appendStringInfoString(str, "ENABLE TRIGGER USER ");
			break;
		case AT_DisableTrigUser: /* DISABLE TRIGGER USER */
			appendStringInfoString(str, "DISABLE TRIGGER USER ");
			break;
		case AT_EnableRule: /* ENABLE RULE name */
			appendStringInfoString(str, "ENABLE RULE ");
			break;
		case AT_EnableAlwaysRule: /* ENABLE ALWAYS RULE name */
			appendStringInfoString(str, "ENABLE ALWAYS RULE ");
			break;
		case AT_EnableReplicaRule: /* ENABLE REPLICA RULE name */
			appendStringInfoString(str, "ENABLE REPLICA RULE ");
			break;
		case AT_DisableRule: /* DISABLE RULE name */
			appendStringInfoString(str, "DISABLE RULE ");
			break;
		case AT_AddInherit: /* INHERIT parent */
			appendStringInfoString(str, "INHERIT ");
			break;
		case AT_DropInherit: /* NO INHERIT parent */
			appendStringInfoString(str, "NO INHERIT ");
			break;
		case AT_AddOf: /* OF <type_name> */
			// TODO
			Assert(false);
			break;
		case AT_DropOf: /* NOT OF */
			// TODO
			Assert(false);
			break;
		case AT_ReplicaIdentity: /* REPLICA IDENTITY */
			appendStringInfoString(str, "REPLICA IDENTITY ");
			break;
		case AT_EnableRowSecurity: /* ENABLE ROW SECURITY */
			appendStringInfoString(str, "ENABLE ROW LEVEL SECURITY ");
			break;
		case AT_DisableRowSecurity: /* DISABLE ROW SECURITY */
			appendStringInfoString(str, "DISABLE ROW LEVEL SECURITY ");
			break;
		case AT_ForceRowSecurity: /* FORCE ROW SECURITY */
			appendStringInfoString(str, "FORCE ROW LEVEL SECURITY ");
			break;
		case AT_NoForceRowSecurity: /* NO FORCE ROW SECURITY */
			appendStringInfoString(str, "NO FORCE ROW LEVEL SECURITY ");
			break;
		case AT_GenericOptions: /* OPTIONS (...) */
			// TODO
			Assert(false);
			break;
		case AT_AttachPartition: /* ATTACH PARTITION */
			appendStringInfoString(str, "ATTACH PARTITION ");
			break;
		case AT_DetachPartition: /* DETACH PARTITION */
			appendStringInfoString(str, "DETACH PARTITION ");
			break;
		case AT_AddIdentity: /* ADD IDENTITY */
			// TODO
			Assert(false);
			break;
		case AT_SetIdentity: /* SET identity column options */
			// TODO
			Assert(false);
			break;
		case AT_DropIdentity: /* DROP IDENTITY */
			// TODO
			Assert(false);
			break;
	}

	if (alter_table_cmd->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	if (alter_table_cmd->name != NULL)
	{
		appendStringInfoString(str, quote_identifier(alter_table_cmd->name));
		appendStringInfoChar(str, ' ');
	}

	if (options != NULL)
	{
		appendStringInfoString(str, options);
		appendStringInfoChar(str, ' ');
	}

	if (alter_table_cmd->def != NULL)
	{
		if (IsA(alter_table_cmd->def, List))
		{
			List *l = castNode(List, alter_table_cmd->def);
			appendStringInfoChar(str, '(');
			foreach(lc, l)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(l, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
		}
		else
		{
			deparseNode(str, alter_table_cmd->def, DEPARSE_NODE_CONTEXT_NONE);
		}
		appendStringInfoChar(str, ' ');
	}

	deparseOptDropBehavior(str, alter_table_cmd->behavior);

	removeTrailingSpace(str);
}

static void deparseRenameStmt(StringInfo str, RenameStmt *rename_stmt)
{
	appendStringInfoString(str, "ALTER ");

	switch (rename_stmt->renameType)
	{
		case OBJECT_CONVERSION:
			appendStringInfoString(str, "CONVERSION ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_TABCONSTRAINT:
			appendStringInfoString(str, "TABLE ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME CONSTRAINT ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoChar(str, ' ');
			break;
		case OBJECT_INDEX:
			appendStringInfoString(str, "INDEX ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_MATVIEW:
			appendStringInfoString(str, "MATERIALIZED VIEW ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_TABLESPACE:
			appendStringInfoString(str, "TABLESPACE ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_VIEW:
			appendStringInfoString(str, "VIEW ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_COLUMN:
			appendStringInfoString(str, "TABLE ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME COLUMN ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoChar(str, ' ');
			break;
		case OBJECT_COLLATION:
			appendStringInfoString(str, "COLLATION ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_DOMCONSTRAINT:
			appendStringInfoString(str, "DOMAIN ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME CONSTRAINT ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoChar(str, ' ');
			break;
		case OBJECT_RULE:
			appendStringInfoString(str, "RULE ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoString(str, " ON ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_TRIGGER:
			appendStringInfoString(str, "TRIGGER ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoString(str, " ON ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_AGGREGATE:
			appendStringInfoString(str, "AGGREGATE ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_SUBSCRIPTION:
			appendStringInfoString(str, "SUBSCRIPTION ");
			deparseNode(str, rename_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_POLICY:
			appendStringInfoString(str, "POLICY ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoString(str, " ON ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " RENAME ");
			break;
		case OBJECT_ATTRIBUTE:
			appendStringInfoString(str, "ALTER TYPE ");
			deparseRangeVar(str, rename_stmt->relation, DEPARSE_NODE_CONTEXT_ALTER_TYPE);
			appendStringInfoString(str, " RENAME ATTRIBUTE ");
			appendStringInfoString(str, quote_identifier(rename_stmt->subname));
			appendStringInfoChar(str, ' ');
		default:
			// TODO: Add additional object types
			Assert(false);
			break;
	}

	appendStringInfoString(str, "TO ");
	appendStringInfoString(str, quote_identifier(rename_stmt->newname));
	appendStringInfoChar(str, ' ');

	deparseOptDropBehavior(str, rename_stmt->behavior);

	removeTrailingSpace(str);
}

static void deparseTransactionStmt(StringInfo str, TransactionStmt *transaction_stmt)
{
	ListCell *lc;
	switch (transaction_stmt->kind)
	{
		case TRANS_STMT_BEGIN:
			appendStringInfoString(str, "BEGIN ");
			foreach(lc, transaction_stmt->options) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(transaction_stmt->options, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case TRANS_STMT_START:
			appendStringInfoString(str, "START TRANSACTION ");
			foreach(lc, transaction_stmt->options) {
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(transaction_stmt->options, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case TRANS_STMT_COMMIT:
			appendStringInfoString(str, "COMMIT ");
			if (transaction_stmt->chain)
				appendStringInfoString(str, "AND CHAIN ");
			break;
		case TRANS_STMT_ROLLBACK:
			appendStringInfoString(str, "ROLLBACK ");
			if (transaction_stmt->chain)
				appendStringInfoString(str, "AND CHAIN ");
			break;
		case TRANS_STMT_SAVEPOINT:
			appendStringInfoString(str, "SAVEPOINT ");
			appendStringInfoString(str, quote_identifier(transaction_stmt->savepoint_name));
			break;
		case TRANS_STMT_RELEASE:
			appendStringInfoString(str, "RELEASE ");
			appendStringInfoString(str, quote_identifier(transaction_stmt->savepoint_name));
			break;
		case TRANS_STMT_ROLLBACK_TO:
			appendStringInfoString(str, "ROLLBACK ");
			appendStringInfoString(str, "TO SAVEPOINT ");
			appendStringInfoString(str, quote_identifier(transaction_stmt->savepoint_name));
			break;
		case TRANS_STMT_PREPARE:
			appendStringInfoString(str, "PREPARE TRANSACTION ");
			deparseStringLiteral(str, transaction_stmt->gid);
			break;
		case TRANS_STMT_COMMIT_PREPARED:
			appendStringInfoString(str, "COMMIT PREPARED ");
			deparseStringLiteral(str, transaction_stmt->gid);
			break;
		case TRANS_STMT_ROLLBACK_PREPARED:
			appendStringInfoString(str, "ROLLBACK PREPARED ");
			deparseStringLiteral(str, transaction_stmt->gid);
			break;
	}

	removeTrailingSpace(str);
}

static void deparseVariableSetStmt(StringInfo str, VariableSetStmt* variable_set_stmt)
{
	ListCell *lc;

	switch (variable_set_stmt->kind)
	{
		case VAR_SET_VALUE: /* SET var = value */
			appendStringInfoString(str, "SET ");
			if (variable_set_stmt->is_local)
				appendStringInfoString(str, "LOCAL ");
			appendStringInfoString(str, quote_identifier(variable_set_stmt->name));
			appendStringInfoString(str, " TO ");
			foreach(lc, variable_set_stmt->args)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(variable_set_stmt->args, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case VAR_SET_DEFAULT: /* SET var TO DEFAULT */
			appendStringInfoString(str, "SET ");
			if (variable_set_stmt->is_local)
				appendStringInfoString(str, "LOCAL ");
			appendStringInfoString(str, quote_identifier(variable_set_stmt->name));
			appendStringInfoString(str, " TO DEFAULT");
			break;
		case VAR_SET_CURRENT: /* SET var FROM CURRENT */
			appendStringInfoString(str, "SET ");
			if (variable_set_stmt->is_local)
				appendStringInfoString(str, "LOCAL ");
			appendStringInfoString(str, quote_identifier(variable_set_stmt->name));
			appendStringInfoString(str, " FROM CURRENT");
			break;
		case VAR_SET_MULTI: /* special case for SET TRANSACTION ... */
			Assert(variable_set_stmt->name != NULL);
			appendStringInfoString(str, "SET ");
			if (variable_set_stmt->is_local)
				appendStringInfoString(str, "LOCAL ");
			if (strcmp(variable_set_stmt->name, "TRANSACTION") == 0)
			{
				appendStringInfoString(str, "TRANSACTION ");
			}
			else if (strcmp(variable_set_stmt->name, "SESSION CHARACTERISTICS") == 0)
			{
				appendStringInfoString(str, "SESSION CHARACTERISTICS AS TRANSACTION ");
			}
			else if (strcmp(variable_set_stmt->name, "TRANSACTION SNAPSHOT") == 0)
			{
				appendStringInfoString(str, "TRANSACTION SNAPSHOT ");
			}
			else
			{
				Assert(false);
			}
			foreach(lc, variable_set_stmt->args)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(variable_set_stmt->args, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case VAR_RESET: /* RESET var */
			appendStringInfoString(str, "RESET ");
			appendStringInfoString(str, quote_identifier(variable_set_stmt->name));
			break;
		case VAR_RESET_ALL: /* RESET ALL */
			appendStringInfoString(str, "RESET ALL");
			break;
	}
}

static void deparseVacuumStmt(StringInfo str, VacuumStmt* vacuum_stmt)
{
	ListCell *lc = NULL;
	ListCell *lc2 = NULL;

	if (vacuum_stmt->is_vacuumcmd)
		appendStringInfoString(str, "VACUUM ");
	else
		appendStringInfoString(str, "ANALYZE ");

	if (list_length(vacuum_stmt->options) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, vacuum_stmt->options)
		{
			DefElem *def_elem = castNode(DefElem, lfirst(lc));
			char *defname = pstrdup(def_elem->defname);
			for (unsigned char *p = (unsigned char *) defname; *p; p++)
				*p = pg_toupper(*p);
			appendStringInfoString(str, defname);
			if (def_elem->arg != NULL)
			{
				appendStringInfoChar(str, ' ');
				deparseNode(str, def_elem->arg, DEPARSE_NODE_CONTEXT_NONE);
			}
			if (lnext(vacuum_stmt->options, lc))
				appendStringInfoString(str, ", ");
			pfree(defname);
		}
		appendStringInfoString(str, ") ");
	}

	foreach(lc, vacuum_stmt->rels)
	{
		Assert(IsA(lfirst(lc), VacuumRelation));
		VacuumRelation *rel = castNode(VacuumRelation, lfirst(lc));

		deparseRangeVar(str, rel->relation, DEPARSE_NODE_CONTEXT_NONE);
		if (list_length(rel->va_cols) > 0)
		{
			appendStringInfoChar(str, '(');
			foreach(lc2, rel->va_cols)
			{
				deparseNode(str, lfirst(lc2), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(rel->va_cols, lc2))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
		}

		if (lnext(vacuum_stmt->rels, lc))
			appendStringInfoString(str, ", ");
	}

	removeTrailingSpace(str);
}

static void deparseLockStmt(StringInfo str, LockStmt *lock_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "LOCK TABLE ");

	foreach(lc, lock_stmt->relations)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(lock_stmt->relations, lc))
			appendStringInfoString(str, ", ");
	}

	// TODO: mode
	// TODO: nowait
}

static void deparseConstraintsSetStmt(StringInfo str, ConstraintsSetStmt *constraints_set_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "SET CONSTRAINTS ");

	if (list_length(constraints_set_stmt->constraints) > 0)
	{
		foreach(lc, constraints_set_stmt->constraints)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(constraints_set_stmt->constraints, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}
	else
	{
		appendStringInfoString(str, "ALL ");
	}

	if (constraints_set_stmt->deferred)
		appendStringInfoString(str, "DEFERRED");
	else
		appendStringInfoString(str, "IMMEDIATE");
}

static void deparseExplainStmt(StringInfo str, ExplainStmt *explain_stmt)
{
	ListCell *lc = NULL;
	char *defname = NULL;

	appendStringInfoString(str, "EXPLAIN ");

	if (list_length(explain_stmt->options) > 0)
	{
		appendStringInfoChar(str, '(');

		foreach(lc, explain_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_EXPLAIN);
			if (lnext(explain_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	deparseNode(str, explain_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseCopyStmt(StringInfo str, CopyStmt *copy_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "COPY ");

	if (copy_stmt->relation != NULL)
	{
		deparseRangeVar(str, copy_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
		if (list_length(copy_stmt->attlist) > 0)
		{
			appendStringInfoChar(str, '(');
			foreach(lc, copy_stmt->attlist)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(copy_stmt->attlist, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
		}
		appendStringInfoChar(str, ' ');
	}

	if (copy_stmt->query != NULL)
	{
		appendStringInfoChar(str, '(');
		deparseNode(str, copy_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (copy_stmt->is_from)
		appendStringInfoString(str, "FROM ");
	else
		appendStringInfoString(str, "TO ");

	if (copy_stmt->is_program)
		appendStringInfoString(str, "PROGRAM ");

	if (copy_stmt->filename != NULL)
	{
		deparseStringLiteral(str, copy_stmt->filename);
		appendStringInfoChar(str, ' ');
	}
	else
	{
		if (copy_stmt->is_from)
			appendStringInfoString(str, "STDIN ");
		else
			appendStringInfoString(str, "STDOUT ");
	}

	if (list_length(copy_stmt->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach(lc, copy_stmt->options) {
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_COPY);
			if (lnext(copy_stmt->options, lc))
				appendStringInfoString(str, " ");
		}
		appendStringInfoChar(str, ')');
	}

	removeTrailingSpace(str);
}

static void deparseDoStmt(StringInfo str, DoStmt *do_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "DO ");

	foreach (lc, do_stmt->args)
	{
		DefElem *defel = castNode(DefElem, lfirst(lc));
		if (strcmp(defel->defname, "language") == 0)
		{
			appendStringInfoString(str, "LANGUAGE ");
			appendStringInfoString(str, quote_identifier(strVal(defel->arg)));
			appendStringInfoChar(str, ' ');
		}
		else if (strcmp(defel->defname, "as") == 0)
		{
			appendStringInfoString(str, "$$");
			appendStringInfoString(str, strVal(defel->arg));
			appendStringInfoString(str, "$$");
			appendStringInfoChar(str, ' ');
		}
	}

	removeTrailingSpace(str);
}

static void deparseDiscardStmt(StringInfo str, DiscardStmt *discard_stmt)
{
	appendStringInfoString(str, "DISCARD ");
	switch (discard_stmt->target)
	{
		case DISCARD_ALL:
			appendStringInfoString(str, "ALL");
			break;
		case DISCARD_PLANS:
			appendStringInfoString(str, "PLANS");
			break;
		case DISCARD_SEQUENCES:
			appendStringInfoString(str, "SEQUENCES");
			break;
		case DISCARD_TEMP:
			appendStringInfoString(str, "TEMP");
			break;
	}
}

static void deparseDefineStmt(StringInfo str, DefineStmt *define_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	// TODO: if_not_exists
	// TODO: replace

	switch (define_stmt->kind)
	{
		case OBJECT_AGGREGATE:
			appendStringInfoString(str, "AGGREGATE ");
			break;
		case OBJECT_OPERATOR:
			appendStringInfoString(str, "OPERATOR ");
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			break;
		case OBJECT_TSPARSER:
			appendStringInfoString(str, "TEXT SEARCH PARSER ");
			break;
		case OBJECT_TSDICTIONARY:
			appendStringInfoString(str, "TEXT SEARCH DICTIONARY ");
			break;
		case OBJECT_TSTEMPLATE:
			appendStringInfoString(str, "TEXT SEARCH TEMPLATE ");
			break;
		case OBJECT_TSCONFIGURATION:
			appendStringInfoString(str, "TEXT SEARCH CONFIGURATION ");
			break;
		case OBJECT_COLLATION:
			appendStringInfoString(str, "COLLATION ");
			break;
		default:
			// This shouldn't happen
			Assert(false);
			break;
	}

	foreach(lc, define_stmt->defnames)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(define_stmt->defnames, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, ' ');

	/*
	 * Per gram.y for aggr_args:
	 *
	 * (*)									- normal agg with no args
	 * (aggr_arg,...)						- normal agg with args
	 * (ORDER BY aggr_arg,...)				- ordered-set agg with no direct args
	 * (aggr_arg,... ORDER BY aggr_arg,...)	- ordered-set agg with direct args
	 *
	 * ...
	 *
	 * The return value of this production is a two-element list, in which the
	 * first item is a sublist of FunctionParameter nodes (with any duplicate
	 * VARIADIC item already dropped, as per above) and the second is an integer
	 * Value node, containing -1 if there was no ORDER BY and otherwise the number
	 * of argument declarations before the ORDER BY.
	 */
	if (define_stmt->kind == OBJECT_AGGREGATE)
	{
		Assert(list_length(define_stmt->args) == 2);
		int order_by_idx = intVal(lsecond(define_stmt->args));
		appendStringInfoChar(str, '(');
		if (list_length(linitial(define_stmt->args)) > 0)
		{
			foreach(lc, linitial(define_stmt->args))
			{
				if (foreach_current_index(lc) == order_by_idx)
					appendStringInfoString(str, "ORDER BY ");

				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);

				if (foreach_current_index(lc) == order_by_idx - 1)
					appendStringInfoChar(str, ' ');
				else if (lnext(linitial(define_stmt->args), lc))
					appendStringInfoString(str, ", ");
			}
		}
		else
		{
			appendStringInfoChar(str, '*');
		}
		appendStringInfoString(str, ") ");
	}

	if (list_length(define_stmt->definition) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, define_stmt->definition)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(define_stmt->definition, lc))
					appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	removeTrailingSpace(str);
}

static void deparseCompositeTypeStmt(StringInfo str, CompositeTypeStmt *composite_type_stmt)
{
	ListCell *lc;
	RangeVar *typevar;

	appendStringInfoString(str, "CREATE TYPE ");
	deparseRangeVar(str, composite_type_stmt->typevar, DEPARSE_NODE_CONTEXT_CREATE_TYPE);

	appendStringInfoString(str, " AS (");
	foreach(lc, composite_type_stmt->coldeflist)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(composite_type_stmt->coldeflist, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseCreateEnumStmt(StringInfo str, CreateEnumStmt *create_enum_stmt)
{
	ListCell *lc;
	appendStringInfoString(str, "CREATE TYPE ");

	foreach(lc, create_enum_stmt->typeName)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_enum_stmt->typeName, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoString(str, " AS ENUM (");
	foreach(lc, create_enum_stmt->vals)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_CONSTANT);
		if (lnext(create_enum_stmt->vals, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseCreateRangeStmt(StringInfo str, CreateRangeStmt *create_range_stmt)
{
	ListCell *lc;
	appendStringInfoString(str, "CREATE TYPE ");

	foreach(lc, create_range_stmt->typeName)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_range_stmt->typeName, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoString(str, " AS RANGE (");
	foreach(lc, create_range_stmt->params)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_CONSTANT);
		if (lnext(create_range_stmt->params, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseGrantStmt(StringInfo str, GrantStmt *grant_stmt)
{
	ListCell *lc;

	if (grant_stmt->is_grant)
		appendStringInfoString(str, "GRANT ");
	else
		appendStringInfoString(str, "REVOKE ");

	if (list_length(grant_stmt->privileges) > 0)
	{
		foreach(lc, grant_stmt->privileges)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_CONSTANT);
			if (lnext(grant_stmt->privileges, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}
	else
	{
		appendStringInfoString(str, "ALL ");
	}
	
	appendStringInfoString(str, "ON ");

	switch (grant_stmt->targtype)
	{
		case ACL_TARGET_OBJECT:
			switch (grant_stmt->objtype)
			{
				case OBJECT_TABLE:
					// Default object type
					break;
				case OBJECT_SEQUENCE:
					appendStringInfoString(str, "SEQUENCE ");
					break;
				case OBJECT_FDW:
					appendStringInfoString(str, "FOREIGN DATA WRAPPER ");
					break;
				case OBJECT_FOREIGN_SERVER:
					appendStringInfoString(str, "FOREIGN SERVER ");
					break;
				case OBJECT_FUNCTION:
					appendStringInfoString(str, "FUNCTION ");
					break;
				case OBJECT_PROCEDURE:
					appendStringInfoString(str, "PROCEDURE ");
					break;
				case OBJECT_ROUTINE:
					appendStringInfoString(str, "ROUTINE ");
					break;
				case OBJECT_DATABASE:
					appendStringInfoString(str, "DATABASE ");
					break;
				case OBJECT_DOMAIN:
					appendStringInfoString(str, "DOMAIN ");
					break;
				case OBJECT_LANGUAGE:
					appendStringInfoString(str, "LANGUAGE ");
					break;
				case OBJECT_LARGEOBJECT:
					appendStringInfoString(str, "LARGE OBJECT ");
					break;
				case OBJECT_SCHEMA:
					appendStringInfoString(str, "SCHEMA ");
					break;
				case OBJECT_TABLESPACE:
					appendStringInfoString(str, "TABLESPACE ");
					break;
				case OBJECT_TYPE:
					appendStringInfoString(str, "TYPE ");
					break;
				default:
					// Other types are not supported here
					Assert(false);
					break;
			}
			break;
		case ACL_TARGET_ALL_IN_SCHEMA:
			switch (grant_stmt->objtype)
			{
				case OBJECT_TABLE:
					appendStringInfoString(str, "ALL TABLES IN SCHEMA ");
					break;
				case OBJECT_SEQUENCE:
					appendStringInfoString(str, "ALL SEQUENCES IN SCHEMA ");
					break;
				case OBJECT_FUNCTION:
					appendStringInfoString(str, "ALL FUNCTIONS IN SCHEMA ");
					break;
				case OBJECT_PROCEDURE:
					appendStringInfoString(str, "ALL PROCEDURES IN SCHEMA ");
					break;
				case OBJECT_ROUTINE:
					appendStringInfoString(str, "ALL ROUTINES IN SCHEMA ");
					break;
				default:
					// Other types are not supported here
					Assert(false);
					break;
			}
			break;
		case ACL_TARGET_DEFAULTS:
			switch (grant_stmt->objtype)
			{
				case OBJECT_TABLE:
					appendStringInfoString(str, "TABLES ");
					break;
				case OBJECT_FUNCTION:
					appendStringInfoString(str, "FUNCTIONS ");
					break;
				case OBJECT_SEQUENCE:
					appendStringInfoString(str, "SEQUENCES ");
					break;
				case OBJECT_TYPE:
					appendStringInfoString(str, "TYPES ");
					break;
				case OBJECT_SCHEMA:
					appendStringInfoString(str, "SCHEMAS ");
					break;
				default:
					// Other types are not supported here
					Assert(false);
					break;
			}
			break;
	}
	
	if (list_length(grant_stmt->objects) > 0)
	{
		foreach(lc, grant_stmt->objects)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(grant_stmt->objects, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ' ');
	}

	if (grant_stmt->is_grant)
		appendStringInfoString(str, "TO ");
	else
		appendStringInfoString(str, "FROM ");

	foreach(lc, grant_stmt->grantees)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_CONSTANT);
		if (lnext(grant_stmt->grantees, lc))
			appendStringInfoChar(str, ',');
		appendStringInfoChar(str, ' ');
	}

	if (grant_stmt->grant_option)
		appendStringInfoString(str, "WITH GRANT OPTION ");

	deparseOptDropBehavior(str, grant_stmt->behavior);

	removeTrailingSpace(str);
}

static void deparseAccessPriv(StringInfo str, AccessPriv *access_priv)
{
	ListCell *lc;

	appendStringInfoString(str, access_priv->priv_name);
	appendStringInfoChar(str, ' ');

	if (list_length(access_priv->cols) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach(lc, access_priv->cols)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(access_priv->cols, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}

	removeTrailingSpace(str);
}

static void deparseGrantRoleStmt(StringInfo str, GrantRoleStmt *grant_role_stmt)
{
	ListCell *lc;

	if (grant_role_stmt->is_grant)
		appendStringInfoString(str, "GRANT ");
	else
		appendStringInfoString(str, "REVOKE ");

	foreach(lc, grant_role_stmt->granted_roles)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(grant_role_stmt->granted_roles, lc))
			appendStringInfoChar(str, ',');
		appendStringInfoChar(str, ' ');
	}

	if (grant_role_stmt->is_grant)
		appendStringInfoString(str, "TO ");
	else
		appendStringInfoString(str, "FROM ");

	deparseRoleList(str, grant_role_stmt->grantee_roles);
	appendStringInfoChar(str, ' ');

	if (grant_role_stmt->admin_opt)
		appendStringInfoString(str, "WITH ADMIN OPTION ");

	removeTrailingSpace(str);
}

static void deparseDropRoleStmt(StringInfo str, DropRoleStmt *drop_role_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "DROP ROLE ");

	if (drop_role_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	deparseRoleList(str, drop_role_stmt->roles);
}

static void deparseIndexStmt(StringInfo str, IndexStmt *index_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	if (index_stmt->unique)
		appendStringInfoString(str, "UNIQUE ");

	appendStringInfoString(str, "INDEX ");

	if (index_stmt->concurrent)
		appendStringInfoString(str, "CONCURRENTLY ");

	if (index_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");

	if (index_stmt->idxname != NULL)
	{
		appendStringInfoString(str, index_stmt->idxname);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "ON ");
	deparseRangeVar(str, index_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (index_stmt->accessMethod != NULL)
	{
		appendStringInfoString(str, "USING ");
		appendStringInfoString(str, quote_identifier(index_stmt->accessMethod));
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoChar(str, '(');
	foreach (lc, index_stmt->indexParams)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(index_stmt->indexParams, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoString(str, ") ");

	if (list_length(index_stmt->indexIncludingParams) > 0)
	{
		appendStringInfoString(str, "INCLUDE (");
		foreach (lc, index_stmt->indexIncludingParams)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(index_stmt->indexIncludingParams, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (list_length(index_stmt->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach (lc, index_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(index_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	if (index_stmt->tableSpace != NULL)
	{
		appendStringInfoString(str, "TABLESPACE ");
		appendStringInfoString(str, quote_identifier(index_stmt->tableSpace));
		appendStringInfoChar(str, ' ');
	}

	if (index_stmt->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, index_stmt->whereClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparsePrepareStmt(StringInfo str, PrepareStmt *prepare_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "PREPARE ");
	appendStringInfoString(str, quote_identifier(prepare_stmt->name));
	if (list_length(prepare_stmt->argtypes) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach (lc, prepare_stmt->argtypes)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(prepare_stmt->argtypes, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoChar(str, ')');
	}
	appendStringInfoString(str, " AS ");
	deparseNode(str, prepare_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseExecuteStmt(StringInfo str, ExecuteStmt *execute_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "EXECUTE ");
	appendStringInfoString(str, quote_identifier(execute_stmt->name));
	if (list_length(execute_stmt->params) > 0)
	{
		appendStringInfoChar(str, '(');
		deparseExprList(str, execute_stmt->params);
		appendStringInfoChar(str, ')');
	}
}

static void deparseDeallocateStmt(StringInfo str, DeallocateStmt *deallocate_stmt)
{
	appendStringInfoString(str, "DEALLOCATE ");
	if (deallocate_stmt->name != NULL)
		appendStringInfoString(str, quote_identifier(deallocate_stmt->name));
	else
		appendStringInfoString(str, "ALL");
}

static void deparseCreateRoleStmt(StringInfo str, CreateRoleStmt *create_role_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	switch (create_role_stmt->stmt_type)
	{
		case ROLESTMT_ROLE:
			appendStringInfoString(str, "ROLE ");
			break;
		case ROLESTMT_USER:
			appendStringInfoString(str, "USER ");
			break;
		case ROLESTMT_GROUP:
			appendStringInfoString(str, "GROUP ");
			break;
	}

	appendStringInfoString(str, quote_identifier(create_role_stmt->role));
	appendStringInfoChar(str, ' ');

	if (create_role_stmt->options != NULL)
	{
		appendStringInfoString(str, "WITH ");
		foreach (lc, create_role_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
	}

	removeTrailingSpace(str);
}

static void deparseAlterRoleStmt(StringInfo str, AlterRoleStmt *alter_role_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER ");

	if (list_length(alter_role_stmt->options) == 1 && strcmp(castNode(DefElem, linitial(alter_role_stmt->options))->defname, "rolemembers") == 0)
	{
		appendStringInfoString(str, "GROUP ");
		deparseRoleSpec(str, alter_role_stmt->role);
		appendStringInfoChar(str, ' ');

		if (alter_role_stmt->action == 1)
		{
			appendStringInfoString(str, "ADD USER ");
		}
		else if (alter_role_stmt->action == -1)
		{
			appendStringInfoString(str, "DROP USER ");
		}
		else
		{
			Assert(false);
		}

		List *l = castNode(List, castNode(DefElem, linitial(alter_role_stmt->options))->arg);
		foreach (lc, l)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
			if (lnext(l, lc))
				appendStringInfoString(str, ", ");
		}
	}
	else
	{
		appendStringInfoString(str, "ROLE ");
		deparseRoleSpec(str, alter_role_stmt->role);
		appendStringInfoChar(str, ' ');

		appendStringInfoString(str, "WITH ");
		foreach (lc, alter_role_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
	}

	removeTrailingSpace(str);
}

static void deparseDeclareCursorStmt(StringInfo str, DeclareCursorStmt *declare_cursor_stmt)
{
	appendStringInfoString(str, "DECLARE ");
	appendStringInfoString(str, quote_identifier(declare_cursor_stmt->portalname));
	appendStringInfoChar(str, ' ');

	if (declare_cursor_stmt->options & CURSOR_OPT_BINARY)
		appendStringInfoString(str, "BINARY ");

	if (declare_cursor_stmt->options & CURSOR_OPT_SCROLL)
		appendStringInfoString(str, "SCROLL ");

	if (declare_cursor_stmt->options & CURSOR_OPT_NO_SCROLL)
		appendStringInfoString(str, "NO SCROLL ");

	if (declare_cursor_stmt->options & CURSOR_OPT_INSENSITIVE)
		appendStringInfoString(str, "INSENSITIVE ");

	appendStringInfoString(str, "CURSOR ");

	if (declare_cursor_stmt->options & CURSOR_OPT_HOLD)
		appendStringInfoString(str, "WITH HOLD ");

	appendStringInfoString(str, "FOR ");

	deparseNode(str, declare_cursor_stmt->query, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseFetchStmt(StringInfo str, FetchStmt *fetch_stmt)
{
	if (fetch_stmt->ismove)
		appendStringInfoString(str, "MOVE ");
	else
		appendStringInfoString(str, "FETCH ");

	switch (fetch_stmt->direction)
	{
		case FETCH_FORWARD:
			if (fetch_stmt->howMany == 1)
			{
				// Default
			}
			else if (fetch_stmt->howMany == FETCH_ALL)
			{
				appendStringInfoString(str, "ALL ");
			}
			else
			{
				appendStringInfo(str, "FORWARD %ld ", fetch_stmt->howMany);
			}
			break;
		case FETCH_BACKWARD:
			if (fetch_stmt->howMany == 1)
			{
				appendStringInfoString(str, "PRIOR ");
			}
			else if (fetch_stmt->howMany == FETCH_ALL)
			{
				appendStringInfoString(str, "BACKWARD ALL ");
			}
			else
			{
				appendStringInfo(str, "BACKWARD %ld ", fetch_stmt->howMany);
			}
			break;
		case FETCH_ABSOLUTE:
			if (fetch_stmt->howMany == 1)
			{
				appendStringInfoString(str, "FIRST ");
			}
			else if (fetch_stmt->howMany != -1)
			{
				appendStringInfoString(str, "LAST ");
			}
			else
			{
				appendStringInfo(str, "ABSOLUTE %ld ", fetch_stmt->howMany);
			}
			break;
		case FETCH_RELATIVE:
			appendStringInfo(str, "RELATIVE %ld ", fetch_stmt->howMany);
	}

	appendStringInfoString(str, fetch_stmt->portalname);
}

static void deparseAlterDefaultPrivilegesStmt(StringInfo str, AlterDefaultPrivilegesStmt *alter_default_privileges_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER DEFAULT PRIVILEGES ");

	foreach (lc, alter_default_privileges_stmt->options)
	{
		DefElem *defelem = castNode(DefElem, lfirst(lc));
		if (strcmp(defelem->defname, "schemas") == 0)
		{
			appendStringInfoString(str, "IN SCHEMA ");
			deparseNode(str, defelem->arg, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
		else if (strcmp(defelem->defname, "roles") == 0)
		{
			appendStringInfoString(str, "FOR ROLE ");
			deparseRoleList(str, (List *) defelem->arg);
			appendStringInfoChar(str, ' ');
		}
		else
		{
			// No other DefElems are supported
			Assert(false);
		}
	}

	deparseGrantStmt(str, alter_default_privileges_stmt->action);
}

static void deparseReindexStmt(StringInfo str, ReindexStmt *reindex_stmt)
{
	appendStringInfoString(str, "REINDEX ");

	if (reindex_stmt->options & REINDEXOPT_VERBOSE)
		appendStringInfoString(str, "(VERBOSE) ");

	switch (reindex_stmt->kind)
	{
		case REINDEX_OBJECT_INDEX:
			appendStringInfoString(str, "INDEX ");
			break;
		case REINDEX_OBJECT_TABLE:
			appendStringInfoString(str, "TABLE ");
			break;
		case REINDEX_OBJECT_SCHEMA:
			appendStringInfoString(str, "SCHEMA ");
			break;
		case REINDEX_OBJECT_SYSTEM:
			appendStringInfoString(str, "SYSTEM ");
			break;
		case REINDEX_OBJECT_DATABASE:
			appendStringInfoString(str, "DATABASE ");
			break;
	}

	if (reindex_stmt->concurrent)
		appendStringInfoString(str, "CONCURRENTLY ");

	if (reindex_stmt->relation != NULL)
	{
		deparseRangeVar(str, reindex_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	}
	else if (reindex_stmt->name != NULL)
	{
		appendStringInfoString(str, quote_identifier(reindex_stmt->name));
	}
}

static void deparseRuleStmt(StringInfo str, RuleStmt* rule_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	if (rule_stmt->replace)
		appendStringInfoString(str, "OR REPLACE ");

	appendStringInfoString(str, "RULE ");
	appendStringInfoString(str, quote_identifier(rule_stmt->rulename));
	appendStringInfoString(str, " AS ON ");

	switch (rule_stmt->event)
	{
		case CMD_UNKNOWN:
		case CMD_UTILITY:
		case CMD_NOTHING:
			// Not supported here
			Assert(false);
			break;
		case CMD_SELECT:
			appendStringInfoString(str, "SELECT ");
			break;
		case CMD_UPDATE:
			appendStringInfoString(str, "UPDATE ");
			break;
		case CMD_INSERT:
			appendStringInfoString(str, "INSERT ");
			break;
		case CMD_DELETE:
			appendStringInfoString(str, "DELETE ");
			break;
	}

	appendStringInfoString(str, "TO ");
	deparseRangeVar(str, rule_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (rule_stmt->whereClause != NULL)
	{
		appendStringInfoString(str, "WHERE ");
		deparseNode(str, rule_stmt->whereClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "DO ");

	if (rule_stmt->instead)
		appendStringInfoString(str, "INSTEAD ");

	if (list_length(rule_stmt->actions) == 0)
	{
		appendStringInfoString(str, "NOTHING");
	}
	else if (list_length(rule_stmt->actions) == 1)
	{
		deparseNode(str, linitial(rule_stmt->actions), DEPARSE_NODE_CONTEXT_NONE);
	}
	else
	{
		appendStringInfoChar(str, '(');
		foreach (lc, rule_stmt->actions)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(rule_stmt->actions, lc))
				appendStringInfoString(str, "; ");
		}
		appendStringInfoChar(str, ')');
	}
}

static void deparseNotifyStmt(StringInfo str, NotifyStmt *notify_stmt)
{
	appendStringInfoString(str, "NOTIFY ");
	appendStringInfoString(str, quote_identifier(notify_stmt->conditionname));

	if (notify_stmt->payload != NULL)
	{
		appendStringInfoString(str, ", ");
		deparseStringLiteral(str, notify_stmt->payload);
	}
}

static void deparseCreateSeqStmt(StringInfo str, CreateSeqStmt *create_seq_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE ");

	switch (create_seq_stmt->sequence->relpersistence)
	{
		case RELPERSISTENCE_TEMP:
			appendStringInfoString(str, "TEMPORARY ");
			break;
		case RELPERSISTENCE_UNLOGGED:
			appendStringInfoString(str, "UNLOGGED ");
			break;
		case RELPERSISTENCE_PERMANENT:
			// Default
			break;
	}

	appendStringInfoString(str, "SEQUENCE ");

	if (create_seq_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");

	deparseRangeVar(str, create_seq_stmt->sequence, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	deparseOptSeqOptList(str, create_seq_stmt->options);

	removeTrailingSpace(str);
}

static void deparseAlterFunctionStmt(StringInfo str, AlterFunctionStmt *alter_function_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER ");

	switch (alter_function_stmt->objtype)
	{
		case OBJECT_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			break;
		case OBJECT_PROCEDURE:
			appendStringInfoString(str, "PROCEDURE ");
			break;
		case OBJECT_ROUTINE:
			appendStringInfoString(str, "ROUTINE ");
			break;
		default:
			// Not supported here
			Assert(false);
			break;
	}

	deparseObjectWithArgs(str, alter_function_stmt->func, true);

	foreach (lc, alter_function_stmt->actions)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(alter_function_stmt->actions, lc))
			appendStringInfoChar(str, ' ');
	}
}

static void deparseTruncateStmt(StringInfo str, TruncateStmt *truncate_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "TRUNCATE ");

	foreach (lc, truncate_stmt->relations)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(truncate_stmt->relations, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ' ');

	if (truncate_stmt->restart_seqs)
		appendStringInfoString(str, "RESTART IDENTITY ");

	deparseOptDropBehavior(str, truncate_stmt->behavior);

	removeTrailingSpace(str);
}

static void deparseRefreshMatViewStmt(StringInfo str, RefreshMatViewStmt *refresh_mat_view_stmt)
{
	appendStringInfoString(str, "REFRESH MATERIALIZED VIEW ");

	if (refresh_mat_view_stmt->concurrent)
		appendStringInfoString(str, "CONCURRENTLY ");

	deparseRangeVar(str, refresh_mat_view_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);

	if (refresh_mat_view_stmt->skipData)
		appendStringInfoString(str, "WITH NO DATA");
}

static void deparseReplicaIdentityStmt(StringInfo str, ReplicaIdentityStmt *replica_identity_stmt)
{
	switch (replica_identity_stmt->identity_type)
	{
		case REPLICA_IDENTITY_NOTHING:
			appendStringInfoString(str, "NOTHING ");
			break;
		case REPLICA_IDENTITY_FULL:
			appendStringInfoString(str, "FULL ");
			break;
		case REPLICA_IDENTITY_DEFAULT:
			appendStringInfoString(str, "DEFAULT ");
			break;
		case REPLICA_IDENTITY_INDEX:
			Assert(replica_identity_stmt->name != NULL);
			appendStringInfoString(str, "USING INDEX ");
			appendStringInfoString(str, quote_identifier(replica_identity_stmt->name));
			break;
	}
}

static void deparseCreatePolicyStmt(StringInfo str, CreatePolicyStmt *create_policy_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "CREATE POLICY ");
	appendStringInfoString(str, quote_identifier(create_policy_stmt->policy_name));
	appendStringInfoString(str, " ON ");
	deparseRangeVar(str, create_policy_stmt->table, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (!create_policy_stmt->permissive)
		appendStringInfoString(str, "AS RESTRICTIVE ");

	if (strcmp(create_policy_stmt->cmd_name, "all") == 0)
		Assert(true); // Default
	else if (strcmp(create_policy_stmt->cmd_name, "select") == 0)
		appendStringInfoString(str, "FOR SELECT ");
	else if (strcmp(create_policy_stmt->cmd_name, "insert") == 0)
		appendStringInfoString(str, "FOR INSERT ");
	else if (strcmp(create_policy_stmt->cmd_name, "update") == 0)
		appendStringInfoString(str, "FOR UPDATE ");
	else if (strcmp(create_policy_stmt->cmd_name, "delete") == 0)
		appendStringInfoString(str, "FOR DELETE ");
	else
		Assert(false);

	appendStringInfoString(str, "TO ");
	deparseRoleList(str, create_policy_stmt->roles);
	appendStringInfoChar(str, ' ');

	if (create_policy_stmt->qual != NULL)
	{
		appendStringInfoString(str, "USING (");
		deparseNode(str, create_policy_stmt->qual, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (create_policy_stmt->with_check != NULL)
	{
		appendStringInfoString(str, "WITH CHECK (");
		deparseNode(str, create_policy_stmt->with_check, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}
}

static void deparseAlterPolicyStmt(StringInfo str, AlterPolicyStmt *alter_policy_stmt)
{
	appendStringInfoString(str, "ALTER POLICY ");
	appendStringInfoString(str, quote_identifier(alter_policy_stmt->policy_name));
	appendStringInfoString(str, " ON ");
	deparseRangeVar(str, alter_policy_stmt->table, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (list_length(alter_policy_stmt->roles) > 0)
	{
		appendStringInfoString(str, "TO ");
		deparseRoleList(str, alter_policy_stmt->roles);
		appendStringInfoChar(str, ' ');
	}
	
	if (alter_policy_stmt->qual != NULL)
	{
		appendStringInfoString(str, "USING (");
		deparseNode(str, alter_policy_stmt->qual, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	if (alter_policy_stmt->with_check != NULL)
	{
		appendStringInfoString(str, "WITH CHECK (");
		deparseNode(str, alter_policy_stmt->with_check, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}
}

static void deparseAlterSeqStmt(StringInfo str, AlterSeqStmt *alter_seq_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER SEQUENCE ");

	if (alter_seq_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	deparseRangeVar(str, alter_seq_stmt->sequence, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	deparseSeqOptList(str, alter_seq_stmt->options);

	removeTrailingSpace(str);
}

static void deparseCommentStmt(StringInfo str, CommentStmt *comment_stmt)
{
	ListCell *lc;
	List *l;

	appendStringInfoString(str, "COMMENT ON ");

	switch (comment_stmt->objtype)
	{
		case OBJECT_SEQUENCE:
			appendStringInfoString(str, "SEQUENCE ");
			deparseNode(str, comment_stmt->object, DEPARSE_NODE_CONTEXT_NONE);
			break;
		case OBJECT_SUBSCRIPTION:
			appendStringInfoString(str, "SUBSCRIPTION ");
			deparseNode(str, comment_stmt->object, DEPARSE_NODE_CONTEXT_NONE);
			break;
		case OBJECT_TRIGGER:
			appendStringInfoString(str, "TRIGGER ");
			l = castNode(List, comment_stmt->object);
			deparseNode(str, llast(l), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " ON ");
			foreach (lc, l)
			{
				if (lnext(l, lc))
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (foreach_current_index(lc) < list_length(l) - 2)
						appendStringInfoChar(str, '.');
				}
			}
			break;
		case OBJECT_RULE:
			appendStringInfoString(str, "RULE ");
			l = castNode(List, comment_stmt->object);
			deparseNode(str, llast(l), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " ON ");
			foreach (lc, l)
			{
				if (lnext(l, lc))
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (foreach_current_index(lc) < list_length(l) - 2)
						appendStringInfoChar(str, '.');
				}
			}
			break;
		default:
			// TODO
			Assert(false);
			break;
	}

	appendStringInfoString(str, " IS ");

	if (comment_stmt->comment != NULL)
		deparseStringLiteral(str, comment_stmt->comment);
	else
		appendStringInfoString(str, "NULL");
}

static void deparseCreateStatsStmt(StringInfo str, CreateStatsStmt *create_stats_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE STATISTICS ");

	if (create_stats_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");

	foreach (lc, create_stats_stmt->defnames)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_stats_stmt->defnames, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, ' ');

	if (list_length(create_stats_stmt->stat_types) > 0)
	{
		appendStringInfoChar(str, '(');
		foreach (lc, create_stats_stmt->stat_types)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_stats_stmt->stat_types, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	appendStringInfoString(str, "ON ");
	deparseExprList(str, create_stats_stmt->exprs);

	appendStringInfoString(str, " FROM ");
	foreach (lc, create_stats_stmt->relations)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(create_stats_stmt->relations, lc))
			appendStringInfoString(str, ", ");
	}
}

static void deparseAlterStatsStmt(StringInfo str, AlterStatsStmt *alter_stats_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER STATISTICS ");

	if (alter_stats_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	foreach (lc, alter_stats_stmt->defnames)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(alter_stats_stmt->defnames, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, ' ');

	appendStringInfo(str, "SET STATISTICS %d", alter_stats_stmt->stxstattarget);
}

static void deparseCreateForeignServerStmt(StringInfo str, CreateForeignServerStmt *create_foreign_server_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE SERVER ");
	if (create_foreign_server_stmt->if_not_exists)
		appendStringInfoString(str, "IF NOT EXISTS ");
	appendStringInfoString(str, quote_identifier(create_foreign_server_stmt->servername));
	appendStringInfoChar(str, ' ');

	if (create_foreign_server_stmt->servertype != NULL)
	{
		appendStringInfoString(str, "TYPE ");
		appendStringInfoString(str, create_foreign_server_stmt->servertype);
		appendStringInfoChar(str, ' ');
	}

	if (create_foreign_server_stmt->version != NULL)
	{
		appendStringInfoString(str, "VERSION ");
		appendStringInfoString(str, create_foreign_server_stmt->version);
		appendStringInfoChar(str, ' ');
	}

	appendStringInfoString(str, "FOREIGN DATA WRAPPER ");
	appendStringInfoString(str, quote_identifier(create_foreign_server_stmt->fdwname));
	appendStringInfoChar(str, ' ');

	if (create_foreign_server_stmt->options != NULL)
	{
		appendStringInfoString(str, "OPTIONS (");
		foreach (lc, create_foreign_server_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
		appendStringInfoString(str, ") ");
	}
	removeTrailingSpace(str);
}

static void deparseAlterTSDictionaryStmt(StringInfo str, AlterTSDictionaryStmt *alter_ts_dictionary_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "ALTER TEXT SEARCH DICTIONARY ");

	foreach (lc, alter_ts_dictionary_stmt->dictname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(alter_ts_dictionary_stmt->dictname, lc))
			appendStringInfoChar(str, '.');
	}

	appendStringInfoString(str, " (");
	foreach (lc, alter_ts_dictionary_stmt->options)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(alter_ts_dictionary_stmt->options, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseAlterTSConfigurationStmt(StringInfo str, AlterTSConfigurationStmt *alter_ts_configuration_stmt)
{
	ListCell *lc = NULL;

	appendStringInfoString(str, "ALTER TEXT SEARCH CONFIGURATION ");
	foreach (lc, alter_ts_configuration_stmt->cfgname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(alter_ts_configuration_stmt->cfgname, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, ' ');

	switch (alter_ts_configuration_stmt->kind)
	{
		case ALTER_TSCONFIG_ADD_MAPPING:
			appendStringInfoString(str, "ADD MAPPING FOR ");
			foreach (lc, alter_ts_configuration_stmt->tokentype)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->tokentype, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, " WITH ");
			foreach (lc, alter_ts_configuration_stmt->dicts)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->dicts, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case ALTER_TSCONFIG_ALTER_MAPPING_FOR_TOKEN:
			appendStringInfoString(str, "ALTER MAPPING FOR ");
			foreach (lc, alter_ts_configuration_stmt->tokentype)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->tokentype, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, " WITH ");
			foreach (lc, alter_ts_configuration_stmt->dicts)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->dicts, lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case ALTER_TSCONFIG_REPLACE_DICT:
			appendStringInfoString(str, "ALTER MAPPING REPLACE ");
			foreach (lc, linitial(alter_ts_configuration_stmt->dicts))
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(linitial(alter_ts_configuration_stmt->dicts), lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, " WITH ");
			foreach (lc, lsecond(alter_ts_configuration_stmt->dicts))
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(lsecond(alter_ts_configuration_stmt->dicts), lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case ALTER_TSCONFIG_REPLACE_DICT_FOR_TOKEN:
			appendStringInfoString(str, "ALTER MAPPING FOR ");
			foreach (lc, alter_ts_configuration_stmt->tokentype)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->tokentype, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, " REPLACE ");
			foreach (lc, linitial(alter_ts_configuration_stmt->dicts))
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(linitial(alter_ts_configuration_stmt->dicts), lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoString(str, " WITH ");
			foreach (lc, lsecond(alter_ts_configuration_stmt->dicts))
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(lsecond(alter_ts_configuration_stmt->dicts), lc))
					appendStringInfoString(str, ", ");
			}
			break;
		case ALTER_TSCONFIG_DROP_MAPPING:
			appendStringInfoString(str, "DROP MAPPING ");
			if (alter_ts_configuration_stmt->missing_ok)
				appendStringInfoString(str, "IF EXISTS ");
			appendStringInfoString(str, "FOR ");
			foreach (lc, alter_ts_configuration_stmt->tokentype)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_ts_configuration_stmt->tokentype, lc))
					appendStringInfoString(str, ", ");
			}
			break;
	}
}

static void deparseCreateFdwStmt(StringInfo str, CreateFdwStmt *create_fdw_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE FOREIGN DATA WRAPPER ");
	appendStringInfoString(str, quote_identifier(create_fdw_stmt->fdwname));
	appendStringInfoChar(str, ' ');

	foreach (lc, create_fdw_stmt->func_options)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (create_fdw_stmt->options != NULL)
	{
		appendStringInfoString(str, "OPTIONS (");
		foreach (lc, create_fdw_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
		appendStringInfoString(str, ") ");
	}

	removeTrailingSpace(str);
}

static void deparseVariableShowStmt(StringInfo str, VariableShowStmt *variable_show_stmt)
{
	appendStringInfoString(str, "SHOW ");

	if (strcmp(variable_show_stmt->name, "timezone") == 0)
		appendStringInfoString(str, "TIME ZONE");
	else if (strcmp(variable_show_stmt->name, "transaction_isolation") == 0)
		appendStringInfoString(str, "TRANSACTION ISOLATION LEVEL");
	else if (strcmp(variable_show_stmt->name, "session_authorization") == 0)
		appendStringInfoString(str, "SESSION AUTHORIZATION");
	else if (strcmp(variable_show_stmt->name, "all") == 0)
		appendStringInfoString(str, "SESSION ALL");
	else
		appendStringInfoString(str, variable_show_stmt->name);
}

static void deparseRangeTableSample(StringInfo str, RangeTableSample *range_table_sample)
{
	ListCell *lc;

	deparseNode(str, range_table_sample->relation, DEPARSE_NODE_CONTEXT_NONE);

	appendStringInfoString(str, " TABLESAMPLE ");

	foreach(lc, range_table_sample->method)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(range_table_sample->method, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, '(');
	deparseExprList(str, range_table_sample->args);
	appendStringInfoString(str, ") ");

	if (range_table_sample->repeatable != NULL)
	{
		appendStringInfoString(str, "REPEATABLE (");
		deparseNode(str, range_table_sample->repeatable, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	removeTrailingSpace(str);
}

static void deparseCreateSubscriptionStmt(StringInfo str, CreateSubscriptionStmt *create_subscription_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "CREATE SUBSCRIPTION ");
	appendStringInfoString(str, quote_identifier(create_subscription_stmt->subname));

	appendStringInfoString(str, " CONNECTION ");
	deparseStringLiteral(str, create_subscription_stmt->conninfo);

	appendStringInfoString(str, " PUBLICATION ");

	foreach(lc, create_subscription_stmt->publication)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_subscription_stmt->publication, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ' ');

	if (list_length(create_subscription_stmt->options) > 0)
	{
		appendStringInfoString(str, "WITH (");
		foreach(lc, create_subscription_stmt->options)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(create_subscription_stmt->options, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, ") ");
	}

	removeTrailingSpace(str);
}

static void deparseAlterSubscriptionStmt(StringInfo str, AlterSubscriptionStmt *alter_subscription_stmt)
{
	ListCell *lc;

	appendStringInfoString(str, "ALTER SUBSCRIPTION ");
	appendStringInfoString(str, quote_identifier(alter_subscription_stmt->subname));
	appendStringInfoChar(str, ' ');

	switch (alter_subscription_stmt->kind)
	{
		case ALTER_SUBSCRIPTION_OPTIONS:
			appendStringInfoString(str, "SET (");
			foreach(lc, alter_subscription_stmt->options)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
				if (lnext(alter_subscription_stmt->options, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			break;
		case ALTER_SUBSCRIPTION_CONNECTION:
			appendStringInfoString(str, "CONNECTION ");
			deparseStringLiteral(str, alter_subscription_stmt->conninfo);
			appendStringInfoChar(str, ' ');
			break;
		case ALTER_SUBSCRIPTION_REFRESH:
			appendStringInfoString(str, "REFRESH PUBLICATION ");

			if (list_length(alter_subscription_stmt->options) > 0)
			{
				appendStringInfoString(str, "WITH (");
				foreach(lc, alter_subscription_stmt->options)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(alter_subscription_stmt->options, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ") ");
			}
			break;
		case ALTER_SUBSCRIPTION_PUBLICATION:
			appendStringInfoString(str, "SET PUBLICATION ");
			foreach(lc, alter_subscription_stmt->publication)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
				if (lnext(alter_subscription_stmt->publication, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ' ');

			if (list_length(alter_subscription_stmt->options) > 0)
			{
				appendStringInfoString(str, "WITH (");
				foreach(lc, alter_subscription_stmt->options)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(alter_subscription_stmt->options, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ") ");
			}
			break;
		case ALTER_SUBSCRIPTION_ENABLED:
			Assert(list_length(alter_subscription_stmt->options) == 1);
			DefElem *defelem = castNode(DefElem, linitial(alter_subscription_stmt->options));
			Assert(strcmp(defelem->defname, "enabled") == 0);
			if (intVal(defelem->arg) == 1)
			{
				appendStringInfoString(str, " ENABLE ");
			}
			else if (intVal(defelem->arg) == 0)
			{
				appendStringInfoString(str, " DISABLE ");
			}
			else
			{
				Assert(false);
			}
			break;
	}
	
	removeTrailingSpace(str);
}

static void deparseDropSubscriptionStmt(StringInfo str, DropSubscriptionStmt *drop_subscription_stmt)
{
	appendStringInfoString(str, "DROP SUBSCRIPTION ");

	if (drop_subscription_stmt->missing_ok)
		appendStringInfoString(str, "IF EXISTS ");

	appendStringInfoString(str, drop_subscription_stmt->subname);
}

static void deparseAlterOwnerStmt(StringInfo str, AlterOwnerStmt *alter_owner_stmt)
{
	appendStringInfoString(str, "ALTER ");

	switch (alter_owner_stmt->objectType)
	{
		case OBJECT_AGGREGATE:
			appendStringInfoString(str, "AGGREGATE ");
			break;
		case OBJECT_COLLATION:
			appendStringInfoString(str, "COLLATION ");
			break;
		case OBJECT_CONVERSION:
			appendStringInfoString(str, "CONVERSION ");
			break;
		case OBJECT_DATABASE:
			appendStringInfoString(str, "DATABASE ");
			break;
		case OBJECT_DOMAIN:
			appendStringInfoString(str, "DOMAIN ");
			break;
		case OBJECT_FUNCTION:
			appendStringInfoString(str, "FUNCTION ");
			break;
		case OBJECT_LANGUAGE:
			appendStringInfoString(str, "LANGUAGE ");
			break;
		case OBJECT_LARGEOBJECT:
			appendStringInfoString(str, "LARGE OBJECT ");
			break;
		case OBJECT_OPERATOR:
			appendStringInfoString(str, "OPERATOR ");
			break;
		case OBJECT_OPCLASS:
			appendStringInfoString(str, "OPERATOR CLASS ");
			// TODO:
			/*| ALTER OPERATOR CLASS any_name USING access_method OWNER TO RoleSpec
				{
					AlterOwnerStmt *n = makeNode(AlterOwnerStmt);
					n->objectType = OBJECT_OPCLASS;
					n->object = (Node *) lcons(makeString($6), $4);
					n->newowner = $9;
					$$ = (Node *)n;
				}*/
			break;
		case OBJECT_OPFAMILY:
			appendStringInfoString(str, "OPERATOR FAMILY ");
			// TODO:
			/* | ALTER OPERATOR FAMILY any_name USING access_method OWNER TO RoleSpec
				{
					AlterOwnerStmt *n = makeNode(AlterOwnerStmt);
					n->objectType = OBJECT_OPFAMILY;
					n->object = (Node *) lcons(makeString($6), $4);
					n->newowner = $9;
					$$ = (Node *)n;
				}*/
			break;
		case OBJECT_PROCEDURE:
			appendStringInfoString(str, "PROCEDURE ");
			break;
		case OBJECT_ROUTINE:
			appendStringInfoString(str, "ROUTINE ");
			break;
		case OBJECT_SCHEMA:
			appendStringInfoString(str, "SCHEMA ");
			break;
		case OBJECT_TYPE:
			appendStringInfoString(str, "TYPE ");
			break;
		case OBJECT_TABLESPACE:
			appendStringInfoString(str, "TABLESPACE ");
			break;
		case OBJECT_STATISTIC_EXT:
			appendStringInfoString(str, "STATISTICS ");
			break;
		case OBJECT_TSDICTIONARY:
			appendStringInfoString(str, "TEXT SEARCH DICTIONARY ");
			break;
		case OBJECT_TSCONFIGURATION:
			appendStringInfoString(str, "TEXT SEARCH CONFIGURATION ");
			break;
		case OBJECT_FDW:
			appendStringInfoString(str, "FOREIGN DATA WRAPPER ");
			break;
		case OBJECT_FOREIGN_SERVER:
			appendStringInfoString(str, "SERVER ");
			break;
		case OBJECT_EVENT_TRIGGER:
			appendStringInfoString(str, "EVENT TRIGGER ");
			break;
		case OBJECT_PUBLICATION:
			appendStringInfoString(str, "PUBLICATION ");
			break;
		case OBJECT_SUBSCRIPTION:
			appendStringInfoString(str, "SUBSCRIPTION ");
			break;
		default:
			Assert(false);
	}

	deparseNode(str, alter_owner_stmt->object, DEPARSE_NODE_CONTEXT_IDENTIFIER);

	appendStringInfoString(str, " OWNER TO ");
	deparseRoleSpec(str, alter_owner_stmt->newowner);
}

static void deparseDropOwnedStmt(StringInfo str, DropOwnedStmt *drop_owned_stmt)
{
	appendStringInfoString(str, "DROP OWNED BY ");
	deparseRoleList(str, drop_owned_stmt->roles);
	appendStringInfoChar(str, ' ');
	deparseOptDropBehavior(str, drop_owned_stmt->behavior);
	removeTrailingSpace(str);
}

static void deparseClosePortalStmt(StringInfo str, ClosePortalStmt *close_portal_stmt)
{
	appendStringInfoString(str, "CLOSE ");
	if (close_portal_stmt->portalname != NULL)
	{
		appendStringInfoString(str, quote_identifier(close_portal_stmt->portalname));
	}
	else
	{
		appendStringInfoString(str, "ALL");
	}
}

static void deparseCurrentOfExpr(StringInfo str, CurrentOfExpr *current_of_expr)
{
	appendStringInfoString(str, "CURRENT OF ");
	appendStringInfoString(str, quote_identifier(current_of_expr->cursor_name));
}

static void deparseCreateTrigStmt(StringInfo str, CreateTrigStmt *create_trig_stmt)
{
	ListCell *lc;
	bool skip_events_or = true;

	appendStringInfoString(str, "CREATE ");
	if (create_trig_stmt->isconstraint)
		appendStringInfoString(str, "CONSTRAINT ");
	appendStringInfoString(str, "TRIGGER ");

	appendStringInfoString(str, quote_identifier(create_trig_stmt->trigname));
	appendStringInfoChar(str, ' ');

	switch (create_trig_stmt->timing)
	{
		case TRIGGER_TYPE_BEFORE:
			appendStringInfoString(str, "BEFORE ");
			break;
		case TRIGGER_TYPE_AFTER:
			appendStringInfoString(str, "AFTER ");
			break;
		case TRIGGER_TYPE_INSTEAD:
			appendStringInfoString(str, "INSTEAD OF ");
			break;
		default:
			Assert(false);
	}

	if (TRIGGER_FOR_INSERT(create_trig_stmt->events))
	{
		appendStringInfoString(str, "INSERT ");
		skip_events_or = false;
	}
	if (TRIGGER_FOR_DELETE(create_trig_stmt->events))
	{
		if (!skip_events_or)
			appendStringInfoString(str, "OR ");
		appendStringInfoString(str, "DELETE ");
		skip_events_or = false;
	}
	if (TRIGGER_FOR_UPDATE(create_trig_stmt->events))
	{
		if (!skip_events_or)
			appendStringInfoString(str, "OR ");
		appendStringInfoString(str, "UPDATE ");
		if (list_length(create_trig_stmt->columns) > 0)
		{
			appendStringInfoString(str, "OF ");
			foreach(lc, create_trig_stmt->columns)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
				if (lnext(create_trig_stmt->columns, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ' ');
		}
		skip_events_or = false;
	}
	if (TRIGGER_FOR_TRUNCATE(create_trig_stmt->events))
	{
		if (!skip_events_or)
			appendStringInfoString(str, "OR ");
		appendStringInfoString(str, "TRUNCATE ");
	}

	appendStringInfoString(str, "ON ");
	deparseRangeVar(str, create_trig_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ' ');

	if (create_trig_stmt->transitionRels != NULL)
	{
		appendStringInfoString(str, "REFERENCING ");
		foreach(lc, create_trig_stmt->transitionRels)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}
	}

	if (create_trig_stmt->constrrel != NULL)
	{
		appendStringInfoString(str, "FROM ");
		deparseRangeVar(str, create_trig_stmt->constrrel, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}
	
	if (create_trig_stmt->deferrable)
		appendStringInfoString(str, "DEFERRABLE ");

	if (create_trig_stmt->initdeferred)
		appendStringInfoString(str, "INITIALLY DEFERRED ");

	if (create_trig_stmt->row)
		appendStringInfoString(str, "FOR EACH ROW ");

	if (create_trig_stmt->whenClause)
	{
		appendStringInfoString(str, "WHEN (");
		deparseNode(str, create_trig_stmt->whenClause, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoString(str, ") ");
	}

	appendStringInfoString(str, "EXECUTE FUNCTION ");
	foreach(lc, create_trig_stmt->funcname)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_trig_stmt->funcname, lc))
			appendStringInfoChar(str, '.');
	}
	appendStringInfoChar(str, '(');
	foreach(lc, create_trig_stmt->args)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_IDENTIFIER);
		if (lnext(create_trig_stmt->args, lc))
			appendStringInfoString(str, ", ");
	}
	appendStringInfoChar(str, ')');
}

static void deparseTriggerTransition(StringInfo str, TriggerTransition *trigger_transition)
{
	if (trigger_transition->isNew)
		appendStringInfoString(str, "NEW ");
	else
		appendStringInfoString(str, "OLD ");

	if (trigger_transition->isTable)
		appendStringInfoString(str, "TABLE ");
	else
		appendStringInfoString(str, "ROW ");

	appendStringInfoString(str, quote_identifier(trigger_transition->name));
}

static void deparseXmlExpr(StringInfo str, XmlExpr* xml_expr)
{
	ListCell *lc;
	switch (xml_expr->op)
	{
		case IS_XMLCONCAT: /* XMLCONCAT(args) */
			appendStringInfoString(str, "xmlconcat(");
			deparseExprList(str, xml_expr->args);
			appendStringInfoChar(str, ')');
			break;
		case IS_XMLELEMENT: /* XMLELEMENT(name, xml_attributes, args) */
			appendStringInfoString(str, "xmlelement(name ");
			appendStringInfoString(str, quote_identifier(xml_expr->name));
			if (xml_expr->named_args != NULL)
			{
				appendStringInfoString(str, ", xmlattributes(");
				foreach(lc, xml_expr->named_args)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_XMLATTRIBUTES);
					if (lnext(xml_expr->named_args, lc))
						appendStringInfoString(str, ", ");
				}
				appendStringInfoString(str, ")");
			}
			if (xml_expr->args != NULL)
			{
				appendStringInfoString(str, ", ");
				deparseExprList(str, xml_expr->args);
			}
			appendStringInfoString(str, ")");
			break;
		case IS_XMLFOREST: /* XMLFOREST(xml_attributes) */
			appendStringInfoString(str, "xmlforest(");
			foreach(lc, xml_expr->named_args)
			{
				deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_XMLATTRIBUTES);
				if (lnext(xml_expr->named_args, lc))
					appendStringInfoString(str, ", ");
			}
			appendStringInfoChar(str, ')');
			break;
		case IS_XMLPARSE: /* XMLPARSE(text, is_doc, preserve_ws) */
			Assert(list_length(xml_expr->args) == 2);
			appendStringInfoString(str, "xmlparse(");
			switch (xml_expr->xmloption)
			{
				case XMLOPTION_DOCUMENT:
					appendStringInfoString(str, "document ");
					break;
				case XMLOPTION_CONTENT:
					appendStringInfoString(str, "content ");
					break;
				default:
					Assert(false);
			}
			deparseNode(str, linitial(xml_expr->args), DEPARSE_NODE_CONTEXT_NONE);
			if (strcmp(strVal(&castNode(A_Const, castNode(TypeCast, lsecond(xml_expr->args))->arg)->val), "t") == 0)
				appendStringInfoString(str, " PRESERVE WHITESPACE");
			appendStringInfoChar(str, ')');
			break;
		case IS_XMLPI: /* XMLPI(name [, args]) */
			appendStringInfoString(str, "xmlpi(name ");
			appendStringInfoString(str, quote_identifier(xml_expr->name));
			if (xml_expr->args != NULL)
			{
				appendStringInfoString(str, ", ");
				foreach(lc, xml_expr->args)
				{
					deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
					if (lnext(xml_expr->args, lc))
						appendStringInfoString(str, ", ");
				}
			}
			appendStringInfoChar(str, ')');
			break;
		case IS_XMLROOT: /* XMLROOT(xml, version, standalone) */
			appendStringInfoString(str, "xmlroot(");
			deparseNode(str, linitial(xml_expr->args), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, ", version ");
			if (nodeTag(&castNode(A_Const, lsecond(xml_expr->args))->val) == T_Null)
				appendStringInfoString(str, "NO VALUE");
			else
				deparseNode(str, lsecond(xml_expr->args), DEPARSE_NODE_CONTEXT_NONE);
			if (intVal(&castNode(A_Const, lthird(xml_expr->args))->val) == XML_STANDALONE_YES)
				appendStringInfoString(str, ", STANDALONE YES");
			else if (intVal(&castNode(A_Const, lthird(xml_expr->args))->val) == XML_STANDALONE_NO)
				appendStringInfoString(str, ", STANDALONE NO");
			else if (intVal(&castNode(A_Const, lthird(xml_expr->args))->val) == XML_STANDALONE_NO_VALUE)
				appendStringInfoString(str, ", STANDALONE NO VALUE");
			appendStringInfoChar(str, ')');
			break;
		case IS_XMLSERIALIZE: /* XMLSERIALIZE(is_document, xmlval) */
			// These are represented as XmlSerialize in raw parse trees
			Assert(false);
			break;
		case IS_DOCUMENT: /* xmlval IS DOCUMENT */
			Assert(list_length(xml_expr->args) == 1);
			deparseNode(str, linitial(xml_expr->args), DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoString(str, " IS DOCUMENT");
			break;
	}
}

static void deparseRangeTableFunc(StringInfo str, RangeTableFunc* range_table_func)
{
	ListCell *lc;

	if (range_table_func->lateral)
		appendStringInfoString(str, "LATERAL ");
	
	appendStringInfoString(str, "xmltable(");
	if (range_table_func->namespaces)
	{
		appendStringInfoString(str, "xmlnamespaces(");
		foreach(lc, range_table_func->namespaces)
		{
			deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_XMLNAMESPACES);
			if (lnext(range_table_func->namespaces, lc))
				appendStringInfoString(str, ", ");
		}
		appendStringInfoString(str, "), ");
	}

	appendStringInfoChar(str, '(');
	deparseNode(str, range_table_func->rowexpr, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoChar(str, ')');

	appendStringInfoString(str, " PASSING ");
	deparseNode(str, range_table_func->docexpr, DEPARSE_NODE_CONTEXT_NONE);

	appendStringInfoString(str, " COLUMNS ");
	foreach(lc, range_table_func->columns)
	{
		deparseNode(str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
		if (lnext(range_table_func->columns, lc))
			appendStringInfoString(str, ", ");
	}

	appendStringInfoString(str, ") ");

	if (range_table_func->alias)
	{
		appendStringInfoString(str, "AS ");
		deparseAlias(str, range_table_func->alias);
	}

	removeTrailingSpace(str);
}

static void deparseRangeTableFuncCol(StringInfo str, RangeTableFuncCol* range_table_func_col)
{
	appendStringInfoString(str, quote_identifier(range_table_func_col->colname));
	appendStringInfoChar(str, ' ');

	if (range_table_func_col->for_ordinality)
	{
		appendStringInfoString(str, "FOR ORDINALITY ");
	}
	else
	{
		deparseTypeName(str, range_table_func_col->typeName);
		appendStringInfoChar(str, ' ');

		if (range_table_func_col->colexpr)
		{
			appendStringInfoString(str, "PATH ");
			deparseNode(str, range_table_func_col->colexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}

		if (range_table_func_col->coldefexpr)
		{
			appendStringInfoString(str, "DEFAULT ");
			deparseNode(str, range_table_func_col->coldefexpr, DEPARSE_NODE_CONTEXT_NONE);
			appendStringInfoChar(str, ' ');
		}

		if (range_table_func_col->is_not_null)
			appendStringInfoString(str, "NOT NULL ");
	}

	removeTrailingSpace(str);
}

static void deparseXmlSerialize(StringInfo str, XmlSerialize *xml_serialize)
{
	appendStringInfoString(str, "xmlserialize(");
	switch (xml_serialize->xmloption)
	{
		case XMLOPTION_DOCUMENT:
			appendStringInfoString(str, "document ");
			break;
		case XMLOPTION_CONTENT:
			appendStringInfoString(str, "content ");
			break;
		default:
			Assert(false);
	}
	deparseNode(str, xml_serialize->expr, DEPARSE_NODE_CONTEXT_NONE);
	appendStringInfoString(str, " AS ");
	deparseTypeName(str, xml_serialize->typeName);
	appendStringInfoString(str, ")");
}

static void deparseNamedArgExpr(StringInfo str, NamedArgExpr *named_arg_expr)
{
	appendStringInfoString(str, named_arg_expr->name);
	appendStringInfoString(str, " := ");
	deparseNode(str, (Node *) named_arg_expr->arg, DEPARSE_NODE_CONTEXT_NONE);
}

static void deparseClusterStmt(StringInfo str, ClusterStmt *cluster_stmt)
{
	appendStringInfoString(str, "CLUSTER ");
	if (cluster_stmt->options & CLUOPT_VERBOSE)
		appendStringInfoString(str, "VERBOSE ");

	if (cluster_stmt->relation != NULL)
	{
		deparseRangeVar(str, cluster_stmt->relation, DEPARSE_NODE_CONTEXT_NONE);
		appendStringInfoChar(str, ' ');
	}

	if (cluster_stmt->indexname != NULL)
	{
		appendStringInfoString(str, "USING ");
		appendStringInfoString(str, quote_identifier(cluster_stmt->indexname));
		appendStringInfoChar(str, ' ');
	}

	removeTrailingSpace(str);
}

static void deparseValue(StringInfo str, Value *value, DeparseNodeContext context)
{
	switch (nodeTag(value))
	{
		case T_Integer:
			appendStringInfo(str, "%d", value->val.ival);
			break;
		case T_Float:
			appendStringInfoString(str, value->val.str);
			break;
		case T_String:
			if (context == DEPARSE_NODE_CONTEXT_IDENTIFIER) {
				appendStringInfoString(str, quote_identifier(value->val.str));
			} else if (context == DEPARSE_NODE_CONTEXT_CONSTANT) {
				deparseStringLiteral(str, value->val.str);
			} else {
				appendStringInfoString(str, value->val.str);
			}
			break;
		case T_BitString:
			appendStringInfo(str, "B'%s'", value->val.str);
			break;
		case T_Null:
			appendStringInfoString(str, "NULL");
			break;
		default:
			elog(ERROR, "unrecognized value node type: %d",
				 (int) nodeTag(value));
			break;
	}
}

static void deparseNode(StringInfo str, Node *node, DeparseNodeContext context)
{
	if (node == NULL)
		return;
	
	switch (nodeTag(node))
	{
		case T_SelectStmt:
			deparseSelectStmt(str, (SelectStmt *) node);
			break;
		case T_ResTarget:
			deparseResTarget(str, (ResTarget *) node, context);
			break;
		case T_RawStmt:
			deparseRawStmt(str, (RawStmt *) node);
			break;
		case T_A_Const:
			deparseAConst(str, (A_Const *) node);
			break;
		case T_FuncCall:
			deparseFuncCall(str, (FuncCall *) node);
			break;
		case T_RangeVar:
			deparseRangeVar(str, (RangeVar *) node, context);
			break;
		case T_ColumnRef:
			deparseColumnRef(str, (ColumnRef *) node);
			break;
		case T_SubLink:
			deparseSubLink(str, (SubLink *) node);
			break;
		case T_A_Expr:
			deparseAExpr(str, (A_Expr *) node, context);
			break;
		case T_BoolExpr:
			deparseBoolExpr(str, (BoolExpr *) node);
			break;
		case T_A_Star:
			deparseAStar(str, (A_Star *) node);
			break;
		case T_CollateClause:
			deparseCollateClause(str, (CollateClause *) node);
			break;
		case T_SortBy:
			deparseSortBy(str, (SortBy *) node);
			break;
		case T_ParamRef:
			deparseParamRef(str, (ParamRef *) node);
			break;
		case T_SQLValueFunction:
			deparseSQLValueFunction(str, (SQLValueFunction *) node);
			break;
		case T_WithClause:
			deparseWithClause(str, (WithClause *) node);
			break;
		case T_JoinExpr:
			deparseJoinExpr(str, (JoinExpr *) node);
			break;
		case T_CommonTableExpr:
			deparseCommonTableExpr(str, (CommonTableExpr *) node);
			break;
		case T_RangeSubselect:
			deparseRangeSubselect(str, (RangeSubselect *) node);
			break;
		case T_RangeFunction:
			deparseRangeFunction(str, (RangeFunction *) node);
			break;
		case T_A_ArrayExpr:
			deparseAArrayExpr(str, (A_ArrayExpr *) node);
			break;
		case T_RowExpr:
			deparseRowExpr(str, (RowExpr *) node);
			break;
		case T_TypeCast:
			deparseTypeCast(str, (TypeCast *) node);
			break;
		case T_TypeName:
			deparseTypeName(str, (TypeName *) node);
			break;
		case T_NullTest:
			deparseNullTest(str, (NullTest *) node);
			break;
		case T_CaseExpr:
			deparseCaseExpr(str, (CaseExpr *) node);
			break;
		case T_CaseWhen:
			deparseCaseWhen(str, (CaseWhen *) node);
			break;
		case T_A_Indirection:
			deparseAIndirection(str, (A_Indirection *) node);
			break;
		case T_A_Indices:
			deparseAIndices(str, (A_Indices *) node);
			break;
		case T_CoalesceExpr:
			deparseCoalesceExpr(str, (CoalesceExpr *) node);
			break;
		case T_MinMaxExpr:
			deparseMinMaxExpr(str, (MinMaxExpr *) node);
			break;
		case T_BooleanTest:
			deparseBooleanTest(str, (BooleanTest *) node);
			break;
		case T_ColumnDef:
			deparseColumnDef(str, (ColumnDef *) node);
			break;
		case T_InsertStmt:
			deparseInsertStmt(str, (InsertStmt *) node);
			break;
		case T_IndexElem:
			deparseIndexElem(str, (IndexElem *) node);
			break;
		case T_UpdateStmt:
			deparseUpdateStmt(str, (UpdateStmt *) node);
			break;
		case T_DeleteStmt:
			deparseDeleteStmt(str, (DeleteStmt *) node);
			break;
		case T_LockingClause:
			deparseLockingClause(str, (LockingClause *) node);
			break;
		case T_SetToDefault:
			deparseSetToDefault(str, (SetToDefault *) node);
			break;
		case T_CreateCastStmt:
			deparseCreateCastStmt(str, (CreateCastStmt *) node);
			break;
		case T_CreateOpClassStmt:
			deparseCreateOpClassStmt(str, (CreateOpClassStmt *) node);
			break;
		case T_CreateOpClassItem:
			deparseCreateOpClassItem(str, (CreateOpClassItem *) node);
			break;
		case T_TableLikeClause:
			deparseTableLikeClause(str, (TableLikeClause *) node);
			break;
		case T_CreateDomainStmt:
			deparseCreateDomainStmt(str, (CreateDomainStmt *) node);
			break;
		case T_Constraint:
			deparseConstraint(str, (Constraint *) node);
			break;
		case T_CreateFunctionStmt:
			deparseCreateFunctionStmt(str, (CreateFunctionStmt *) node);
			break;
		case T_FunctionParameter:
			deparseFunctionParameter(str, (FunctionParameter *) node);
			break;
		case T_DefElem:
			deparseDefElem(str, (DefElem *) node, context);
			break;
		case T_CreateSchemaStmt:
			deparseCreateSchemaStmt(str, (CreateSchemaStmt *) node);
			break;
		case T_RoleSpec:
			deparseRoleSpec(str, (RoleSpec *) node);
			break;
		case T_CreateStmt:
			deparseCreateStmt(str, (CreateStmt *) node, false);
			break;
		case T_SecLabelStmt:
			deparseSecLabelStmt(str, (SecLabelStmt *) node);
			break;
		case T_CreateForeignTableStmt:
			deparseCreateForeignTableStmt(str, (CreateForeignTableStmt *) node);
			break;
		case T_CreateTableAsStmt:
			deparseCreateTableAsStmt(str, (CreateTableAsStmt *) node);
			break;
		case T_ViewStmt:
			deparseViewStmt(str, (ViewStmt *) node);
			break;
		case T_DropStmt:
			deparseDropStmt(str, (DropStmt *) node);
			break;
		case T_GroupingSet:
			deparseGroupingSet(str, (GroupingSet *) node);
			break;
		case T_ObjectWithArgs:
			deparseObjectWithArgs(str, (ObjectWithArgs *) node, true);
			break;
		case T_DropTableSpaceStmt:
			deparseDropTableSpaceStmt(str, (DropTableSpaceStmt *) node);
			break;
		case T_AlterTableStmt:
			deparseAlterTableStmt(str, (AlterTableStmt *) node);
			break;
		case T_AlterTableCmd:
			deparseAlterTableCmd(str, (AlterTableCmd *) node, context);
			break;
		case T_RenameStmt:
			deparseRenameStmt(str, (RenameStmt *) node);
			break;
		case T_TransactionStmt:
			deparseTransactionStmt(str, (TransactionStmt *) node);
			break;
		case T_VariableSetStmt:
			deparseVariableSetStmt(str, (VariableSetStmt *) node);
			break;
		case T_VacuumStmt:
			deparseVacuumStmt(str, (VacuumStmt *) node);
			break;
		case T_LockStmt:
			deparseLockStmt(str, (LockStmt *) node);
			break;
		case T_ConstraintsSetStmt:
			deparseConstraintsSetStmt(str, (ConstraintsSetStmt *) node);
			break;
		case T_ExplainStmt:
			deparseExplainStmt(str, (ExplainStmt *) node);
			break;
		case T_CopyStmt:
			deparseCopyStmt(str, (CopyStmt *) node);
			break;
		case T_DoStmt:
			deparseDoStmt(str, (DoStmt *) node);
			break;
		case T_DiscardStmt:
			deparseDiscardStmt(str, (DiscardStmt *) node);
			break;
		case T_DefineStmt:
			deparseDefineStmt(str, (DefineStmt *) node);
			break;
		case T_CompositeTypeStmt:
			deparseCompositeTypeStmt(str, (CompositeTypeStmt *) node);
			break;
		case T_CreateEnumStmt:
			deparseCreateEnumStmt(str, (CreateEnumStmt *) node);
			break;
		case T_CreateRangeStmt:
			deparseCreateRangeStmt(str, (CreateRangeStmt *) node);
			break;
		case T_GrantStmt:
			deparseGrantStmt(str, (GrantStmt *) node);
			break;
		case T_AccessPriv:
			deparseAccessPriv(str, (AccessPriv *) node);
			break;
		case T_GrantRoleStmt:
			deparseGrantRoleStmt(str, (GrantRoleStmt *) node);
			break;
		case T_DropRoleStmt:
			deparseDropRoleStmt(str, (DropRoleStmt *) node);
			break;
		case T_IndexStmt:
			deparseIndexStmt(str, (IndexStmt *) node);
			break;
		case T_PartitionSpec:
			deparsePartitionSpec(str, (PartitionSpec *) node);
			break;
		case T_PartitionElem:
			deparsePartitionElem(str, (PartitionElem *) node);
			break;
		case T_PartitionCmd:
			deparsePartitionCmd(str, (PartitionCmd *) node);
			break;
		case T_PrepareStmt:
			deparsePrepareStmt(str, (PrepareStmt *) node);
			break;
		case T_ExecuteStmt:
			deparseExecuteStmt(str, (ExecuteStmt *) node);
			break;
		case T_DeallocateStmt:
			deparseDeallocateStmt(str, (DeallocateStmt *) node);
			break;
		case T_CreateRoleStmt:
			deparseCreateRoleStmt(str, (CreateRoleStmt *) node);
			break;
		case T_AlterRoleStmt:
			deparseAlterRoleStmt(str, (AlterRoleStmt *) node);
			break;
		case T_DeclareCursorStmt:
			deparseDeclareCursorStmt(str, (DeclareCursorStmt *) node);
			break;
		case T_FetchStmt:
			deparseFetchStmt(str, (FetchStmt *) node);
			break;
		case T_AlterDefaultPrivilegesStmt:
			deparseAlterDefaultPrivilegesStmt(str, (AlterDefaultPrivilegesStmt *) node);
			break;
		case T_ReindexStmt:
			deparseReindexStmt(str, (ReindexStmt *) node);
			break;
		case T_RuleStmt:
			deparseRuleStmt(str, (RuleStmt *) node);
			break;
		case T_NotifyStmt:
			deparseNotifyStmt(str, (NotifyStmt *) node);
			break;
		case T_CreateSeqStmt:
			deparseCreateSeqStmt(str, (CreateSeqStmt *) node);
			break;
		case T_AlterFunctionStmt:
			deparseAlterFunctionStmt(str, (AlterFunctionStmt *) node);
			break;
		case T_TruncateStmt:
			deparseTruncateStmt(str, (TruncateStmt *) node);
			break;
		case T_RefreshMatViewStmt:
			deparseRefreshMatViewStmt(str, (RefreshMatViewStmt *) node);
			break;
		case T_ReplicaIdentityStmt:
			deparseReplicaIdentityStmt(str, (ReplicaIdentityStmt *) node);
			break;
		case T_CreatePolicyStmt:
			deparseCreatePolicyStmt(str, (CreatePolicyStmt *) node);
			break;
		case T_AlterPolicyStmt:
			deparseAlterPolicyStmt(str, (AlterPolicyStmt *) node);
			break;
		case T_AlterSeqStmt:
			deparseAlterSeqStmt(str, (AlterSeqStmt *) node);
			break;
		case T_CommentStmt:
			deparseCommentStmt(str, (CommentStmt *) node);
			break;
		case T_CreateStatsStmt:
			deparseCreateStatsStmt(str, (CreateStatsStmt *) node);
			break;
		case T_AlterStatsStmt:
			deparseAlterStatsStmt(str, (AlterStatsStmt *) node);
			break;
		case T_CreateForeignServerStmt:
			deparseCreateForeignServerStmt(str, (CreateForeignServerStmt *) node);
			break;
		case T_AlterTSDictionaryStmt:
			deparseAlterTSDictionaryStmt(str, (AlterTSDictionaryStmt *) node);
			break;
		case T_AlterTSConfigurationStmt:
			deparseAlterTSConfigurationStmt(str, (AlterTSConfigurationStmt *) node);
			break;
		case T_CreateFdwStmt:
			deparseCreateFdwStmt(str, (CreateFdwStmt *) node);
			break;
		case T_VariableShowStmt:
			deparseVariableShowStmt(str, (VariableShowStmt *) node);
			break;
		case T_RangeTableSample:
			deparseRangeTableSample(str, (RangeTableSample *) node);
			break;
		case T_CreateSubscriptionStmt:
			deparseCreateSubscriptionStmt(str, (CreateSubscriptionStmt *) node);
			break;
		case T_AlterSubscriptionStmt:
			deparseAlterSubscriptionStmt(str, (AlterSubscriptionStmt *) node);
			break;
		case T_DropSubscriptionStmt:
			deparseDropSubscriptionStmt(str, (DropSubscriptionStmt *) node);
			break;
		case T_AlterOwnerStmt:
			deparseAlterOwnerStmt(str, (AlterOwnerStmt *) node);
			break;
		case T_DropOwnedStmt:
			deparseDropOwnedStmt(str, (DropOwnedStmt *) node);
			break;
		case T_ClosePortalStmt:
			deparseClosePortalStmt(str, (ClosePortalStmt *) node);
			break;
		case T_CurrentOfExpr:
			deparseCurrentOfExpr(str, (CurrentOfExpr *) node);
			break;
		case T_CreateTrigStmt:
			deparseCreateTrigStmt(str, (CreateTrigStmt *) node);
			break;
		case T_TriggerTransition:
			deparseTriggerTransition(str, (TriggerTransition *) node);
			break;
		case T_XmlExpr:
			deparseXmlExpr(str, (XmlExpr *) node);
			break;
		case T_RangeTableFunc:
			deparseRangeTableFunc(str, (RangeTableFunc *) node);
			break;
		case T_RangeTableFuncCol:
			deparseRangeTableFuncCol(str, (RangeTableFuncCol *) node);
			break;
		case T_XmlSerialize:
			deparseXmlSerialize(str, (XmlSerialize *) node);
			break;
		case T_WindowDef:
			deparseWindowDef(str, (WindowDef *) node);
			break;
		case T_NamedArgExpr:
			deparseNamedArgExpr(str, (NamedArgExpr *) node);
			break;
		case T_ClusterStmt:
			deparseClusterStmt(str, (ClusterStmt *) node);
			break;
		case T_Integer:
		case T_Float:
		case T_String:
		case T_BitString:
		case T_Null:
			deparseValue(str, (Value *) node, context);
			break;
		case T_List:
			{
				ListCell *lc;
				foreach(lc, (List *) node) {
					deparseNode(str, lfirst(lc), context);
					if (lnext((List *) node, lc))
						appendStringInfoString(str, " ");
				}
			}
			break;
		default:
			elog(ERROR, "unrecognized node type: %d",
				 (int) nodeTag(node));
			break;
	}
}

PgQueryDeparseResult pg_query_deparse_protobuf(PgQueryProtobuf parse_tree)
{
    PgQueryDeparseResult result = {0};
	StringInfoData str;
    MemoryContext ctx;
	List *stmts;
	ListCell *lc;

	ctx = pg_query_enter_memory_context();

	PG_TRY();
	{
		stmts = pg_query_protobuf_to_nodes(parse_tree);

		initStringInfo(&str);

		foreach(lc, stmts) {
			deparseNode(&str, lfirst(lc), DEPARSE_NODE_CONTEXT_NONE);
			if (lnext(stmts, lc))
				appendStringInfoString(&str, "; ");
		}
		result.query = strdup(str.data);
	}
	PG_CATCH();
	{
		ErrorData* error_data;
		PgQueryError* error;

		MemoryContextSwitchTo(ctx);
		error_data = CopyErrorData();

		// Note: This is intentionally malloc so exiting the memory context doesn't free this
		error = malloc(sizeof(PgQueryError));
		error->message   = strdup(error_data->message);
		error->filename  = strdup(error_data->filename);
		error->funcname  = strdup(error_data->funcname);
		error->context   = NULL;
		error->lineno    = error_data->lineno;
		error->cursorpos = error_data->cursorpos;

		result.error = error;
		FlushErrorState();
	}
	PG_END_TRY();

	pg_query_exit_memory_context(ctx);

    return result;
}

void pg_query_free_deparse_result(PgQueryDeparseResult result)
{
    if (result.error) {
        pg_query_free_error(result.error);
    }

    free(result.query);
}

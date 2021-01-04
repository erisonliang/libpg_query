static void _outAlias(OUT_TYPE(Alias) out_node, const Alias *node);
static void _outRangeVar(OUT_TYPE(RangeVar) out_node, const RangeVar *node);
static void _outTableFunc(OUT_TYPE(TableFunc) out_node, const TableFunc *node);
static void _outExpr(OUT_TYPE(Expr) out_node, const Expr *node);
static void _outVar(OUT_TYPE(Var) out_node, const Var *node);
static void _outParam(OUT_TYPE(Param) out_node, const Param *node);
static void _outAggref(OUT_TYPE(Aggref) out_node, const Aggref *node);
static void _outGroupingFunc(OUT_TYPE(GroupingFunc) out_node, const GroupingFunc *node);
static void _outWindowFunc(OUT_TYPE(WindowFunc) out_node, const WindowFunc *node);
static void _outSubscriptingRef(OUT_TYPE(SubscriptingRef) out_node, const SubscriptingRef *node);
static void _outFuncExpr(OUT_TYPE(FuncExpr) out_node, const FuncExpr *node);
static void _outNamedArgExpr(OUT_TYPE(NamedArgExpr) out_node, const NamedArgExpr *node);
static void _outOpExpr(OUT_TYPE(OpExpr) out_node, const OpExpr *node);
static void _outDistinctExpr(OUT_TYPE(DistinctExpr) out_node, const DistinctExpr *node);
static void _outNullIfExpr(OUT_TYPE(NullIfExpr) out_node, const NullIfExpr *node);
static void _outScalarArrayOpExpr(OUT_TYPE(ScalarArrayOpExpr) out_node, const ScalarArrayOpExpr *node);
static void _outBoolExpr(OUT_TYPE(BoolExpr) out_node, const BoolExpr *node);
static void _outSubLink(OUT_TYPE(SubLink) out_node, const SubLink *node);
static void _outSubPlan(OUT_TYPE(SubPlan) out_node, const SubPlan *node);
static void _outAlternativeSubPlan(OUT_TYPE(AlternativeSubPlan) out_node, const AlternativeSubPlan *node);
static void _outFieldSelect(OUT_TYPE(FieldSelect) out_node, const FieldSelect *node);
static void _outFieldStore(OUT_TYPE(FieldStore) out_node, const FieldStore *node);
static void _outRelabelType(OUT_TYPE(RelabelType) out_node, const RelabelType *node);
static void _outCoerceViaIO(OUT_TYPE(CoerceViaIO) out_node, const CoerceViaIO *node);
static void _outArrayCoerceExpr(OUT_TYPE(ArrayCoerceExpr) out_node, const ArrayCoerceExpr *node);
static void _outConvertRowtypeExpr(OUT_TYPE(ConvertRowtypeExpr) out_node, const ConvertRowtypeExpr *node);
static void _outCollateExpr(OUT_TYPE(CollateExpr) out_node, const CollateExpr *node);
static void _outCaseExpr(OUT_TYPE(CaseExpr) out_node, const CaseExpr *node);
static void _outCaseWhen(OUT_TYPE(CaseWhen) out_node, const CaseWhen *node);
static void _outCaseTestExpr(OUT_TYPE(CaseTestExpr) out_node, const CaseTestExpr *node);
static void _outArrayExpr(OUT_TYPE(ArrayExpr) out_node, const ArrayExpr *node);
static void _outRowExpr(OUT_TYPE(RowExpr) out_node, const RowExpr *node);
static void _outRowCompareExpr(OUT_TYPE(RowCompareExpr) out_node, const RowCompareExpr *node);
static void _outCoalesceExpr(OUT_TYPE(CoalesceExpr) out_node, const CoalesceExpr *node);
static void _outMinMaxExpr(OUT_TYPE(MinMaxExpr) out_node, const MinMaxExpr *node);
static void _outSQLValueFunction(OUT_TYPE(SQLValueFunction) out_node, const SQLValueFunction *node);
static void _outXmlExpr(OUT_TYPE(XmlExpr) out_node, const XmlExpr *node);
static void _outNullTest(OUT_TYPE(NullTest) out_node, const NullTest *node);
static void _outBooleanTest(OUT_TYPE(BooleanTest) out_node, const BooleanTest *node);
static void _outCoerceToDomain(OUT_TYPE(CoerceToDomain) out_node, const CoerceToDomain *node);
static void _outCoerceToDomainValue(OUT_TYPE(CoerceToDomainValue) out_node, const CoerceToDomainValue *node);
static void _outSetToDefault(OUT_TYPE(SetToDefault) out_node, const SetToDefault *node);
static void _outCurrentOfExpr(OUT_TYPE(CurrentOfExpr) out_node, const CurrentOfExpr *node);
static void _outNextValueExpr(OUT_TYPE(NextValueExpr) out_node, const NextValueExpr *node);
static void _outInferenceElem(OUT_TYPE(InferenceElem) out_node, const InferenceElem *node);
static void _outTargetEntry(OUT_TYPE(TargetEntry) out_node, const TargetEntry *node);
static void _outRangeTblRef(OUT_TYPE(RangeTblRef) out_node, const RangeTblRef *node);
static void _outJoinExpr(OUT_TYPE(JoinExpr) out_node, const JoinExpr *node);
static void _outFromExpr(OUT_TYPE(FromExpr) out_node, const FromExpr *node);
static void _outOnConflictExpr(OUT_TYPE(OnConflictExpr) out_node, const OnConflictExpr *node);
static void _outIntoClause(OUT_TYPE(IntoClause) out_node, const IntoClause *node);
static void _outRawStmt(OUT_TYPE(RawStmt) out_node, const RawStmt *node);
static void _outQuery(OUT_TYPE(Query) out_node, const Query *node);
static void _outInsertStmt(OUT_TYPE(InsertStmt) out_node, const InsertStmt *node);
static void _outDeleteStmt(OUT_TYPE(DeleteStmt) out_node, const DeleteStmt *node);
static void _outUpdateStmt(OUT_TYPE(UpdateStmt) out_node, const UpdateStmt *node);
static void _outSelectStmt(OUT_TYPE(SelectStmt) out_node, const SelectStmt *node);
static void _outAlterTableStmt(OUT_TYPE(AlterTableStmt) out_node, const AlterTableStmt *node);
static void _outAlterTableCmd(OUT_TYPE(AlterTableCmd) out_node, const AlterTableCmd *node);
static void _outAlterDomainStmt(OUT_TYPE(AlterDomainStmt) out_node, const AlterDomainStmt *node);
static void _outSetOperationStmt(OUT_TYPE(SetOperationStmt) out_node, const SetOperationStmt *node);
static void _outGrantStmt(OUT_TYPE(GrantStmt) out_node, const GrantStmt *node);
static void _outGrantRoleStmt(OUT_TYPE(GrantRoleStmt) out_node, const GrantRoleStmt *node);
static void _outAlterDefaultPrivilegesStmt(OUT_TYPE(AlterDefaultPrivilegesStmt) out_node, const AlterDefaultPrivilegesStmt *node);
static void _outClosePortalStmt(OUT_TYPE(ClosePortalStmt) out_node, const ClosePortalStmt *node);
static void _outClusterStmt(OUT_TYPE(ClusterStmt) out_node, const ClusterStmt *node);
static void _outCopyStmt(OUT_TYPE(CopyStmt) out_node, const CopyStmt *node);
static void _outCreateStmt(OUT_TYPE(CreateStmt) out_node, const CreateStmt *node);
static void _outDefineStmt(OUT_TYPE(DefineStmt) out_node, const DefineStmt *node);
static void _outDropStmt(OUT_TYPE(DropStmt) out_node, const DropStmt *node);
static void _outTruncateStmt(OUT_TYPE(TruncateStmt) out_node, const TruncateStmt *node);
static void _outCommentStmt(OUT_TYPE(CommentStmt) out_node, const CommentStmt *node);
static void _outFetchStmt(OUT_TYPE(FetchStmt) out_node, const FetchStmt *node);
static void _outIndexStmt(OUT_TYPE(IndexStmt) out_node, const IndexStmt *node);
static void _outCreateFunctionStmt(OUT_TYPE(CreateFunctionStmt) out_node, const CreateFunctionStmt *node);
static void _outAlterFunctionStmt(OUT_TYPE(AlterFunctionStmt) out_node, const AlterFunctionStmt *node);
static void _outDoStmt(OUT_TYPE(DoStmt) out_node, const DoStmt *node);
static void _outRenameStmt(OUT_TYPE(RenameStmt) out_node, const RenameStmt *node);
static void _outRuleStmt(OUT_TYPE(RuleStmt) out_node, const RuleStmt *node);
static void _outNotifyStmt(OUT_TYPE(NotifyStmt) out_node, const NotifyStmt *node);
static void _outListenStmt(OUT_TYPE(ListenStmt) out_node, const ListenStmt *node);
static void _outUnlistenStmt(OUT_TYPE(UnlistenStmt) out_node, const UnlistenStmt *node);
static void _outTransactionStmt(OUT_TYPE(TransactionStmt) out_node, const TransactionStmt *node);
static void _outViewStmt(OUT_TYPE(ViewStmt) out_node, const ViewStmt *node);
static void _outLoadStmt(OUT_TYPE(LoadStmt) out_node, const LoadStmt *node);
static void _outCreateDomainStmt(OUT_TYPE(CreateDomainStmt) out_node, const CreateDomainStmt *node);
static void _outCreatedbStmt(OUT_TYPE(CreatedbStmt) out_node, const CreatedbStmt *node);
static void _outDropdbStmt(OUT_TYPE(DropdbStmt) out_node, const DropdbStmt *node);
static void _outVacuumStmt(OUT_TYPE(VacuumStmt) out_node, const VacuumStmt *node);
static void _outExplainStmt(OUT_TYPE(ExplainStmt) out_node, const ExplainStmt *node);
static void _outCreateTableAsStmt(OUT_TYPE(CreateTableAsStmt) out_node, const CreateTableAsStmt *node);
static void _outCreateSeqStmt(OUT_TYPE(CreateSeqStmt) out_node, const CreateSeqStmt *node);
static void _outAlterSeqStmt(OUT_TYPE(AlterSeqStmt) out_node, const AlterSeqStmt *node);
static void _outVariableSetStmt(OUT_TYPE(VariableSetStmt) out_node, const VariableSetStmt *node);
static void _outVariableShowStmt(OUT_TYPE(VariableShowStmt) out_node, const VariableShowStmt *node);
static void _outDiscardStmt(OUT_TYPE(DiscardStmt) out_node, const DiscardStmt *node);
static void _outCreateTrigStmt(OUT_TYPE(CreateTrigStmt) out_node, const CreateTrigStmt *node);
static void _outCreatePLangStmt(OUT_TYPE(CreatePLangStmt) out_node, const CreatePLangStmt *node);
static void _outCreateRoleStmt(OUT_TYPE(CreateRoleStmt) out_node, const CreateRoleStmt *node);
static void _outAlterRoleStmt(OUT_TYPE(AlterRoleStmt) out_node, const AlterRoleStmt *node);
static void _outDropRoleStmt(OUT_TYPE(DropRoleStmt) out_node, const DropRoleStmt *node);
static void _outLockStmt(OUT_TYPE(LockStmt) out_node, const LockStmt *node);
static void _outConstraintsSetStmt(OUT_TYPE(ConstraintsSetStmt) out_node, const ConstraintsSetStmt *node);
static void _outReindexStmt(OUT_TYPE(ReindexStmt) out_node, const ReindexStmt *node);
static void _outCheckPointStmt(OUT_TYPE(CheckPointStmt) out_node, const CheckPointStmt *node);
static void _outCreateSchemaStmt(OUT_TYPE(CreateSchemaStmt) out_node, const CreateSchemaStmt *node);
static void _outAlterDatabaseStmt(OUT_TYPE(AlterDatabaseStmt) out_node, const AlterDatabaseStmt *node);
static void _outAlterDatabaseSetStmt(OUT_TYPE(AlterDatabaseSetStmt) out_node, const AlterDatabaseSetStmt *node);
static void _outAlterRoleSetStmt(OUT_TYPE(AlterRoleSetStmt) out_node, const AlterRoleSetStmt *node);
static void _outCreateConversionStmt(OUT_TYPE(CreateConversionStmt) out_node, const CreateConversionStmt *node);
static void _outCreateCastStmt(OUT_TYPE(CreateCastStmt) out_node, const CreateCastStmt *node);
static void _outCreateOpClassStmt(OUT_TYPE(CreateOpClassStmt) out_node, const CreateOpClassStmt *node);
static void _outCreateOpFamilyStmt(OUT_TYPE(CreateOpFamilyStmt) out_node, const CreateOpFamilyStmt *node);
static void _outAlterOpFamilyStmt(OUT_TYPE(AlterOpFamilyStmt) out_node, const AlterOpFamilyStmt *node);
static void _outPrepareStmt(OUT_TYPE(PrepareStmt) out_node, const PrepareStmt *node);
static void _outExecuteStmt(OUT_TYPE(ExecuteStmt) out_node, const ExecuteStmt *node);
static void _outDeallocateStmt(OUT_TYPE(DeallocateStmt) out_node, const DeallocateStmt *node);
static void _outDeclareCursorStmt(OUT_TYPE(DeclareCursorStmt) out_node, const DeclareCursorStmt *node);
static void _outCreateTableSpaceStmt(OUT_TYPE(CreateTableSpaceStmt) out_node, const CreateTableSpaceStmt *node);
static void _outDropTableSpaceStmt(OUT_TYPE(DropTableSpaceStmt) out_node, const DropTableSpaceStmt *node);
static void _outAlterObjectDependsStmt(OUT_TYPE(AlterObjectDependsStmt) out_node, const AlterObjectDependsStmt *node);
static void _outAlterObjectSchemaStmt(OUT_TYPE(AlterObjectSchemaStmt) out_node, const AlterObjectSchemaStmt *node);
static void _outAlterOwnerStmt(OUT_TYPE(AlterOwnerStmt) out_node, const AlterOwnerStmt *node);
static void _outAlterOperatorStmt(OUT_TYPE(AlterOperatorStmt) out_node, const AlterOperatorStmt *node);
static void _outAlterTypeStmt(OUT_TYPE(AlterTypeStmt) out_node, const AlterTypeStmt *node);
static void _outDropOwnedStmt(OUT_TYPE(DropOwnedStmt) out_node, const DropOwnedStmt *node);
static void _outReassignOwnedStmt(OUT_TYPE(ReassignOwnedStmt) out_node, const ReassignOwnedStmt *node);
static void _outCompositeTypeStmt(OUT_TYPE(CompositeTypeStmt) out_node, const CompositeTypeStmt *node);
static void _outCreateEnumStmt(OUT_TYPE(CreateEnumStmt) out_node, const CreateEnumStmt *node);
static void _outCreateRangeStmt(OUT_TYPE(CreateRangeStmt) out_node, const CreateRangeStmt *node);
static void _outAlterEnumStmt(OUT_TYPE(AlterEnumStmt) out_node, const AlterEnumStmt *node);
static void _outAlterTSDictionaryStmt(OUT_TYPE(AlterTSDictionaryStmt) out_node, const AlterTSDictionaryStmt *node);
static void _outAlterTSConfigurationStmt(OUT_TYPE(AlterTSConfigurationStmt) out_node, const AlterTSConfigurationStmt *node);
static void _outCreateFdwStmt(OUT_TYPE(CreateFdwStmt) out_node, const CreateFdwStmt *node);
static void _outAlterFdwStmt(OUT_TYPE(AlterFdwStmt) out_node, const AlterFdwStmt *node);
static void _outCreateForeignServerStmt(OUT_TYPE(CreateForeignServerStmt) out_node, const CreateForeignServerStmt *node);
static void _outAlterForeignServerStmt(OUT_TYPE(AlterForeignServerStmt) out_node, const AlterForeignServerStmt *node);
static void _outCreateUserMappingStmt(OUT_TYPE(CreateUserMappingStmt) out_node, const CreateUserMappingStmt *node);
static void _outAlterUserMappingStmt(OUT_TYPE(AlterUserMappingStmt) out_node, const AlterUserMappingStmt *node);
static void _outDropUserMappingStmt(OUT_TYPE(DropUserMappingStmt) out_node, const DropUserMappingStmt *node);
static void _outAlterTableSpaceOptionsStmt(OUT_TYPE(AlterTableSpaceOptionsStmt) out_node, const AlterTableSpaceOptionsStmt *node);
static void _outAlterTableMoveAllStmt(OUT_TYPE(AlterTableMoveAllStmt) out_node, const AlterTableMoveAllStmt *node);
static void _outSecLabelStmt(OUT_TYPE(SecLabelStmt) out_node, const SecLabelStmt *node);
static void _outCreateForeignTableStmt(OUT_TYPE(CreateForeignTableStmt) out_node, const CreateForeignTableStmt *node);
static void _outImportForeignSchemaStmt(OUT_TYPE(ImportForeignSchemaStmt) out_node, const ImportForeignSchemaStmt *node);
static void _outCreateExtensionStmt(OUT_TYPE(CreateExtensionStmt) out_node, const CreateExtensionStmt *node);
static void _outAlterExtensionStmt(OUT_TYPE(AlterExtensionStmt) out_node, const AlterExtensionStmt *node);
static void _outAlterExtensionContentsStmt(OUT_TYPE(AlterExtensionContentsStmt) out_node, const AlterExtensionContentsStmt *node);
static void _outCreateEventTrigStmt(OUT_TYPE(CreateEventTrigStmt) out_node, const CreateEventTrigStmt *node);
static void _outAlterEventTrigStmt(OUT_TYPE(AlterEventTrigStmt) out_node, const AlterEventTrigStmt *node);
static void _outRefreshMatViewStmt(OUT_TYPE(RefreshMatViewStmt) out_node, const RefreshMatViewStmt *node);
static void _outReplicaIdentityStmt(OUT_TYPE(ReplicaIdentityStmt) out_node, const ReplicaIdentityStmt *node);
static void _outAlterSystemStmt(OUT_TYPE(AlterSystemStmt) out_node, const AlterSystemStmt *node);
static void _outCreatePolicyStmt(OUT_TYPE(CreatePolicyStmt) out_node, const CreatePolicyStmt *node);
static void _outAlterPolicyStmt(OUT_TYPE(AlterPolicyStmt) out_node, const AlterPolicyStmt *node);
static void _outCreateTransformStmt(OUT_TYPE(CreateTransformStmt) out_node, const CreateTransformStmt *node);
static void _outCreateAmStmt(OUT_TYPE(CreateAmStmt) out_node, const CreateAmStmt *node);
static void _outCreatePublicationStmt(OUT_TYPE(CreatePublicationStmt) out_node, const CreatePublicationStmt *node);
static void _outAlterPublicationStmt(OUT_TYPE(AlterPublicationStmt) out_node, const AlterPublicationStmt *node);
static void _outCreateSubscriptionStmt(OUT_TYPE(CreateSubscriptionStmt) out_node, const CreateSubscriptionStmt *node);
static void _outAlterSubscriptionStmt(OUT_TYPE(AlterSubscriptionStmt) out_node, const AlterSubscriptionStmt *node);
static void _outDropSubscriptionStmt(OUT_TYPE(DropSubscriptionStmt) out_node, const DropSubscriptionStmt *node);
static void _outCreateStatsStmt(OUT_TYPE(CreateStatsStmt) out_node, const CreateStatsStmt *node);
static void _outAlterCollationStmt(OUT_TYPE(AlterCollationStmt) out_node, const AlterCollationStmt *node);
static void _outCallStmt(OUT_TYPE(CallStmt) out_node, const CallStmt *node);
static void _outAlterStatsStmt(OUT_TYPE(AlterStatsStmt) out_node, const AlterStatsStmt *node);
static void _outAExpr(OUT_TYPE(AExpr) out_node, const A_Expr *node);
static void _outColumnRef(OUT_TYPE(ColumnRef) out_node, const ColumnRef *node);
static void _outParamRef(OUT_TYPE(ParamRef) out_node, const ParamRef *node);
static void _outAConst(OUT_TYPE(AConst) out_node, const A_Const *node);
static void _outFuncCall(OUT_TYPE(FuncCall) out_node, const FuncCall *node);
static void _outAStar(OUT_TYPE(AStar) out_node, const A_Star *node);
static void _outAIndices(OUT_TYPE(AIndices) out_node, const A_Indices *node);
static void _outAIndirection(OUT_TYPE(AIndirection) out_node, const A_Indirection *node);
static void _outAArrayExpr(OUT_TYPE(AArrayExpr) out_node, const A_ArrayExpr *node);
static void _outResTarget(OUT_TYPE(ResTarget) out_node, const ResTarget *node);
static void _outMultiAssignRef(OUT_TYPE(MultiAssignRef) out_node, const MultiAssignRef *node);
static void _outTypeCast(OUT_TYPE(TypeCast) out_node, const TypeCast *node);
static void _outCollateClause(OUT_TYPE(CollateClause) out_node, const CollateClause *node);
static void _outSortBy(OUT_TYPE(SortBy) out_node, const SortBy *node);
static void _outWindowDef(OUT_TYPE(WindowDef) out_node, const WindowDef *node);
static void _outRangeSubselect(OUT_TYPE(RangeSubselect) out_node, const RangeSubselect *node);
static void _outRangeFunction(OUT_TYPE(RangeFunction) out_node, const RangeFunction *node);
static void _outRangeTableSample(OUT_TYPE(RangeTableSample) out_node, const RangeTableSample *node);
static void _outRangeTableFunc(OUT_TYPE(RangeTableFunc) out_node, const RangeTableFunc *node);
static void _outRangeTableFuncCol(OUT_TYPE(RangeTableFuncCol) out_node, const RangeTableFuncCol *node);
static void _outTypeName(OUT_TYPE(TypeName) out_node, const TypeName *node);
static void _outColumnDef(OUT_TYPE(ColumnDef) out_node, const ColumnDef *node);
static void _outIndexElem(OUT_TYPE(IndexElem) out_node, const IndexElem *node);
static void _outConstraint(OUT_TYPE(Constraint) out_node, const Constraint *node);
static void _outDefElem(OUT_TYPE(DefElem) out_node, const DefElem *node);
static void _outRangeTblEntry(OUT_TYPE(RangeTblEntry) out_node, const RangeTblEntry *node);
static void _outRangeTblFunction(OUT_TYPE(RangeTblFunction) out_node, const RangeTblFunction *node);
static void _outTableSampleClause(OUT_TYPE(TableSampleClause) out_node, const TableSampleClause *node);
static void _outWithCheckOption(OUT_TYPE(WithCheckOption) out_node, const WithCheckOption *node);
static void _outSortGroupClause(OUT_TYPE(SortGroupClause) out_node, const SortGroupClause *node);
static void _outGroupingSet(OUT_TYPE(GroupingSet) out_node, const GroupingSet *node);
static void _outWindowClause(OUT_TYPE(WindowClause) out_node, const WindowClause *node);
static void _outObjectWithArgs(OUT_TYPE(ObjectWithArgs) out_node, const ObjectWithArgs *node);
static void _outAccessPriv(OUT_TYPE(AccessPriv) out_node, const AccessPriv *node);
static void _outCreateOpClassItem(OUT_TYPE(CreateOpClassItem) out_node, const CreateOpClassItem *node);
static void _outTableLikeClause(OUT_TYPE(TableLikeClause) out_node, const TableLikeClause *node);
static void _outFunctionParameter(OUT_TYPE(FunctionParameter) out_node, const FunctionParameter *node);
static void _outLockingClause(OUT_TYPE(LockingClause) out_node, const LockingClause *node);
static void _outRowMarkClause(OUT_TYPE(RowMarkClause) out_node, const RowMarkClause *node);
static void _outXmlSerialize(OUT_TYPE(XmlSerialize) out_node, const XmlSerialize *node);
static void _outWithClause(OUT_TYPE(WithClause) out_node, const WithClause *node);
static void _outInferClause(OUT_TYPE(InferClause) out_node, const InferClause *node);
static void _outOnConflictClause(OUT_TYPE(OnConflictClause) out_node, const OnConflictClause *node);
static void _outCommonTableExpr(OUT_TYPE(CommonTableExpr) out_node, const CommonTableExpr *node);
static void _outRoleSpec(OUT_TYPE(RoleSpec) out_node, const RoleSpec *node);
static void _outTriggerTransition(OUT_TYPE(TriggerTransition) out_node, const TriggerTransition *node);
static void _outPartitionElem(OUT_TYPE(PartitionElem) out_node, const PartitionElem *node);
static void _outPartitionSpec(OUT_TYPE(PartitionSpec) out_node, const PartitionSpec *node);
static void _outPartitionBoundSpec(OUT_TYPE(PartitionBoundSpec) out_node, const PartitionBoundSpec *node);
static void _outPartitionRangeDatum(OUT_TYPE(PartitionRangeDatum) out_node, const PartitionRangeDatum *node);
static void _outPartitionCmd(OUT_TYPE(PartitionCmd) out_node, const PartitionCmd *node);
static void _outVacuumRelation(OUT_TYPE(VacuumRelation) out_node, const VacuumRelation *node);
static void _outInlineCodeBlock(OUT_TYPE(InlineCodeBlock) out_node, const InlineCodeBlock *node);
static void _outCallContext(OUT_TYPE(CallContext) out_node, const CallContext *node);
static const char*
_enumToStringOverridingKind(OverridingKind value) {
  switch(value) {
    case OVERRIDING_NOT_SET: return "OVERRIDING_NOT_SET";
    case OVERRIDING_USER_VALUE: return "OVERRIDING_USER_VALUE";
    case OVERRIDING_SYSTEM_VALUE: return "OVERRIDING_SYSTEM_VALUE";
  }
  return NULL;
}

static const char*
_enumToStringQuerySource(QuerySource value) {
  switch(value) {
    case QSRC_ORIGINAL: return "QSRC_ORIGINAL";
    case QSRC_PARSER: return "QSRC_PARSER";
    case QSRC_INSTEAD_RULE: return "QSRC_INSTEAD_RULE";
    case QSRC_QUAL_INSTEAD_RULE: return "QSRC_QUAL_INSTEAD_RULE";
    case QSRC_NON_INSTEAD_RULE: return "QSRC_NON_INSTEAD_RULE";
  }
  return NULL;
}

static const char*
_enumToStringSortByDir(SortByDir value) {
  switch(value) {
    case SORTBY_DEFAULT: return "SORTBY_DEFAULT";
    case SORTBY_ASC: return "SORTBY_ASC";
    case SORTBY_DESC: return "SORTBY_DESC";
    case SORTBY_USING: return "SORTBY_USING";
  }
  return NULL;
}

static const char*
_enumToStringSortByNulls(SortByNulls value) {
  switch(value) {
    case SORTBY_NULLS_DEFAULT: return "SORTBY_NULLS_DEFAULT";
    case SORTBY_NULLS_FIRST: return "SORTBY_NULLS_FIRST";
    case SORTBY_NULLS_LAST: return "SORTBY_NULLS_LAST";
  }
  return NULL;
}

static const char*
_enumToStringA_Expr_Kind(A_Expr_Kind value) {
  switch(value) {
    case AEXPR_OP: return "AEXPR_OP";
    case AEXPR_OP_ANY: return "AEXPR_OP_ANY";
    case AEXPR_OP_ALL: return "AEXPR_OP_ALL";
    case AEXPR_DISTINCT: return "AEXPR_DISTINCT";
    case AEXPR_NOT_DISTINCT: return "AEXPR_NOT_DISTINCT";
    case AEXPR_NULLIF: return "AEXPR_NULLIF";
    case AEXPR_OF: return "AEXPR_OF";
    case AEXPR_IN: return "AEXPR_IN";
    case AEXPR_LIKE: return "AEXPR_LIKE";
    case AEXPR_ILIKE: return "AEXPR_ILIKE";
    case AEXPR_SIMILAR: return "AEXPR_SIMILAR";
    case AEXPR_BETWEEN: return "AEXPR_BETWEEN";
    case AEXPR_NOT_BETWEEN: return "AEXPR_NOT_BETWEEN";
    case AEXPR_BETWEEN_SYM: return "AEXPR_BETWEEN_SYM";
    case AEXPR_NOT_BETWEEN_SYM: return "AEXPR_NOT_BETWEEN_SYM";
    case AEXPR_PAREN: return "AEXPR_PAREN";
  }
  return NULL;
}

static const char*
_enumToStringRoleSpecType(RoleSpecType value) {
  switch(value) {
    case ROLESPEC_CSTRING: return "ROLESPEC_CSTRING";
    case ROLESPEC_CURRENT_USER: return "ROLESPEC_CURRENT_USER";
    case ROLESPEC_SESSION_USER: return "ROLESPEC_SESSION_USER";
    case ROLESPEC_PUBLIC: return "ROLESPEC_PUBLIC";
  }
  return NULL;
}

static const char*
_enumToStringTableLikeOption(TableLikeOption value) {
  switch(value) {
    case CREATE_TABLE_LIKE_COMMENTS: return "CREATE_TABLE_LIKE_COMMENTS";
    case CREATE_TABLE_LIKE_CONSTRAINTS: return "CREATE_TABLE_LIKE_CONSTRAINTS";
    case CREATE_TABLE_LIKE_DEFAULTS: return "CREATE_TABLE_LIKE_DEFAULTS";
    case CREATE_TABLE_LIKE_GENERATED: return "CREATE_TABLE_LIKE_GENERATED";
    case CREATE_TABLE_LIKE_IDENTITY: return "CREATE_TABLE_LIKE_IDENTITY";
    case CREATE_TABLE_LIKE_INDEXES: return "CREATE_TABLE_LIKE_INDEXES";
    case CREATE_TABLE_LIKE_STATISTICS: return "CREATE_TABLE_LIKE_STATISTICS";
    case CREATE_TABLE_LIKE_STORAGE: return "CREATE_TABLE_LIKE_STORAGE";
    case CREATE_TABLE_LIKE_ALL: return "CREATE_TABLE_LIKE_ALL";
  }
  return NULL;
}

static const char*
_enumToStringDefElemAction(DefElemAction value) {
  switch(value) {
    case DEFELEM_UNSPEC: return "DEFELEM_UNSPEC";
    case DEFELEM_SET: return "DEFELEM_SET";
    case DEFELEM_ADD: return "DEFELEM_ADD";
    case DEFELEM_DROP: return "DEFELEM_DROP";
  }
  return NULL;
}

static const char*
_enumToStringPartitionRangeDatumKind(PartitionRangeDatumKind value) {
  switch(value) {
    case PARTITION_RANGE_DATUM_MINVALUE: return "PARTITION_RANGE_DATUM_MINVALUE";
    case PARTITION_RANGE_DATUM_VALUE: return "PARTITION_RANGE_DATUM_VALUE";
    case PARTITION_RANGE_DATUM_MAXVALUE: return "PARTITION_RANGE_DATUM_MAXVALUE";
  }
  return NULL;
}

static const char*
_enumToStringRTEKind(RTEKind value) {
  switch(value) {
    case RTE_RELATION: return "RTE_RELATION";
    case RTE_SUBQUERY: return "RTE_SUBQUERY";
    case RTE_JOIN: return "RTE_JOIN";
    case RTE_FUNCTION: return "RTE_FUNCTION";
    case RTE_TABLEFUNC: return "RTE_TABLEFUNC";
    case RTE_VALUES: return "RTE_VALUES";
    case RTE_CTE: return "RTE_CTE";
    case RTE_NAMEDTUPLESTORE: return "RTE_NAMEDTUPLESTORE";
    case RTE_RESULT: return "RTE_RESULT";
  }
  return NULL;
}

static const char*
_enumToStringWCOKind(WCOKind value) {
  switch(value) {
    case WCO_VIEW_CHECK: return "WCO_VIEW_CHECK";
    case WCO_RLS_INSERT_CHECK: return "WCO_RLS_INSERT_CHECK";
    case WCO_RLS_UPDATE_CHECK: return "WCO_RLS_UPDATE_CHECK";
    case WCO_RLS_CONFLICT_CHECK: return "WCO_RLS_CONFLICT_CHECK";
  }
  return NULL;
}

static const char*
_enumToStringGroupingSetKind(GroupingSetKind value) {
  switch(value) {
    case GROUPING_SET_EMPTY: return "GROUPING_SET_EMPTY";
    case GROUPING_SET_SIMPLE: return "GROUPING_SET_SIMPLE";
    case GROUPING_SET_ROLLUP: return "GROUPING_SET_ROLLUP";
    case GROUPING_SET_CUBE: return "GROUPING_SET_CUBE";
    case GROUPING_SET_SETS: return "GROUPING_SET_SETS";
  }
  return NULL;
}

static const char*
_enumToStringCTEMaterialize(CTEMaterialize value) {
  switch(value) {
    case CTEMaterializeDefault: return "CTEMaterializeDefault";
    case CTEMaterializeAlways: return "CTEMaterializeAlways";
    case CTEMaterializeNever: return "CTEMaterializeNever";
  }
  return NULL;
}

static const char*
_enumToStringSetOperation(SetOperation value) {
  switch(value) {
    case SETOP_NONE: return "SETOP_NONE";
    case SETOP_UNION: return "SETOP_UNION";
    case SETOP_INTERSECT: return "SETOP_INTERSECT";
    case SETOP_EXCEPT: return "SETOP_EXCEPT";
  }
  return NULL;
}

static const char*
_enumToStringObjectType(ObjectType value) {
  switch(value) {
    case OBJECT_ACCESS_METHOD: return "OBJECT_ACCESS_METHOD";
    case OBJECT_AGGREGATE: return "OBJECT_AGGREGATE";
    case OBJECT_AMOP: return "OBJECT_AMOP";
    case OBJECT_AMPROC: return "OBJECT_AMPROC";
    case OBJECT_ATTRIBUTE: return "OBJECT_ATTRIBUTE";
    case OBJECT_CAST: return "OBJECT_CAST";
    case OBJECT_COLUMN: return "OBJECT_COLUMN";
    case OBJECT_COLLATION: return "OBJECT_COLLATION";
    case OBJECT_CONVERSION: return "OBJECT_CONVERSION";
    case OBJECT_DATABASE: return "OBJECT_DATABASE";
    case OBJECT_DEFAULT: return "OBJECT_DEFAULT";
    case OBJECT_DEFACL: return "OBJECT_DEFACL";
    case OBJECT_DOMAIN: return "OBJECT_DOMAIN";
    case OBJECT_DOMCONSTRAINT: return "OBJECT_DOMCONSTRAINT";
    case OBJECT_EVENT_TRIGGER: return "OBJECT_EVENT_TRIGGER";
    case OBJECT_EXTENSION: return "OBJECT_EXTENSION";
    case OBJECT_FDW: return "OBJECT_FDW";
    case OBJECT_FOREIGN_SERVER: return "OBJECT_FOREIGN_SERVER";
    case OBJECT_FOREIGN_TABLE: return "OBJECT_FOREIGN_TABLE";
    case OBJECT_FUNCTION: return "OBJECT_FUNCTION";
    case OBJECT_INDEX: return "OBJECT_INDEX";
    case OBJECT_LANGUAGE: return "OBJECT_LANGUAGE";
    case OBJECT_LARGEOBJECT: return "OBJECT_LARGEOBJECT";
    case OBJECT_MATVIEW: return "OBJECT_MATVIEW";
    case OBJECT_OPCLASS: return "OBJECT_OPCLASS";
    case OBJECT_OPERATOR: return "OBJECT_OPERATOR";
    case OBJECT_OPFAMILY: return "OBJECT_OPFAMILY";
    case OBJECT_POLICY: return "OBJECT_POLICY";
    case OBJECT_PROCEDURE: return "OBJECT_PROCEDURE";
    case OBJECT_PUBLICATION: return "OBJECT_PUBLICATION";
    case OBJECT_PUBLICATION_REL: return "OBJECT_PUBLICATION_REL";
    case OBJECT_ROLE: return "OBJECT_ROLE";
    case OBJECT_ROUTINE: return "OBJECT_ROUTINE";
    case OBJECT_RULE: return "OBJECT_RULE";
    case OBJECT_SCHEMA: return "OBJECT_SCHEMA";
    case OBJECT_SEQUENCE: return "OBJECT_SEQUENCE";
    case OBJECT_SUBSCRIPTION: return "OBJECT_SUBSCRIPTION";
    case OBJECT_STATISTIC_EXT: return "OBJECT_STATISTIC_EXT";
    case OBJECT_TABCONSTRAINT: return "OBJECT_TABCONSTRAINT";
    case OBJECT_TABLE: return "OBJECT_TABLE";
    case OBJECT_TABLESPACE: return "OBJECT_TABLESPACE";
    case OBJECT_TRANSFORM: return "OBJECT_TRANSFORM";
    case OBJECT_TRIGGER: return "OBJECT_TRIGGER";
    case OBJECT_TSCONFIGURATION: return "OBJECT_TSCONFIGURATION";
    case OBJECT_TSDICTIONARY: return "OBJECT_TSDICTIONARY";
    case OBJECT_TSPARSER: return "OBJECT_TSPARSER";
    case OBJECT_TSTEMPLATE: return "OBJECT_TSTEMPLATE";
    case OBJECT_TYPE: return "OBJECT_TYPE";
    case OBJECT_USER_MAPPING: return "OBJECT_USER_MAPPING";
    case OBJECT_VIEW: return "OBJECT_VIEW";
  }
  return NULL;
}

static const char*
_enumToStringDropBehavior(DropBehavior value) {
  switch(value) {
    case DROP_RESTRICT: return "DROP_RESTRICT";
    case DROP_CASCADE: return "DROP_CASCADE";
  }
  return NULL;
}

static const char*
_enumToStringAlterTableType(AlterTableType value) {
  switch(value) {
    case AT_AddColumn: return "AT_AddColumn";
    case AT_AddColumnRecurse: return "AT_AddColumnRecurse";
    case AT_AddColumnToView: return "AT_AddColumnToView";
    case AT_ColumnDefault: return "AT_ColumnDefault";
    case AT_CookedColumnDefault: return "AT_CookedColumnDefault";
    case AT_DropNotNull: return "AT_DropNotNull";
    case AT_SetNotNull: return "AT_SetNotNull";
    case AT_DropExpression: return "AT_DropExpression";
    case AT_CheckNotNull: return "AT_CheckNotNull";
    case AT_SetStatistics: return "AT_SetStatistics";
    case AT_SetOptions: return "AT_SetOptions";
    case AT_ResetOptions: return "AT_ResetOptions";
    case AT_SetStorage: return "AT_SetStorage";
    case AT_DropColumn: return "AT_DropColumn";
    case AT_DropColumnRecurse: return "AT_DropColumnRecurse";
    case AT_AddIndex: return "AT_AddIndex";
    case AT_ReAddIndex: return "AT_ReAddIndex";
    case AT_AddConstraint: return "AT_AddConstraint";
    case AT_AddConstraintRecurse: return "AT_AddConstraintRecurse";
    case AT_ReAddConstraint: return "AT_ReAddConstraint";
    case AT_ReAddDomainConstraint: return "AT_ReAddDomainConstraint";
    case AT_AlterConstraint: return "AT_AlterConstraint";
    case AT_ValidateConstraint: return "AT_ValidateConstraint";
    case AT_ValidateConstraintRecurse: return "AT_ValidateConstraintRecurse";
    case AT_AddIndexConstraint: return "AT_AddIndexConstraint";
    case AT_DropConstraint: return "AT_DropConstraint";
    case AT_DropConstraintRecurse: return "AT_DropConstraintRecurse";
    case AT_ReAddComment: return "AT_ReAddComment";
    case AT_AlterColumnType: return "AT_AlterColumnType";
    case AT_AlterColumnGenericOptions: return "AT_AlterColumnGenericOptions";
    case AT_ChangeOwner: return "AT_ChangeOwner";
    case AT_ClusterOn: return "AT_ClusterOn";
    case AT_DropCluster: return "AT_DropCluster";
    case AT_SetLogged: return "AT_SetLogged";
    case AT_SetUnLogged: return "AT_SetUnLogged";
    case AT_DropOids: return "AT_DropOids";
    case AT_SetTableSpace: return "AT_SetTableSpace";
    case AT_SetRelOptions: return "AT_SetRelOptions";
    case AT_ResetRelOptions: return "AT_ResetRelOptions";
    case AT_ReplaceRelOptions: return "AT_ReplaceRelOptions";
    case AT_EnableTrig: return "AT_EnableTrig";
    case AT_EnableAlwaysTrig: return "AT_EnableAlwaysTrig";
    case AT_EnableReplicaTrig: return "AT_EnableReplicaTrig";
    case AT_DisableTrig: return "AT_DisableTrig";
    case AT_EnableTrigAll: return "AT_EnableTrigAll";
    case AT_DisableTrigAll: return "AT_DisableTrigAll";
    case AT_EnableTrigUser: return "AT_EnableTrigUser";
    case AT_DisableTrigUser: return "AT_DisableTrigUser";
    case AT_EnableRule: return "AT_EnableRule";
    case AT_EnableAlwaysRule: return "AT_EnableAlwaysRule";
    case AT_EnableReplicaRule: return "AT_EnableReplicaRule";
    case AT_DisableRule: return "AT_DisableRule";
    case AT_AddInherit: return "AT_AddInherit";
    case AT_DropInherit: return "AT_DropInherit";
    case AT_AddOf: return "AT_AddOf";
    case AT_DropOf: return "AT_DropOf";
    case AT_ReplicaIdentity: return "AT_ReplicaIdentity";
    case AT_EnableRowSecurity: return "AT_EnableRowSecurity";
    case AT_DisableRowSecurity: return "AT_DisableRowSecurity";
    case AT_ForceRowSecurity: return "AT_ForceRowSecurity";
    case AT_NoForceRowSecurity: return "AT_NoForceRowSecurity";
    case AT_GenericOptions: return "AT_GenericOptions";
    case AT_AttachPartition: return "AT_AttachPartition";
    case AT_DetachPartition: return "AT_DetachPartition";
    case AT_AddIdentity: return "AT_AddIdentity";
    case AT_SetIdentity: return "AT_SetIdentity";
    case AT_DropIdentity: return "AT_DropIdentity";
  }
  return NULL;
}

static const char*
_enumToStringGrantTargetType(GrantTargetType value) {
  switch(value) {
    case ACL_TARGET_OBJECT: return "ACL_TARGET_OBJECT";
    case ACL_TARGET_ALL_IN_SCHEMA: return "ACL_TARGET_ALL_IN_SCHEMA";
    case ACL_TARGET_DEFAULTS: return "ACL_TARGET_DEFAULTS";
  }
  return NULL;
}

static const char*
_enumToStringVariableSetKind(VariableSetKind value) {
  switch(value) {
    case VAR_SET_VALUE: return "VAR_SET_VALUE";
    case VAR_SET_DEFAULT: return "VAR_SET_DEFAULT";
    case VAR_SET_CURRENT: return "VAR_SET_CURRENT";
    case VAR_SET_MULTI: return "VAR_SET_MULTI";
    case VAR_RESET: return "VAR_RESET";
    case VAR_RESET_ALL: return "VAR_RESET_ALL";
  }
  return NULL;
}

static const char*
_enumToStringConstrType(ConstrType value) {
  switch(value) {
    case CONSTR_NULL: return "CONSTR_NULL";
    case CONSTR_NOTNULL: return "CONSTR_NOTNULL";
    case CONSTR_DEFAULT: return "CONSTR_DEFAULT";
    case CONSTR_IDENTITY: return "CONSTR_IDENTITY";
    case CONSTR_GENERATED: return "CONSTR_GENERATED";
    case CONSTR_CHECK: return "CONSTR_CHECK";
    case CONSTR_PRIMARY: return "CONSTR_PRIMARY";
    case CONSTR_UNIQUE: return "CONSTR_UNIQUE";
    case CONSTR_EXCLUSION: return "CONSTR_EXCLUSION";
    case CONSTR_FOREIGN: return "CONSTR_FOREIGN";
    case CONSTR_ATTR_DEFERRABLE: return "CONSTR_ATTR_DEFERRABLE";
    case CONSTR_ATTR_NOT_DEFERRABLE: return "CONSTR_ATTR_NOT_DEFERRABLE";
    case CONSTR_ATTR_DEFERRED: return "CONSTR_ATTR_DEFERRED";
    case CONSTR_ATTR_IMMEDIATE: return "CONSTR_ATTR_IMMEDIATE";
  }
  return NULL;
}

static const char*
_enumToStringImportForeignSchemaType(ImportForeignSchemaType value) {
  switch(value) {
    case FDW_IMPORT_SCHEMA_ALL: return "FDW_IMPORT_SCHEMA_ALL";
    case FDW_IMPORT_SCHEMA_LIMIT_TO: return "FDW_IMPORT_SCHEMA_LIMIT_TO";
    case FDW_IMPORT_SCHEMA_EXCEPT: return "FDW_IMPORT_SCHEMA_EXCEPT";
  }
  return NULL;
}

static const char*
_enumToStringRoleStmtType(RoleStmtType value) {
  switch(value) {
    case ROLESTMT_ROLE: return "ROLESTMT_ROLE";
    case ROLESTMT_USER: return "ROLESTMT_USER";
    case ROLESTMT_GROUP: return "ROLESTMT_GROUP";
  }
  return NULL;
}

static const char*
_enumToStringFetchDirection(FetchDirection value) {
  switch(value) {
    case FETCH_FORWARD: return "FETCH_FORWARD";
    case FETCH_BACKWARD: return "FETCH_BACKWARD";
    case FETCH_ABSOLUTE: return "FETCH_ABSOLUTE";
    case FETCH_RELATIVE: return "FETCH_RELATIVE";
  }
  return NULL;
}

static const char*
_enumToStringFunctionParameterMode(FunctionParameterMode value) {
  switch(value) {
    case FUNC_PARAM_IN: return "FUNC_PARAM_IN";
    case FUNC_PARAM_OUT: return "FUNC_PARAM_OUT";
    case FUNC_PARAM_INOUT: return "FUNC_PARAM_INOUT";
    case FUNC_PARAM_VARIADIC: return "FUNC_PARAM_VARIADIC";
    case FUNC_PARAM_TABLE: return "FUNC_PARAM_TABLE";
  }
  return NULL;
}

static const char*
_enumToStringTransactionStmtKind(TransactionStmtKind value) {
  switch(value) {
    case TRANS_STMT_BEGIN: return "TRANS_STMT_BEGIN";
    case TRANS_STMT_START: return "TRANS_STMT_START";
    case TRANS_STMT_COMMIT: return "TRANS_STMT_COMMIT";
    case TRANS_STMT_ROLLBACK: return "TRANS_STMT_ROLLBACK";
    case TRANS_STMT_SAVEPOINT: return "TRANS_STMT_SAVEPOINT";
    case TRANS_STMT_RELEASE: return "TRANS_STMT_RELEASE";
    case TRANS_STMT_ROLLBACK_TO: return "TRANS_STMT_ROLLBACK_TO";
    case TRANS_STMT_PREPARE: return "TRANS_STMT_PREPARE";
    case TRANS_STMT_COMMIT_PREPARED: return "TRANS_STMT_COMMIT_PREPARED";
    case TRANS_STMT_ROLLBACK_PREPARED: return "TRANS_STMT_ROLLBACK_PREPARED";
  }
  return NULL;
}

static const char*
_enumToStringViewCheckOption(ViewCheckOption value) {
  switch(value) {
    case NO_CHECK_OPTION: return "NO_CHECK_OPTION";
    case LOCAL_CHECK_OPTION: return "LOCAL_CHECK_OPTION";
    case CASCADED_CHECK_OPTION: return "CASCADED_CHECK_OPTION";
  }
  return NULL;
}

static const char*
_enumToStringClusterOption(ClusterOption value) {
  switch(value) {
    case CLUOPT_RECHECK: return "CLUOPT_RECHECK";
    case CLUOPT_VERBOSE: return "CLUOPT_VERBOSE";
  }
  return NULL;
}

static const char*
_enumToStringDiscardMode(DiscardMode value) {
  switch(value) {
    case DISCARD_ALL: return "DISCARD_ALL";
    case DISCARD_PLANS: return "DISCARD_PLANS";
    case DISCARD_SEQUENCES: return "DISCARD_SEQUENCES";
    case DISCARD_TEMP: return "DISCARD_TEMP";
  }
  return NULL;
}

static const char*
_enumToStringReindexObjectType(ReindexObjectType value) {
  switch(value) {
    case REINDEX_OBJECT_INDEX: return "REINDEX_OBJECT_INDEX";
    case REINDEX_OBJECT_TABLE: return "REINDEX_OBJECT_TABLE";
    case REINDEX_OBJECT_SCHEMA: return "REINDEX_OBJECT_SCHEMA";
    case REINDEX_OBJECT_SYSTEM: return "REINDEX_OBJECT_SYSTEM";
    case REINDEX_OBJECT_DATABASE: return "REINDEX_OBJECT_DATABASE";
  }
  return NULL;
}

static const char*
_enumToStringAlterTSConfigType(AlterTSConfigType value) {
  switch(value) {
    case ALTER_TSCONFIG_ADD_MAPPING: return "ALTER_TSCONFIG_ADD_MAPPING";
    case ALTER_TSCONFIG_ALTER_MAPPING_FOR_TOKEN: return "ALTER_TSCONFIG_ALTER_MAPPING_FOR_TOKEN";
    case ALTER_TSCONFIG_REPLACE_DICT: return "ALTER_TSCONFIG_REPLACE_DICT";
    case ALTER_TSCONFIG_REPLACE_DICT_FOR_TOKEN: return "ALTER_TSCONFIG_REPLACE_DICT_FOR_TOKEN";
    case ALTER_TSCONFIG_DROP_MAPPING: return "ALTER_TSCONFIG_DROP_MAPPING";
  }
  return NULL;
}

static const char*
_enumToStringAlterSubscriptionType(AlterSubscriptionType value) {
  switch(value) {
    case ALTER_SUBSCRIPTION_OPTIONS: return "ALTER_SUBSCRIPTION_OPTIONS";
    case ALTER_SUBSCRIPTION_CONNECTION: return "ALTER_SUBSCRIPTION_CONNECTION";
    case ALTER_SUBSCRIPTION_PUBLICATION: return "ALTER_SUBSCRIPTION_PUBLICATION";
    case ALTER_SUBSCRIPTION_REFRESH: return "ALTER_SUBSCRIPTION_REFRESH";
    case ALTER_SUBSCRIPTION_ENABLED: return "ALTER_SUBSCRIPTION_ENABLED";
  }
  return NULL;
}

static const char*
_enumToStringOnCommitAction(OnCommitAction value) {
  switch(value) {
    case ONCOMMIT_NOOP: return "ONCOMMIT_NOOP";
    case ONCOMMIT_PRESERVE_ROWS: return "ONCOMMIT_PRESERVE_ROWS";
    case ONCOMMIT_DELETE_ROWS: return "ONCOMMIT_DELETE_ROWS";
    case ONCOMMIT_DROP: return "ONCOMMIT_DROP";
  }
  return NULL;
}

static const char*
_enumToStringParamKind(ParamKind value) {
  switch(value) {
    case PARAM_EXTERN: return "PARAM_EXTERN";
    case PARAM_EXEC: return "PARAM_EXEC";
    case PARAM_SUBLINK: return "PARAM_SUBLINK";
    case PARAM_MULTIEXPR: return "PARAM_MULTIEXPR";
  }
  return NULL;
}

static const char*
_enumToStringCoercionContext(CoercionContext value) {
  switch(value) {
    case COERCION_IMPLICIT: return "COERCION_IMPLICIT";
    case COERCION_ASSIGNMENT: return "COERCION_ASSIGNMENT";
    case COERCION_EXPLICIT: return "COERCION_EXPLICIT";
  }
  return NULL;
}

static const char*
_enumToStringCoercionForm(CoercionForm value) {
  switch(value) {
    case COERCE_EXPLICIT_CALL: return "COERCE_EXPLICIT_CALL";
    case COERCE_EXPLICIT_CAST: return "COERCE_EXPLICIT_CAST";
    case COERCE_IMPLICIT_CAST: return "COERCE_IMPLICIT_CAST";
  }
  return NULL;
}

static const char*
_enumToStringBoolExprType(BoolExprType value) {
  switch(value) {
    case AND_EXPR: return "AND_EXPR";
    case OR_EXPR: return "OR_EXPR";
    case NOT_EXPR: return "NOT_EXPR";
  }
  return NULL;
}

static const char*
_enumToStringSubLinkType(SubLinkType value) {
  switch(value) {
    case EXISTS_SUBLINK: return "EXISTS_SUBLINK";
    case ALL_SUBLINK: return "ALL_SUBLINK";
    case ANY_SUBLINK: return "ANY_SUBLINK";
    case ROWCOMPARE_SUBLINK: return "ROWCOMPARE_SUBLINK";
    case EXPR_SUBLINK: return "EXPR_SUBLINK";
    case MULTIEXPR_SUBLINK: return "MULTIEXPR_SUBLINK";
    case ARRAY_SUBLINK: return "ARRAY_SUBLINK";
    case CTE_SUBLINK: return "CTE_SUBLINK";
  }
  return NULL;
}

static const char*
_enumToStringRowCompareType(RowCompareType value) {
  switch(value) {
    case ROWCOMPARE_LT: return "ROWCOMPARE_LT";
    case ROWCOMPARE_LE: return "ROWCOMPARE_LE";
    case ROWCOMPARE_EQ: return "ROWCOMPARE_EQ";
    case ROWCOMPARE_GE: return "ROWCOMPARE_GE";
    case ROWCOMPARE_GT: return "ROWCOMPARE_GT";
    case ROWCOMPARE_NE: return "ROWCOMPARE_NE";
  }
  return NULL;
}

static const char*
_enumToStringMinMaxOp(MinMaxOp value) {
  switch(value) {
    case IS_GREATEST: return "IS_GREATEST";
    case IS_LEAST: return "IS_LEAST";
  }
  return NULL;
}

static const char*
_enumToStringSQLValueFunctionOp(SQLValueFunctionOp value) {
  switch(value) {
    case SVFOP_CURRENT_DATE: return "SVFOP_CURRENT_DATE";
    case SVFOP_CURRENT_TIME: return "SVFOP_CURRENT_TIME";
    case SVFOP_CURRENT_TIME_N: return "SVFOP_CURRENT_TIME_N";
    case SVFOP_CURRENT_TIMESTAMP: return "SVFOP_CURRENT_TIMESTAMP";
    case SVFOP_CURRENT_TIMESTAMP_N: return "SVFOP_CURRENT_TIMESTAMP_N";
    case SVFOP_LOCALTIME: return "SVFOP_LOCALTIME";
    case SVFOP_LOCALTIME_N: return "SVFOP_LOCALTIME_N";
    case SVFOP_LOCALTIMESTAMP: return "SVFOP_LOCALTIMESTAMP";
    case SVFOP_LOCALTIMESTAMP_N: return "SVFOP_LOCALTIMESTAMP_N";
    case SVFOP_CURRENT_ROLE: return "SVFOP_CURRENT_ROLE";
    case SVFOP_CURRENT_USER: return "SVFOP_CURRENT_USER";
    case SVFOP_USER: return "SVFOP_USER";
    case SVFOP_SESSION_USER: return "SVFOP_SESSION_USER";
    case SVFOP_CURRENT_CATALOG: return "SVFOP_CURRENT_CATALOG";
    case SVFOP_CURRENT_SCHEMA: return "SVFOP_CURRENT_SCHEMA";
  }
  return NULL;
}

static const char*
_enumToStringXmlExprOp(XmlExprOp value) {
  switch(value) {
    case IS_XMLCONCAT: return "IS_XMLCONCAT";
    case IS_XMLELEMENT: return "IS_XMLELEMENT";
    case IS_XMLFOREST: return "IS_XMLFOREST";
    case IS_XMLPARSE: return "IS_XMLPARSE";
    case IS_XMLPI: return "IS_XMLPI";
    case IS_XMLROOT: return "IS_XMLROOT";
    case IS_XMLSERIALIZE: return "IS_XMLSERIALIZE";
    case IS_DOCUMENT: return "IS_DOCUMENT";
  }
  return NULL;
}

static const char*
_enumToStringXmlOptionType(XmlOptionType value) {
  switch(value) {
    case XMLOPTION_DOCUMENT: return "XMLOPTION_DOCUMENT";
    case XMLOPTION_CONTENT: return "XMLOPTION_CONTENT";
  }
  return NULL;
}

static const char*
_enumToStringNullTestType(NullTestType value) {
  switch(value) {
    case IS_NULL: return "IS_NULL";
    case IS_NOT_NULL: return "IS_NOT_NULL";
  }
  return NULL;
}

static const char*
_enumToStringBoolTestType(BoolTestType value) {
  switch(value) {
    case IS_TRUE: return "IS_TRUE";
    case IS_NOT_TRUE: return "IS_NOT_TRUE";
    case IS_FALSE: return "IS_FALSE";
    case IS_NOT_FALSE: return "IS_NOT_FALSE";
    case IS_UNKNOWN: return "IS_UNKNOWN";
    case IS_NOT_UNKNOWN: return "IS_NOT_UNKNOWN";
  }
  return NULL;
}

static const char*
_enumToStringCmdType(CmdType value) {
  switch(value) {
    case CMD_UNKNOWN: return "CMD_UNKNOWN";
    case CMD_SELECT: return "CMD_SELECT";
    case CMD_UPDATE: return "CMD_UPDATE";
    case CMD_INSERT: return "CMD_INSERT";
    case CMD_DELETE: return "CMD_DELETE";
    case CMD_UTILITY: return "CMD_UTILITY";
    case CMD_NOTHING: return "CMD_NOTHING";
  }
  return NULL;
}

static const char*
_enumToStringJoinType(JoinType value) {
  switch(value) {
    case JOIN_INNER: return "JOIN_INNER";
    case JOIN_LEFT: return "JOIN_LEFT";
    case JOIN_FULL: return "JOIN_FULL";
    case JOIN_RIGHT: return "JOIN_RIGHT";
    case JOIN_SEMI: return "JOIN_SEMI";
    case JOIN_ANTI: return "JOIN_ANTI";
    case JOIN_UNIQUE_OUTER: return "JOIN_UNIQUE_OUTER";
    case JOIN_UNIQUE_INNER: return "JOIN_UNIQUE_INNER";
  }
  return NULL;
}

static const char*
_enumToStringAggStrategy(AggStrategy value) {
  switch(value) {
    case AGG_PLAIN: return "AGG_PLAIN";
    case AGG_SORTED: return "AGG_SORTED";
    case AGG_HASHED: return "AGG_HASHED";
    case AGG_MIXED: return "AGG_MIXED";
  }
  return NULL;
}

static const char*
_enumToStringAggSplit(AggSplit value) {
  switch(value) {
    case AGGSPLIT_SIMPLE: return "AGGSPLIT_SIMPLE";
    case AGGSPLIT_INITIAL_SERIAL: return "AGGSPLIT_INITIAL_SERIAL";
    case AGGSPLIT_FINAL_DESERIAL: return "AGGSPLIT_FINAL_DESERIAL";
  }
  return NULL;
}

static const char*
_enumToStringSetOpCmd(SetOpCmd value) {
  switch(value) {
    case SETOPCMD_INTERSECT: return "SETOPCMD_INTERSECT";
    case SETOPCMD_INTERSECT_ALL: return "SETOPCMD_INTERSECT_ALL";
    case SETOPCMD_EXCEPT: return "SETOPCMD_EXCEPT";
    case SETOPCMD_EXCEPT_ALL: return "SETOPCMD_EXCEPT_ALL";
  }
  return NULL;
}

static const char*
_enumToStringSetOpStrategy(SetOpStrategy value) {
  switch(value) {
    case SETOP_SORTED: return "SETOP_SORTED";
    case SETOP_HASHED: return "SETOP_HASHED";
  }
  return NULL;
}

static const char*
_enumToStringOnConflictAction(OnConflictAction value) {
  switch(value) {
    case ONCONFLICT_NONE: return "ONCONFLICT_NONE";
    case ONCONFLICT_NOTHING: return "ONCONFLICT_NOTHING";
    case ONCONFLICT_UPDATE: return "ONCONFLICT_UPDATE";
  }
  return NULL;
}

static const char*
_enumToStringLimitOption(LimitOption value) {
  switch(value) {
    case LIMIT_OPTION_DEFAULT: return "LIMIT_OPTION_DEFAULT";
    case LIMIT_OPTION_COUNT: return "LIMIT_OPTION_COUNT";
    case LIMIT_OPTION_WITH_TIES: return "LIMIT_OPTION_WITH_TIES";
  }
  return NULL;
}

static const char*
_enumToStringLockClauseStrength(LockClauseStrength value) {
  switch(value) {
    case LCS_NONE: return "LCS_NONE";
    case LCS_FORKEYSHARE: return "LCS_FORKEYSHARE";
    case LCS_FORSHARE: return "LCS_FORSHARE";
    case LCS_FORNOKEYUPDATE: return "LCS_FORNOKEYUPDATE";
    case LCS_FORUPDATE: return "LCS_FORUPDATE";
  }
  return NULL;
}

static const char*
_enumToStringLockWaitPolicy(LockWaitPolicy value) {
  switch(value) {
    case LockWaitBlock: return "LockWaitBlock";
    case LockWaitSkip: return "LockWaitSkip";
    case LockWaitError: return "LockWaitError";
  }
  return NULL;
}

static const char*
_enumToStringLockTupleMode(LockTupleMode value) {
  switch(value) {
    case LockTupleKeyShare: return "LockTupleKeyShare";
    case LockTupleShare: return "LockTupleShare";
    case LockTupleNoKeyExclusive: return "LockTupleNoKeyExclusive";
    case LockTupleExclusive: return "LockTupleExclusive";
  }
  return NULL;
}static int
_enumToIntOverridingKind(OverridingKind value) {
  switch(value) {
    case OVERRIDING_NOT_SET: return 0;
    case OVERRIDING_USER_VALUE: return 1;
    case OVERRIDING_SYSTEM_VALUE: return 2;
  }
  return -1;
}

static int
_enumToIntQuerySource(QuerySource value) {
  switch(value) {
    case QSRC_ORIGINAL: return 0;
    case QSRC_PARSER: return 1;
    case QSRC_INSTEAD_RULE: return 2;
    case QSRC_QUAL_INSTEAD_RULE: return 3;
    case QSRC_NON_INSTEAD_RULE: return 4;
  }
  return -1;
}

static int
_enumToIntSortByDir(SortByDir value) {
  switch(value) {
    case SORTBY_DEFAULT: return 0;
    case SORTBY_ASC: return 1;
    case SORTBY_DESC: return 2;
    case SORTBY_USING: return 3;
  }
  return -1;
}

static int
_enumToIntSortByNulls(SortByNulls value) {
  switch(value) {
    case SORTBY_NULLS_DEFAULT: return 0;
    case SORTBY_NULLS_FIRST: return 1;
    case SORTBY_NULLS_LAST: return 2;
  }
  return -1;
}

static int
_enumToIntA_Expr_Kind(A_Expr_Kind value) {
  switch(value) {
    case AEXPR_OP: return 0;
    case AEXPR_OP_ANY: return 1;
    case AEXPR_OP_ALL: return 2;
    case AEXPR_DISTINCT: return 3;
    case AEXPR_NOT_DISTINCT: return 4;
    case AEXPR_NULLIF: return 5;
    case AEXPR_OF: return 6;
    case AEXPR_IN: return 7;
    case AEXPR_LIKE: return 8;
    case AEXPR_ILIKE: return 9;
    case AEXPR_SIMILAR: return 10;
    case AEXPR_BETWEEN: return 11;
    case AEXPR_NOT_BETWEEN: return 12;
    case AEXPR_BETWEEN_SYM: return 13;
    case AEXPR_NOT_BETWEEN_SYM: return 14;
    case AEXPR_PAREN: return 15;
  }
  return -1;
}

static int
_enumToIntRoleSpecType(RoleSpecType value) {
  switch(value) {
    case ROLESPEC_CSTRING: return 0;
    case ROLESPEC_CURRENT_USER: return 1;
    case ROLESPEC_SESSION_USER: return 2;
    case ROLESPEC_PUBLIC: return 3;
  }
  return -1;
}

static int
_enumToIntTableLikeOption(TableLikeOption value) {
  switch(value) {
    case CREATE_TABLE_LIKE_COMMENTS: return 0;
    case CREATE_TABLE_LIKE_CONSTRAINTS: return 1;
    case CREATE_TABLE_LIKE_DEFAULTS: return 2;
    case CREATE_TABLE_LIKE_GENERATED: return 3;
    case CREATE_TABLE_LIKE_IDENTITY: return 4;
    case CREATE_TABLE_LIKE_INDEXES: return 5;
    case CREATE_TABLE_LIKE_STATISTICS: return 6;
    case CREATE_TABLE_LIKE_STORAGE: return 7;
    case CREATE_TABLE_LIKE_ALL: return 8;
  }
  return -1;
}

static int
_enumToIntDefElemAction(DefElemAction value) {
  switch(value) {
    case DEFELEM_UNSPEC: return 0;
    case DEFELEM_SET: return 1;
    case DEFELEM_ADD: return 2;
    case DEFELEM_DROP: return 3;
  }
  return -1;
}

static int
_enumToIntPartitionRangeDatumKind(PartitionRangeDatumKind value) {
  switch(value) {
    case PARTITION_RANGE_DATUM_MINVALUE: return 0;
    case PARTITION_RANGE_DATUM_VALUE: return 1;
    case PARTITION_RANGE_DATUM_MAXVALUE: return 2;
  }
  return -1;
}

static int
_enumToIntRTEKind(RTEKind value) {
  switch(value) {
    case RTE_RELATION: return 0;
    case RTE_SUBQUERY: return 1;
    case RTE_JOIN: return 2;
    case RTE_FUNCTION: return 3;
    case RTE_TABLEFUNC: return 4;
    case RTE_VALUES: return 5;
    case RTE_CTE: return 6;
    case RTE_NAMEDTUPLESTORE: return 7;
    case RTE_RESULT: return 8;
  }
  return -1;
}

static int
_enumToIntWCOKind(WCOKind value) {
  switch(value) {
    case WCO_VIEW_CHECK: return 0;
    case WCO_RLS_INSERT_CHECK: return 1;
    case WCO_RLS_UPDATE_CHECK: return 2;
    case WCO_RLS_CONFLICT_CHECK: return 3;
  }
  return -1;
}

static int
_enumToIntGroupingSetKind(GroupingSetKind value) {
  switch(value) {
    case GROUPING_SET_EMPTY: return 0;
    case GROUPING_SET_SIMPLE: return 1;
    case GROUPING_SET_ROLLUP: return 2;
    case GROUPING_SET_CUBE: return 3;
    case GROUPING_SET_SETS: return 4;
  }
  return -1;
}

static int
_enumToIntCTEMaterialize(CTEMaterialize value) {
  switch(value) {
    case CTEMaterializeDefault: return 0;
    case CTEMaterializeAlways: return 1;
    case CTEMaterializeNever: return 2;
  }
  return -1;
}

static int
_enumToIntSetOperation(SetOperation value) {
  switch(value) {
    case SETOP_NONE: return 0;
    case SETOP_UNION: return 1;
    case SETOP_INTERSECT: return 2;
    case SETOP_EXCEPT: return 3;
  }
  return -1;
}

static int
_enumToIntObjectType(ObjectType value) {
  switch(value) {
    case OBJECT_ACCESS_METHOD: return 0;
    case OBJECT_AGGREGATE: return 1;
    case OBJECT_AMOP: return 2;
    case OBJECT_AMPROC: return 3;
    case OBJECT_ATTRIBUTE: return 4;
    case OBJECT_CAST: return 5;
    case OBJECT_COLUMN: return 6;
    case OBJECT_COLLATION: return 7;
    case OBJECT_CONVERSION: return 8;
    case OBJECT_DATABASE: return 9;
    case OBJECT_DEFAULT: return 10;
    case OBJECT_DEFACL: return 11;
    case OBJECT_DOMAIN: return 12;
    case OBJECT_DOMCONSTRAINT: return 13;
    case OBJECT_EVENT_TRIGGER: return 14;
    case OBJECT_EXTENSION: return 15;
    case OBJECT_FDW: return 16;
    case OBJECT_FOREIGN_SERVER: return 17;
    case OBJECT_FOREIGN_TABLE: return 18;
    case OBJECT_FUNCTION: return 19;
    case OBJECT_INDEX: return 20;
    case OBJECT_LANGUAGE: return 21;
    case OBJECT_LARGEOBJECT: return 22;
    case OBJECT_MATVIEW: return 23;
    case OBJECT_OPCLASS: return 24;
    case OBJECT_OPERATOR: return 25;
    case OBJECT_OPFAMILY: return 26;
    case OBJECT_POLICY: return 27;
    case OBJECT_PROCEDURE: return 28;
    case OBJECT_PUBLICATION: return 29;
    case OBJECT_PUBLICATION_REL: return 30;
    case OBJECT_ROLE: return 31;
    case OBJECT_ROUTINE: return 32;
    case OBJECT_RULE: return 33;
    case OBJECT_SCHEMA: return 34;
    case OBJECT_SEQUENCE: return 35;
    case OBJECT_SUBSCRIPTION: return 36;
    case OBJECT_STATISTIC_EXT: return 37;
    case OBJECT_TABCONSTRAINT: return 38;
    case OBJECT_TABLE: return 39;
    case OBJECT_TABLESPACE: return 40;
    case OBJECT_TRANSFORM: return 41;
    case OBJECT_TRIGGER: return 42;
    case OBJECT_TSCONFIGURATION: return 43;
    case OBJECT_TSDICTIONARY: return 44;
    case OBJECT_TSPARSER: return 45;
    case OBJECT_TSTEMPLATE: return 46;
    case OBJECT_TYPE: return 47;
    case OBJECT_USER_MAPPING: return 48;
    case OBJECT_VIEW: return 49;
  }
  return -1;
}

static int
_enumToIntDropBehavior(DropBehavior value) {
  switch(value) {
    case DROP_RESTRICT: return 0;
    case DROP_CASCADE: return 1;
  }
  return -1;
}

static int
_enumToIntAlterTableType(AlterTableType value) {
  switch(value) {
    case AT_AddColumn: return 0;
    case AT_AddColumnRecurse: return 1;
    case AT_AddColumnToView: return 2;
    case AT_ColumnDefault: return 3;
    case AT_CookedColumnDefault: return 4;
    case AT_DropNotNull: return 5;
    case AT_SetNotNull: return 6;
    case AT_DropExpression: return 7;
    case AT_CheckNotNull: return 8;
    case AT_SetStatistics: return 9;
    case AT_SetOptions: return 10;
    case AT_ResetOptions: return 11;
    case AT_SetStorage: return 12;
    case AT_DropColumn: return 13;
    case AT_DropColumnRecurse: return 14;
    case AT_AddIndex: return 15;
    case AT_ReAddIndex: return 16;
    case AT_AddConstraint: return 17;
    case AT_AddConstraintRecurse: return 18;
    case AT_ReAddConstraint: return 19;
    case AT_ReAddDomainConstraint: return 20;
    case AT_AlterConstraint: return 21;
    case AT_ValidateConstraint: return 22;
    case AT_ValidateConstraintRecurse: return 23;
    case AT_AddIndexConstraint: return 24;
    case AT_DropConstraint: return 25;
    case AT_DropConstraintRecurse: return 26;
    case AT_ReAddComment: return 27;
    case AT_AlterColumnType: return 28;
    case AT_AlterColumnGenericOptions: return 29;
    case AT_ChangeOwner: return 30;
    case AT_ClusterOn: return 31;
    case AT_DropCluster: return 32;
    case AT_SetLogged: return 33;
    case AT_SetUnLogged: return 34;
    case AT_DropOids: return 35;
    case AT_SetTableSpace: return 36;
    case AT_SetRelOptions: return 37;
    case AT_ResetRelOptions: return 38;
    case AT_ReplaceRelOptions: return 39;
    case AT_EnableTrig: return 40;
    case AT_EnableAlwaysTrig: return 41;
    case AT_EnableReplicaTrig: return 42;
    case AT_DisableTrig: return 43;
    case AT_EnableTrigAll: return 44;
    case AT_DisableTrigAll: return 45;
    case AT_EnableTrigUser: return 46;
    case AT_DisableTrigUser: return 47;
    case AT_EnableRule: return 48;
    case AT_EnableAlwaysRule: return 49;
    case AT_EnableReplicaRule: return 50;
    case AT_DisableRule: return 51;
    case AT_AddInherit: return 52;
    case AT_DropInherit: return 53;
    case AT_AddOf: return 54;
    case AT_DropOf: return 55;
    case AT_ReplicaIdentity: return 56;
    case AT_EnableRowSecurity: return 57;
    case AT_DisableRowSecurity: return 58;
    case AT_ForceRowSecurity: return 59;
    case AT_NoForceRowSecurity: return 60;
    case AT_GenericOptions: return 61;
    case AT_AttachPartition: return 62;
    case AT_DetachPartition: return 63;
    case AT_AddIdentity: return 64;
    case AT_SetIdentity: return 65;
    case AT_DropIdentity: return 66;
  }
  return -1;
}

static int
_enumToIntGrantTargetType(GrantTargetType value) {
  switch(value) {
    case ACL_TARGET_OBJECT: return 0;
    case ACL_TARGET_ALL_IN_SCHEMA: return 1;
    case ACL_TARGET_DEFAULTS: return 2;
  }
  return -1;
}

static int
_enumToIntVariableSetKind(VariableSetKind value) {
  switch(value) {
    case VAR_SET_VALUE: return 0;
    case VAR_SET_DEFAULT: return 1;
    case VAR_SET_CURRENT: return 2;
    case VAR_SET_MULTI: return 3;
    case VAR_RESET: return 4;
    case VAR_RESET_ALL: return 5;
  }
  return -1;
}

static int
_enumToIntConstrType(ConstrType value) {
  switch(value) {
    case CONSTR_NULL: return 0;
    case CONSTR_NOTNULL: return 1;
    case CONSTR_DEFAULT: return 2;
    case CONSTR_IDENTITY: return 3;
    case CONSTR_GENERATED: return 4;
    case CONSTR_CHECK: return 5;
    case CONSTR_PRIMARY: return 6;
    case CONSTR_UNIQUE: return 7;
    case CONSTR_EXCLUSION: return 8;
    case CONSTR_FOREIGN: return 9;
    case CONSTR_ATTR_DEFERRABLE: return 10;
    case CONSTR_ATTR_NOT_DEFERRABLE: return 11;
    case CONSTR_ATTR_DEFERRED: return 12;
    case CONSTR_ATTR_IMMEDIATE: return 13;
  }
  return -1;
}

static int
_enumToIntImportForeignSchemaType(ImportForeignSchemaType value) {
  switch(value) {
    case FDW_IMPORT_SCHEMA_ALL: return 0;
    case FDW_IMPORT_SCHEMA_LIMIT_TO: return 1;
    case FDW_IMPORT_SCHEMA_EXCEPT: return 2;
  }
  return -1;
}

static int
_enumToIntRoleStmtType(RoleStmtType value) {
  switch(value) {
    case ROLESTMT_ROLE: return 0;
    case ROLESTMT_USER: return 1;
    case ROLESTMT_GROUP: return 2;
  }
  return -1;
}

static int
_enumToIntFetchDirection(FetchDirection value) {
  switch(value) {
    case FETCH_FORWARD: return 0;
    case FETCH_BACKWARD: return 1;
    case FETCH_ABSOLUTE: return 2;
    case FETCH_RELATIVE: return 3;
  }
  return -1;
}

static int
_enumToIntFunctionParameterMode(FunctionParameterMode value) {
  switch(value) {
    case FUNC_PARAM_IN: return 0;
    case FUNC_PARAM_OUT: return 1;
    case FUNC_PARAM_INOUT: return 2;
    case FUNC_PARAM_VARIADIC: return 3;
    case FUNC_PARAM_TABLE: return 4;
  }
  return -1;
}

static int
_enumToIntTransactionStmtKind(TransactionStmtKind value) {
  switch(value) {
    case TRANS_STMT_BEGIN: return 0;
    case TRANS_STMT_START: return 1;
    case TRANS_STMT_COMMIT: return 2;
    case TRANS_STMT_ROLLBACK: return 3;
    case TRANS_STMT_SAVEPOINT: return 4;
    case TRANS_STMT_RELEASE: return 5;
    case TRANS_STMT_ROLLBACK_TO: return 6;
    case TRANS_STMT_PREPARE: return 7;
    case TRANS_STMT_COMMIT_PREPARED: return 8;
    case TRANS_STMT_ROLLBACK_PREPARED: return 9;
  }
  return -1;
}

static int
_enumToIntViewCheckOption(ViewCheckOption value) {
  switch(value) {
    case NO_CHECK_OPTION: return 0;
    case LOCAL_CHECK_OPTION: return 1;
    case CASCADED_CHECK_OPTION: return 2;
  }
  return -1;
}

static int
_enumToIntClusterOption(ClusterOption value) {
  switch(value) {
    case CLUOPT_RECHECK: return 0;
    case CLUOPT_VERBOSE: return 1;
  }
  return -1;
}

static int
_enumToIntDiscardMode(DiscardMode value) {
  switch(value) {
    case DISCARD_ALL: return 0;
    case DISCARD_PLANS: return 1;
    case DISCARD_SEQUENCES: return 2;
    case DISCARD_TEMP: return 3;
  }
  return -1;
}

static int
_enumToIntReindexObjectType(ReindexObjectType value) {
  switch(value) {
    case REINDEX_OBJECT_INDEX: return 0;
    case REINDEX_OBJECT_TABLE: return 1;
    case REINDEX_OBJECT_SCHEMA: return 2;
    case REINDEX_OBJECT_SYSTEM: return 3;
    case REINDEX_OBJECT_DATABASE: return 4;
  }
  return -1;
}

static int
_enumToIntAlterTSConfigType(AlterTSConfigType value) {
  switch(value) {
    case ALTER_TSCONFIG_ADD_MAPPING: return 0;
    case ALTER_TSCONFIG_ALTER_MAPPING_FOR_TOKEN: return 1;
    case ALTER_TSCONFIG_REPLACE_DICT: return 2;
    case ALTER_TSCONFIG_REPLACE_DICT_FOR_TOKEN: return 3;
    case ALTER_TSCONFIG_DROP_MAPPING: return 4;
  }
  return -1;
}

static int
_enumToIntAlterSubscriptionType(AlterSubscriptionType value) {
  switch(value) {
    case ALTER_SUBSCRIPTION_OPTIONS: return 0;
    case ALTER_SUBSCRIPTION_CONNECTION: return 1;
    case ALTER_SUBSCRIPTION_PUBLICATION: return 2;
    case ALTER_SUBSCRIPTION_REFRESH: return 3;
    case ALTER_SUBSCRIPTION_ENABLED: return 4;
  }
  return -1;
}

static int
_enumToIntOnCommitAction(OnCommitAction value) {
  switch(value) {
    case ONCOMMIT_NOOP: return 0;
    case ONCOMMIT_PRESERVE_ROWS: return 1;
    case ONCOMMIT_DELETE_ROWS: return 2;
    case ONCOMMIT_DROP: return 3;
  }
  return -1;
}

static int
_enumToIntParamKind(ParamKind value) {
  switch(value) {
    case PARAM_EXTERN: return 0;
    case PARAM_EXEC: return 1;
    case PARAM_SUBLINK: return 2;
    case PARAM_MULTIEXPR: return 3;
  }
  return -1;
}

static int
_enumToIntCoercionContext(CoercionContext value) {
  switch(value) {
    case COERCION_IMPLICIT: return 0;
    case COERCION_ASSIGNMENT: return 1;
    case COERCION_EXPLICIT: return 2;
  }
  return -1;
}

static int
_enumToIntCoercionForm(CoercionForm value) {
  switch(value) {
    case COERCE_EXPLICIT_CALL: return 0;
    case COERCE_EXPLICIT_CAST: return 1;
    case COERCE_IMPLICIT_CAST: return 2;
  }
  return -1;
}

static int
_enumToIntBoolExprType(BoolExprType value) {
  switch(value) {
    case AND_EXPR: return 0;
    case OR_EXPR: return 1;
    case NOT_EXPR: return 2;
  }
  return -1;
}

static int
_enumToIntSubLinkType(SubLinkType value) {
  switch(value) {
    case EXISTS_SUBLINK: return 0;
    case ALL_SUBLINK: return 1;
    case ANY_SUBLINK: return 2;
    case ROWCOMPARE_SUBLINK: return 3;
    case EXPR_SUBLINK: return 4;
    case MULTIEXPR_SUBLINK: return 5;
    case ARRAY_SUBLINK: return 6;
    case CTE_SUBLINK: return 7;
  }
  return -1;
}

static int
_enumToIntRowCompareType(RowCompareType value) {
  switch(value) {
    case ROWCOMPARE_LT: return 0;
    case ROWCOMPARE_LE: return 1;
    case ROWCOMPARE_EQ: return 2;
    case ROWCOMPARE_GE: return 3;
    case ROWCOMPARE_GT: return 4;
    case ROWCOMPARE_NE: return 5;
  }
  return -1;
}

static int
_enumToIntMinMaxOp(MinMaxOp value) {
  switch(value) {
    case IS_GREATEST: return 0;
    case IS_LEAST: return 1;
  }
  return -1;
}

static int
_enumToIntSQLValueFunctionOp(SQLValueFunctionOp value) {
  switch(value) {
    case SVFOP_CURRENT_DATE: return 0;
    case SVFOP_CURRENT_TIME: return 1;
    case SVFOP_CURRENT_TIME_N: return 2;
    case SVFOP_CURRENT_TIMESTAMP: return 3;
    case SVFOP_CURRENT_TIMESTAMP_N: return 4;
    case SVFOP_LOCALTIME: return 5;
    case SVFOP_LOCALTIME_N: return 6;
    case SVFOP_LOCALTIMESTAMP: return 7;
    case SVFOP_LOCALTIMESTAMP_N: return 8;
    case SVFOP_CURRENT_ROLE: return 9;
    case SVFOP_CURRENT_USER: return 10;
    case SVFOP_USER: return 11;
    case SVFOP_SESSION_USER: return 12;
    case SVFOP_CURRENT_CATALOG: return 13;
    case SVFOP_CURRENT_SCHEMA: return 14;
  }
  return -1;
}

static int
_enumToIntXmlExprOp(XmlExprOp value) {
  switch(value) {
    case IS_XMLCONCAT: return 0;
    case IS_XMLELEMENT: return 1;
    case IS_XMLFOREST: return 2;
    case IS_XMLPARSE: return 3;
    case IS_XMLPI: return 4;
    case IS_XMLROOT: return 5;
    case IS_XMLSERIALIZE: return 6;
    case IS_DOCUMENT: return 7;
  }
  return -1;
}

static int
_enumToIntXmlOptionType(XmlOptionType value) {
  switch(value) {
    case XMLOPTION_DOCUMENT: return 0;
    case XMLOPTION_CONTENT: return 1;
  }
  return -1;
}

static int
_enumToIntNullTestType(NullTestType value) {
  switch(value) {
    case IS_NULL: return 0;
    case IS_NOT_NULL: return 1;
  }
  return -1;
}

static int
_enumToIntBoolTestType(BoolTestType value) {
  switch(value) {
    case IS_TRUE: return 0;
    case IS_NOT_TRUE: return 1;
    case IS_FALSE: return 2;
    case IS_NOT_FALSE: return 3;
    case IS_UNKNOWN: return 4;
    case IS_NOT_UNKNOWN: return 5;
  }
  return -1;
}

static int
_enumToIntCmdType(CmdType value) {
  switch(value) {
    case CMD_UNKNOWN: return 0;
    case CMD_SELECT: return 1;
    case CMD_UPDATE: return 2;
    case CMD_INSERT: return 3;
    case CMD_DELETE: return 4;
    case CMD_UTILITY: return 5;
    case CMD_NOTHING: return 6;
  }
  return -1;
}

static int
_enumToIntJoinType(JoinType value) {
  switch(value) {
    case JOIN_INNER: return 0;
    case JOIN_LEFT: return 1;
    case JOIN_FULL: return 2;
    case JOIN_RIGHT: return 3;
    case JOIN_SEMI: return 4;
    case JOIN_ANTI: return 5;
    case JOIN_UNIQUE_OUTER: return 6;
    case JOIN_UNIQUE_INNER: return 7;
  }
  return -1;
}

static int
_enumToIntAggStrategy(AggStrategy value) {
  switch(value) {
    case AGG_PLAIN: return 0;
    case AGG_SORTED: return 1;
    case AGG_HASHED: return 2;
    case AGG_MIXED: return 3;
  }
  return -1;
}

static int
_enumToIntAggSplit(AggSplit value) {
  switch(value) {
    case AGGSPLIT_SIMPLE: return 0;
    case AGGSPLIT_INITIAL_SERIAL: return 1;
    case AGGSPLIT_FINAL_DESERIAL: return 2;
  }
  return -1;
}

static int
_enumToIntSetOpCmd(SetOpCmd value) {
  switch(value) {
    case SETOPCMD_INTERSECT: return 0;
    case SETOPCMD_INTERSECT_ALL: return 1;
    case SETOPCMD_EXCEPT: return 2;
    case SETOPCMD_EXCEPT_ALL: return 3;
  }
  return -1;
}

static int
_enumToIntSetOpStrategy(SetOpStrategy value) {
  switch(value) {
    case SETOP_SORTED: return 0;
    case SETOP_HASHED: return 1;
  }
  return -1;
}

static int
_enumToIntOnConflictAction(OnConflictAction value) {
  switch(value) {
    case ONCONFLICT_NONE: return 0;
    case ONCONFLICT_NOTHING: return 1;
    case ONCONFLICT_UPDATE: return 2;
  }
  return -1;
}

static int
_enumToIntLimitOption(LimitOption value) {
  switch(value) {
    case LIMIT_OPTION_DEFAULT: return 0;
    case LIMIT_OPTION_COUNT: return 1;
    case LIMIT_OPTION_WITH_TIES: return 2;
  }
  return -1;
}

static int
_enumToIntLockClauseStrength(LockClauseStrength value) {
  switch(value) {
    case LCS_NONE: return 0;
    case LCS_FORKEYSHARE: return 1;
    case LCS_FORSHARE: return 2;
    case LCS_FORNOKEYUPDATE: return 3;
    case LCS_FORUPDATE: return 4;
  }
  return -1;
}

static int
_enumToIntLockWaitPolicy(LockWaitPolicy value) {
  switch(value) {
    case LockWaitBlock: return 0;
    case LockWaitSkip: return 1;
    case LockWaitError: return 2;
  }
  return -1;
}

static int
_enumToIntLockTupleMode(LockTupleMode value) {
  switch(value) {
    case LockTupleKeyShare: return 0;
    case LockTupleShare: return 1;
    case LockTupleNoKeyExclusive: return 2;
    case LockTupleExclusive: return 3;
  }
  return -1;
}static void
_outAlias(OUT_TYPE(Alias) out, const Alias *node)
{
  WRITE_STRING_FIELD(aliasname, aliasname, aliasname);
  WRITE_LIST_FIELD(colnames, colnames, colnames);
}

static void
_outRangeVar(OUT_TYPE(RangeVar) out, const RangeVar *node)
{
  WRITE_STRING_FIELD(catalogname, catalogname, catalogname);
  WRITE_STRING_FIELD(schemaname, schemaname, schemaname);
  WRITE_STRING_FIELD(relname, relname, relname);
  WRITE_BOOL_FIELD(inh, inh, inh);
  WRITE_CHAR_FIELD(relpersistence, relpersistence, relpersistence);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outTableFunc(OUT_TYPE(TableFunc) out, const TableFunc *node)
{
  WRITE_LIST_FIELD(ns_uris, ns_uris, ns_uris);
  WRITE_LIST_FIELD(ns_names, ns_names, ns_names);
  WRITE_NODE_PTR_FIELD(docexpr, docexpr, docexpr);
  WRITE_NODE_PTR_FIELD(rowexpr, rowexpr, rowexpr);
  WRITE_LIST_FIELD(colnames, colnames, colnames);
  WRITE_LIST_FIELD(coltypes, coltypes, coltypes);
  WRITE_LIST_FIELD(coltypmods, coltypmods, coltypmods);
  WRITE_LIST_FIELD(colcollations, colcollations, colcollations);
  WRITE_LIST_FIELD(colexprs, colexprs, colexprs);
  WRITE_LIST_FIELD(coldefexprs, coldefexprs, coldefexprs);
  WRITE_BITMAPSET_FIELD(notnulls, notnulls, notnulls);
  WRITE_INT_FIELD(ordinalitycol, ordinalitycol, ordinalitycol);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outExpr(OUT_TYPE(Expr) out, const Expr *node)
{
}

static void
_outVar(OUT_TYPE(Var) out, const Var *node)
{
  WRITE_UINT_FIELD(varno, varno, varno);
  WRITE_INT_FIELD(varattno, varattno, varattno);
  WRITE_UINT_FIELD(vartype, vartype, vartype);
  WRITE_INT_FIELD(vartypmod, vartypmod, vartypmod);
  WRITE_UINT_FIELD(varcollid, varcollid, varcollid);
  WRITE_UINT_FIELD(varlevelsup, varlevelsup, varlevelsup);
  WRITE_UINT_FIELD(varnosyn, varnosyn, varnosyn);
  WRITE_INT_FIELD(varattnosyn, varattnosyn, varattnosyn);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outParam(OUT_TYPE(Param) out, const Param *node)
{
  WRITE_ENUM_FIELD(ParamKind, paramkind, paramkind, paramkind);
  WRITE_INT_FIELD(paramid, paramid, paramid);
  WRITE_UINT_FIELD(paramtype, paramtype, paramtype);
  WRITE_INT_FIELD(paramtypmod, paramtypmod, paramtypmod);
  WRITE_UINT_FIELD(paramcollid, paramcollid, paramcollid);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outAggref(OUT_TYPE(Aggref) out, const Aggref *node)
{
  WRITE_UINT_FIELD(aggfnoid, aggfnoid, aggfnoid);
  WRITE_UINT_FIELD(aggtype, aggtype, aggtype);
  WRITE_UINT_FIELD(aggcollid, aggcollid, aggcollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_UINT_FIELD(aggtranstype, aggtranstype, aggtranstype);
  WRITE_LIST_FIELD(aggargtypes, aggargtypes, aggargtypes);
  WRITE_LIST_FIELD(aggdirectargs, aggdirectargs, aggdirectargs);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_LIST_FIELD(aggorder, aggorder, aggorder);
  WRITE_LIST_FIELD(aggdistinct, aggdistinct, aggdistinct);
  WRITE_NODE_PTR_FIELD(aggfilter, aggfilter, aggfilter);
  WRITE_BOOL_FIELD(aggstar, aggstar, aggstar);
  WRITE_BOOL_FIELD(aggvariadic, aggvariadic, aggvariadic);
  WRITE_CHAR_FIELD(aggkind, aggkind, aggkind);
  WRITE_UINT_FIELD(agglevelsup, agglevelsup, agglevelsup);
  WRITE_ENUM_FIELD(AggSplit, aggsplit, aggsplit, aggsplit);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outGroupingFunc(OUT_TYPE(GroupingFunc) out, const GroupingFunc *node)
{
  WRITE_LIST_FIELD(args, args, args);
  WRITE_LIST_FIELD(refs, refs, refs);
  WRITE_LIST_FIELD(cols, cols, cols);
  WRITE_UINT_FIELD(agglevelsup, agglevelsup, agglevelsup);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outWindowFunc(OUT_TYPE(WindowFunc) out, const WindowFunc *node)
{
  WRITE_UINT_FIELD(winfnoid, winfnoid, winfnoid);
  WRITE_UINT_FIELD(wintype, wintype, wintype);
  WRITE_UINT_FIELD(wincollid, wincollid, wincollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_NODE_PTR_FIELD(aggfilter, aggfilter, aggfilter);
  WRITE_UINT_FIELD(winref, winref, winref);
  WRITE_BOOL_FIELD(winstar, winstar, winstar);
  WRITE_BOOL_FIELD(winagg, winagg, winagg);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSubscriptingRef(OUT_TYPE(SubscriptingRef) out, const SubscriptingRef *node)
{
  WRITE_UINT_FIELD(refcontainertype, refcontainertype, refcontainertype);
  WRITE_UINT_FIELD(refelemtype, refelemtype, refelemtype);
  WRITE_INT_FIELD(reftypmod, reftypmod, reftypmod);
  WRITE_UINT_FIELD(refcollid, refcollid, refcollid);
  WRITE_LIST_FIELD(refupperindexpr, refupperindexpr, refupperindexpr);
  WRITE_LIST_FIELD(reflowerindexpr, reflowerindexpr, reflowerindexpr);
  WRITE_NODE_PTR_FIELD(refexpr, refexpr, refexpr);
  WRITE_NODE_PTR_FIELD(refassgnexpr, refassgnexpr, refassgnexpr);
}

static void
_outFuncExpr(OUT_TYPE(FuncExpr) out, const FuncExpr *node)
{
  WRITE_UINT_FIELD(funcid, funcid, funcid);
  WRITE_UINT_FIELD(funcresulttype, funcresulttype, funcresulttype);
  WRITE_BOOL_FIELD(funcretset, funcretset, funcretset);
  WRITE_BOOL_FIELD(funcvariadic, funcvariadic, funcvariadic);
  WRITE_ENUM_FIELD(CoercionForm, funcformat, funcformat, funcformat);
  WRITE_UINT_FIELD(funccollid, funccollid, funccollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outNamedArgExpr(OUT_TYPE(NamedArgExpr) out, const NamedArgExpr *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_INT_FIELD(argnumber, argnumber, argnumber);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outOpExpr(OUT_TYPE(OpExpr) out, const OpExpr *node)
{
  WRITE_UINT_FIELD(opno, opno, opno);
  WRITE_UINT_FIELD(opfuncid, opfuncid, opfuncid);
  WRITE_UINT_FIELD(opresulttype, opresulttype, opresulttype);
  WRITE_BOOL_FIELD(opretset, opretset, opretset);
  WRITE_UINT_FIELD(opcollid, opcollid, opcollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outDistinctExpr(OUT_TYPE(DistinctExpr) out, const DistinctExpr *node)
{
  WRITE_UINT_FIELD(opno, opno, opno);
  WRITE_UINT_FIELD(opfuncid, opfuncid, opfuncid);
  WRITE_UINT_FIELD(opresulttype, opresulttype, opresulttype);
  WRITE_BOOL_FIELD(opretset, opretset, opretset);
  WRITE_UINT_FIELD(opcollid, opcollid, opcollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outNullIfExpr(OUT_TYPE(NullIfExpr) out, const NullIfExpr *node)
{
  WRITE_UINT_FIELD(opno, opno, opno);
  WRITE_UINT_FIELD(opfuncid, opfuncid, opfuncid);
  WRITE_UINT_FIELD(opresulttype, opresulttype, opresulttype);
  WRITE_BOOL_FIELD(opretset, opretset, opretset);
  WRITE_UINT_FIELD(opcollid, opcollid, opcollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outScalarArrayOpExpr(OUT_TYPE(ScalarArrayOpExpr) out, const ScalarArrayOpExpr *node)
{
  WRITE_UINT_FIELD(opno, opno, opno);
  WRITE_UINT_FIELD(opfuncid, opfuncid, opfuncid);
  WRITE_BOOL_FIELD(use_or, useOr, useOr);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outBoolExpr(OUT_TYPE(BoolExpr) out, const BoolExpr *node)
{
  WRITE_ENUM_FIELD(BoolExprType, boolop, boolop, boolop);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSubLink(OUT_TYPE(SubLink) out, const SubLink *node)
{
  WRITE_ENUM_FIELD(SubLinkType, sub_link_type, subLinkType, subLinkType);
  WRITE_INT_FIELD(sub_link_id, subLinkId, subLinkId);
  WRITE_NODE_PTR_FIELD(testexpr, testexpr, testexpr);
  WRITE_LIST_FIELD(oper_name, operName, operName);
  WRITE_NODE_PTR_FIELD(subselect, subselect, subselect);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSubPlan(OUT_TYPE(SubPlan) out, const SubPlan *node)
{
  WRITE_ENUM_FIELD(SubLinkType, sub_link_type, subLinkType, subLinkType);
  WRITE_NODE_PTR_FIELD(testexpr, testexpr, testexpr);
  WRITE_LIST_FIELD(param_ids, paramIds, paramIds);
  WRITE_INT_FIELD(plan_id, plan_id, plan_id);
  WRITE_STRING_FIELD(plan_name, plan_name, plan_name);
  WRITE_UINT_FIELD(first_col_type, firstColType, firstColType);
  WRITE_INT_FIELD(first_col_typmod, firstColTypmod, firstColTypmod);
  WRITE_UINT_FIELD(first_col_collation, firstColCollation, firstColCollation);
  WRITE_BOOL_FIELD(use_hash_table, useHashTable, useHashTable);
  WRITE_BOOL_FIELD(unknown_eq_false, unknownEqFalse, unknownEqFalse);
  WRITE_BOOL_FIELD(parallel_safe, parallel_safe, parallel_safe);
  WRITE_LIST_FIELD(set_param, setParam, setParam);
  WRITE_LIST_FIELD(par_param, parParam, parParam);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_FLOAT_FIELD(startup_cost, startup_cost, startup_cost);
  WRITE_FLOAT_FIELD(per_call_cost, per_call_cost, per_call_cost);
}

static void
_outAlternativeSubPlan(OUT_TYPE(AlternativeSubPlan) out, const AlternativeSubPlan *node)
{
  WRITE_LIST_FIELD(subplans, subplans, subplans);
}

static void
_outFieldSelect(OUT_TYPE(FieldSelect) out, const FieldSelect *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_INT_FIELD(fieldnum, fieldnum, fieldnum);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_INT_FIELD(resulttypmod, resulttypmod, resulttypmod);
  WRITE_UINT_FIELD(resultcollid, resultcollid, resultcollid);
}

static void
_outFieldStore(OUT_TYPE(FieldStore) out, const FieldStore *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_LIST_FIELD(newvals, newvals, newvals);
  WRITE_LIST_FIELD(fieldnums, fieldnums, fieldnums);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
}

static void
_outRelabelType(OUT_TYPE(RelabelType) out, const RelabelType *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_INT_FIELD(resulttypmod, resulttypmod, resulttypmod);
  WRITE_UINT_FIELD(resultcollid, resultcollid, resultcollid);
  WRITE_ENUM_FIELD(CoercionForm, relabelformat, relabelformat, relabelformat);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCoerceViaIO(OUT_TYPE(CoerceViaIO) out, const CoerceViaIO *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_UINT_FIELD(resultcollid, resultcollid, resultcollid);
  WRITE_ENUM_FIELD(CoercionForm, coerceformat, coerceformat, coerceformat);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outArrayCoerceExpr(OUT_TYPE(ArrayCoerceExpr) out, const ArrayCoerceExpr *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_NODE_PTR_FIELD(elemexpr, elemexpr, elemexpr);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_INT_FIELD(resulttypmod, resulttypmod, resulttypmod);
  WRITE_UINT_FIELD(resultcollid, resultcollid, resultcollid);
  WRITE_ENUM_FIELD(CoercionForm, coerceformat, coerceformat, coerceformat);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outConvertRowtypeExpr(OUT_TYPE(ConvertRowtypeExpr) out, const ConvertRowtypeExpr *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_ENUM_FIELD(CoercionForm, convertformat, convertformat, convertformat);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCollateExpr(OUT_TYPE(CollateExpr) out, const CollateExpr *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_UINT_FIELD(coll_oid, collOid, collOid);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCaseExpr(OUT_TYPE(CaseExpr) out, const CaseExpr *node)
{
  WRITE_UINT_FIELD(casetype, casetype, casetype);
  WRITE_UINT_FIELD(casecollid, casecollid, casecollid);
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_NODE_PTR_FIELD(defresult, defresult, defresult);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCaseWhen(OUT_TYPE(CaseWhen) out, const CaseWhen *node)
{
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_NODE_PTR_FIELD(result, result, result);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCaseTestExpr(OUT_TYPE(CaseTestExpr) out, const CaseTestExpr *node)
{
  WRITE_UINT_FIELD(type_id, typeId, typeId);
  WRITE_INT_FIELD(type_mod, typeMod, typeMod);
  WRITE_UINT_FIELD(collation, collation, collation);
}

static void
_outArrayExpr(OUT_TYPE(ArrayExpr) out, const ArrayExpr *node)
{
  WRITE_UINT_FIELD(array_typeid, array_typeid, array_typeid);
  WRITE_UINT_FIELD(array_collid, array_collid, array_collid);
  WRITE_UINT_FIELD(element_typeid, element_typeid, element_typeid);
  WRITE_LIST_FIELD(elements, elements, elements);
  WRITE_BOOL_FIELD(multidims, multidims, multidims);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRowExpr(OUT_TYPE(RowExpr) out, const RowExpr *node)
{
  WRITE_LIST_FIELD(args, args, args);
  WRITE_UINT_FIELD(row_typeid, row_typeid, row_typeid);
  WRITE_ENUM_FIELD(CoercionForm, row_format, row_format, row_format);
  WRITE_LIST_FIELD(colnames, colnames, colnames);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRowCompareExpr(OUT_TYPE(RowCompareExpr) out, const RowCompareExpr *node)
{
  WRITE_ENUM_FIELD(RowCompareType, rctype, rctype, rctype);
  WRITE_LIST_FIELD(opnos, opnos, opnos);
  WRITE_LIST_FIELD(opfamilies, opfamilies, opfamilies);
  WRITE_LIST_FIELD(inputcollids, inputcollids, inputcollids);
  WRITE_LIST_FIELD(largs, largs, largs);
  WRITE_LIST_FIELD(rargs, rargs, rargs);
}

static void
_outCoalesceExpr(OUT_TYPE(CoalesceExpr) out, const CoalesceExpr *node)
{
  WRITE_UINT_FIELD(coalescetype, coalescetype, coalescetype);
  WRITE_UINT_FIELD(coalescecollid, coalescecollid, coalescecollid);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outMinMaxExpr(OUT_TYPE(MinMaxExpr) out, const MinMaxExpr *node)
{
  WRITE_UINT_FIELD(minmaxtype, minmaxtype, minmaxtype);
  WRITE_UINT_FIELD(minmaxcollid, minmaxcollid, minmaxcollid);
  WRITE_UINT_FIELD(inputcollid, inputcollid, inputcollid);
  WRITE_ENUM_FIELD(MinMaxOp, op, op, op);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSQLValueFunction(OUT_TYPE(SQLValueFunction) out, const SQLValueFunction *node)
{
  WRITE_ENUM_FIELD(SQLValueFunctionOp, op, op, op);
  WRITE_UINT_FIELD(type, type, type);
  WRITE_INT_FIELD(typmod, typmod, typmod);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outXmlExpr(OUT_TYPE(XmlExpr) out, const XmlExpr *node)
{
  WRITE_ENUM_FIELD(XmlExprOp, op, op, op);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_LIST_FIELD(named_args, named_args, named_args);
  WRITE_LIST_FIELD(arg_names, arg_names, arg_names);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_ENUM_FIELD(XmlOptionType, xmloption, xmloption, xmloption);
  WRITE_UINT_FIELD(type, type, type);
  WRITE_INT_FIELD(typmod, typmod, typmod);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outNullTest(OUT_TYPE(NullTest) out, const NullTest *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_ENUM_FIELD(NullTestType, nulltesttype, nulltesttype, nulltesttype);
  WRITE_BOOL_FIELD(argisrow, argisrow, argisrow);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outBooleanTest(OUT_TYPE(BooleanTest) out, const BooleanTest *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_ENUM_FIELD(BoolTestType, booltesttype, booltesttype, booltesttype);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCoerceToDomain(OUT_TYPE(CoerceToDomain) out, const CoerceToDomain *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_UINT_FIELD(resulttype, resulttype, resulttype);
  WRITE_INT_FIELD(resulttypmod, resulttypmod, resulttypmod);
  WRITE_UINT_FIELD(resultcollid, resultcollid, resultcollid);
  WRITE_ENUM_FIELD(CoercionForm, coercionformat, coercionformat, coercionformat);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCoerceToDomainValue(OUT_TYPE(CoerceToDomainValue) out, const CoerceToDomainValue *node)
{
  WRITE_UINT_FIELD(type_id, typeId, typeId);
  WRITE_INT_FIELD(type_mod, typeMod, typeMod);
  WRITE_UINT_FIELD(collation, collation, collation);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSetToDefault(OUT_TYPE(SetToDefault) out, const SetToDefault *node)
{
  WRITE_UINT_FIELD(type_id, typeId, typeId);
  WRITE_INT_FIELD(type_mod, typeMod, typeMod);
  WRITE_UINT_FIELD(collation, collation, collation);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCurrentOfExpr(OUT_TYPE(CurrentOfExpr) out, const CurrentOfExpr *node)
{
  WRITE_UINT_FIELD(cvarno, cvarno, cvarno);
  WRITE_STRING_FIELD(cursor_name, cursor_name, cursor_name);
  WRITE_INT_FIELD(cursor_param, cursor_param, cursor_param);
}

static void
_outNextValueExpr(OUT_TYPE(NextValueExpr) out, const NextValueExpr *node)
{
  WRITE_UINT_FIELD(seqid, seqid, seqid);
  WRITE_UINT_FIELD(type_id, typeId, typeId);
}

static void
_outInferenceElem(OUT_TYPE(InferenceElem) out, const InferenceElem *node)
{
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_UINT_FIELD(infercollid, infercollid, infercollid);
  WRITE_UINT_FIELD(inferopclass, inferopclass, inferopclass);
}

static void
_outTargetEntry(OUT_TYPE(TargetEntry) out, const TargetEntry *node)
{
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_INT_FIELD(resno, resno, resno);
  WRITE_STRING_FIELD(resname, resname, resname);
  WRITE_UINT_FIELD(ressortgroupref, ressortgroupref, ressortgroupref);
  WRITE_UINT_FIELD(resorigtbl, resorigtbl, resorigtbl);
  WRITE_INT_FIELD(resorigcol, resorigcol, resorigcol);
  WRITE_BOOL_FIELD(resjunk, resjunk, resjunk);
}

static void
_outRangeTblRef(OUT_TYPE(RangeTblRef) out, const RangeTblRef *node)
{
  WRITE_INT_FIELD(rtindex, rtindex, rtindex);
}

static void
_outJoinExpr(OUT_TYPE(JoinExpr) out, const JoinExpr *node)
{
  WRITE_ENUM_FIELD(JoinType, jointype, jointype, jointype);
  WRITE_BOOL_FIELD(is_natural, isNatural, isNatural);
  WRITE_NODE_PTR_FIELD(larg, larg, larg);
  WRITE_NODE_PTR_FIELD(rarg, rarg, rarg);
  WRITE_LIST_FIELD(using_clause, usingClause, usingClause);
  WRITE_NODE_PTR_FIELD(quals, quals, quals);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
  WRITE_INT_FIELD(rtindex, rtindex, rtindex);
}

static void
_outFromExpr(OUT_TYPE(FromExpr) out, const FromExpr *node)
{
  WRITE_LIST_FIELD(fromlist, fromlist, fromlist);
  WRITE_NODE_PTR_FIELD(quals, quals, quals);
}

static void
_outOnConflictExpr(OUT_TYPE(OnConflictExpr) out, const OnConflictExpr *node)
{
  WRITE_ENUM_FIELD(OnConflictAction, action, action, action);
  WRITE_LIST_FIELD(arbiter_elems, arbiterElems, arbiterElems);
  WRITE_NODE_PTR_FIELD(arbiter_where, arbiterWhere, arbiterWhere);
  WRITE_UINT_FIELD(constraint, constraint, constraint);
  WRITE_LIST_FIELD(on_conflict_set, onConflictSet, onConflictSet);
  WRITE_NODE_PTR_FIELD(on_conflict_where, onConflictWhere, onConflictWhere);
  WRITE_INT_FIELD(excl_rel_index, exclRelIndex, exclRelIndex);
  WRITE_LIST_FIELD(excl_rel_tlist, exclRelTlist, exclRelTlist);
}

static void
_outIntoClause(OUT_TYPE(IntoClause) out, const IntoClause *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, rel, rel, rel);
  WRITE_LIST_FIELD(col_names, colNames, colNames);
  WRITE_STRING_FIELD(access_method, accessMethod, accessMethod);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_ENUM_FIELD(OnCommitAction, on_commit, onCommit, onCommit);
  WRITE_STRING_FIELD(table_space_name, tableSpaceName, tableSpaceName);
  WRITE_NODE_PTR_FIELD(view_query, viewQuery, viewQuery);
  WRITE_BOOL_FIELD(skip_data, skipData, skipData);
}

static void
_outRawStmt(OUT_TYPE(RawStmt) out, const RawStmt *node)
{
  WRITE_NODE_PTR_FIELD(stmt, stmt, stmt);
  WRITE_INT_FIELD(stmt_location, stmt_location, stmt_location);
  WRITE_INT_FIELD(stmt_len, stmt_len, stmt_len);
}

static void
_outQuery(OUT_TYPE(Query) out, const Query *node)
{
  WRITE_ENUM_FIELD(CmdType, command_type, commandType, commandType);
  WRITE_ENUM_FIELD(QuerySource, query_source, querySource, querySource);
  WRITE_BOOL_FIELD(can_set_tag, canSetTag, canSetTag);
  WRITE_NODE_PTR_FIELD(utility_stmt, utilityStmt, utilityStmt);
  WRITE_INT_FIELD(result_relation, resultRelation, resultRelation);
  WRITE_BOOL_FIELD(has_aggs, hasAggs, hasAggs);
  WRITE_BOOL_FIELD(has_window_funcs, hasWindowFuncs, hasWindowFuncs);
  WRITE_BOOL_FIELD(has_target_srfs, hasTargetSRFs, hasTargetSRFs);
  WRITE_BOOL_FIELD(has_sub_links, hasSubLinks, hasSubLinks);
  WRITE_BOOL_FIELD(has_distinct_on, hasDistinctOn, hasDistinctOn);
  WRITE_BOOL_FIELD(has_recursive, hasRecursive, hasRecursive);
  WRITE_BOOL_FIELD(has_modifying_cte, hasModifyingCTE, hasModifyingCTE);
  WRITE_BOOL_FIELD(has_for_update, hasForUpdate, hasForUpdate);
  WRITE_BOOL_FIELD(has_row_security, hasRowSecurity, hasRowSecurity);
  WRITE_LIST_FIELD(cte_list, cteList, cteList);
  WRITE_LIST_FIELD(rtable, rtable, rtable);
  WRITE_SPECIFIC_NODE_PTR_FIELD(FromExpr, from_expr, jointree, jointree, jointree);
  WRITE_LIST_FIELD(target_list, targetList, targetList);
  WRITE_ENUM_FIELD(OverridingKind, override, override, override);
  WRITE_SPECIFIC_NODE_PTR_FIELD(OnConflictExpr, on_conflict_expr, on_conflict, onConflict, onConflict);
  WRITE_LIST_FIELD(returning_list, returningList, returningList);
  WRITE_LIST_FIELD(group_clause, groupClause, groupClause);
  WRITE_LIST_FIELD(grouping_sets, groupingSets, groupingSets);
  WRITE_NODE_PTR_FIELD(having_qual, havingQual, havingQual);
  WRITE_LIST_FIELD(window_clause, windowClause, windowClause);
  WRITE_LIST_FIELD(distinct_clause, distinctClause, distinctClause);
  WRITE_LIST_FIELD(sort_clause, sortClause, sortClause);
  WRITE_NODE_PTR_FIELD(limit_offset, limitOffset, limitOffset);
  WRITE_NODE_PTR_FIELD(limit_count, limitCount, limitCount);
  WRITE_ENUM_FIELD(LimitOption, limit_option, limitOption, limitOption);
  WRITE_LIST_FIELD(row_marks, rowMarks, rowMarks);
  WRITE_NODE_PTR_FIELD(set_operations, setOperations, setOperations);
  WRITE_LIST_FIELD(constraint_deps, constraintDeps, constraintDeps);
  WRITE_LIST_FIELD(with_check_options, withCheckOptions, withCheckOptions);
  WRITE_INT_FIELD(stmt_location, stmt_location, stmt_location);
  WRITE_INT_FIELD(stmt_len, stmt_len, stmt_len);
}

static void
_outInsertStmt(OUT_TYPE(InsertStmt) out, const InsertStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(cols, cols, cols);
  WRITE_NODE_PTR_FIELD(select_stmt, selectStmt, selectStmt);
  WRITE_SPECIFIC_NODE_PTR_FIELD(OnConflictClause, on_conflict_clause, on_conflict_clause, onConflictClause, onConflictClause);
  WRITE_LIST_FIELD(returning_list, returningList, returningList);
  WRITE_SPECIFIC_NODE_PTR_FIELD(WithClause, with_clause, with_clause, withClause, withClause);
  WRITE_ENUM_FIELD(OverridingKind, override, override, override);
}

static void
_outDeleteStmt(OUT_TYPE(DeleteStmt) out, const DeleteStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(using_clause, usingClause, usingClause);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_LIST_FIELD(returning_list, returningList, returningList);
  WRITE_SPECIFIC_NODE_PTR_FIELD(WithClause, with_clause, with_clause, withClause, withClause);
}

static void
_outUpdateStmt(OUT_TYPE(UpdateStmt) out, const UpdateStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(target_list, targetList, targetList);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_LIST_FIELD(from_clause, fromClause, fromClause);
  WRITE_LIST_FIELD(returning_list, returningList, returningList);
  WRITE_SPECIFIC_NODE_PTR_FIELD(WithClause, with_clause, with_clause, withClause, withClause);
}

static void
_outSelectStmt(OUT_TYPE(SelectStmt) out, const SelectStmt *node)
{
  WRITE_LIST_FIELD(distinct_clause, distinctClause, distinctClause);
  WRITE_SPECIFIC_NODE_PTR_FIELD(IntoClause, into_clause, into_clause, intoClause, intoClause);
  WRITE_LIST_FIELD(target_list, targetList, targetList);
  WRITE_LIST_FIELD(from_clause, fromClause, fromClause);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_LIST_FIELD(group_clause, groupClause, groupClause);
  WRITE_NODE_PTR_FIELD(having_clause, havingClause, havingClause);
  WRITE_LIST_FIELD(window_clause, windowClause, windowClause);
  WRITE_LIST_FIELD(values_lists, valuesLists, valuesLists);
  WRITE_LIST_FIELD(sort_clause, sortClause, sortClause);
  WRITE_NODE_PTR_FIELD(limit_offset, limitOffset, limitOffset);
  WRITE_NODE_PTR_FIELD(limit_count, limitCount, limitCount);
  WRITE_ENUM_FIELD(LimitOption, limit_option, limitOption, limitOption);
  WRITE_LIST_FIELD(locking_clause, lockingClause, lockingClause);
  WRITE_SPECIFIC_NODE_PTR_FIELD(WithClause, with_clause, with_clause, withClause, withClause);
  WRITE_ENUM_FIELD(SetOperation, op, op, op);
  WRITE_BOOL_FIELD(all, all, all);
  WRITE_SPECIFIC_NODE_PTR_FIELD(SelectStmt, select_stmt, larg, larg, larg);
  WRITE_SPECIFIC_NODE_PTR_FIELD(SelectStmt, select_stmt, rarg, rarg, rarg);
}

static void
_outAlterTableStmt(OUT_TYPE(AlterTableStmt) out, const AlterTableStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(cmds, cmds, cmds);
  WRITE_ENUM_FIELD(ObjectType, relkind, relkind, relkind);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAlterTableCmd(OUT_TYPE(AlterTableCmd) out, const AlterTableCmd *node)
{
  WRITE_ENUM_FIELD(AlterTableType, subtype, subtype, subtype);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_INT_FIELD(num, num, num);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, newowner, newowner, newowner);
  WRITE_NODE_PTR_FIELD(def, def, def);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAlterDomainStmt(OUT_TYPE(AlterDomainStmt) out, const AlterDomainStmt *node)
{
  WRITE_CHAR_FIELD(subtype, subtype, subtype);
  WRITE_LIST_FIELD(type_name, typeName, typeName);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_NODE_PTR_FIELD(def, def, def);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outSetOperationStmt(OUT_TYPE(SetOperationStmt) out, const SetOperationStmt *node)
{
  WRITE_ENUM_FIELD(SetOperation, op, op, op);
  WRITE_BOOL_FIELD(all, all, all);
  WRITE_NODE_PTR_FIELD(larg, larg, larg);
  WRITE_NODE_PTR_FIELD(rarg, rarg, rarg);
  WRITE_LIST_FIELD(col_types, colTypes, colTypes);
  WRITE_LIST_FIELD(col_typmods, colTypmods, colTypmods);
  WRITE_LIST_FIELD(col_collations, colCollations, colCollations);
  WRITE_LIST_FIELD(group_clauses, groupClauses, groupClauses);
}

static void
_outGrantStmt(OUT_TYPE(GrantStmt) out, const GrantStmt *node)
{
  WRITE_BOOL_FIELD(is_grant, is_grant, is_grant);
  WRITE_ENUM_FIELD(GrantTargetType, targtype, targtype, targtype);
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_LIST_FIELD(objects, objects, objects);
  WRITE_LIST_FIELD(privileges, privileges, privileges);
  WRITE_LIST_FIELD(grantees, grantees, grantees);
  WRITE_BOOL_FIELD(grant_option, grant_option, grant_option);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
}

static void
_outGrantRoleStmt(OUT_TYPE(GrantRoleStmt) out, const GrantRoleStmt *node)
{
  WRITE_LIST_FIELD(granted_roles, granted_roles, granted_roles);
  WRITE_LIST_FIELD(grantee_roles, grantee_roles, grantee_roles);
  WRITE_BOOL_FIELD(is_grant, is_grant, is_grant);
  WRITE_BOOL_FIELD(admin_opt, admin_opt, admin_opt);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, grantor, grantor, grantor);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
}

static void
_outAlterDefaultPrivilegesStmt(OUT_TYPE(AlterDefaultPrivilegesStmt) out, const AlterDefaultPrivilegesStmt *node)
{
  WRITE_LIST_FIELD(options, options, options);
  WRITE_SPECIFIC_NODE_PTR_FIELD(GrantStmt, grant_stmt, action, action, action);
}

static void
_outClosePortalStmt(OUT_TYPE(ClosePortalStmt) out, const ClosePortalStmt *node)
{
  WRITE_STRING_FIELD(portalname, portalname, portalname);
}

static void
_outClusterStmt(OUT_TYPE(ClusterStmt) out, const ClusterStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_STRING_FIELD(indexname, indexname, indexname);
  WRITE_INT_FIELD(options, options, options);
}

static void
_outCopyStmt(OUT_TYPE(CopyStmt) out, const CopyStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_NODE_PTR_FIELD(query, query, query);
  WRITE_LIST_FIELD(attlist, attlist, attlist);
  WRITE_BOOL_FIELD(is_from, is_from, is_from);
  WRITE_BOOL_FIELD(is_program, is_program, is_program);
  WRITE_STRING_FIELD(filename, filename, filename);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
}

static void
_outCreateStmt(OUT_TYPE(CreateStmt) out, const CreateStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(table_elts, tableElts, tableElts);
  WRITE_LIST_FIELD(inh_relations, inhRelations, inhRelations);
  WRITE_SPECIFIC_NODE_PTR_FIELD(PartitionBoundSpec, partition_bound_spec, partbound, partbound, partbound);
  WRITE_SPECIFIC_NODE_PTR_FIELD(PartitionSpec, partition_spec, partspec, partspec, partspec);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, of_typename, ofTypename, ofTypename);
  WRITE_LIST_FIELD(constraints, constraints, constraints);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_ENUM_FIELD(OnCommitAction, oncommit, oncommit, oncommit);
  WRITE_STRING_FIELD(tablespacename, tablespacename, tablespacename);
  WRITE_STRING_FIELD(access_method, accessMethod, accessMethod);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
}

static void
_outDefineStmt(OUT_TYPE(DefineStmt) out, const DefineStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, kind, kind, kind);
  WRITE_BOOL_FIELD(oldstyle, oldstyle, oldstyle);
  WRITE_LIST_FIELD(defnames, defnames, defnames);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_LIST_FIELD(definition, definition, definition);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
  WRITE_BOOL_FIELD(replace, replace, replace);
}

static void
_outDropStmt(OUT_TYPE(DropStmt) out, const DropStmt *node)
{
  WRITE_LIST_FIELD(objects, objects, objects);
  WRITE_ENUM_FIELD(ObjectType, remove_type, removeType, removeType);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
  WRITE_BOOL_FIELD(concurrent, concurrent, concurrent);
}

static void
_outTruncateStmt(OUT_TYPE(TruncateStmt) out, const TruncateStmt *node)
{
  WRITE_LIST_FIELD(relations, relations, relations);
  WRITE_BOOL_FIELD(restart_seqs, restart_seqs, restart_seqs);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
}

static void
_outCommentStmt(OUT_TYPE(CommentStmt) out, const CommentStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_STRING_FIELD(comment, comment, comment);
}

static void
_outFetchStmt(OUT_TYPE(FetchStmt) out, const FetchStmt *node)
{
  WRITE_ENUM_FIELD(FetchDirection, direction, direction, direction);
  WRITE_LONG_FIELD(how_many, howMany, howMany);
  WRITE_STRING_FIELD(portalname, portalname, portalname);
  WRITE_BOOL_FIELD(ismove, ismove, ismove);
}

static void
_outIndexStmt(OUT_TYPE(IndexStmt) out, const IndexStmt *node)
{
  WRITE_STRING_FIELD(idxname, idxname, idxname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_STRING_FIELD(access_method, accessMethod, accessMethod);
  WRITE_STRING_FIELD(table_space, tableSpace, tableSpace);
  WRITE_LIST_FIELD(index_params, indexParams, indexParams);
  WRITE_LIST_FIELD(index_including_params, indexIncludingParams, indexIncludingParams);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_LIST_FIELD(exclude_op_names, excludeOpNames, excludeOpNames);
  WRITE_STRING_FIELD(idxcomment, idxcomment, idxcomment);
  WRITE_UINT_FIELD(index_oid, indexOid, indexOid);
  WRITE_UINT_FIELD(old_node, oldNode, oldNode);
  WRITE_UINT_FIELD(old_create_subid, oldCreateSubid, oldCreateSubid);
  WRITE_UINT_FIELD(old_first_relfilenode_subid, oldFirstRelfilenodeSubid, oldFirstRelfilenodeSubid);
  WRITE_BOOL_FIELD(unique, unique, unique);
  WRITE_BOOL_FIELD(primary, primary, primary);
  WRITE_BOOL_FIELD(isconstraint, isconstraint, isconstraint);
  WRITE_BOOL_FIELD(deferrable, deferrable, deferrable);
  WRITE_BOOL_FIELD(initdeferred, initdeferred, initdeferred);
  WRITE_BOOL_FIELD(transformed, transformed, transformed);
  WRITE_BOOL_FIELD(concurrent, concurrent, concurrent);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
  WRITE_BOOL_FIELD(reset_default_tblspc, reset_default_tblspc, reset_default_tblspc);
}

static void
_outCreateFunctionStmt(OUT_TYPE(CreateFunctionStmt) out, const CreateFunctionStmt *node)
{
  WRITE_BOOL_FIELD(is_procedure, is_procedure, is_procedure);
  WRITE_BOOL_FIELD(replace, replace, replace);
  WRITE_LIST_FIELD(funcname, funcname, funcname);
  WRITE_LIST_FIELD(parameters, parameters, parameters);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, return_type, returnType, returnType);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterFunctionStmt(OUT_TYPE(AlterFunctionStmt) out, const AlterFunctionStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, func, func, func);
  WRITE_LIST_FIELD(actions, actions, actions);
}

static void
_outDoStmt(OUT_TYPE(DoStmt) out, const DoStmt *node)
{
  WRITE_LIST_FIELD(args, args, args);
}

static void
_outRenameStmt(OUT_TYPE(RenameStmt) out, const RenameStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, rename_type, renameType, renameType);
  WRITE_ENUM_FIELD(ObjectType, relation_type, relationType, relationType);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_STRING_FIELD(subname, subname, subname);
  WRITE_STRING_FIELD(newname, newname, newname);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outRuleStmt(OUT_TYPE(RuleStmt) out, const RuleStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_STRING_FIELD(rulename, rulename, rulename);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_ENUM_FIELD(CmdType, event, event, event);
  WRITE_BOOL_FIELD(instead, instead, instead);
  WRITE_LIST_FIELD(actions, actions, actions);
  WRITE_BOOL_FIELD(replace, replace, replace);
}

static void
_outNotifyStmt(OUT_TYPE(NotifyStmt) out, const NotifyStmt *node)
{
  WRITE_STRING_FIELD(conditionname, conditionname, conditionname);
  WRITE_STRING_FIELD(payload, payload, payload);
}

static void
_outListenStmt(OUT_TYPE(ListenStmt) out, const ListenStmt *node)
{
  WRITE_STRING_FIELD(conditionname, conditionname, conditionname);
}

static void
_outUnlistenStmt(OUT_TYPE(UnlistenStmt) out, const UnlistenStmt *node)
{
  WRITE_STRING_FIELD(conditionname, conditionname, conditionname);
}

static void
_outTransactionStmt(OUT_TYPE(TransactionStmt) out, const TransactionStmt *node)
{
  WRITE_ENUM_FIELD(TransactionStmtKind, kind, kind, kind);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_STRING_FIELD(savepoint_name, savepoint_name, savepoint_name);
  WRITE_STRING_FIELD(gid, gid, gid);
  WRITE_BOOL_FIELD(chain, chain, chain);
}

static void
_outViewStmt(OUT_TYPE(ViewStmt) out, const ViewStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, view, view, view);
  WRITE_LIST_FIELD(aliases, aliases, aliases);
  WRITE_NODE_PTR_FIELD(query, query, query);
  WRITE_BOOL_FIELD(replace, replace, replace);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_ENUM_FIELD(ViewCheckOption, with_check_option, withCheckOption, withCheckOption);
}

static void
_outLoadStmt(OUT_TYPE(LoadStmt) out, const LoadStmt *node)
{
  WRITE_STRING_FIELD(filename, filename, filename);
}

static void
_outCreateDomainStmt(OUT_TYPE(CreateDomainStmt) out, const CreateDomainStmt *node)
{
  WRITE_LIST_FIELD(domainname, domainname, domainname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, typeName, typeName);
  WRITE_SPECIFIC_NODE_PTR_FIELD(CollateClause, collate_clause, coll_clause, collClause, collClause);
  WRITE_LIST_FIELD(constraints, constraints, constraints);
}

static void
_outCreatedbStmt(OUT_TYPE(CreatedbStmt) out, const CreatedbStmt *node)
{
  WRITE_STRING_FIELD(dbname, dbname, dbname);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outDropdbStmt(OUT_TYPE(DropdbStmt) out, const DropdbStmt *node)
{
  WRITE_STRING_FIELD(dbname, dbname, dbname);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outVacuumStmt(OUT_TYPE(VacuumStmt) out, const VacuumStmt *node)
{
  WRITE_LIST_FIELD(options, options, options);
  WRITE_LIST_FIELD(rels, rels, rels);
  WRITE_BOOL_FIELD(is_vacuumcmd, is_vacuumcmd, is_vacuumcmd);
}

static void
_outExplainStmt(OUT_TYPE(ExplainStmt) out, const ExplainStmt *node)
{
  WRITE_NODE_PTR_FIELD(query, query, query);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outCreateTableAsStmt(OUT_TYPE(CreateTableAsStmt) out, const CreateTableAsStmt *node)
{
  WRITE_NODE_PTR_FIELD(query, query, query);
  WRITE_SPECIFIC_NODE_PTR_FIELD(IntoClause, into_clause, into, into, into);
  WRITE_ENUM_FIELD(ObjectType, relkind, relkind, relkind);
  WRITE_BOOL_FIELD(is_select_into, is_select_into, is_select_into);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
}

static void
_outCreateSeqStmt(OUT_TYPE(CreateSeqStmt) out, const CreateSeqStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, sequence, sequence, sequence);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_UINT_FIELD(owner_id, ownerId, ownerId);
  WRITE_BOOL_FIELD(for_identity, for_identity, for_identity);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
}

static void
_outAlterSeqStmt(OUT_TYPE(AlterSeqStmt) out, const AlterSeqStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, sequence, sequence, sequence);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_BOOL_FIELD(for_identity, for_identity, for_identity);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outVariableSetStmt(OUT_TYPE(VariableSetStmt) out, const VariableSetStmt *node)
{
  WRITE_ENUM_FIELD(VariableSetKind, kind, kind, kind);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_BOOL_FIELD(is_local, is_local, is_local);
}

static void
_outVariableShowStmt(OUT_TYPE(VariableShowStmt) out, const VariableShowStmt *node)
{
  WRITE_STRING_FIELD(name, name, name);
}

static void
_outDiscardStmt(OUT_TYPE(DiscardStmt) out, const DiscardStmt *node)
{
  WRITE_ENUM_FIELD(DiscardMode, target, target, target);
}

static void
_outCreateTrigStmt(OUT_TYPE(CreateTrigStmt) out, const CreateTrigStmt *node)
{
  WRITE_STRING_FIELD(trigname, trigname, trigname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_LIST_FIELD(funcname, funcname, funcname);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_BOOL_FIELD(row, row, row);
  WRITE_INT_FIELD(timing, timing, timing);
  WRITE_INT_FIELD(events, events, events);
  WRITE_LIST_FIELD(columns, columns, columns);
  WRITE_NODE_PTR_FIELD(when_clause, whenClause, whenClause);
  WRITE_BOOL_FIELD(isconstraint, isconstraint, isconstraint);
  WRITE_LIST_FIELD(transition_rels, transitionRels, transitionRels);
  WRITE_BOOL_FIELD(deferrable, deferrable, deferrable);
  WRITE_BOOL_FIELD(initdeferred, initdeferred, initdeferred);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, constrrel, constrrel, constrrel);
}

static void
_outCreatePLangStmt(OUT_TYPE(CreatePLangStmt) out, const CreatePLangStmt *node)
{
  WRITE_BOOL_FIELD(replace, replace, replace);
  WRITE_STRING_FIELD(plname, plname, plname);
  WRITE_LIST_FIELD(plhandler, plhandler, plhandler);
  WRITE_LIST_FIELD(plinline, plinline, plinline);
  WRITE_LIST_FIELD(plvalidator, plvalidator, plvalidator);
  WRITE_BOOL_FIELD(pltrusted, pltrusted, pltrusted);
}

static void
_outCreateRoleStmt(OUT_TYPE(CreateRoleStmt) out, const CreateRoleStmt *node)
{
  WRITE_ENUM_FIELD(RoleStmtType, stmt_type, stmt_type, stmt_type);
  WRITE_STRING_FIELD(role, role, role);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterRoleStmt(OUT_TYPE(AlterRoleStmt) out, const AlterRoleStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, role, role, role);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_INT_FIELD(action, action, action);
}

static void
_outDropRoleStmt(OUT_TYPE(DropRoleStmt) out, const DropRoleStmt *node)
{
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outLockStmt(OUT_TYPE(LockStmt) out, const LockStmt *node)
{
  WRITE_LIST_FIELD(relations, relations, relations);
  WRITE_INT_FIELD(mode, mode, mode);
  WRITE_BOOL_FIELD(nowait, nowait, nowait);
}

static void
_outConstraintsSetStmt(OUT_TYPE(ConstraintsSetStmt) out, const ConstraintsSetStmt *node)
{
  WRITE_LIST_FIELD(constraints, constraints, constraints);
  WRITE_BOOL_FIELD(deferred, deferred, deferred);
}

static void
_outReindexStmt(OUT_TYPE(ReindexStmt) out, const ReindexStmt *node)
{
  WRITE_ENUM_FIELD(ReindexObjectType, kind, kind, kind);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_STRING_FIELD(name, name, name);
  WRITE_INT_FIELD(options, options, options);
  WRITE_BOOL_FIELD(concurrent, concurrent, concurrent);
}

static void
_outCheckPointStmt(OUT_TYPE(CheckPointStmt) out, const CheckPointStmt *node)
{
}

static void
_outCreateSchemaStmt(OUT_TYPE(CreateSchemaStmt) out, const CreateSchemaStmt *node)
{
  WRITE_STRING_FIELD(schemaname, schemaname, schemaname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, authrole, authrole, authrole);
  WRITE_LIST_FIELD(schema_elts, schemaElts, schemaElts);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
}

static void
_outAlterDatabaseStmt(OUT_TYPE(AlterDatabaseStmt) out, const AlterDatabaseStmt *node)
{
  WRITE_STRING_FIELD(dbname, dbname, dbname);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterDatabaseSetStmt(OUT_TYPE(AlterDatabaseSetStmt) out, const AlterDatabaseSetStmt *node)
{
  WRITE_STRING_FIELD(dbname, dbname, dbname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(VariableSetStmt, variable_set_stmt, setstmt, setstmt, setstmt);
}

static void
_outAlterRoleSetStmt(OUT_TYPE(AlterRoleSetStmt) out, const AlterRoleSetStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, role, role, role);
  WRITE_STRING_FIELD(database, database, database);
  WRITE_SPECIFIC_NODE_PTR_FIELD(VariableSetStmt, variable_set_stmt, setstmt, setstmt, setstmt);
}

static void
_outCreateConversionStmt(OUT_TYPE(CreateConversionStmt) out, const CreateConversionStmt *node)
{
  WRITE_LIST_FIELD(conversion_name, conversion_name, conversion_name);
  WRITE_STRING_FIELD(for_encoding_name, for_encoding_name, for_encoding_name);
  WRITE_STRING_FIELD(to_encoding_name, to_encoding_name, to_encoding_name);
  WRITE_LIST_FIELD(func_name, func_name, func_name);
  WRITE_BOOL_FIELD(def, def, def);
}

static void
_outCreateCastStmt(OUT_TYPE(CreateCastStmt) out, const CreateCastStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, sourcetype, sourcetype, sourcetype);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, targettype, targettype, targettype);
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, func, func, func);
  WRITE_ENUM_FIELD(CoercionContext, context, context, context);
  WRITE_BOOL_FIELD(inout, inout, inout);
}

static void
_outCreateOpClassStmt(OUT_TYPE(CreateOpClassStmt) out, const CreateOpClassStmt *node)
{
  WRITE_LIST_FIELD(opclassname, opclassname, opclassname);
  WRITE_LIST_FIELD(opfamilyname, opfamilyname, opfamilyname);
  WRITE_STRING_FIELD(amname, amname, amname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, datatype, datatype, datatype);
  WRITE_LIST_FIELD(items, items, items);
  WRITE_BOOL_FIELD(is_default, isDefault, isDefault);
}

static void
_outCreateOpFamilyStmt(OUT_TYPE(CreateOpFamilyStmt) out, const CreateOpFamilyStmt *node)
{
  WRITE_LIST_FIELD(opfamilyname, opfamilyname, opfamilyname);
  WRITE_STRING_FIELD(amname, amname, amname);
}

static void
_outAlterOpFamilyStmt(OUT_TYPE(AlterOpFamilyStmt) out, const AlterOpFamilyStmt *node)
{
  WRITE_LIST_FIELD(opfamilyname, opfamilyname, opfamilyname);
  WRITE_STRING_FIELD(amname, amname, amname);
  WRITE_BOOL_FIELD(is_drop, isDrop, isDrop);
  WRITE_LIST_FIELD(items, items, items);
}

static void
_outPrepareStmt(OUT_TYPE(PrepareStmt) out, const PrepareStmt *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_LIST_FIELD(argtypes, argtypes, argtypes);
  WRITE_NODE_PTR_FIELD(query, query, query);
}

static void
_outExecuteStmt(OUT_TYPE(ExecuteStmt) out, const ExecuteStmt *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_LIST_FIELD(params, params, params);
}

static void
_outDeallocateStmt(OUT_TYPE(DeallocateStmt) out, const DeallocateStmt *node)
{
  WRITE_STRING_FIELD(name, name, name);
}

static void
_outDeclareCursorStmt(OUT_TYPE(DeclareCursorStmt) out, const DeclareCursorStmt *node)
{
  WRITE_STRING_FIELD(portalname, portalname, portalname);
  WRITE_INT_FIELD(options, options, options);
  WRITE_NODE_PTR_FIELD(query, query, query);
}

static void
_outCreateTableSpaceStmt(OUT_TYPE(CreateTableSpaceStmt) out, const CreateTableSpaceStmt *node)
{
  WRITE_STRING_FIELD(tablespacename, tablespacename, tablespacename);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, owner, owner, owner);
  WRITE_STRING_FIELD(location, location, location);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outDropTableSpaceStmt(OUT_TYPE(DropTableSpaceStmt) out, const DropTableSpaceStmt *node)
{
  WRITE_STRING_FIELD(tablespacename, tablespacename, tablespacename);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAlterObjectDependsStmt(OUT_TYPE(AlterObjectDependsStmt) out, const AlterObjectDependsStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, object_type, objectType, objectType);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_NODE_PTR_FIELD(extname, extname, extname);
  WRITE_BOOL_FIELD(remove, remove, remove);
}

static void
_outAlterObjectSchemaStmt(OUT_TYPE(AlterObjectSchemaStmt) out, const AlterObjectSchemaStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, object_type, objectType, objectType);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_STRING_FIELD(newschema, newschema, newschema);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAlterOwnerStmt(OUT_TYPE(AlterOwnerStmt) out, const AlterOwnerStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, object_type, objectType, objectType);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, newowner, newowner, newowner);
}

static void
_outAlterOperatorStmt(OUT_TYPE(AlterOperatorStmt) out, const AlterOperatorStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, opername, opername, opername);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterTypeStmt(OUT_TYPE(AlterTypeStmt) out, const AlterTypeStmt *node)
{
  WRITE_LIST_FIELD(type_name, typeName, typeName);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outDropOwnedStmt(OUT_TYPE(DropOwnedStmt) out, const DropOwnedStmt *node)
{
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
}

static void
_outReassignOwnedStmt(OUT_TYPE(ReassignOwnedStmt) out, const ReassignOwnedStmt *node)
{
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, newrole, newrole, newrole);
}

static void
_outCompositeTypeStmt(OUT_TYPE(CompositeTypeStmt) out, const CompositeTypeStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, typevar, typevar, typevar);
  WRITE_LIST_FIELD(coldeflist, coldeflist, coldeflist);
}

static void
_outCreateEnumStmt(OUT_TYPE(CreateEnumStmt) out, const CreateEnumStmt *node)
{
  WRITE_LIST_FIELD(type_name, typeName, typeName);
  WRITE_LIST_FIELD(vals, vals, vals);
}

static void
_outCreateRangeStmt(OUT_TYPE(CreateRangeStmt) out, const CreateRangeStmt *node)
{
  WRITE_LIST_FIELD(type_name, typeName, typeName);
  WRITE_LIST_FIELD(params, params, params);
}

static void
_outAlterEnumStmt(OUT_TYPE(AlterEnumStmt) out, const AlterEnumStmt *node)
{
  WRITE_LIST_FIELD(type_name, typeName, typeName);
  WRITE_STRING_FIELD(old_val, oldVal, oldVal);
  WRITE_STRING_FIELD(new_val, newVal, newVal);
  WRITE_STRING_FIELD(new_val_neighbor, newValNeighbor, newValNeighbor);
  WRITE_BOOL_FIELD(new_val_is_after, newValIsAfter, newValIsAfter);
  WRITE_BOOL_FIELD(skip_if_new_val_exists, skipIfNewValExists, skipIfNewValExists);
}

static void
_outAlterTSDictionaryStmt(OUT_TYPE(AlterTSDictionaryStmt) out, const AlterTSDictionaryStmt *node)
{
  WRITE_LIST_FIELD(dictname, dictname, dictname);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterTSConfigurationStmt(OUT_TYPE(AlterTSConfigurationStmt) out, const AlterTSConfigurationStmt *node)
{
  WRITE_ENUM_FIELD(AlterTSConfigType, kind, kind, kind);
  WRITE_LIST_FIELD(cfgname, cfgname, cfgname);
  WRITE_LIST_FIELD(tokentype, tokentype, tokentype);
  WRITE_LIST_FIELD(dicts, dicts, dicts);
  WRITE_BOOL_FIELD(override, override, override);
  WRITE_BOOL_FIELD(replace, replace, replace);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outCreateFdwStmt(OUT_TYPE(CreateFdwStmt) out, const CreateFdwStmt *node)
{
  WRITE_STRING_FIELD(fdwname, fdwname, fdwname);
  WRITE_LIST_FIELD(func_options, func_options, func_options);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterFdwStmt(OUT_TYPE(AlterFdwStmt) out, const AlterFdwStmt *node)
{
  WRITE_STRING_FIELD(fdwname, fdwname, fdwname);
  WRITE_LIST_FIELD(func_options, func_options, func_options);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outCreateForeignServerStmt(OUT_TYPE(CreateForeignServerStmt) out, const CreateForeignServerStmt *node)
{
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_STRING_FIELD(servertype, servertype, servertype);
  WRITE_STRING_FIELD(version, version, version);
  WRITE_STRING_FIELD(fdwname, fdwname, fdwname);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterForeignServerStmt(OUT_TYPE(AlterForeignServerStmt) out, const AlterForeignServerStmt *node)
{
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_STRING_FIELD(version, version, version);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_BOOL_FIELD(has_version, has_version, has_version);
}

static void
_outCreateUserMappingStmt(OUT_TYPE(CreateUserMappingStmt) out, const CreateUserMappingStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, user, user, user);
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterUserMappingStmt(OUT_TYPE(AlterUserMappingStmt) out, const AlterUserMappingStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, user, user, user);
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outDropUserMappingStmt(OUT_TYPE(DropUserMappingStmt) out, const DropUserMappingStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RoleSpec, role_spec, user, user, user);
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAlterTableSpaceOptionsStmt(OUT_TYPE(AlterTableSpaceOptionsStmt) out, const AlterTableSpaceOptionsStmt *node)
{
  WRITE_STRING_FIELD(tablespacename, tablespacename, tablespacename);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_BOOL_FIELD(is_reset, isReset, isReset);
}

static void
_outAlterTableMoveAllStmt(OUT_TYPE(AlterTableMoveAllStmt) out, const AlterTableMoveAllStmt *node)
{
  WRITE_STRING_FIELD(orig_tablespacename, orig_tablespacename, orig_tablespacename);
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_STRING_FIELD(new_tablespacename, new_tablespacename, new_tablespacename);
  WRITE_BOOL_FIELD(nowait, nowait, nowait);
}

static void
_outSecLabelStmt(OUT_TYPE(SecLabelStmt) out, const SecLabelStmt *node)
{
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_NODE_PTR_FIELD(object, object, object);
  WRITE_STRING_FIELD(provider, provider, provider);
  WRITE_STRING_FIELD(label, label, label);
}

static void
_outCreateForeignTableStmt(OUT_TYPE(CreateForeignTableStmt) out, const CreateForeignTableStmt *node)
{
  WRITE_SPECIFIC_NODE_FIELD(CreateStmt, create_stmt, base_stmt, base, base);
  WRITE_STRING_FIELD(servername, servername, servername);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outImportForeignSchemaStmt(OUT_TYPE(ImportForeignSchemaStmt) out, const ImportForeignSchemaStmt *node)
{
  WRITE_STRING_FIELD(server_name, server_name, server_name);
  WRITE_STRING_FIELD(remote_schema, remote_schema, remote_schema);
  WRITE_STRING_FIELD(local_schema, local_schema, local_schema);
  WRITE_ENUM_FIELD(ImportForeignSchemaType, list_type, list_type, list_type);
  WRITE_LIST_FIELD(table_list, table_list, table_list);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outCreateExtensionStmt(OUT_TYPE(CreateExtensionStmt) out, const CreateExtensionStmt *node)
{
  WRITE_STRING_FIELD(extname, extname, extname);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterExtensionStmt(OUT_TYPE(AlterExtensionStmt) out, const AlterExtensionStmt *node)
{
  WRITE_STRING_FIELD(extname, extname, extname);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterExtensionContentsStmt(OUT_TYPE(AlterExtensionContentsStmt) out, const AlterExtensionContentsStmt *node)
{
  WRITE_STRING_FIELD(extname, extname, extname);
  WRITE_INT_FIELD(action, action, action);
  WRITE_ENUM_FIELD(ObjectType, objtype, objtype, objtype);
  WRITE_NODE_PTR_FIELD(object, object, object);
}

static void
_outCreateEventTrigStmt(OUT_TYPE(CreateEventTrigStmt) out, const CreateEventTrigStmt *node)
{
  WRITE_STRING_FIELD(trigname, trigname, trigname);
  WRITE_STRING_FIELD(eventname, eventname, eventname);
  WRITE_LIST_FIELD(whenclause, whenclause, whenclause);
  WRITE_LIST_FIELD(funcname, funcname, funcname);
}

static void
_outAlterEventTrigStmt(OUT_TYPE(AlterEventTrigStmt) out, const AlterEventTrigStmt *node)
{
  WRITE_STRING_FIELD(trigname, trigname, trigname);
  WRITE_CHAR_FIELD(tgenabled, tgenabled, tgenabled);
}

static void
_outRefreshMatViewStmt(OUT_TYPE(RefreshMatViewStmt) out, const RefreshMatViewStmt *node)
{
  WRITE_BOOL_FIELD(concurrent, concurrent, concurrent);
  WRITE_BOOL_FIELD(skip_data, skipData, skipData);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
}

static void
_outReplicaIdentityStmt(OUT_TYPE(ReplicaIdentityStmt) out, const ReplicaIdentityStmt *node)
{
  WRITE_CHAR_FIELD(identity_type, identity_type, identity_type);
  WRITE_STRING_FIELD(name, name, name);
}

static void
_outAlterSystemStmt(OUT_TYPE(AlterSystemStmt) out, const AlterSystemStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(VariableSetStmt, variable_set_stmt, setstmt, setstmt, setstmt);
}

static void
_outCreatePolicyStmt(OUT_TYPE(CreatePolicyStmt) out, const CreatePolicyStmt *node)
{
  WRITE_STRING_FIELD(policy_name, policy_name, policy_name);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, table, table, table);
  WRITE_STRING_FIELD(cmd_name, cmd_name, cmd_name);
  WRITE_BOOL_FIELD(permissive, permissive, permissive);
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_NODE_PTR_FIELD(qual, qual, qual);
  WRITE_NODE_PTR_FIELD(with_check, with_check, with_check);
}

static void
_outAlterPolicyStmt(OUT_TYPE(AlterPolicyStmt) out, const AlterPolicyStmt *node)
{
  WRITE_STRING_FIELD(policy_name, policy_name, policy_name);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, table, table, table);
  WRITE_LIST_FIELD(roles, roles, roles);
  WRITE_NODE_PTR_FIELD(qual, qual, qual);
  WRITE_NODE_PTR_FIELD(with_check, with_check, with_check);
}

static void
_outCreateTransformStmt(OUT_TYPE(CreateTransformStmt) out, const CreateTransformStmt *node)
{
  WRITE_BOOL_FIELD(replace, replace, replace);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, type_name, type_name);
  WRITE_STRING_FIELD(lang, lang, lang);
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, fromsql, fromsql, fromsql);
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, tosql, tosql, tosql);
}

static void
_outCreateAmStmt(OUT_TYPE(CreateAmStmt) out, const CreateAmStmt *node)
{
  WRITE_STRING_FIELD(amname, amname, amname);
  WRITE_LIST_FIELD(handler_name, handler_name, handler_name);
  WRITE_CHAR_FIELD(amtype, amtype, amtype);
}

static void
_outCreatePublicationStmt(OUT_TYPE(CreatePublicationStmt) out, const CreatePublicationStmt *node)
{
  WRITE_STRING_FIELD(pubname, pubname, pubname);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_LIST_FIELD(tables, tables, tables);
  WRITE_BOOL_FIELD(for_all_tables, for_all_tables, for_all_tables);
}

static void
_outAlterPublicationStmt(OUT_TYPE(AlterPublicationStmt) out, const AlterPublicationStmt *node)
{
  WRITE_STRING_FIELD(pubname, pubname, pubname);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_LIST_FIELD(tables, tables, tables);
  WRITE_BOOL_FIELD(for_all_tables, for_all_tables, for_all_tables);
  WRITE_ENUM_FIELD(DefElemAction, table_action, tableAction, tableAction);
}

static void
_outCreateSubscriptionStmt(OUT_TYPE(CreateSubscriptionStmt) out, const CreateSubscriptionStmt *node)
{
  WRITE_STRING_FIELD(subname, subname, subname);
  WRITE_STRING_FIELD(conninfo, conninfo, conninfo);
  WRITE_LIST_FIELD(publication, publication, publication);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outAlterSubscriptionStmt(OUT_TYPE(AlterSubscriptionStmt) out, const AlterSubscriptionStmt *node)
{
  WRITE_ENUM_FIELD(AlterSubscriptionType, kind, kind, kind);
  WRITE_STRING_FIELD(subname, subname, subname);
  WRITE_STRING_FIELD(conninfo, conninfo, conninfo);
  WRITE_LIST_FIELD(publication, publication, publication);
  WRITE_LIST_FIELD(options, options, options);
}

static void
_outDropSubscriptionStmt(OUT_TYPE(DropSubscriptionStmt) out, const DropSubscriptionStmt *node)
{
  WRITE_STRING_FIELD(subname, subname, subname);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
  WRITE_ENUM_FIELD(DropBehavior, behavior, behavior, behavior);
}

static void
_outCreateStatsStmt(OUT_TYPE(CreateStatsStmt) out, const CreateStatsStmt *node)
{
  WRITE_LIST_FIELD(defnames, defnames, defnames);
  WRITE_LIST_FIELD(stat_types, stat_types, stat_types);
  WRITE_LIST_FIELD(exprs, exprs, exprs);
  WRITE_LIST_FIELD(relations, relations, relations);
  WRITE_STRING_FIELD(stxcomment, stxcomment, stxcomment);
  WRITE_BOOL_FIELD(if_not_exists, if_not_exists, if_not_exists);
}

static void
_outAlterCollationStmt(OUT_TYPE(AlterCollationStmt) out, const AlterCollationStmt *node)
{
  WRITE_LIST_FIELD(collname, collname, collname);
}

static void
_outCallStmt(OUT_TYPE(CallStmt) out, const CallStmt *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(FuncCall, func_call, funccall, funccall, funccall);
  WRITE_SPECIFIC_NODE_PTR_FIELD(FuncExpr, func_expr, funcexpr, funcexpr, funcexpr);
}

static void
_outAlterStatsStmt(OUT_TYPE(AlterStatsStmt) out, const AlterStatsStmt *node)
{
  WRITE_LIST_FIELD(defnames, defnames, defnames);
  WRITE_INT_FIELD(stxstattarget, stxstattarget, stxstattarget);
  WRITE_BOOL_FIELD(missing_ok, missing_ok, missing_ok);
}

static void
_outAExpr(OUT_TYPE(AExpr) out, const A_Expr *node)
{
  WRITE_ENUM_FIELD(A_Expr_Kind, kind, kind, kind);
  WRITE_LIST_FIELD(name, name, name);
  WRITE_NODE_PTR_FIELD(lexpr, lexpr, lexpr);
  WRITE_NODE_PTR_FIELD(rexpr, rexpr, rexpr);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outColumnRef(OUT_TYPE(ColumnRef) out, const ColumnRef *node)
{
  WRITE_LIST_FIELD(fields, fields, fields);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outParamRef(OUT_TYPE(ParamRef) out, const ParamRef *node)
{
  WRITE_INT_FIELD(number, number, number);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outAConst(OUT_TYPE(AConst) out, const A_Const *node)
{
  WRITE_NODE_FIELD(val, val, val);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outFuncCall(OUT_TYPE(FuncCall) out, const FuncCall *node)
{
  WRITE_LIST_FIELD(funcname, funcname, funcname);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_LIST_FIELD(agg_order, agg_order, agg_order);
  WRITE_NODE_PTR_FIELD(agg_filter, agg_filter, agg_filter);
  WRITE_BOOL_FIELD(agg_within_group, agg_within_group, agg_within_group);
  WRITE_BOOL_FIELD(agg_star, agg_star, agg_star);
  WRITE_BOOL_FIELD(agg_distinct, agg_distinct, agg_distinct);
  WRITE_BOOL_FIELD(func_variadic, func_variadic, func_variadic);
  WRITE_SPECIFIC_NODE_PTR_FIELD(WindowDef, window_def, over, over, over);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outAStar(OUT_TYPE(AStar) out, const A_Star *node)
{
}

static void
_outAIndices(OUT_TYPE(AIndices) out, const A_Indices *node)
{
  WRITE_BOOL_FIELD(is_slice, is_slice, is_slice);
  WRITE_NODE_PTR_FIELD(lidx, lidx, lidx);
  WRITE_NODE_PTR_FIELD(uidx, uidx, uidx);
}

static void
_outAIndirection(OUT_TYPE(AIndirection) out, const A_Indirection *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_LIST_FIELD(indirection, indirection, indirection);
}

static void
_outAArrayExpr(OUT_TYPE(AArrayExpr) out, const A_ArrayExpr *node)
{
  WRITE_LIST_FIELD(elements, elements, elements);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outResTarget(OUT_TYPE(ResTarget) out, const ResTarget *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_LIST_FIELD(indirection, indirection, indirection);
  WRITE_NODE_PTR_FIELD(val, val, val);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outMultiAssignRef(OUT_TYPE(MultiAssignRef) out, const MultiAssignRef *node)
{
  WRITE_NODE_PTR_FIELD(source, source, source);
  WRITE_INT_FIELD(colno, colno, colno);
  WRITE_INT_FIELD(ncolumns, ncolumns, ncolumns);
}

static void
_outTypeCast(OUT_TYPE(TypeCast) out, const TypeCast *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, typeName, typeName);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCollateClause(OUT_TYPE(CollateClause) out, const CollateClause *node)
{
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_LIST_FIELD(collname, collname, collname);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outSortBy(OUT_TYPE(SortBy) out, const SortBy *node)
{
  WRITE_NODE_PTR_FIELD(node, node, node);
  WRITE_ENUM_FIELD(SortByDir, sortby_dir, sortby_dir, sortby_dir);
  WRITE_ENUM_FIELD(SortByNulls, sortby_nulls, sortby_nulls, sortby_nulls);
  WRITE_LIST_FIELD(use_op, useOp, useOp);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outWindowDef(OUT_TYPE(WindowDef) out, const WindowDef *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_STRING_FIELD(refname, refname, refname);
  WRITE_LIST_FIELD(partition_clause, partitionClause, partitionClause);
  WRITE_LIST_FIELD(order_clause, orderClause, orderClause);
  WRITE_INT_FIELD(frame_options, frameOptions, frameOptions);
  WRITE_NODE_PTR_FIELD(start_offset, startOffset, startOffset);
  WRITE_NODE_PTR_FIELD(end_offset, endOffset, endOffset);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRangeSubselect(OUT_TYPE(RangeSubselect) out, const RangeSubselect *node)
{
  WRITE_BOOL_FIELD(lateral, lateral, lateral);
  WRITE_NODE_PTR_FIELD(subquery, subquery, subquery);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
}

static void
_outRangeFunction(OUT_TYPE(RangeFunction) out, const RangeFunction *node)
{
  WRITE_BOOL_FIELD(lateral, lateral, lateral);
  WRITE_BOOL_FIELD(ordinality, ordinality, ordinality);
  WRITE_BOOL_FIELD(is_rowsfrom, is_rowsfrom, is_rowsfrom);
  WRITE_LIST_FIELD(functions, functions, functions);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
  WRITE_LIST_FIELD(coldeflist, coldeflist, coldeflist);
}

static void
_outRangeTableSample(OUT_TYPE(RangeTableSample) out, const RangeTableSample *node)
{
  WRITE_NODE_PTR_FIELD(relation, relation, relation);
  WRITE_LIST_FIELD(method, method, method);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_NODE_PTR_FIELD(repeatable, repeatable, repeatable);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRangeTableFunc(OUT_TYPE(RangeTableFunc) out, const RangeTableFunc *node)
{
  WRITE_BOOL_FIELD(lateral, lateral, lateral);
  WRITE_NODE_PTR_FIELD(docexpr, docexpr, docexpr);
  WRITE_NODE_PTR_FIELD(rowexpr, rowexpr, rowexpr);
  WRITE_LIST_FIELD(namespaces, namespaces, namespaces);
  WRITE_LIST_FIELD(columns, columns, columns);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRangeTableFuncCol(OUT_TYPE(RangeTableFuncCol) out, const RangeTableFuncCol *node)
{
  WRITE_STRING_FIELD(colname, colname, colname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, typeName, typeName);
  WRITE_BOOL_FIELD(for_ordinality, for_ordinality, for_ordinality);
  WRITE_BOOL_FIELD(is_not_null, is_not_null, is_not_null);
  WRITE_NODE_PTR_FIELD(colexpr, colexpr, colexpr);
  WRITE_NODE_PTR_FIELD(coldefexpr, coldefexpr, coldefexpr);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outTypeName(OUT_TYPE(TypeName) out, const TypeName *node)
{
  WRITE_LIST_FIELD(names, names, names);
  WRITE_UINT_FIELD(type_oid, typeOid, typeOid);
  WRITE_BOOL_FIELD(setof, setof, setof);
  WRITE_BOOL_FIELD(pct_type, pct_type, pct_type);
  WRITE_LIST_FIELD(typmods, typmods, typmods);
  WRITE_INT_FIELD(typemod, typemod, typemod);
  WRITE_LIST_FIELD(array_bounds, arrayBounds, arrayBounds);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outColumnDef(OUT_TYPE(ColumnDef) out, const ColumnDef *node)
{
  WRITE_STRING_FIELD(colname, colname, colname);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, typeName, typeName);
  WRITE_INT_FIELD(inhcount, inhcount, inhcount);
  WRITE_BOOL_FIELD(is_local, is_local, is_local);
  WRITE_BOOL_FIELD(is_not_null, is_not_null, is_not_null);
  WRITE_BOOL_FIELD(is_from_type, is_from_type, is_from_type);
  WRITE_CHAR_FIELD(storage, storage, storage);
  WRITE_NODE_PTR_FIELD(raw_default, raw_default, raw_default);
  WRITE_NODE_PTR_FIELD(cooked_default, cooked_default, cooked_default);
  WRITE_CHAR_FIELD(identity, identity, identity);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, identity_sequence, identitySequence, identitySequence);
  WRITE_CHAR_FIELD(generated, generated, generated);
  WRITE_SPECIFIC_NODE_PTR_FIELD(CollateClause, collate_clause, coll_clause, collClause, collClause);
  WRITE_UINT_FIELD(coll_oid, collOid, collOid);
  WRITE_LIST_FIELD(constraints, constraints, constraints);
  WRITE_LIST_FIELD(fdwoptions, fdwoptions, fdwoptions);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outIndexElem(OUT_TYPE(IndexElem) out, const IndexElem *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_STRING_FIELD(indexcolname, indexcolname, indexcolname);
  WRITE_LIST_FIELD(collation, collation, collation);
  WRITE_LIST_FIELD(opclass, opclass, opclass);
  WRITE_LIST_FIELD(opclassopts, opclassopts, opclassopts);
  WRITE_ENUM_FIELD(SortByDir, ordering, ordering, ordering);
  WRITE_ENUM_FIELD(SortByNulls, nulls_ordering, nulls_ordering, nulls_ordering);
}

static void
_outConstraint(OUT_TYPE(Constraint) out, const Constraint *node)
{
  WRITE_ENUM_FIELD(ConstrType, contype, contype, contype);
  WRITE_STRING_FIELD(conname, conname, conname);
  WRITE_BOOL_FIELD(deferrable, deferrable, deferrable);
  WRITE_BOOL_FIELD(initdeferred, initdeferred, initdeferred);
  WRITE_INT_FIELD(location, location, location);
  WRITE_BOOL_FIELD(is_no_inherit, is_no_inherit, is_no_inherit);
  WRITE_NODE_PTR_FIELD(raw_expr, raw_expr, raw_expr);
  WRITE_STRING_FIELD(cooked_expr, cooked_expr, cooked_expr);
  WRITE_CHAR_FIELD(generated_when, generated_when, generated_when);
  WRITE_LIST_FIELD(keys, keys, keys);
  WRITE_LIST_FIELD(including, including, including);
  WRITE_LIST_FIELD(exclusions, exclusions, exclusions);
  WRITE_LIST_FIELD(options, options, options);
  WRITE_STRING_FIELD(indexname, indexname, indexname);
  WRITE_STRING_FIELD(indexspace, indexspace, indexspace);
  WRITE_BOOL_FIELD(reset_default_tblspc, reset_default_tblspc, reset_default_tblspc);
  WRITE_STRING_FIELD(access_method, access_method, access_method);
  WRITE_NODE_PTR_FIELD(where_clause, where_clause, where_clause);
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, pktable, pktable, pktable);
  WRITE_LIST_FIELD(fk_attrs, fk_attrs, fk_attrs);
  WRITE_LIST_FIELD(pk_attrs, pk_attrs, pk_attrs);
  WRITE_CHAR_FIELD(fk_matchtype, fk_matchtype, fk_matchtype);
  WRITE_CHAR_FIELD(fk_upd_action, fk_upd_action, fk_upd_action);
  WRITE_CHAR_FIELD(fk_del_action, fk_del_action, fk_del_action);
  WRITE_LIST_FIELD(old_conpfeqop, old_conpfeqop, old_conpfeqop);
  WRITE_UINT_FIELD(old_pktable_oid, old_pktable_oid, old_pktable_oid);
  WRITE_BOOL_FIELD(skip_validation, skip_validation, skip_validation);
  WRITE_BOOL_FIELD(initially_valid, initially_valid, initially_valid);
}

static void
_outDefElem(OUT_TYPE(DefElem) out, const DefElem *node)
{
  WRITE_STRING_FIELD(defnamespace, defnamespace, defnamespace);
  WRITE_STRING_FIELD(defname, defname, defname);
  WRITE_NODE_PTR_FIELD(arg, arg, arg);
  WRITE_ENUM_FIELD(DefElemAction, defaction, defaction, defaction);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outRangeTblEntry(OUT_TYPE(RangeTblEntry) out, const RangeTblEntry *node)
{
  WRITE_ENUM_FIELD(RTEKind, rtekind, rtekind, rtekind);
  WRITE_UINT_FIELD(relid, relid, relid);
  WRITE_CHAR_FIELD(relkind, relkind, relkind);
  WRITE_INT_FIELD(rellockmode, rellockmode, rellockmode);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TableSampleClause, table_sample_clause, tablesample, tablesample, tablesample);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Query, query, subquery, subquery, subquery);
  WRITE_BOOL_FIELD(security_barrier, security_barrier, security_barrier);
  WRITE_ENUM_FIELD(JoinType, jointype, jointype, jointype);
  WRITE_INT_FIELD(joinmergedcols, joinmergedcols, joinmergedcols);
  WRITE_LIST_FIELD(joinaliasvars, joinaliasvars, joinaliasvars);
  WRITE_LIST_FIELD(joinleftcols, joinleftcols, joinleftcols);
  WRITE_LIST_FIELD(joinrightcols, joinrightcols, joinrightcols);
  WRITE_LIST_FIELD(functions, functions, functions);
  WRITE_BOOL_FIELD(funcordinality, funcordinality, funcordinality);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TableFunc, table_func, tablefunc, tablefunc, tablefunc);
  WRITE_LIST_FIELD(values_lists, values_lists, values_lists);
  WRITE_STRING_FIELD(ctename, ctename, ctename);
  WRITE_UINT_FIELD(ctelevelsup, ctelevelsup, ctelevelsup);
  WRITE_BOOL_FIELD(self_reference, self_reference, self_reference);
  WRITE_LIST_FIELD(coltypes, coltypes, coltypes);
  WRITE_LIST_FIELD(coltypmods, coltypmods, coltypmods);
  WRITE_LIST_FIELD(colcollations, colcollations, colcollations);
  WRITE_STRING_FIELD(enrname, enrname, enrname);
  WRITE_FLOAT_FIELD(enrtuples, enrtuples, enrtuples);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, alias, alias, alias);
  WRITE_SPECIFIC_NODE_PTR_FIELD(Alias, alias, eref, eref, eref);
  WRITE_BOOL_FIELD(lateral, lateral, lateral);
  WRITE_BOOL_FIELD(inh, inh, inh);
  WRITE_BOOL_FIELD(in_from_cl, inFromCl, inFromCl);
  WRITE_UINT_FIELD(required_perms, requiredPerms, requiredPerms);
  WRITE_UINT_FIELD(check_as_user, checkAsUser, checkAsUser);
  WRITE_BITMAPSET_FIELD(selected_cols, selectedCols, selectedCols);
  WRITE_BITMAPSET_FIELD(inserted_cols, insertedCols, insertedCols);
  WRITE_BITMAPSET_FIELD(updated_cols, updatedCols, updatedCols);
  WRITE_BITMAPSET_FIELD(extra_updated_cols, extraUpdatedCols, extraUpdatedCols);
  WRITE_LIST_FIELD(security_quals, securityQuals, securityQuals);
}

static void
_outRangeTblFunction(OUT_TYPE(RangeTblFunction) out, const RangeTblFunction *node)
{
  WRITE_NODE_PTR_FIELD(funcexpr, funcexpr, funcexpr);
  WRITE_INT_FIELD(funccolcount, funccolcount, funccolcount);
  WRITE_LIST_FIELD(funccolnames, funccolnames, funccolnames);
  WRITE_LIST_FIELD(funccoltypes, funccoltypes, funccoltypes);
  WRITE_LIST_FIELD(funccoltypmods, funccoltypmods, funccoltypmods);
  WRITE_LIST_FIELD(funccolcollations, funccolcollations, funccolcollations);
  WRITE_BITMAPSET_FIELD(funcparams, funcparams, funcparams);
}

static void
_outTableSampleClause(OUT_TYPE(TableSampleClause) out, const TableSampleClause *node)
{
  WRITE_UINT_FIELD(tsmhandler, tsmhandler, tsmhandler);
  WRITE_LIST_FIELD(args, args, args);
  WRITE_NODE_PTR_FIELD(repeatable, repeatable, repeatable);
}

static void
_outWithCheckOption(OUT_TYPE(WithCheckOption) out, const WithCheckOption *node)
{
  WRITE_ENUM_FIELD(WCOKind, kind, kind, kind);
  WRITE_STRING_FIELD(relname, relname, relname);
  WRITE_STRING_FIELD(polname, polname, polname);
  WRITE_NODE_PTR_FIELD(qual, qual, qual);
  WRITE_BOOL_FIELD(cascaded, cascaded, cascaded);
}

static void
_outSortGroupClause(OUT_TYPE(SortGroupClause) out, const SortGroupClause *node)
{
  WRITE_UINT_FIELD(tle_sort_group_ref, tleSortGroupRef, tleSortGroupRef);
  WRITE_UINT_FIELD(eqop, eqop, eqop);
  WRITE_UINT_FIELD(sortop, sortop, sortop);
  WRITE_BOOL_FIELD(nulls_first, nulls_first, nulls_first);
  WRITE_BOOL_FIELD(hashable, hashable, hashable);
}

static void
_outGroupingSet(OUT_TYPE(GroupingSet) out, const GroupingSet *node)
{
  WRITE_ENUM_FIELD(GroupingSetKind, kind, kind, kind);
  WRITE_LIST_FIELD(content, content, content);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outWindowClause(OUT_TYPE(WindowClause) out, const WindowClause *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_STRING_FIELD(refname, refname, refname);
  WRITE_LIST_FIELD(partition_clause, partitionClause, partitionClause);
  WRITE_LIST_FIELD(order_clause, orderClause, orderClause);
  WRITE_INT_FIELD(frame_options, frameOptions, frameOptions);
  WRITE_NODE_PTR_FIELD(start_offset, startOffset, startOffset);
  WRITE_NODE_PTR_FIELD(end_offset, endOffset, endOffset);
  WRITE_UINT_FIELD(start_in_range_func, startInRangeFunc, startInRangeFunc);
  WRITE_UINT_FIELD(end_in_range_func, endInRangeFunc, endInRangeFunc);
  WRITE_UINT_FIELD(in_range_coll, inRangeColl, inRangeColl);
  WRITE_BOOL_FIELD(in_range_asc, inRangeAsc, inRangeAsc);
  WRITE_BOOL_FIELD(in_range_nulls_first, inRangeNullsFirst, inRangeNullsFirst);
  WRITE_UINT_FIELD(winref, winref, winref);
  WRITE_BOOL_FIELD(copied_order, copiedOrder, copiedOrder);
}

static void
_outObjectWithArgs(OUT_TYPE(ObjectWithArgs) out, const ObjectWithArgs *node)
{
  WRITE_LIST_FIELD(objname, objname, objname);
  WRITE_LIST_FIELD(objargs, objargs, objargs);
  WRITE_BOOL_FIELD(args_unspecified, args_unspecified, args_unspecified);
}

static void
_outAccessPriv(OUT_TYPE(AccessPriv) out, const AccessPriv *node)
{
  WRITE_STRING_FIELD(priv_name, priv_name, priv_name);
  WRITE_LIST_FIELD(cols, cols, cols);
}

static void
_outCreateOpClassItem(OUT_TYPE(CreateOpClassItem) out, const CreateOpClassItem *node)
{
  WRITE_INT_FIELD(itemtype, itemtype, itemtype);
  WRITE_SPECIFIC_NODE_PTR_FIELD(ObjectWithArgs, object_with_args, name, name, name);
  WRITE_INT_FIELD(number, number, number);
  WRITE_LIST_FIELD(order_family, order_family, order_family);
  WRITE_LIST_FIELD(class_args, class_args, class_args);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, storedtype, storedtype, storedtype);
}

static void
_outTableLikeClause(OUT_TYPE(TableLikeClause) out, const TableLikeClause *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_UINT_FIELD(options, options, options);
}

static void
_outFunctionParameter(OUT_TYPE(FunctionParameter) out, const FunctionParameter *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, arg_type, argType, argType);
  WRITE_ENUM_FIELD(FunctionParameterMode, mode, mode, mode);
  WRITE_NODE_PTR_FIELD(defexpr, defexpr, defexpr);
}

static void
_outLockingClause(OUT_TYPE(LockingClause) out, const LockingClause *node)
{
  WRITE_LIST_FIELD(locked_rels, lockedRels, lockedRels);
  WRITE_ENUM_FIELD(LockClauseStrength, strength, strength, strength);
  WRITE_ENUM_FIELD(LockWaitPolicy, wait_policy, waitPolicy, waitPolicy);
}

static void
_outRowMarkClause(OUT_TYPE(RowMarkClause) out, const RowMarkClause *node)
{
  WRITE_UINT_FIELD(rti, rti, rti);
  WRITE_ENUM_FIELD(LockClauseStrength, strength, strength, strength);
  WRITE_ENUM_FIELD(LockWaitPolicy, wait_policy, waitPolicy, waitPolicy);
  WRITE_BOOL_FIELD(pushed_down, pushedDown, pushedDown);
}

static void
_outXmlSerialize(OUT_TYPE(XmlSerialize) out, const XmlSerialize *node)
{
  WRITE_ENUM_FIELD(XmlOptionType, xmloption, xmloption, xmloption);
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_SPECIFIC_NODE_PTR_FIELD(TypeName, type_name, type_name, typeName, typeName);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outWithClause(OUT_TYPE(WithClause) out, const WithClause *node)
{
  WRITE_LIST_FIELD(ctes, ctes, ctes);
  WRITE_BOOL_FIELD(recursive, recursive, recursive);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outInferClause(OUT_TYPE(InferClause) out, const InferClause *node)
{
  WRITE_LIST_FIELD(index_elems, indexElems, indexElems);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_STRING_FIELD(conname, conname, conname);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outOnConflictClause(OUT_TYPE(OnConflictClause) out, const OnConflictClause *node)
{
  WRITE_ENUM_FIELD(OnConflictAction, action, action, action);
  WRITE_SPECIFIC_NODE_PTR_FIELD(InferClause, infer_clause, infer, infer, infer);
  WRITE_LIST_FIELD(target_list, targetList, targetList);
  WRITE_NODE_PTR_FIELD(where_clause, whereClause, whereClause);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outCommonTableExpr(OUT_TYPE(CommonTableExpr) out, const CommonTableExpr *node)
{
  WRITE_STRING_FIELD(ctename, ctename, ctename);
  WRITE_LIST_FIELD(aliascolnames, aliascolnames, aliascolnames);
  WRITE_ENUM_FIELD(CTEMaterialize, ctematerialized, ctematerialized, ctematerialized);
  WRITE_NODE_PTR_FIELD(ctequery, ctequery, ctequery);
  WRITE_INT_FIELD(location, location, location);
  WRITE_BOOL_FIELD(cterecursive, cterecursive, cterecursive);
  WRITE_INT_FIELD(cterefcount, cterefcount, cterefcount);
  WRITE_LIST_FIELD(ctecolnames, ctecolnames, ctecolnames);
  WRITE_LIST_FIELD(ctecoltypes, ctecoltypes, ctecoltypes);
  WRITE_LIST_FIELD(ctecoltypmods, ctecoltypmods, ctecoltypmods);
  WRITE_LIST_FIELD(ctecolcollations, ctecolcollations, ctecolcollations);
}

static void
_outRoleSpec(OUT_TYPE(RoleSpec) out, const RoleSpec *node)
{
  WRITE_ENUM_FIELD(RoleSpecType, roletype, roletype, roletype);
  WRITE_STRING_FIELD(rolename, rolename, rolename);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outTriggerTransition(OUT_TYPE(TriggerTransition) out, const TriggerTransition *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_BOOL_FIELD(is_new, isNew, isNew);
  WRITE_BOOL_FIELD(is_table, isTable, isTable);
}

static void
_outPartitionElem(OUT_TYPE(PartitionElem) out, const PartitionElem *node)
{
  WRITE_STRING_FIELD(name, name, name);
  WRITE_NODE_PTR_FIELD(expr, expr, expr);
  WRITE_LIST_FIELD(collation, collation, collation);
  WRITE_LIST_FIELD(opclass, opclass, opclass);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outPartitionSpec(OUT_TYPE(PartitionSpec) out, const PartitionSpec *node)
{
  WRITE_STRING_FIELD(strategy, strategy, strategy);
  WRITE_LIST_FIELD(part_params, partParams, partParams);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outPartitionBoundSpec(OUT_TYPE(PartitionBoundSpec) out, const PartitionBoundSpec *node)
{
  WRITE_CHAR_FIELD(strategy, strategy, strategy);
  WRITE_BOOL_FIELD(is_default, is_default, is_default);
  WRITE_INT_FIELD(modulus, modulus, modulus);
  WRITE_INT_FIELD(remainder, remainder, remainder);
  WRITE_LIST_FIELD(listdatums, listdatums, listdatums);
  WRITE_LIST_FIELD(lowerdatums, lowerdatums, lowerdatums);
  WRITE_LIST_FIELD(upperdatums, upperdatums, upperdatums);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outPartitionRangeDatum(OUT_TYPE(PartitionRangeDatum) out, const PartitionRangeDatum *node)
{
  WRITE_ENUM_FIELD(PartitionRangeDatumKind, kind, kind, kind);
  WRITE_NODE_PTR_FIELD(value, value, value);
  WRITE_INT_FIELD(location, location, location);
}

static void
_outPartitionCmd(OUT_TYPE(PartitionCmd) out, const PartitionCmd *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, name, name, name);
  WRITE_SPECIFIC_NODE_PTR_FIELD(PartitionBoundSpec, partition_bound_spec, bound, bound, bound);
}

static void
_outVacuumRelation(OUT_TYPE(VacuumRelation) out, const VacuumRelation *node)
{
  WRITE_SPECIFIC_NODE_PTR_FIELD(RangeVar, range_var, relation, relation, relation);
  WRITE_UINT_FIELD(oid, oid, oid);
  WRITE_LIST_FIELD(va_cols, va_cols, va_cols);
}

static void
_outInlineCodeBlock(OUT_TYPE(InlineCodeBlock) out, const InlineCodeBlock *node)
{
  WRITE_STRING_FIELD(source_text, source_text, source_text);
  WRITE_UINT_FIELD(lang_oid, langOid, langOid);
  WRITE_BOOL_FIELD(lang_is_trusted, langIsTrusted, langIsTrusted);
  WRITE_BOOL_FIELD(atomic, atomic, atomic);
}

static void
_outCallContext(OUT_TYPE(CallContext) out, const CallContext *node)
{
  WRITE_BOOL_FIELD(atomic, atomic, atomic);
}


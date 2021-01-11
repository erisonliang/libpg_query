// This file is autogenerated by ./scripts/generate_protobuf_and_outfuncs.rb

case T_Integer:
  OUT_NODE(Integer, Integer, integer, INTEGER, Value, integer);
  break;
case T_Float:
  OUT_NODE(Float, Float, float, FLOAT, Value, float_);
  break;
case T_String:
  OUT_NODE(String, String, string, STRING, Value, string);
  break;
case T_BitString:
  OUT_NODE(BitString, BitString, bit_string, BIT_STRING, Value, bit_string);
  break;
case T_Null:
  OUT_NODE(Null, Null, null, NULL, Value, null);
  break;
case T_List:
  OUT_NODE(List, List, list, LIST, List, list);
  break;
case T_IntList:
  OUT_NODE(IntList, IntList, int_list, INT_LIST, List, int_list);
  break;
case T_OidList:
  OUT_NODE(OidList, OidList, oid_list, OID_LIST, List, oid_list);
  break;
case T_Alias:
  OUT_NODE(Alias, Alias, alias, ALIAS, Alias, alias);
  break;
case T_RangeVar:
  OUT_NODE(RangeVar, RangeVar, range_var, RANGE_VAR, RangeVar, range_var);
  break;
case T_TableFunc:
  OUT_NODE(TableFunc, TableFunc, table_func, TABLE_FUNC, TableFunc, table_func);
  break;
case T_Expr:
  OUT_NODE(Expr, Expr, expr, EXPR, Expr, expr);
  break;
case T_Var:
  OUT_NODE(Var, Var, var, VAR, Var, var);
  break;
case T_Param:
  OUT_NODE(Param, Param, param, PARAM, Param, param);
  break;
case T_Aggref:
  OUT_NODE(Aggref, Aggref, aggref, AGGREF, Aggref, aggref);
  break;
case T_GroupingFunc:
  OUT_NODE(GroupingFunc, GroupingFunc, grouping_func, GROUPING_FUNC, GroupingFunc, grouping_func);
  break;
case T_WindowFunc:
  OUT_NODE(WindowFunc, WindowFunc, window_func, WINDOW_FUNC, WindowFunc, window_func);
  break;
case T_SubscriptingRef:
  OUT_NODE(SubscriptingRef, SubscriptingRef, subscripting_ref, SUBSCRIPTING_REF, SubscriptingRef, subscripting_ref);
  break;
case T_FuncExpr:
  OUT_NODE(FuncExpr, FuncExpr, func_expr, FUNC_EXPR, FuncExpr, func_expr);
  break;
case T_NamedArgExpr:
  OUT_NODE(NamedArgExpr, NamedArgExpr, named_arg_expr, NAMED_ARG_EXPR, NamedArgExpr, named_arg_expr);
  break;
case T_OpExpr:
  OUT_NODE(OpExpr, OpExpr, op_expr, OP_EXPR, OpExpr, op_expr);
  break;
case T_DistinctExpr:
  OUT_NODE(DistinctExpr, DistinctExpr, distinct_expr, DISTINCT_EXPR, DistinctExpr, distinct_expr);
  break;
case T_NullIfExpr:
  OUT_NODE(NullIfExpr, NullIfExpr, null_if_expr, NULL_IF_EXPR, NullIfExpr, null_if_expr);
  break;
case T_ScalarArrayOpExpr:
  OUT_NODE(ScalarArrayOpExpr, ScalarArrayOpExpr, scalar_array_op_expr, SCALAR_ARRAY_OP_EXPR, ScalarArrayOpExpr, scalar_array_op_expr);
  break;
case T_BoolExpr:
  OUT_NODE(BoolExpr, BoolExpr, bool_expr, BOOL_EXPR, BoolExpr, bool_expr);
  break;
case T_SubLink:
  OUT_NODE(SubLink, SubLink, sub_link, SUB_LINK, SubLink, sub_link);
  break;
case T_SubPlan:
  OUT_NODE(SubPlan, SubPlan, sub_plan, SUB_PLAN, SubPlan, sub_plan);
  break;
case T_AlternativeSubPlan:
  OUT_NODE(AlternativeSubPlan, AlternativeSubPlan, alternative_sub_plan, ALTERNATIVE_SUB_PLAN, AlternativeSubPlan, alternative_sub_plan);
  break;
case T_FieldSelect:
  OUT_NODE(FieldSelect, FieldSelect, field_select, FIELD_SELECT, FieldSelect, field_select);
  break;
case T_FieldStore:
  OUT_NODE(FieldStore, FieldStore, field_store, FIELD_STORE, FieldStore, field_store);
  break;
case T_RelabelType:
  OUT_NODE(RelabelType, RelabelType, relabel_type, RELABEL_TYPE, RelabelType, relabel_type);
  break;
case T_CoerceViaIO:
  OUT_NODE(CoerceViaIO, CoerceViaIO, coerce_via_io, COERCE_VIA_IO, CoerceViaIO, coerce_via_io);
  break;
case T_ArrayCoerceExpr:
  OUT_NODE(ArrayCoerceExpr, ArrayCoerceExpr, array_coerce_expr, ARRAY_COERCE_EXPR, ArrayCoerceExpr, array_coerce_expr);
  break;
case T_ConvertRowtypeExpr:
  OUT_NODE(ConvertRowtypeExpr, ConvertRowtypeExpr, convert_rowtype_expr, CONVERT_ROWTYPE_EXPR, ConvertRowtypeExpr, convert_rowtype_expr);
  break;
case T_CollateExpr:
  OUT_NODE(CollateExpr, CollateExpr, collate_expr, COLLATE_EXPR, CollateExpr, collate_expr);
  break;
case T_CaseExpr:
  OUT_NODE(CaseExpr, CaseExpr, case_expr, CASE_EXPR, CaseExpr, case_expr);
  break;
case T_CaseWhen:
  OUT_NODE(CaseWhen, CaseWhen, case_when, CASE_WHEN, CaseWhen, case_when);
  break;
case T_CaseTestExpr:
  OUT_NODE(CaseTestExpr, CaseTestExpr, case_test_expr, CASE_TEST_EXPR, CaseTestExpr, case_test_expr);
  break;
case T_ArrayExpr:
  OUT_NODE(ArrayExpr, ArrayExpr, array_expr, ARRAY_EXPR, ArrayExpr, array_expr);
  break;
case T_RowExpr:
  OUT_NODE(RowExpr, RowExpr, row_expr, ROW_EXPR, RowExpr, row_expr);
  break;
case T_RowCompareExpr:
  OUT_NODE(RowCompareExpr, RowCompareExpr, row_compare_expr, ROW_COMPARE_EXPR, RowCompareExpr, row_compare_expr);
  break;
case T_CoalesceExpr:
  OUT_NODE(CoalesceExpr, CoalesceExpr, coalesce_expr, COALESCE_EXPR, CoalesceExpr, coalesce_expr);
  break;
case T_MinMaxExpr:
  OUT_NODE(MinMaxExpr, MinMaxExpr, min_max_expr, MIN_MAX_EXPR, MinMaxExpr, min_max_expr);
  break;
case T_SQLValueFunction:
  OUT_NODE(SQLValueFunction, SQLValueFunction, sqlvalue_function, SQLVALUE_FUNCTION, SQLValueFunction, sqlvalue_function);
  break;
case T_XmlExpr:
  OUT_NODE(XmlExpr, XmlExpr, xml_expr, XML_EXPR, XmlExpr, xml_expr);
  break;
case T_NullTest:
  OUT_NODE(NullTest, NullTest, null_test, NULL_TEST, NullTest, null_test);
  break;
case T_BooleanTest:
  OUT_NODE(BooleanTest, BooleanTest, boolean_test, BOOLEAN_TEST, BooleanTest, boolean_test);
  break;
case T_CoerceToDomain:
  OUT_NODE(CoerceToDomain, CoerceToDomain, coerce_to_domain, COERCE_TO_DOMAIN, CoerceToDomain, coerce_to_domain);
  break;
case T_CoerceToDomainValue:
  OUT_NODE(CoerceToDomainValue, CoerceToDomainValue, coerce_to_domain_value, COERCE_TO_DOMAIN_VALUE, CoerceToDomainValue, coerce_to_domain_value);
  break;
case T_SetToDefault:
  OUT_NODE(SetToDefault, SetToDefault, set_to_default, SET_TO_DEFAULT, SetToDefault, set_to_default);
  break;
case T_CurrentOfExpr:
  OUT_NODE(CurrentOfExpr, CurrentOfExpr, current_of_expr, CURRENT_OF_EXPR, CurrentOfExpr, current_of_expr);
  break;
case T_NextValueExpr:
  OUT_NODE(NextValueExpr, NextValueExpr, next_value_expr, NEXT_VALUE_EXPR, NextValueExpr, next_value_expr);
  break;
case T_InferenceElem:
  OUT_NODE(InferenceElem, InferenceElem, inference_elem, INFERENCE_ELEM, InferenceElem, inference_elem);
  break;
case T_TargetEntry:
  OUT_NODE(TargetEntry, TargetEntry, target_entry, TARGET_ENTRY, TargetEntry, target_entry);
  break;
case T_RangeTblRef:
  OUT_NODE(RangeTblRef, RangeTblRef, range_tbl_ref, RANGE_TBL_REF, RangeTblRef, range_tbl_ref);
  break;
case T_JoinExpr:
  OUT_NODE(JoinExpr, JoinExpr, join_expr, JOIN_EXPR, JoinExpr, join_expr);
  break;
case T_FromExpr:
  OUT_NODE(FromExpr, FromExpr, from_expr, FROM_EXPR, FromExpr, from_expr);
  break;
case T_OnConflictExpr:
  OUT_NODE(OnConflictExpr, OnConflictExpr, on_conflict_expr, ON_CONFLICT_EXPR, OnConflictExpr, on_conflict_expr);
  break;
case T_IntoClause:
  OUT_NODE(IntoClause, IntoClause, into_clause, INTO_CLAUSE, IntoClause, into_clause);
  break;
case T_RawStmt:
  OUT_NODE(RawStmt, RawStmt, raw_stmt, RAW_STMT, RawStmt, raw_stmt);
  break;
case T_Query:
  OUT_NODE(Query, Query, query, QUERY, Query, query);
  break;
case T_InsertStmt:
  OUT_NODE(InsertStmt, InsertStmt, insert_stmt, INSERT_STMT, InsertStmt, insert_stmt);
  break;
case T_DeleteStmt:
  OUT_NODE(DeleteStmt, DeleteStmt, delete_stmt, DELETE_STMT, DeleteStmt, delete_stmt);
  break;
case T_UpdateStmt:
  OUT_NODE(UpdateStmt, UpdateStmt, update_stmt, UPDATE_STMT, UpdateStmt, update_stmt);
  break;
case T_SelectStmt:
  OUT_NODE(SelectStmt, SelectStmt, select_stmt, SELECT_STMT, SelectStmt, select_stmt);
  break;
case T_AlterTableStmt:
  OUT_NODE(AlterTableStmt, AlterTableStmt, alter_table_stmt, ALTER_TABLE_STMT, AlterTableStmt, alter_table_stmt);
  break;
case T_AlterTableCmd:
  OUT_NODE(AlterTableCmd, AlterTableCmd, alter_table_cmd, ALTER_TABLE_CMD, AlterTableCmd, alter_table_cmd);
  break;
case T_AlterDomainStmt:
  OUT_NODE(AlterDomainStmt, AlterDomainStmt, alter_domain_stmt, ALTER_DOMAIN_STMT, AlterDomainStmt, alter_domain_stmt);
  break;
case T_SetOperationStmt:
  OUT_NODE(SetOperationStmt, SetOperationStmt, set_operation_stmt, SET_OPERATION_STMT, SetOperationStmt, set_operation_stmt);
  break;
case T_GrantStmt:
  OUT_NODE(GrantStmt, GrantStmt, grant_stmt, GRANT_STMT, GrantStmt, grant_stmt);
  break;
case T_GrantRoleStmt:
  OUT_NODE(GrantRoleStmt, GrantRoleStmt, grant_role_stmt, GRANT_ROLE_STMT, GrantRoleStmt, grant_role_stmt);
  break;
case T_AlterDefaultPrivilegesStmt:
  OUT_NODE(AlterDefaultPrivilegesStmt, AlterDefaultPrivilegesStmt, alter_default_privileges_stmt, ALTER_DEFAULT_PRIVILEGES_STMT, AlterDefaultPrivilegesStmt, alter_default_privileges_stmt);
  break;
case T_ClosePortalStmt:
  OUT_NODE(ClosePortalStmt, ClosePortalStmt, close_portal_stmt, CLOSE_PORTAL_STMT, ClosePortalStmt, close_portal_stmt);
  break;
case T_ClusterStmt:
  OUT_NODE(ClusterStmt, ClusterStmt, cluster_stmt, CLUSTER_STMT, ClusterStmt, cluster_stmt);
  break;
case T_CopyStmt:
  OUT_NODE(CopyStmt, CopyStmt, copy_stmt, COPY_STMT, CopyStmt, copy_stmt);
  break;
case T_CreateStmt:
  OUT_NODE(CreateStmt, CreateStmt, create_stmt, CREATE_STMT, CreateStmt, create_stmt);
  break;
case T_DefineStmt:
  OUT_NODE(DefineStmt, DefineStmt, define_stmt, DEFINE_STMT, DefineStmt, define_stmt);
  break;
case T_DropStmt:
  OUT_NODE(DropStmt, DropStmt, drop_stmt, DROP_STMT, DropStmt, drop_stmt);
  break;
case T_TruncateStmt:
  OUT_NODE(TruncateStmt, TruncateStmt, truncate_stmt, TRUNCATE_STMT, TruncateStmt, truncate_stmt);
  break;
case T_CommentStmt:
  OUT_NODE(CommentStmt, CommentStmt, comment_stmt, COMMENT_STMT, CommentStmt, comment_stmt);
  break;
case T_FetchStmt:
  OUT_NODE(FetchStmt, FetchStmt, fetch_stmt, FETCH_STMT, FetchStmt, fetch_stmt);
  break;
case T_IndexStmt:
  OUT_NODE(IndexStmt, IndexStmt, index_stmt, INDEX_STMT, IndexStmt, index_stmt);
  break;
case T_CreateFunctionStmt:
  OUT_NODE(CreateFunctionStmt, CreateFunctionStmt, create_function_stmt, CREATE_FUNCTION_STMT, CreateFunctionStmt, create_function_stmt);
  break;
case T_AlterFunctionStmt:
  OUT_NODE(AlterFunctionStmt, AlterFunctionStmt, alter_function_stmt, ALTER_FUNCTION_STMT, AlterFunctionStmt, alter_function_stmt);
  break;
case T_DoStmt:
  OUT_NODE(DoStmt, DoStmt, do_stmt, DO_STMT, DoStmt, do_stmt);
  break;
case T_RenameStmt:
  OUT_NODE(RenameStmt, RenameStmt, rename_stmt, RENAME_STMT, RenameStmt, rename_stmt);
  break;
case T_RuleStmt:
  OUT_NODE(RuleStmt, RuleStmt, rule_stmt, RULE_STMT, RuleStmt, rule_stmt);
  break;
case T_NotifyStmt:
  OUT_NODE(NotifyStmt, NotifyStmt, notify_stmt, NOTIFY_STMT, NotifyStmt, notify_stmt);
  break;
case T_ListenStmt:
  OUT_NODE(ListenStmt, ListenStmt, listen_stmt, LISTEN_STMT, ListenStmt, listen_stmt);
  break;
case T_UnlistenStmt:
  OUT_NODE(UnlistenStmt, UnlistenStmt, unlisten_stmt, UNLISTEN_STMT, UnlistenStmt, unlisten_stmt);
  break;
case T_TransactionStmt:
  OUT_NODE(TransactionStmt, TransactionStmt, transaction_stmt, TRANSACTION_STMT, TransactionStmt, transaction_stmt);
  break;
case T_ViewStmt:
  OUT_NODE(ViewStmt, ViewStmt, view_stmt, VIEW_STMT, ViewStmt, view_stmt);
  break;
case T_LoadStmt:
  OUT_NODE(LoadStmt, LoadStmt, load_stmt, LOAD_STMT, LoadStmt, load_stmt);
  break;
case T_CreateDomainStmt:
  OUT_NODE(CreateDomainStmt, CreateDomainStmt, create_domain_stmt, CREATE_DOMAIN_STMT, CreateDomainStmt, create_domain_stmt);
  break;
case T_CreatedbStmt:
  OUT_NODE(CreatedbStmt, CreatedbStmt, createdb_stmt, CREATEDB_STMT, CreatedbStmt, createdb_stmt);
  break;
case T_DropdbStmt:
  OUT_NODE(DropdbStmt, DropdbStmt, dropdb_stmt, DROPDB_STMT, DropdbStmt, dropdb_stmt);
  break;
case T_VacuumStmt:
  OUT_NODE(VacuumStmt, VacuumStmt, vacuum_stmt, VACUUM_STMT, VacuumStmt, vacuum_stmt);
  break;
case T_ExplainStmt:
  OUT_NODE(ExplainStmt, ExplainStmt, explain_stmt, EXPLAIN_STMT, ExplainStmt, explain_stmt);
  break;
case T_CreateTableAsStmt:
  OUT_NODE(CreateTableAsStmt, CreateTableAsStmt, create_table_as_stmt, CREATE_TABLE_AS_STMT, CreateTableAsStmt, create_table_as_stmt);
  break;
case T_CreateSeqStmt:
  OUT_NODE(CreateSeqStmt, CreateSeqStmt, create_seq_stmt, CREATE_SEQ_STMT, CreateSeqStmt, create_seq_stmt);
  break;
case T_AlterSeqStmt:
  OUT_NODE(AlterSeqStmt, AlterSeqStmt, alter_seq_stmt, ALTER_SEQ_STMT, AlterSeqStmt, alter_seq_stmt);
  break;
case T_VariableSetStmt:
  OUT_NODE(VariableSetStmt, VariableSetStmt, variable_set_stmt, VARIABLE_SET_STMT, VariableSetStmt, variable_set_stmt);
  break;
case T_VariableShowStmt:
  OUT_NODE(VariableShowStmt, VariableShowStmt, variable_show_stmt, VARIABLE_SHOW_STMT, VariableShowStmt, variable_show_stmt);
  break;
case T_DiscardStmt:
  OUT_NODE(DiscardStmt, DiscardStmt, discard_stmt, DISCARD_STMT, DiscardStmt, discard_stmt);
  break;
case T_CreateTrigStmt:
  OUT_NODE(CreateTrigStmt, CreateTrigStmt, create_trig_stmt, CREATE_TRIG_STMT, CreateTrigStmt, create_trig_stmt);
  break;
case T_CreatePLangStmt:
  OUT_NODE(CreatePLangStmt, CreatePLangStmt, create_plang_stmt, CREATE_PLANG_STMT, CreatePLangStmt, create_plang_stmt);
  break;
case T_CreateRoleStmt:
  OUT_NODE(CreateRoleStmt, CreateRoleStmt, create_role_stmt, CREATE_ROLE_STMT, CreateRoleStmt, create_role_stmt);
  break;
case T_AlterRoleStmt:
  OUT_NODE(AlterRoleStmt, AlterRoleStmt, alter_role_stmt, ALTER_ROLE_STMT, AlterRoleStmt, alter_role_stmt);
  break;
case T_DropRoleStmt:
  OUT_NODE(DropRoleStmt, DropRoleStmt, drop_role_stmt, DROP_ROLE_STMT, DropRoleStmt, drop_role_stmt);
  break;
case T_LockStmt:
  OUT_NODE(LockStmt, LockStmt, lock_stmt, LOCK_STMT, LockStmt, lock_stmt);
  break;
case T_ConstraintsSetStmt:
  OUT_NODE(ConstraintsSetStmt, ConstraintsSetStmt, constraints_set_stmt, CONSTRAINTS_SET_STMT, ConstraintsSetStmt, constraints_set_stmt);
  break;
case T_ReindexStmt:
  OUT_NODE(ReindexStmt, ReindexStmt, reindex_stmt, REINDEX_STMT, ReindexStmt, reindex_stmt);
  break;
case T_CheckPointStmt:
  OUT_NODE(CheckPointStmt, CheckPointStmt, check_point_stmt, CHECK_POINT_STMT, CheckPointStmt, check_point_stmt);
  break;
case T_CreateSchemaStmt:
  OUT_NODE(CreateSchemaStmt, CreateSchemaStmt, create_schema_stmt, CREATE_SCHEMA_STMT, CreateSchemaStmt, create_schema_stmt);
  break;
case T_AlterDatabaseStmt:
  OUT_NODE(AlterDatabaseStmt, AlterDatabaseStmt, alter_database_stmt, ALTER_DATABASE_STMT, AlterDatabaseStmt, alter_database_stmt);
  break;
case T_AlterDatabaseSetStmt:
  OUT_NODE(AlterDatabaseSetStmt, AlterDatabaseSetStmt, alter_database_set_stmt, ALTER_DATABASE_SET_STMT, AlterDatabaseSetStmt, alter_database_set_stmt);
  break;
case T_AlterRoleSetStmt:
  OUT_NODE(AlterRoleSetStmt, AlterRoleSetStmt, alter_role_set_stmt, ALTER_ROLE_SET_STMT, AlterRoleSetStmt, alter_role_set_stmt);
  break;
case T_CreateConversionStmt:
  OUT_NODE(CreateConversionStmt, CreateConversionStmt, create_conversion_stmt, CREATE_CONVERSION_STMT, CreateConversionStmt, create_conversion_stmt);
  break;
case T_CreateCastStmt:
  OUT_NODE(CreateCastStmt, CreateCastStmt, create_cast_stmt, CREATE_CAST_STMT, CreateCastStmt, create_cast_stmt);
  break;
case T_CreateOpClassStmt:
  OUT_NODE(CreateOpClassStmt, CreateOpClassStmt, create_op_class_stmt, CREATE_OP_CLASS_STMT, CreateOpClassStmt, create_op_class_stmt);
  break;
case T_CreateOpFamilyStmt:
  OUT_NODE(CreateOpFamilyStmt, CreateOpFamilyStmt, create_op_family_stmt, CREATE_OP_FAMILY_STMT, CreateOpFamilyStmt, create_op_family_stmt);
  break;
case T_AlterOpFamilyStmt:
  OUT_NODE(AlterOpFamilyStmt, AlterOpFamilyStmt, alter_op_family_stmt, ALTER_OP_FAMILY_STMT, AlterOpFamilyStmt, alter_op_family_stmt);
  break;
case T_PrepareStmt:
  OUT_NODE(PrepareStmt, PrepareStmt, prepare_stmt, PREPARE_STMT, PrepareStmt, prepare_stmt);
  break;
case T_ExecuteStmt:
  OUT_NODE(ExecuteStmt, ExecuteStmt, execute_stmt, EXECUTE_STMT, ExecuteStmt, execute_stmt);
  break;
case T_DeallocateStmt:
  OUT_NODE(DeallocateStmt, DeallocateStmt, deallocate_stmt, DEALLOCATE_STMT, DeallocateStmt, deallocate_stmt);
  break;
case T_DeclareCursorStmt:
  OUT_NODE(DeclareCursorStmt, DeclareCursorStmt, declare_cursor_stmt, DECLARE_CURSOR_STMT, DeclareCursorStmt, declare_cursor_stmt);
  break;
case T_CreateTableSpaceStmt:
  OUT_NODE(CreateTableSpaceStmt, CreateTableSpaceStmt, create_table_space_stmt, CREATE_TABLE_SPACE_STMT, CreateTableSpaceStmt, create_table_space_stmt);
  break;
case T_DropTableSpaceStmt:
  OUT_NODE(DropTableSpaceStmt, DropTableSpaceStmt, drop_table_space_stmt, DROP_TABLE_SPACE_STMT, DropTableSpaceStmt, drop_table_space_stmt);
  break;
case T_AlterObjectDependsStmt:
  OUT_NODE(AlterObjectDependsStmt, AlterObjectDependsStmt, alter_object_depends_stmt, ALTER_OBJECT_DEPENDS_STMT, AlterObjectDependsStmt, alter_object_depends_stmt);
  break;
case T_AlterObjectSchemaStmt:
  OUT_NODE(AlterObjectSchemaStmt, AlterObjectSchemaStmt, alter_object_schema_stmt, ALTER_OBJECT_SCHEMA_STMT, AlterObjectSchemaStmt, alter_object_schema_stmt);
  break;
case T_AlterOwnerStmt:
  OUT_NODE(AlterOwnerStmt, AlterOwnerStmt, alter_owner_stmt, ALTER_OWNER_STMT, AlterOwnerStmt, alter_owner_stmt);
  break;
case T_AlterOperatorStmt:
  OUT_NODE(AlterOperatorStmt, AlterOperatorStmt, alter_operator_stmt, ALTER_OPERATOR_STMT, AlterOperatorStmt, alter_operator_stmt);
  break;
case T_AlterTypeStmt:
  OUT_NODE(AlterTypeStmt, AlterTypeStmt, alter_type_stmt, ALTER_TYPE_STMT, AlterTypeStmt, alter_type_stmt);
  break;
case T_DropOwnedStmt:
  OUT_NODE(DropOwnedStmt, DropOwnedStmt, drop_owned_stmt, DROP_OWNED_STMT, DropOwnedStmt, drop_owned_stmt);
  break;
case T_ReassignOwnedStmt:
  OUT_NODE(ReassignOwnedStmt, ReassignOwnedStmt, reassign_owned_stmt, REASSIGN_OWNED_STMT, ReassignOwnedStmt, reassign_owned_stmt);
  break;
case T_CompositeTypeStmt:
  OUT_NODE(CompositeTypeStmt, CompositeTypeStmt, composite_type_stmt, COMPOSITE_TYPE_STMT, CompositeTypeStmt, composite_type_stmt);
  break;
case T_CreateEnumStmt:
  OUT_NODE(CreateEnumStmt, CreateEnumStmt, create_enum_stmt, CREATE_ENUM_STMT, CreateEnumStmt, create_enum_stmt);
  break;
case T_CreateRangeStmt:
  OUT_NODE(CreateRangeStmt, CreateRangeStmt, create_range_stmt, CREATE_RANGE_STMT, CreateRangeStmt, create_range_stmt);
  break;
case T_AlterEnumStmt:
  OUT_NODE(AlterEnumStmt, AlterEnumStmt, alter_enum_stmt, ALTER_ENUM_STMT, AlterEnumStmt, alter_enum_stmt);
  break;
case T_AlterTSDictionaryStmt:
  OUT_NODE(AlterTSDictionaryStmt, AlterTSDictionaryStmt, alter_tsdictionary_stmt, ALTER_TSDICTIONARY_STMT, AlterTSDictionaryStmt, alter_tsdictionary_stmt);
  break;
case T_AlterTSConfigurationStmt:
  OUT_NODE(AlterTSConfigurationStmt, AlterTSConfigurationStmt, alter_tsconfiguration_stmt, ALTER_TSCONFIGURATION_STMT, AlterTSConfigurationStmt, alter_tsconfiguration_stmt);
  break;
case T_CreateFdwStmt:
  OUT_NODE(CreateFdwStmt, CreateFdwStmt, create_fdw_stmt, CREATE_FDW_STMT, CreateFdwStmt, create_fdw_stmt);
  break;
case T_AlterFdwStmt:
  OUT_NODE(AlterFdwStmt, AlterFdwStmt, alter_fdw_stmt, ALTER_FDW_STMT, AlterFdwStmt, alter_fdw_stmt);
  break;
case T_CreateForeignServerStmt:
  OUT_NODE(CreateForeignServerStmt, CreateForeignServerStmt, create_foreign_server_stmt, CREATE_FOREIGN_SERVER_STMT, CreateForeignServerStmt, create_foreign_server_stmt);
  break;
case T_AlterForeignServerStmt:
  OUT_NODE(AlterForeignServerStmt, AlterForeignServerStmt, alter_foreign_server_stmt, ALTER_FOREIGN_SERVER_STMT, AlterForeignServerStmt, alter_foreign_server_stmt);
  break;
case T_CreateUserMappingStmt:
  OUT_NODE(CreateUserMappingStmt, CreateUserMappingStmt, create_user_mapping_stmt, CREATE_USER_MAPPING_STMT, CreateUserMappingStmt, create_user_mapping_stmt);
  break;
case T_AlterUserMappingStmt:
  OUT_NODE(AlterUserMappingStmt, AlterUserMappingStmt, alter_user_mapping_stmt, ALTER_USER_MAPPING_STMT, AlterUserMappingStmt, alter_user_mapping_stmt);
  break;
case T_DropUserMappingStmt:
  OUT_NODE(DropUserMappingStmt, DropUserMappingStmt, drop_user_mapping_stmt, DROP_USER_MAPPING_STMT, DropUserMappingStmt, drop_user_mapping_stmt);
  break;
case T_AlterTableSpaceOptionsStmt:
  OUT_NODE(AlterTableSpaceOptionsStmt, AlterTableSpaceOptionsStmt, alter_table_space_options_stmt, ALTER_TABLE_SPACE_OPTIONS_STMT, AlterTableSpaceOptionsStmt, alter_table_space_options_stmt);
  break;
case T_AlterTableMoveAllStmt:
  OUT_NODE(AlterTableMoveAllStmt, AlterTableMoveAllStmt, alter_table_move_all_stmt, ALTER_TABLE_MOVE_ALL_STMT, AlterTableMoveAllStmt, alter_table_move_all_stmt);
  break;
case T_SecLabelStmt:
  OUT_NODE(SecLabelStmt, SecLabelStmt, sec_label_stmt, SEC_LABEL_STMT, SecLabelStmt, sec_label_stmt);
  break;
case T_CreateForeignTableStmt:
  OUT_NODE(CreateForeignTableStmt, CreateForeignTableStmt, create_foreign_table_stmt, CREATE_FOREIGN_TABLE_STMT, CreateForeignTableStmt, create_foreign_table_stmt);
  break;
case T_ImportForeignSchemaStmt:
  OUT_NODE(ImportForeignSchemaStmt, ImportForeignSchemaStmt, import_foreign_schema_stmt, IMPORT_FOREIGN_SCHEMA_STMT, ImportForeignSchemaStmt, import_foreign_schema_stmt);
  break;
case T_CreateExtensionStmt:
  OUT_NODE(CreateExtensionStmt, CreateExtensionStmt, create_extension_stmt, CREATE_EXTENSION_STMT, CreateExtensionStmt, create_extension_stmt);
  break;
case T_AlterExtensionStmt:
  OUT_NODE(AlterExtensionStmt, AlterExtensionStmt, alter_extension_stmt, ALTER_EXTENSION_STMT, AlterExtensionStmt, alter_extension_stmt);
  break;
case T_AlterExtensionContentsStmt:
  OUT_NODE(AlterExtensionContentsStmt, AlterExtensionContentsStmt, alter_extension_contents_stmt, ALTER_EXTENSION_CONTENTS_STMT, AlterExtensionContentsStmt, alter_extension_contents_stmt);
  break;
case T_CreateEventTrigStmt:
  OUT_NODE(CreateEventTrigStmt, CreateEventTrigStmt, create_event_trig_stmt, CREATE_EVENT_TRIG_STMT, CreateEventTrigStmt, create_event_trig_stmt);
  break;
case T_AlterEventTrigStmt:
  OUT_NODE(AlterEventTrigStmt, AlterEventTrigStmt, alter_event_trig_stmt, ALTER_EVENT_TRIG_STMT, AlterEventTrigStmt, alter_event_trig_stmt);
  break;
case T_RefreshMatViewStmt:
  OUT_NODE(RefreshMatViewStmt, RefreshMatViewStmt, refresh_mat_view_stmt, REFRESH_MAT_VIEW_STMT, RefreshMatViewStmt, refresh_mat_view_stmt);
  break;
case T_ReplicaIdentityStmt:
  OUT_NODE(ReplicaIdentityStmt, ReplicaIdentityStmt, replica_identity_stmt, REPLICA_IDENTITY_STMT, ReplicaIdentityStmt, replica_identity_stmt);
  break;
case T_AlterSystemStmt:
  OUT_NODE(AlterSystemStmt, AlterSystemStmt, alter_system_stmt, ALTER_SYSTEM_STMT, AlterSystemStmt, alter_system_stmt);
  break;
case T_CreatePolicyStmt:
  OUT_NODE(CreatePolicyStmt, CreatePolicyStmt, create_policy_stmt, CREATE_POLICY_STMT, CreatePolicyStmt, create_policy_stmt);
  break;
case T_AlterPolicyStmt:
  OUT_NODE(AlterPolicyStmt, AlterPolicyStmt, alter_policy_stmt, ALTER_POLICY_STMT, AlterPolicyStmt, alter_policy_stmt);
  break;
case T_CreateTransformStmt:
  OUT_NODE(CreateTransformStmt, CreateTransformStmt, create_transform_stmt, CREATE_TRANSFORM_STMT, CreateTransformStmt, create_transform_stmt);
  break;
case T_CreateAmStmt:
  OUT_NODE(CreateAmStmt, CreateAmStmt, create_am_stmt, CREATE_AM_STMT, CreateAmStmt, create_am_stmt);
  break;
case T_CreatePublicationStmt:
  OUT_NODE(CreatePublicationStmt, CreatePublicationStmt, create_publication_stmt, CREATE_PUBLICATION_STMT, CreatePublicationStmt, create_publication_stmt);
  break;
case T_AlterPublicationStmt:
  OUT_NODE(AlterPublicationStmt, AlterPublicationStmt, alter_publication_stmt, ALTER_PUBLICATION_STMT, AlterPublicationStmt, alter_publication_stmt);
  break;
case T_CreateSubscriptionStmt:
  OUT_NODE(CreateSubscriptionStmt, CreateSubscriptionStmt, create_subscription_stmt, CREATE_SUBSCRIPTION_STMT, CreateSubscriptionStmt, create_subscription_stmt);
  break;
case T_AlterSubscriptionStmt:
  OUT_NODE(AlterSubscriptionStmt, AlterSubscriptionStmt, alter_subscription_stmt, ALTER_SUBSCRIPTION_STMT, AlterSubscriptionStmt, alter_subscription_stmt);
  break;
case T_DropSubscriptionStmt:
  OUT_NODE(DropSubscriptionStmt, DropSubscriptionStmt, drop_subscription_stmt, DROP_SUBSCRIPTION_STMT, DropSubscriptionStmt, drop_subscription_stmt);
  break;
case T_CreateStatsStmt:
  OUT_NODE(CreateStatsStmt, CreateStatsStmt, create_stats_stmt, CREATE_STATS_STMT, CreateStatsStmt, create_stats_stmt);
  break;
case T_AlterCollationStmt:
  OUT_NODE(AlterCollationStmt, AlterCollationStmt, alter_collation_stmt, ALTER_COLLATION_STMT, AlterCollationStmt, alter_collation_stmt);
  break;
case T_CallStmt:
  OUT_NODE(CallStmt, CallStmt, call_stmt, CALL_STMT, CallStmt, call_stmt);
  break;
case T_AlterStatsStmt:
  OUT_NODE(AlterStatsStmt, AlterStatsStmt, alter_stats_stmt, ALTER_STATS_STMT, AlterStatsStmt, alter_stats_stmt);
  break;
case T_A_Expr:
  OUT_NODE(A_Expr, AExpr, a__expr, A_EXPR, A_Expr, a_expr);
  break;
case T_ColumnRef:
  OUT_NODE(ColumnRef, ColumnRef, column_ref, COLUMN_REF, ColumnRef, column_ref);
  break;
case T_ParamRef:
  OUT_NODE(ParamRef, ParamRef, param_ref, PARAM_REF, ParamRef, param_ref);
  break;
case T_A_Const:
  OUT_NODE(A_Const, AConst, a__const, A_CONST, A_Const, a_const);
  break;
case T_FuncCall:
  OUT_NODE(FuncCall, FuncCall, func_call, FUNC_CALL, FuncCall, func_call);
  break;
case T_A_Star:
  OUT_NODE(A_Star, AStar, a__star, A_STAR, A_Star, a_star);
  break;
case T_A_Indices:
  OUT_NODE(A_Indices, AIndices, a__indices, A_INDICES, A_Indices, a_indices);
  break;
case T_A_Indirection:
  OUT_NODE(A_Indirection, AIndirection, a__indirection, A_INDIRECTION, A_Indirection, a_indirection);
  break;
case T_A_ArrayExpr:
  OUT_NODE(A_ArrayExpr, AArrayExpr, a__array_expr, A_ARRAY_EXPR, A_ArrayExpr, a_array_expr);
  break;
case T_ResTarget:
  OUT_NODE(ResTarget, ResTarget, res_target, RES_TARGET, ResTarget, res_target);
  break;
case T_MultiAssignRef:
  OUT_NODE(MultiAssignRef, MultiAssignRef, multi_assign_ref, MULTI_ASSIGN_REF, MultiAssignRef, multi_assign_ref);
  break;
case T_TypeCast:
  OUT_NODE(TypeCast, TypeCast, type_cast, TYPE_CAST, TypeCast, type_cast);
  break;
case T_CollateClause:
  OUT_NODE(CollateClause, CollateClause, collate_clause, COLLATE_CLAUSE, CollateClause, collate_clause);
  break;
case T_SortBy:
  OUT_NODE(SortBy, SortBy, sort_by, SORT_BY, SortBy, sort_by);
  break;
case T_WindowDef:
  OUT_NODE(WindowDef, WindowDef, window_def, WINDOW_DEF, WindowDef, window_def);
  break;
case T_RangeSubselect:
  OUT_NODE(RangeSubselect, RangeSubselect, range_subselect, RANGE_SUBSELECT, RangeSubselect, range_subselect);
  break;
case T_RangeFunction:
  OUT_NODE(RangeFunction, RangeFunction, range_function, RANGE_FUNCTION, RangeFunction, range_function);
  break;
case T_RangeTableSample:
  OUT_NODE(RangeTableSample, RangeTableSample, range_table_sample, RANGE_TABLE_SAMPLE, RangeTableSample, range_table_sample);
  break;
case T_RangeTableFunc:
  OUT_NODE(RangeTableFunc, RangeTableFunc, range_table_func, RANGE_TABLE_FUNC, RangeTableFunc, range_table_func);
  break;
case T_RangeTableFuncCol:
  OUT_NODE(RangeTableFuncCol, RangeTableFuncCol, range_table_func_col, RANGE_TABLE_FUNC_COL, RangeTableFuncCol, range_table_func_col);
  break;
case T_TypeName:
  OUT_NODE(TypeName, TypeName, type_name, TYPE_NAME, TypeName, type_name);
  break;
case T_ColumnDef:
  OUT_NODE(ColumnDef, ColumnDef, column_def, COLUMN_DEF, ColumnDef, column_def);
  break;
case T_IndexElem:
  OUT_NODE(IndexElem, IndexElem, index_elem, INDEX_ELEM, IndexElem, index_elem);
  break;
case T_Constraint:
  OUT_NODE(Constraint, Constraint, constraint, CONSTRAINT, Constraint, constraint);
  break;
case T_DefElem:
  OUT_NODE(DefElem, DefElem, def_elem, DEF_ELEM, DefElem, def_elem);
  break;
case T_RangeTblEntry:
  OUT_NODE(RangeTblEntry, RangeTblEntry, range_tbl_entry, RANGE_TBL_ENTRY, RangeTblEntry, range_tbl_entry);
  break;
case T_RangeTblFunction:
  OUT_NODE(RangeTblFunction, RangeTblFunction, range_tbl_function, RANGE_TBL_FUNCTION, RangeTblFunction, range_tbl_function);
  break;
case T_TableSampleClause:
  OUT_NODE(TableSampleClause, TableSampleClause, table_sample_clause, TABLE_SAMPLE_CLAUSE, TableSampleClause, table_sample_clause);
  break;
case T_WithCheckOption:
  OUT_NODE(WithCheckOption, WithCheckOption, with_check_option, WITH_CHECK_OPTION, WithCheckOption, with_check_option);
  break;
case T_SortGroupClause:
  OUT_NODE(SortGroupClause, SortGroupClause, sort_group_clause, SORT_GROUP_CLAUSE, SortGroupClause, sort_group_clause);
  break;
case T_GroupingSet:
  OUT_NODE(GroupingSet, GroupingSet, grouping_set, GROUPING_SET, GroupingSet, grouping_set);
  break;
case T_WindowClause:
  OUT_NODE(WindowClause, WindowClause, window_clause, WINDOW_CLAUSE, WindowClause, window_clause);
  break;
case T_ObjectWithArgs:
  OUT_NODE(ObjectWithArgs, ObjectWithArgs, object_with_args, OBJECT_WITH_ARGS, ObjectWithArgs, object_with_args);
  break;
case T_AccessPriv:
  OUT_NODE(AccessPriv, AccessPriv, access_priv, ACCESS_PRIV, AccessPriv, access_priv);
  break;
case T_CreateOpClassItem:
  OUT_NODE(CreateOpClassItem, CreateOpClassItem, create_op_class_item, CREATE_OP_CLASS_ITEM, CreateOpClassItem, create_op_class_item);
  break;
case T_TableLikeClause:
  OUT_NODE(TableLikeClause, TableLikeClause, table_like_clause, TABLE_LIKE_CLAUSE, TableLikeClause, table_like_clause);
  break;
case T_FunctionParameter:
  OUT_NODE(FunctionParameter, FunctionParameter, function_parameter, FUNCTION_PARAMETER, FunctionParameter, function_parameter);
  break;
case T_LockingClause:
  OUT_NODE(LockingClause, LockingClause, locking_clause, LOCKING_CLAUSE, LockingClause, locking_clause);
  break;
case T_RowMarkClause:
  OUT_NODE(RowMarkClause, RowMarkClause, row_mark_clause, ROW_MARK_CLAUSE, RowMarkClause, row_mark_clause);
  break;
case T_XmlSerialize:
  OUT_NODE(XmlSerialize, XmlSerialize, xml_serialize, XML_SERIALIZE, XmlSerialize, xml_serialize);
  break;
case T_WithClause:
  OUT_NODE(WithClause, WithClause, with_clause, WITH_CLAUSE, WithClause, with_clause);
  break;
case T_InferClause:
  OUT_NODE(InferClause, InferClause, infer_clause, INFER_CLAUSE, InferClause, infer_clause);
  break;
case T_OnConflictClause:
  OUT_NODE(OnConflictClause, OnConflictClause, on_conflict_clause, ON_CONFLICT_CLAUSE, OnConflictClause, on_conflict_clause);
  break;
case T_CommonTableExpr:
  OUT_NODE(CommonTableExpr, CommonTableExpr, common_table_expr, COMMON_TABLE_EXPR, CommonTableExpr, common_table_expr);
  break;
case T_RoleSpec:
  OUT_NODE(RoleSpec, RoleSpec, role_spec, ROLE_SPEC, RoleSpec, role_spec);
  break;
case T_TriggerTransition:
  OUT_NODE(TriggerTransition, TriggerTransition, trigger_transition, TRIGGER_TRANSITION, TriggerTransition, trigger_transition);
  break;
case T_PartitionElem:
  OUT_NODE(PartitionElem, PartitionElem, partition_elem, PARTITION_ELEM, PartitionElem, partition_elem);
  break;
case T_PartitionSpec:
  OUT_NODE(PartitionSpec, PartitionSpec, partition_spec, PARTITION_SPEC, PartitionSpec, partition_spec);
  break;
case T_PartitionBoundSpec:
  OUT_NODE(PartitionBoundSpec, PartitionBoundSpec, partition_bound_spec, PARTITION_BOUND_SPEC, PartitionBoundSpec, partition_bound_spec);
  break;
case T_PartitionRangeDatum:
  OUT_NODE(PartitionRangeDatum, PartitionRangeDatum, partition_range_datum, PARTITION_RANGE_DATUM, PartitionRangeDatum, partition_range_datum);
  break;
case T_PartitionCmd:
  OUT_NODE(PartitionCmd, PartitionCmd, partition_cmd, PARTITION_CMD, PartitionCmd, partition_cmd);
  break;
case T_VacuumRelation:
  OUT_NODE(VacuumRelation, VacuumRelation, vacuum_relation, VACUUM_RELATION, VacuumRelation, vacuum_relation);
  break;
case T_InlineCodeBlock:
  OUT_NODE(InlineCodeBlock, InlineCodeBlock, inline_code_block, INLINE_CODE_BLOCK, InlineCodeBlock, inline_code_block);
  break;
case T_CallContext:
  OUT_NODE(CallContext, CallContext, call_context, CALL_CONTEXT, CallContext, call_context);
  break;

#ifndef PG_QUERY_PROTOBUF_H
#define PG_QUERY_PROTOBUF_H

#include "pg_query.h"

#ifdef __cplusplus
extern "C" {
#endif

PgQueryProtobuf pg_query_nodes_to_protobuf(const void *obj);
char *pg_query_nodes_to_protobuf_json(const void *obj);

#ifdef __cplusplus
}
#endif

#endif

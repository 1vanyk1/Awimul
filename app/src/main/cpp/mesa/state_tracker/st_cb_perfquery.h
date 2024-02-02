#ifndef ST_CB_PERFQUERY_H
#define ST_CB_PERFQUERY_H

bool
st_have_perfquery(struct st_context *st);

unsigned st_InitPerfQueryInfo(struct gl_context *ctx);

void st_GetPerfQueryInfo(struct gl_context *ctx,
                         unsigned query_index,
                         const char **name,
                         GLuint *data_size,
                         GLuint *n_counters,
                         GLuint *n_active);

void st_GetPerfCounterInfo(struct gl_context *ctx,
                           unsigned query_index,
                           unsigned counter_index,
                           const char **name,
                           const char **desc,
                           GLuint *offset,
                           GLuint *data_size,
                           GLuint *type_enum,
                           GLuint *data_type_enum,
                           GLuint64 *raw_max);

void st_DeletePerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o);
bool st_BeginPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o);
void st_EndPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o);
void st_WaitPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o);
bool st_IsPerfQueryReady(struct gl_context *ctx, struct gl_perf_query_object *o);
bool st_GetPerfQueryData(struct gl_context *ctx,
                         struct gl_perf_query_object *o,
                         GLsizei data_size,
                         GLuint *data,
                         GLuint *bytes_written);

struct gl_perf_query_object *
st_NewPerfQueryObject(struct gl_context *ctx, unsigned query_index);
#endif

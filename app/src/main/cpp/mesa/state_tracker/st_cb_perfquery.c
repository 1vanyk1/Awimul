#include "st_debug.h"
#include "st_context.h"
#include "st_cb_bitmap.h"
#include "st_cb_perfquery.h"
#include "st_util.h"

#include "../util/bitset.h"

#include "../include/pipe/p_context.h"
#include "../include/pipe/p_screen.h"
#include "../util/u_memory.h"

bool
st_have_perfquery(struct st_context *st)
{
    struct pipe_context *pipe = st->pipe;

    return pipe->init_intel_perf_query_info && pipe->get_intel_perf_query_info &&
           pipe->get_intel_perf_query_counter_info &&
           pipe->new_intel_perf_query_obj && pipe->begin_intel_perf_query &&
           pipe->end_intel_perf_query && pipe->delete_intel_perf_query &&
           pipe->wait_intel_perf_query && pipe->is_intel_perf_query_ready &&
           pipe->get_intel_perf_query_data;
}

unsigned
st_InitPerfQueryInfo(struct gl_context *ctx)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    return pipe->init_intel_perf_query_info(pipe);
}

void
st_GetPerfQueryInfo(struct gl_context *ctx,
                    unsigned query_index,
                    const char **name,
                    GLuint *data_size,
                    GLuint *n_counters,
                    GLuint *n_active)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    pipe->get_intel_perf_query_info(pipe, query_index, name, data_size,
                                    n_counters, n_active);
}

static uint32_t
pipe_counter_type_enum_to_gl_type(enum pipe_perf_counter_type type)
{
    switch (type) {
        case PIPE_PERF_COUNTER_TYPE_EVENT: return GL_PERFQUERY_COUNTER_EVENT_INTEL;
        case PIPE_PERF_COUNTER_TYPE_DURATION_NORM: return GL_PERFQUERY_COUNTER_DURATION_NORM_INTEL;
        case PIPE_PERF_COUNTER_TYPE_DURATION_RAW: return GL_PERFQUERY_COUNTER_DURATION_RAW_INTEL;
        case PIPE_PERF_COUNTER_TYPE_THROUGHPUT: return GL_PERFQUERY_COUNTER_THROUGHPUT_INTEL;
        case PIPE_PERF_COUNTER_TYPE_RAW: return GL_PERFQUERY_COUNTER_RAW_INTEL;
        case PIPE_PERF_COUNTER_TYPE_TIMESTAMP: return GL_PERFQUERY_COUNTER_TIMESTAMP_INTEL;
        default:
            unreachable("Unknown counter type");
    }
}

static uint32_t
pipe_counter_data_type_to_gl_type(enum pipe_perf_counter_data_type type)
{
    switch (type) {
        case PIPE_PERF_COUNTER_DATA_TYPE_BOOL32: return GL_PERFQUERY_COUNTER_DATA_BOOL32_INTEL;
        case PIPE_PERF_COUNTER_DATA_TYPE_UINT32: return GL_PERFQUERY_COUNTER_DATA_UINT32_INTEL;
        case PIPE_PERF_COUNTER_DATA_TYPE_UINT64: return GL_PERFQUERY_COUNTER_DATA_UINT64_INTEL;
        case PIPE_PERF_COUNTER_DATA_TYPE_FLOAT: return GL_PERFQUERY_COUNTER_DATA_FLOAT_INTEL;
        case PIPE_PERF_COUNTER_DATA_TYPE_DOUBLE: return GL_PERFQUERY_COUNTER_DATA_DOUBLE_INTEL;
        default:
            unreachable("Unknown counter data type");
    }
}

void
st_GetPerfCounterInfo(struct gl_context *ctx,
                      unsigned query_index,
                      unsigned counter_index,
                      const char **name,
                      const char **desc,
                      GLuint *offset,
                      GLuint *data_size,
                      GLuint *type_enum,
                      GLuint *data_type_enum,
                      GLuint64 *raw_max)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;
    uint32_t pipe_type_enum;
    uint32_t pipe_data_type_enum;

    pipe->get_intel_perf_query_counter_info(pipe, query_index, counter_index,
                                            name, desc, offset, data_size,
                                            &pipe_type_enum, &pipe_data_type_enum, raw_max);
    *type_enum = pipe_counter_type_enum_to_gl_type(pipe_type_enum);
    *data_type_enum = pipe_counter_data_type_to_gl_type(pipe_data_type_enum);
}

void
st_DeletePerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    /* We can assume that the frontend waits for a query to complete
     * before ever calling into here, so we don't have to worry about
     * deleting an in-flight query object.
     */
    assert(!o->Active);
    assert(!o->Used || o->Ready);

    pipe->delete_intel_perf_query(pipe, (struct pipe_query *)o);
}

bool
st_BeginPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    /* We can assume the frontend hides mistaken attempts to Begin a
     * query object multiple times before its End. Similarly if an
     * application reuses a query object before results have arrived
     * the frontend will wait for prior results so we don't need
     * to support abandoning in-flight results.
     */
    assert(!o->Active);
    assert(!o->Used || o->Ready); /* no in-flight query to worry about */

    return pipe->begin_intel_perf_query(pipe, (struct pipe_query *)o);
}

void
st_EndPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    pipe->end_intel_perf_query(pipe, (struct pipe_query *)o);
}

void
st_WaitPerfQuery(struct gl_context *ctx, struct gl_perf_query_object *o)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    assert(!o->Ready);

    pipe->wait_intel_perf_query(pipe, (struct pipe_query *)o);
}

bool
st_IsPerfQueryReady(struct gl_context *ctx, struct gl_perf_query_object *o)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    if (o->Ready)
        return true;

    return pipe->is_intel_perf_query_ready(pipe, (struct pipe_query *)o);
}

bool
st_GetPerfQueryData(struct gl_context *ctx,
                    struct gl_perf_query_object *o,
                    GLsizei data_size,
                    GLuint *data,
                    GLuint *bytes_written)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;

    assert(st_IsPerfQueryReady(ctx, o));

    /* We expect that the frontend only calls this hook when it knows
     * that results are available.
     */
    assert(o->Ready);

    return pipe->get_intel_perf_query_data(pipe, (struct pipe_query *)o,
                                           data_size, data, bytes_written);
}

struct gl_perf_query_object *
st_NewPerfQueryObject(struct gl_context *ctx, unsigned query_index)
{
    struct pipe_context *pipe = st_context(ctx)->pipe;
    struct pipe_query *q;

    q = pipe->new_intel_perf_query_obj(pipe, query_index);

    return (struct gl_perf_query_object *)q;
}

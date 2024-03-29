#include "../main/context.h"
#include "../main/debug_output.h"
#include "../program/prog_print.h"

#include "../include/pipe/p_state.h"
#include "../include/pipe/p_shader_tokens.h"

#include "../gallium/cso_cache/cso_cache.h"

#include "st_context.h"
#include "st_debug.h"
#include "st_program.h"



int ST_DEBUG = 0;

static const struct debug_named_value st_debug_flags[] = {
        { "mesa",     DEBUG_MESA, NULL },
        { "tgsi",     DEBUG_PRINT_IR, NULL },
        { "nir",      DEBUG_PRINT_IR, NULL },
        { "fallback", DEBUG_FALLBACK, NULL },
        { "buffer",   DEBUG_BUFFER, NULL },
        { "wf",       DEBUG_WIREFRAME, NULL },
        { "gremedy",  DEBUG_GREMEDY, "Enable GREMEDY debug extensions" },
        { "noreadpixcache", DEBUG_NOREADPIXCACHE, NULL },
        DEBUG_NAMED_VALUE_END
};

DEBUG_GET_ONCE_FLAGS_OPTION(st_debug, "ST_DEBUG", st_debug_flags, 0)


void
st_debug_init(void)
{
    ST_DEBUG = debug_get_option_st_debug();
}


/**
 * Installed as pipe_debug_callback when GL_DEBUG_OUTPUT is enabled.
 */
static void
st_debug_message(void *data,
                 unsigned *id,
                 enum pipe_debug_type ptype,
                 const char *fmt,
                 va_list args)
{
    struct st_context *st = data;
    enum mesa_debug_source source;
    enum mesa_debug_type type;
    enum mesa_debug_severity severity;

    switch (ptype) {
        case PIPE_DEBUG_TYPE_OUT_OF_MEMORY:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_ERROR;
            severity = MESA_DEBUG_SEVERITY_MEDIUM;
            break;
        case PIPE_DEBUG_TYPE_ERROR:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_ERROR;
            severity = MESA_DEBUG_SEVERITY_MEDIUM;
            break;
        case PIPE_DEBUG_TYPE_SHADER_INFO:
            source = MESA_DEBUG_SOURCE_SHADER_COMPILER;
            type = MESA_DEBUG_TYPE_OTHER;
            severity = MESA_DEBUG_SEVERITY_NOTIFICATION;
            break;
        case PIPE_DEBUG_TYPE_PERF_INFO:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_PERFORMANCE;
            severity = MESA_DEBUG_SEVERITY_NOTIFICATION;
            break;
        case PIPE_DEBUG_TYPE_INFO:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_OTHER;
            severity = MESA_DEBUG_SEVERITY_NOTIFICATION;
            break;
        case PIPE_DEBUG_TYPE_FALLBACK:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_PERFORMANCE;
            severity = MESA_DEBUG_SEVERITY_NOTIFICATION;
            break;
        case PIPE_DEBUG_TYPE_CONFORMANCE:
            source = MESA_DEBUG_SOURCE_API;
            type = MESA_DEBUG_TYPE_OTHER;
            severity = MESA_DEBUG_SEVERITY_NOTIFICATION;
            break;
        default:
            unreachable("invalid debug type");
    }
    _mesa_gl_vdebugf(st->ctx, id, source, type, severity, fmt, args);
}

void
st_update_debug_callback(struct st_context *st)
{
    struct pipe_context *pipe = st->pipe;

    if (!pipe->set_debug_callback)
        return;

    if (_mesa_get_debug_state_int(st->ctx, GL_DEBUG_OUTPUT)) {
        struct pipe_debug_callback cb;
        memset(&cb, 0, sizeof(cb));
        cb.async = !_mesa_get_debug_state_int(st->ctx, GL_DEBUG_OUTPUT_SYNCHRONOUS);
        cb.debug_message = st_debug_message;
        cb.data = st;
        pipe->set_debug_callback(pipe, &cb);
    } else {
        pipe->set_debug_callback(pipe, NULL);
    }
}

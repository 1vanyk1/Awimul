#include <stdarg.h>
#include <stdio.h>
#include "errors.h"
#include "dri_private.h"

static inline struct pipe_screen *
debug_screen_wrap(struct pipe_screen *screen)
{
//    screen = ddebug_screen_create(screen);
//    screen = rbug_screen_create(screen);
//    screen = trace_screen_create(screen);
//    screen = noop_screen_create(screen);
//
//    if (debug_get_bool_option("GALLIUM_TESTS", FALSE))
//        util_run_tests(screen);

    return screen;
}

void
_mesa_debug( const struct gl_context *ctx, const char *fmtString, ... )
{
//#ifndef NDEBUG
//    char s[MAX_DEBUG_MESSAGE_LENGTH];
//    va_list args;
//    va_start(args, fmtString);
//    vsnprintf(s, MAX_DEBUG_MESSAGE_LENGTH, fmtString, args);
//    va_end(args);
//    output_if_debug("Mesa", s, GL_FALSE);
//#endif /* DEBUG */
//    (void) ctx;
//    (void) fmtString;
}


void _mesa_problem( const struct gl_context *ctx, const char *fmtString, ... )
{
    va_list args;
    char str[MAX_DEBUG_MESSAGE_LENGTH];
    static int numCalls = 0;

    (void) ctx;

    if (numCalls < 50) {
        numCalls++;

        va_start( args, fmtString );
        vsnprintf( str, MAX_DEBUG_MESSAGE_LENGTH, fmtString, args );
        va_end( args );
        fprintf(stderr, "Mesa " PACKAGE_VERSION " implementation error: %s\n",
                str);
        fprintf(stderr, "Please report at " PACKAGE_BUGREPORT "\n");
    }
}

void
_mesa_error( struct gl_context *ctx, GLenum error, const char *fmtString, ... )
{
//    GLboolean do_output, do_log;
//    /* Ideally this would be set up by the caller, so that we had proper IDs
//     * per different message.
//     */
//    static GLuint error_msg_id = 0;
//
//    _mesa_debug_get_id(&error_msg_id);
//
//    do_output = should_output(ctx, error, fmtString);
//
//    simple_mtx_lock(&ctx->DebugMutex);
//    if (ctx->Debug) {
//        do_log = _mesa_debug_is_message_enabled(ctx->Debug,
//                                                MESA_DEBUG_SOURCE_API,
//                                                MESA_DEBUG_TYPE_ERROR,
//                                                error_msg_id,
//                                                MESA_DEBUG_SEVERITY_HIGH);
//    }
//    else {
//        do_log = GL_FALSE;
//    }
//    simple_mtx_unlock(&ctx->DebugMutex);
//
//    if (do_output || do_log) {
//        char s[MAX_DEBUG_MESSAGE_LENGTH], s2[MAX_DEBUG_MESSAGE_LENGTH];
//        int len;
//        va_list args;
//
//        va_start(args, fmtString);
//        len = vsnprintf(s, MAX_DEBUG_MESSAGE_LENGTH, fmtString, args);
//        va_end(args);
//
//        if (len >= MAX_DEBUG_MESSAGE_LENGTH) {
//            /* Too long error message. Whoever calls _mesa_error should use
//             * shorter strings.
//             */
//            assert(0);
//            return;
//        }
//
//        len = snprintf(s2, MAX_DEBUG_MESSAGE_LENGTH, "%s in %s",
//                       _mesa_enum_to_string(error), s);
//        if (len >= MAX_DEBUG_MESSAGE_LENGTH) {
//            /* Same as above. */
//            assert(0);
//            return;
//        }
//
//        /* Print the error to stderr if needed. */
//        if (do_output) {
//            output_if_debug("Mesa: User error", s2, GL_TRUE);
//        }
//
//        /* Log the error via ARB_debug_output if needed.*/
//        if (do_log) {
//            _mesa_log_msg(ctx, MESA_DEBUG_SOURCE_API, MESA_DEBUG_TYPE_ERROR,
//                          error_msg_id, MESA_DEBUG_SEVERITY_HIGH, len, s2);
//        }
//    }

    /* Set the GL context error state for glGetError. */
    if (ctx->ErrorValue == GL_NO_ERROR)
        ctx->ErrorValue = error;
}

void
_mesa_warning( struct gl_context *ctx, const char *fmtString, ... )
{
//    char str[MAX_DEBUG_MESSAGE_LENGTH];
//    va_list args;
//    va_start( args, fmtString );
//    (void) vsnprintf( str, MAX_DEBUG_MESSAGE_LENGTH, fmtString, args );
//    va_end( args );
//
//    if (ctx)
//        flush_delayed_errors( ctx );
//
//    output_if_debug("Mesa warning", str, GL_TRUE);
}
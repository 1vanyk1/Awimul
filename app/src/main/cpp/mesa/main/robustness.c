#include <stdbool.h>
#include "context.h"
#include "debug_output.h"
#include "get.h"
#include "mtypes.h"
#include "macros.h"
#include "dispatch.h" /* for _gloffset_COUNT */

static void GLAPIENTRY
_context_lost_GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize,
                        GLsizei *length, GLint *values)
{
    GET_CURRENT_CONTEXT(ctx);
    if (ctx)
        _mesa_error(ctx, GL_CONTEXT_LOST, "GetSynciv(invalid call)");

    if (pname == GL_SYNC_STATUS && bufSize >= 1)
        *values = GL_SIGNALED;
}

static void GLAPIENTRY
_context_lost_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
    GET_CURRENT_CONTEXT(ctx);
    if (ctx)
        _mesa_error(ctx, GL_CONTEXT_LOST, "GetQueryObjectuiv(context lost)");

    if (pname == GL_QUERY_RESULT_AVAILABLE)
        *params = GL_TRUE;
}

static int
context_lost_nop_handler(void)
{
    GET_CURRENT_CONTEXT(ctx);
    if (ctx)
        _mesa_error(ctx, GL_CONTEXT_LOST, "context lost");

    return 0;
}

void
_mesa_set_context_lost_dispatch(struct gl_context *ctx)
{
    if (ctx->ContextLost == NULL) {
        int numEntries = MAX2(_glapi_get_dispatch_table_size(), _gloffset_COUNT);

        ctx->ContextLost = malloc(numEntries * sizeof(_glapi_proc));
        if (!ctx->ContextLost)
            return;

        _glapi_proc *entry = (_glapi_proc *) ctx->ContextLost;
        unsigned i;
        for (i = 0; i < numEntries; i++)
            entry[i] = (_glapi_proc) context_lost_nop_handler;

        /* The ARB_robustness specification says:
         *
         *    "* GetError and GetGraphicsResetStatus behave normally following a
         *       graphics reset, so that the application can determine a reset
         *       has occurred, and when it is safe to destroy and recreate the
         *       context.
         *
         *     * Any commands which might cause a polling application to block
         *       indefinitely will generate a CONTEXT_LOST error, but will also
         *       return a value indicating completion to the application. Such
         *       commands include:
         *
         *        + GetSynciv with <pname> SYNC_STATUS ignores the other
         *          parameters and returns SIGNALED in <values>.
         *
         *        + GetQueryObjectuiv with <pname> QUERY_RESULT_AVAILABLE
         *          ignores the other parameters and returns TRUE in <params>."
         */
        SET_GetError(ctx->ContextLost, _mesa_GetError);
        SET_GetGraphicsResetStatusARB(ctx->ContextLost, _mesa_GetGraphicsResetStatusARB);
        SET_GetSynciv(ctx->ContextLost, _context_lost_GetSynciv);
        SET_GetQueryObjectuiv(ctx->ContextLost, _context_lost_GetQueryObjectuiv);
    }

    ctx->CurrentServerDispatch = ctx->ContextLost;
    _glapi_set_dispatch(ctx->CurrentServerDispatch);
}

/**
 * Returns an error code specified by GL_ARB_robustness, or GL_NO_ERROR.
 * \return current context status
 */
GLenum GLAPIENTRY
_mesa_GetGraphicsResetStatusARB( void )
{
    GET_CURRENT_CONTEXT(ctx);
    GLenum status = GL_NO_ERROR;

    /* The ARB_robustness specification says:
     *
     *     "If the reset notification behavior is NO_RESET_NOTIFICATION_ARB,
     *     then the implementation will never deliver notification of reset
     *     events, and GetGraphicsResetStatusARB will always return NO_ERROR."
     */
    if (ctx->Const.ResetStrategy == GL_NO_RESET_NOTIFICATION_ARB) {
        if (MESA_VERBOSE & VERBOSE_API)
            _mesa_debug(ctx,
                        "glGetGraphicsResetStatusARB always returns GL_NO_ERROR "
                        "because reset notifictation was not requested at context "
                        "creation.\n");

        return GL_NO_ERROR;
    }

    if (ctx->Driver.GetGraphicsResetStatus) {
        /* Query the reset status of this context from the driver core.
         */
        status = ctx->Driver.GetGraphicsResetStatus(ctx);

        simple_mtx_lock(&ctx->Shared->Mutex);

        /* If this context has not been affected by a GPU reset, check to see if
         * some other context in the share group has been affected by a reset.
         * If another context saw a reset but this context did not, assume that
         * this context was not guilty.
         */
        if (status != GL_NO_ERROR) {
            ctx->Shared->ShareGroupReset = true;
            ctx->Shared->DisjointOperation = true;
        } else if (ctx->Shared->ShareGroupReset && !ctx->ShareGroupReset) {
            status = GL_INNOCENT_CONTEXT_RESET_ARB;
        }

        ctx->ShareGroupReset = ctx->Shared->ShareGroupReset;
        simple_mtx_unlock(&ctx->Shared->Mutex);
    }

    if (status != GL_NO_ERROR)
        _mesa_set_context_lost_dispatch(ctx);

    if (!ctx->Driver.GetGraphicsResetStatus && (MESA_VERBOSE & VERBOSE_API))
        _mesa_debug(ctx,
                    "glGetGraphicsResetStatusARB always returns GL_NO_ERROR "
                    "because the driver doesn't track reset status.\n");

    return status;
}

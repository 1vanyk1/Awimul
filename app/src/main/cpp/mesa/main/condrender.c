#include "glheader.h"
#include "condrender.h"
#include "enums.h"
#include "mtypes.h"
#include "queryobj.h"


static ALWAYS_INLINE void
begin_conditional_render(struct gl_context *ctx, GLuint queryId, GLenum mode,
                         bool no_error)
{
    struct gl_query_object *q = NULL;

    assert(ctx->Query.CondRenderMode == GL_NONE);

    if (queryId != 0)
        q = _mesa_lookup_query_object(ctx, queryId);

    if (!no_error) {
        /* Section 2.14 (Conditional Rendering) of the OpenGL 3.0 spec says:
         *
         *     "The error INVALID_VALUE is generated if <id> is not the name of an
         *     existing query object query."
         */
        if (!q) {
            _mesa_error(ctx, GL_INVALID_VALUE,
                        "glBeginConditionalRender(bad queryId=%u)", queryId);
            return;
        }
        assert(q->Id == queryId);

        switch (mode) {
            case GL_QUERY_WAIT:
            case GL_QUERY_NO_WAIT:
            case GL_QUERY_BY_REGION_WAIT:
            case GL_QUERY_BY_REGION_NO_WAIT:
                break; /* OK */
            case GL_QUERY_WAIT_INVERTED:
            case GL_QUERY_NO_WAIT_INVERTED:
            case GL_QUERY_BY_REGION_WAIT_INVERTED:
            case GL_QUERY_BY_REGION_NO_WAIT_INVERTED:
                if (ctx->Extensions.ARB_conditional_render_inverted)
                    break; /* OK */
                FALLTHROUGH;
            default:
                _mesa_error(ctx, GL_INVALID_ENUM, "glBeginConditionalRender(mode=%s)",
                            _mesa_enum_to_string(mode));
                return;
        }

        /* Section 2.14 (Conditional Rendering) of the OpenGL 3.0 spec says:
         *
         *     "The error INVALID_OPERATION is generated if <id> is the name of a
         *     query object with a target other than SAMPLES_PASSED, or <id> is
         *     the name of a query currently in progress."
         */
        if ((q->Target != GL_SAMPLES_PASSED &&
             q->Target != GL_ANY_SAMPLES_PASSED &&
             q->Target != GL_ANY_SAMPLES_PASSED_CONSERVATIVE &&
             q->Target != GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB &&
             q->Target != GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB) || q->Active) {
            _mesa_error(ctx, GL_INVALID_OPERATION, "glBeginConditionalRender()");
            return;
        }
    }

    ctx->Query.CondRenderQuery = q;
    ctx->Query.CondRenderMode = mode;

    if (ctx->Driver.BeginConditionalRender)
        ctx->Driver.BeginConditionalRender(ctx, q, mode);
}


void GLAPIENTRY
_mesa_BeginConditionalRender_no_error(GLuint queryId, GLenum mode)
{
    GET_CURRENT_CONTEXT(ctx);
    begin_conditional_render(ctx, queryId, mode, true);
}


void GLAPIENTRY
_mesa_BeginConditionalRender(GLuint queryId, GLenum mode)
{
    GET_CURRENT_CONTEXT(ctx);

    /* Section 2.14 (Conditional Rendering) of the OpenGL 3.0 spec says:
     *
     *     "If BeginConditionalRender is called while conditional rendering is
     *     in progress, or if EndConditionalRender is called while conditional
     *     rendering is not in progress, the error INVALID_OPERATION is
     *     generated."
     */
    if (!ctx->Extensions.NV_conditional_render || ctx->Query.CondRenderQuery) {
        _mesa_error(ctx, GL_INVALID_OPERATION, "glBeginConditionalRender()");
        return;
    }

    begin_conditional_render(ctx, queryId, mode, false);
}


static void
end_conditional_render(struct gl_context *ctx)
{
    FLUSH_VERTICES(ctx, 0, 0);

    if (ctx->Driver.EndConditionalRender)
        ctx->Driver.EndConditionalRender(ctx, ctx->Query.CondRenderQuery);

    ctx->Query.CondRenderQuery = NULL;
    ctx->Query.CondRenderMode = GL_NONE;
}


void APIENTRY
_mesa_EndConditionalRender_no_error(void)
{
    GET_CURRENT_CONTEXT(ctx);
    end_conditional_render(ctx);
}


void APIENTRY
_mesa_EndConditionalRender(void)
{
    GET_CURRENT_CONTEXT(ctx);

    if (!ctx->Extensions.NV_conditional_render || !ctx->Query.CondRenderQuery) {
        _mesa_error(ctx, GL_INVALID_OPERATION, "glEndConditionalRender()");
        return;
    }

    end_conditional_render(ctx);
}


/**
 * This function is called by software rendering commands (all point,
 * line triangle drawing, glClear, glDrawPixels, glCopyPixels, and
 * glBitmap, glBlitFramebuffer) to determine if subsequent drawing
 * commands should be
 * executed or discarded depending on the current conditional
 * rendering state.  Ideally, this check would be implemented by the
 * GPU when doing hardware rendering.  XXX should this function be
 * called via a new driver hook?
 *
 * \return GL_TRUE if we should render, GL_FALSE if we should discard
 */
GLboolean
_mesa_check_conditional_render(struct gl_context *ctx)
{
    struct gl_query_object *q = ctx->Query.CondRenderQuery;

    if (!q) {
        /* no query in progress - draw normally */
        return GL_TRUE;
    }

    switch (ctx->Query.CondRenderMode) {
        case GL_QUERY_BY_REGION_WAIT:
            /* fall-through */
        case GL_QUERY_WAIT:
            if (!q->Ready) {
                ctx->Driver.WaitQuery(ctx, q);
            }
            return q->Result > 0;
        case GL_QUERY_BY_REGION_WAIT_INVERTED:
            /* fall-through */
        case GL_QUERY_WAIT_INVERTED:
            if (!q->Ready) {
                ctx->Driver.WaitQuery(ctx, q);
            }
            return q->Result == 0;
        case GL_QUERY_BY_REGION_NO_WAIT:
            /* fall-through */
        case GL_QUERY_NO_WAIT:
            if (!q->Ready)
                ctx->Driver.CheckQuery(ctx, q);
            return q->Ready ? (q->Result > 0) : GL_TRUE;
        case GL_QUERY_BY_REGION_NO_WAIT_INVERTED:
            /* fall-through */
        case GL_QUERY_NO_WAIT_INVERTED:
            if (!q->Ready)
                ctx->Driver.CheckQuery(ctx, q);
            return q->Ready ? (q->Result == 0) : GL_TRUE;
        default:
            _mesa_problem(ctx, "Bad cond render mode %s in "
                               " _mesa_check_conditional_render()",
                          _mesa_enum_to_string(ctx->Query.CondRenderMode));
            return GL_TRUE;
    }
}

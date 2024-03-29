#include "errors.h"
#include "drawtex.h"
#include "state.h"

#include "mtypes.h"


static void
draw_texture(struct gl_context *ctx, GLfloat x, GLfloat y, GLfloat z,
             GLfloat width, GLfloat height)
{
    if (!ctx->Extensions.OES_draw_texture) {
        _mesa_error(ctx, GL_INVALID_OPERATION,
                    "glDrawTex(unsupported)");
        return;
    }
    if (width <= 0.0f || height <= 0.0f) {
        _mesa_error(ctx, GL_INVALID_VALUE, "glDrawTex(width or height <= 0)");
        return;
    }

    _mesa_set_vp_override(ctx, GL_TRUE);

    if (ctx->NewState)
        _mesa_update_state(ctx);

    assert(ctx->Driver.DrawTex);
    ctx->Driver.DrawTex(ctx, x, y, z, width, height);

    _mesa_set_vp_override(ctx, GL_FALSE);
}


void GLAPIENTRY
_mesa_DrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
GET_CURRENT_CONTEXT(ctx);
draw_texture(ctx, x, y, z, width, height);
}


void GLAPIENTRY
_mesa_DrawTexfvOES(const GLfloat *coords)
{
    GET_CURRENT_CONTEXT(ctx);
    draw_texture(ctx, coords[0], coords[1], coords[2], coords[3], coords[4]);
}


void GLAPIENTRY
_mesa_DrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
GET_CURRENT_CONTEXT(ctx);
draw_texture(ctx, (GLfloat) x, (GLfloat) y, (GLfloat) z,
(GLfloat) width, (GLfloat) height);
}


void GLAPIENTRY
_mesa_DrawTexivOES(const GLint *coords)
{
    GET_CURRENT_CONTEXT(ctx);
    draw_texture(ctx, (GLfloat) coords[0], (GLfloat) coords[1],
            (GLfloat) coords[2], (GLfloat) coords[3], (GLfloat) coords[4]);
}


void GLAPIENTRY
_mesa_DrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
GET_CURRENT_CONTEXT(ctx);
draw_texture(ctx, (GLfloat) x, (GLfloat) y, (GLfloat) z,
(GLfloat) width, (GLfloat) height);
}


void GLAPIENTRY
_mesa_DrawTexsvOES(const GLshort *coords)
{
    GET_CURRENT_CONTEXT(ctx);
    draw_texture(ctx, (GLfloat) coords[0], (GLfloat) coords[1],
            (GLfloat) coords[2], (GLfloat) coords[3], (GLfloat) coords[4]);
}


void GLAPIENTRY
_mesa_DrawTexx(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
GET_CURRENT_CONTEXT(ctx);
draw_texture(ctx,
(GLfloat) x / 65536.0f,
(GLfloat) y / 65536.0f,
(GLfloat) z / 65536.0f,
(GLfloat) width / 65536.0f,
(GLfloat) height / 65536.0f);
}


void GLAPIENTRY
_mesa_DrawTexxv(const GLfixed *coords)
{
    GET_CURRENT_CONTEXT(ctx);
    draw_texture(ctx,
                 (GLfloat) coords[0] / 65536.0f,
            (GLfloat) coords[1] / 65536.0f,
            (GLfloat) coords[2] / 65536.0f,
            (GLfloat) coords[3] / 65536.0f,
            (GLfloat) coords[4] / 65536.0f);
}

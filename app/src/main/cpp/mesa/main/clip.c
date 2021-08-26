#include "glheader.h"
#include "clip.h"
#include "context.h"
#include "macros.h"
#include "mtypes.h"

#include "../math/m_matrix.h"


/**
 * Update derived clip plane state.
 */
void
_mesa_update_clip_plane(struct gl_context *ctx, GLuint plane)
{
    /* make sure the inverse is up to date */
    if (_math_matrix_is_dirty(ctx->ProjectionMatrixStack.Top))
        _math_matrix_analyse( ctx->ProjectionMatrixStack.Top );

    /* Clip-Space Plane = Eye-Space Plane * Projection Matrix */
    _mesa_transform_vector(ctx->Transform._ClipUserPlane[plane],
                           ctx->Transform.EyeUserPlane[plane],
                           ctx->ProjectionMatrixStack.Top->inv);
}


void GLAPIENTRY
_mesa_ClipPlane( GLenum plane, const GLdouble *eq )
{
    GET_CURRENT_CONTEXT(ctx);
    GLint p;
    GLfloat equation[4];

    p = (GLint) plane - (GLint) GL_CLIP_PLANE0;
    if (p < 0 || p >= (GLint) ctx->Const.MaxClipPlanes) {
        _mesa_error( ctx, GL_INVALID_ENUM, "glClipPlane" );
        return;
    }

    equation[0] = (GLfloat) eq[0];
    equation[1] = (GLfloat) eq[1];
    equation[2] = (GLfloat) eq[2];
    equation[3] = (GLfloat) eq[3];

    /*
     * The equation is transformed by the transpose of the inverse of the
     * current modelview matrix and stored in the resulting eye coordinates.
     *
     * KW: Eqn is then transformed to the current clip space, where user
     * clipping now takes place.  The clip-space equations are recalculated
     * whenever the projection matrix changes.
     */
    if (_math_matrix_is_dirty(ctx->ModelviewMatrixStack.Top))
        _math_matrix_analyse( ctx->ModelviewMatrixStack.Top );

    _mesa_transform_vector( equation, equation,
                            ctx->ModelviewMatrixStack.Top->inv );

    if (TEST_EQ_4V(ctx->Transform.EyeUserPlane[p], equation))
        return;

    /* EyeUserPlane is used by program state constants. */
    FLUSH_VERTICES(ctx, _NEW_TRANSFORM, GL_TRANSFORM_BIT);
    ctx->NewDriverState |= ctx->DriverFlags.NewClipPlane;
    COPY_4FV(ctx->Transform.EyeUserPlane[p], equation);

    if (ctx->Transform.ClipPlanesEnabled & (1 << p)) {
        _mesa_update_clip_plane(ctx, p);
    }

    if (ctx->Driver.ClipPlane)
        ctx->Driver.ClipPlane( ctx, plane, equation );
}


void GLAPIENTRY
_mesa_GetClipPlane( GLenum plane, GLdouble *equation )
{
    GET_CURRENT_CONTEXT(ctx);
    GLint p;

    p = (GLint) (plane - GL_CLIP_PLANE0);
    if (p < 0 || p >= (GLint) ctx->Const.MaxClipPlanes) {
        _mesa_error( ctx, GL_INVALID_ENUM, "glGetClipPlane" );
        return;
    }

    equation[0] = (GLdouble) ctx->Transform.EyeUserPlane[p][0];
    equation[1] = (GLdouble) ctx->Transform.EyeUserPlane[p][1];
    equation[2] = (GLdouble) ctx->Transform.EyeUserPlane[p][2];
    equation[3] = (GLdouble) ctx->Transform.EyeUserPlane[p][3];
}

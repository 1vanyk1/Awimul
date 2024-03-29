#include "glthread_marshal.h"
#include "dispatch.h"

void
_mesa_unmarshal_GetIntegerv(struct gl_context *ctx,
                            const struct marshal_cmd_GetIntegerv *cmd)
{
    unreachable("never executed");
}

void GLAPIENTRY
_mesa_marshal_GetIntegerv(GLenum pname, GLint *p)
{
GET_CURRENT_CONTEXT(ctx);

/* TODO: Use get_hash_params.py to return values for items containing:
 * - CONST(
 * - CONTEXT_[A-Z]*(Const
 */

if (ctx->API != API_OPENGL_COMPAT) {
/* glthread only tracks these states for the compatibility profile. */
_mesa_glthread_finish_before(ctx, "GetIntegerv");
CALL_GetIntegerv(ctx->CurrentServerDispatch, (pname, p));
return;
}

switch (pname) {
case GL_ACTIVE_TEXTURE:
*p = GL_TEXTURE0 + ctx->GLThread.ActiveTexture;
return;
case GL_ARRAY_BUFFER_BINDING:
*p = ctx->GLThread.CurrentArrayBufferName;
return;
case GL_ATTRIB_STACK_DEPTH:
*p = ctx->GLThread.AttribStackDepth;
return;
case GL_CLIENT_ACTIVE_TEXTURE:
*p = ctx->GLThread.ClientActiveTexture;
return;
case GL_CLIENT_ATTRIB_STACK_DEPTH:
*p = ctx->GLThread.ClientAttribStackTop;
return;
case GL_DRAW_INDIRECT_BUFFER_BINDING:
*p = ctx->GLThread.CurrentDrawIndirectBufferName;
return;

case GL_MATRIX_MODE:
*p = ctx->GLThread.MatrixMode;
return;
case GL_CURRENT_MATRIX_STACK_DEPTH_ARB:
*p = ctx->GLThread.MatrixStackDepth[ctx->GLThread.MatrixIndex] + 1;
return;
case GL_MODELVIEW_STACK_DEPTH:
*p = ctx->GLThread.MatrixStackDepth[M_MODELVIEW] + 1;
return;
case GL_PROJECTION_STACK_DEPTH:
*p = ctx->GLThread.MatrixStackDepth[M_PROJECTION] + 1;
return;
case GL_TEXTURE_STACK_DEPTH:
*p = ctx->GLThread.MatrixStackDepth[M_TEXTURE0 + ctx->GLThread.ActiveTexture] + 1;
return;

case GL_VERTEX_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_POS)) != 0;
return;
case GL_NORMAL_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_NORMAL)) != 0;
return;
case GL_COLOR_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_COLOR0)) != 0;
return;
case GL_SECONDARY_COLOR_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_COLOR1)) != 0;
return;
case GL_FOG_COORD_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_FOG)) != 0;
return;
case GL_INDEX_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_COLOR_INDEX)) != 0;
return;
case GL_EDGE_FLAG_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_EDGEFLAG)) != 0;
return;
case GL_TEXTURE_COORD_ARRAY:
*p = (ctx->GLThread.CurrentVAO->UserEnabled &
      (1 << (VERT_ATTRIB_TEX0 + ctx->GLThread.ClientActiveTexture))) != 0;
return;
case GL_POINT_SIZE_ARRAY_OES:
*p = (ctx->GLThread.CurrentVAO->UserEnabled & (1 << VERT_ATTRIB_POINT_SIZE)) != 0;
return;
}

_mesa_glthread_finish_before(ctx, "GetIntegerv");
CALL_GetIntegerv(ctx->CurrentServerDispatch, (pname, p));
}

/* TODO: Implement glGetBooleanv, glGetFloatv, etc. if needed */

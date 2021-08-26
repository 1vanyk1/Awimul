#include "bbox.h"
#include "context.h"

void GLAPIENTRY
_mesa_PrimitiveBoundingBox(
        GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW,
        GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)
{
    GET_CURRENT_CONTEXT(ctx);

    ctx->PrimitiveBoundingBox[0] = minX;
    ctx->PrimitiveBoundingBox[1] = minY;
    ctx->PrimitiveBoundingBox[2] = minZ;
    ctx->PrimitiveBoundingBox[3] = minW;
    ctx->PrimitiveBoundingBox[4] = maxX;
    ctx->PrimitiveBoundingBox[5] = maxY;
    ctx->PrimitiveBoundingBox[6] = maxZ;
    ctx->PrimitiveBoundingBox[7] = maxW;
}

void
_mesa_init_bbox(struct gl_context *ctx)
{
    ctx->PrimitiveBoundingBox[0] =
    ctx->PrimitiveBoundingBox[1] =
    ctx->PrimitiveBoundingBox[2] = -1.0f;

    ctx->PrimitiveBoundingBox[3] =
    ctx->PrimitiveBoundingBox[4] =
    ctx->PrimitiveBoundingBox[5] =
    ctx->PrimitiveBoundingBox[6] =
    ctx->PrimitiveBoundingBox[7] = 1.0f;
}

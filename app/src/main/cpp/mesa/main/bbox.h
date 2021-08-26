#ifndef BBOX_H
#define BBOX_H

#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_PrimitiveBoundingBox(
        GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW,
        GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);

extern void
_mesa_init_bbox(struct gl_context *ctx);

#endif

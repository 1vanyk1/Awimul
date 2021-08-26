#ifndef CLIP_H
#define CLIP_H

#include "glheader.h"

struct gl_context;

extern void
_mesa_update_clip_plane(struct gl_context *ctx, GLuint plane);

extern void GLAPIENTRY
_mesa_ClipPlane( GLenum plane, const GLdouble *equation );

extern void GLAPIENTRY
_mesa_GetClipPlane( GLenum plane, GLdouble *equation );

#endif

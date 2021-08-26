#ifndef CONSERVATIVERASTER_H
#define CONSERVATIVERASTER_H

#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_ConservativeRasterParameteriNV_no_error(GLenum pname, GLint param);

extern void GLAPIENTRY
_mesa_ConservativeRasterParameteriNV(GLenum pname, GLint param);

extern void GLAPIENTRY
_mesa_ConservativeRasterParameterfNV_no_error(GLenum pname, GLfloat param);

extern void GLAPIENTRY
_mesa_ConservativeRasterParameterfNV(GLenum pname, GLfloat param);

extern void
_mesa_init_conservative_raster(struct gl_context *ctx);

#endif

#ifndef ACCUM_H
#define ACCUM_H

#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_ClearAccum( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
void GLAPIENTRY
_mesa_Accum( GLenum op, GLfloat value );

extern void
_mesa_clear_accum_buffer(struct gl_context *ctx);

extern void
_mesa_init_accum( struct gl_context *ctx );

#endif /* ACCUM_H */

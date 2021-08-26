#ifndef FOG_H
#define FOG_H


#include "glheader.h"

struct gl_context;


extern void GLAPIENTRY
_mesa_Fogf(GLenum pname, GLfloat param);

extern void GLAPIENTRY
_mesa_Fogi(GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_Fogfv(GLenum pname, const GLfloat *params );

extern void GLAPIENTRY
_mesa_Fogiv(GLenum pname, const GLint *params );

extern void _mesa_init_fog( struct gl_context * ctx );

#endif

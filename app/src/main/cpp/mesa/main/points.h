#ifndef POINTS_H
#define POINTS_H


#include "glheader.h"

struct gl_context;


void GLAPIENTRY
_mesa_PointSize_no_error(GLfloat size);

extern void GLAPIENTRY
_mesa_PointSize( GLfloat size );

extern void GLAPIENTRY
_mesa_PointParameteri( GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_PointParameteriv( GLenum pname, const GLint *params );

extern void GLAPIENTRY
_mesa_PointParameterf( GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_PointParameterfv( GLenum pname, const GLfloat *params );

extern void
_mesa_init_point( struct gl_context * ctx );


#endif

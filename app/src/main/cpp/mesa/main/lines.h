#ifndef LINES_H
#define LINES_H


#include "glheader.h"

struct gl_context;

void GLAPIENTRY
_mesa_LineWidth_no_error(GLfloat width);

extern void GLAPIENTRY
_mesa_LineWidth( GLfloat width );

extern void GLAPIENTRY
_mesa_LineStipple( GLint factor, GLushort pattern );

extern void
_mesa_init_line( struct gl_context * ctx );

#endif

#ifndef POLYGON_H
#define POLYGON_H


#include "glheader.h"

struct gl_context;

extern void GLAPIENTRY
_mesa_GetnPolygonStippleARB( GLsizei bufSize, GLubyte *dest );

void GLAPIENTRY
_mesa_CullFace_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_CullFace(GLenum mode);

void GLAPIENTRY
_mesa_FrontFace_no_error(GLenum mode);

extern void GLAPIENTRY
_mesa_FrontFace(GLenum mode);

void GLAPIENTRY
_mesa_PolygonMode_no_error(GLenum face, GLenum mode);

extern void GLAPIENTRY
_mesa_PolygonMode( GLenum face, GLenum mode );

extern void GLAPIENTRY
_mesa_PolygonOffset( GLfloat factor, GLfloat units );

extern void GLAPIENTRY
_mesa_PolygonOffsetClampEXT( GLfloat factor, GLfloat units, GLfloat clamp );

extern void GLAPIENTRY
_mesa_PolygonStipple( const GLubyte *mask );

extern void GLAPIENTRY
_mesa_GetPolygonStipple( GLubyte *mask );

extern void
_mesa_polygon_offset_clamp(struct gl_context *ctx,
                           GLfloat factor, GLfloat units, GLfloat clamp);
extern void
_mesa_init_polygon( struct gl_context * ctx );

#endif

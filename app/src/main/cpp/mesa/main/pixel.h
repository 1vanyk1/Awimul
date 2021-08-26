#ifndef PIXEL_H
#define PIXEL_H


#include "glheader.h"

struct gl_context;


void GLAPIENTRY
_mesa_PixelZoom( GLfloat xfactor, GLfloat yfactor );
void GLAPIENTRY
_mesa_PixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values );
void GLAPIENTRY
_mesa_PixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values );
void GLAPIENTRY
_mesa_PixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values );
void GLAPIENTRY
_mesa_GetnPixelMapfvARB( GLenum map, GLsizei bufSize, GLfloat *values );
void GLAPIENTRY
_mesa_GetPixelMapfv( GLenum map, GLfloat *values );
void GLAPIENTRY
_mesa_GetnPixelMapuivARB( GLenum map, GLsizei bufSize, GLuint *values );
void GLAPIENTRY
_mesa_GetPixelMapuiv( GLenum map, GLuint *values );
void GLAPIENTRY
_mesa_GetnPixelMapusvARB( GLenum map, GLsizei bufSize, GLushort *values );
void GLAPIENTRY
_mesa_GetPixelMapusv( GLenum map, GLushort *values );
void GLAPIENTRY
_mesa_PixelTransferf(GLenum pname, GLfloat param);
void GLAPIENTRY
_mesa_PixelTransferi( GLenum pname, GLint param );

extern void
_mesa_update_pixel( struct gl_context *ctx );

extern void
_mesa_init_pixel( struct gl_context * ctx );

/*@}*/

#endif /* PIXEL_H */

#ifndef GET_H
#define GET_H


#include "glheader.h"


extern void GLAPIENTRY
_mesa_GetBooleanv( GLenum pname, GLboolean *params );

extern void GLAPIENTRY
_mesa_GetDoublev( GLenum pname, GLdouble *params );

extern void GLAPIENTRY
_mesa_GetFloatv( GLenum pname, GLfloat *params );

extern void GLAPIENTRY
_mesa_GetIntegerv( GLenum pname, GLint *params );

extern void GLAPIENTRY
_mesa_GetInteger64v( GLenum pname, GLint64 *params );

extern void GLAPIENTRY
_mesa_GetFixedv(GLenum pname, GLfixed *params);

extern void GLAPIENTRY
_mesa_GetUnsignedBytevEXT(GLenum pname, GLubyte *data);

extern void GLAPIENTRY
_mesa_GetBooleani_v( GLenum pname, GLuint index, GLboolean *params );

extern void GLAPIENTRY
_mesa_GetIntegeri_v( GLenum pname, GLuint index, GLint *params );

extern void GLAPIENTRY
_mesa_GetInteger64i_v( GLenum pname, GLuint index, GLint64 *params );

extern void GLAPIENTRY
_mesa_GetPointerv( GLenum pname, GLvoid **params );

extern void GLAPIENTRY
_mesa_GetPointerIndexedvEXT( GLenum pname, GLuint index, GLvoid **params );

extern void GLAPIENTRY
_mesa_GetFloati_v(GLenum target, GLuint index, GLfloat *data);

extern void GLAPIENTRY
_mesa_GetDoublei_v(GLenum target, GLuint index, GLdouble *data);

extern void GLAPIENTRY
_mesa_GetUnsignedBytei_vEXT(GLenum target, GLuint index, GLubyte *data);

extern const GLubyte * GLAPIENTRY
_mesa_GetString( GLenum name );

extern const GLubyte * GLAPIENTRY
_mesa_GetStringi(GLenum name, GLuint index);

extern GLenum GLAPIENTRY
_mesa_GetError( void );

/* GL_ARB_robustness */
extern GLenum GLAPIENTRY
_mesa_GetGraphicsResetStatusARB( void );

struct gl_vertex_array_object;

extern void
_get_vao_pointerv(GLenum pname, struct gl_vertex_array_object* vao,
                  GLvoid **params, const char* callerstr);

#endif

#ifndef TEXENV_H
#define TEXENV_H


#include "glheader.h"


extern void GLAPIENTRY
_mesa_GetTexEnvfv( GLenum target, GLenum pname, GLfloat *params );

extern void GLAPIENTRY
_mesa_GetTexEnviv( GLenum target, GLenum pname, GLint *params );

extern void GLAPIENTRY
_mesa_TexEnvf( GLenum target, GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_TexEnvfv( GLenum target, GLenum pname, const GLfloat *param );

extern void GLAPIENTRY
_mesa_TexEnvi( GLenum target, GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_TexEnviv( GLenum target, GLenum pname, const GLint *param );

extern void GLAPIENTRY
_mesa_MultiTexEnvfEXT( GLenum texunit, GLenum target,
        GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_MultiTexEnvfvEXT( GLenum texunit, GLenum target,
        GLenum pname, const GLfloat *param );

extern void GLAPIENTRY
_mesa_MultiTexEnviEXT( GLenum texunit, GLenum target,
        GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_MultiTexEnvivEXT( GLenum texunit, GLenum target,
        GLenum pname, const GLint *param );

extern void GLAPIENTRY
_mesa_GetMultiTexEnvivEXT( GLenum texunit, GLenum target,
        GLenum pname, GLint *param );

extern void GLAPIENTRY
_mesa_GetMultiTexEnvfvEXT( GLenum texunit, GLenum target,
        GLenum pname, GLfloat *param );

#endif /* TEXENV_H */

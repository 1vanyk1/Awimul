#ifndef TEXGEN_H
#define TEXGEN_H


#include "glheader.h"

struct _glapi_table;


void GLAPIENTRY
_mesa_TexGenfv( GLenum coord, GLenum pname, const GLfloat *params );
void GLAPIENTRY
_mesa_TexGeniv(GLenum coord, GLenum pname, const GLint *params );
void GLAPIENTRY
_mesa_TexGend(GLenum coord, GLenum pname, GLdouble param );
void GLAPIENTRY
_mesa_TexGendv(GLenum coord, GLenum pname, const GLdouble *params );
void GLAPIENTRY
_mesa_TexGenf( GLenum coord, GLenum pname, GLfloat param );
void GLAPIENTRY
_mesa_TexGeni( GLenum coord, GLenum pname, GLint param );
void GLAPIENTRY
_mesa_GetTexGendv( GLenum coord, GLenum pname, GLdouble *params );
void GLAPIENTRY
_mesa_GetTexGenfv( GLenum coord, GLenum pname, GLfloat *params );
void GLAPIENTRY
_mesa_GetTexGeniv( GLenum coord, GLenum pname, GLint *params );

void GLAPIENTRY
_mesa_MultiTexGenfvEXT( GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params );
void GLAPIENTRY
_mesa_MultiTexGenivEXT(GLenum texunit, GLenum coord, GLenum pname, const GLint *params );
void GLAPIENTRY
_mesa_MultiTexGendEXT(GLenum texunit, GLenum coord, GLenum pname, GLdouble param );
void GLAPIENTRY
_mesa_MultiTexGendvEXT(GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params );
void GLAPIENTRY
_mesa_MultiTexGenfEXT( GLenum texunit, GLenum coord, GLenum pname, GLfloat param );
void GLAPIENTRY
_mesa_MultiTexGeniEXT( GLenum texunit, GLenum coord, GLenum pname, GLint param );
void GLAPIENTRY
_mesa_GetMultiTexGendvEXT( GLenum texunit, GLenum coord, GLenum pname, GLdouble *params );
void GLAPIENTRY
_mesa_GetMultiTexGenfvEXT( GLenum texunit, GLenum coord, GLenum pname, GLfloat *params );
void GLAPIENTRY
_mesa_GetMultiTexGenivEXT( GLenum texunit, GLenum coord, GLenum pname, GLint *params );

extern void GLAPIENTRY
_es_GetTexGenfv(GLenum coord, GLenum pname, GLfloat *params);

extern void GLAPIENTRY
_es_TexGenf(GLenum coord, GLenum pname, GLfloat param);

extern void GLAPIENTRY
_es_TexGenfv(GLenum coord, GLenum pname, const GLfloat *params);


#endif /* TEXGEN_H */

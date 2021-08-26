#ifndef RASTPOS_H
#define RASTPOS_H


#include "glheader.h"


struct _glapi_table;
struct gl_context;

extern void
_mesa_init_rastpos(struct gl_context *ctx);

void
_mesa_RasterPos(struct gl_context *ctx, const GLfloat vObj[4]);

void GLAPIENTRY
_mesa_RasterPos2d(GLdouble x, GLdouble y);
void GLAPIENTRY
_mesa_RasterPos2f(GLfloat x, GLfloat y);
void GLAPIENTRY
_mesa_RasterPos2i(GLint x, GLint y);
void GLAPIENTRY
_mesa_RasterPos2s(GLshort x, GLshort y);
void GLAPIENTRY
_mesa_RasterPos3d(GLdouble x, GLdouble y, GLdouble z);
void GLAPIENTRY
_mesa_RasterPos3f(GLfloat x, GLfloat y, GLfloat z);
void GLAPIENTRY
_mesa_RasterPos3i(GLint x, GLint y, GLint z);
void GLAPIENTRY
_mesa_RasterPos3s(GLshort x, GLshort y, GLshort z);
void GLAPIENTRY
_mesa_RasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void GLAPIENTRY
_mesa_RasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void GLAPIENTRY
_mesa_RasterPos4i(GLint x, GLint y, GLint z, GLint w);
void GLAPIENTRY
_mesa_RasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);
void GLAPIENTRY
_mesa_RasterPos2dv(const GLdouble *v);
void GLAPIENTRY
_mesa_RasterPos2fv(const GLfloat *v);
void GLAPIENTRY
_mesa_RasterPos2iv(const GLint *v);
void GLAPIENTRY
_mesa_RasterPos2sv(const GLshort *v);
void GLAPIENTRY
_mesa_RasterPos3dv(const GLdouble *v);
void GLAPIENTRY
_mesa_RasterPos3fv(const GLfloat *v);
void GLAPIENTRY
_mesa_RasterPos3iv(const GLint *v);
void GLAPIENTRY
_mesa_RasterPos3sv(const GLshort *v);
void GLAPIENTRY
_mesa_RasterPos4dv(const GLdouble *v);
void GLAPIENTRY
_mesa_RasterPos4fv(const GLfloat *v);
void GLAPIENTRY
_mesa_RasterPos4iv(const GLint *v);
void GLAPIENTRY
_mesa_RasterPos4sv(const GLshort *v);
void GLAPIENTRY
_mesa_WindowPos2d(GLdouble x, GLdouble y);
void GLAPIENTRY
_mesa_WindowPos2f(GLfloat x, GLfloat y);
void GLAPIENTRY
_mesa_WindowPos2i(GLint x, GLint y);
void GLAPIENTRY
_mesa_WindowPos2s(GLshort x, GLshort y);
void GLAPIENTRY
_mesa_WindowPos3d(GLdouble x, GLdouble y, GLdouble z);
void GLAPIENTRY
_mesa_WindowPos3f(GLfloat x, GLfloat y, GLfloat z);
void GLAPIENTRY
_mesa_WindowPos3i(GLint x, GLint y, GLint z);
void GLAPIENTRY
_mesa_WindowPos3s(GLshort x, GLshort y, GLshort z);
void GLAPIENTRY
_mesa_WindowPos4dMESA(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void GLAPIENTRY
_mesa_WindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void GLAPIENTRY
_mesa_WindowPos4iMESA(GLint x, GLint y, GLint z, GLint w);
void GLAPIENTRY
_mesa_WindowPos4sMESA(GLshort x, GLshort y, GLshort z, GLshort w);
void GLAPIENTRY
_mesa_WindowPos2dv(const GLdouble *v);
void GLAPIENTRY
_mesa_WindowPos2fv(const GLfloat *v);
void GLAPIENTRY
_mesa_WindowPos2iv(const GLint *v);
void GLAPIENTRY
_mesa_WindowPos2sv(const GLshort *v);
void GLAPIENTRY
_mesa_WindowPos3dv(const GLdouble *v);
void GLAPIENTRY
_mesa_WindowPos3fv(const GLfloat *v);
void GLAPIENTRY
_mesa_WindowPos3iv(const GLint *v);
void GLAPIENTRY
_mesa_WindowPos3sv(const GLshort *v);
void GLAPIENTRY
_mesa_WindowPos4dvMESA(const GLdouble *v);
void GLAPIENTRY
_mesa_WindowPos4fvMESA(const GLfloat *v);
void GLAPIENTRY
_mesa_WindowPos4ivMESA(const GLint *v);
void GLAPIENTRY
_mesa_WindowPos4svMESA(const GLshort *v);


/*@}*/

#endif /* RASTPOS_H */

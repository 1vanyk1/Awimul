#ifndef DRAWTEX_H
#define DRAWTEX_H


#include "glheader.h"


extern void GLAPIENTRY
_mesa_DrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);

extern void GLAPIENTRY
_mesa_DrawTexfvOES(const GLfloat *coords);

extern void GLAPIENTRY
_mesa_DrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height);

extern void GLAPIENTRY
_mesa_DrawTexivOES(const GLint *coords);

extern void GLAPIENTRY
_mesa_DrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);

extern void GLAPIENTRY
_mesa_DrawTexsvOES(const GLshort *coords);

extern void GLAPIENTRY
_mesa_DrawTexx(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);

extern void GLAPIENTRY
_mesa_DrawTexxv(const GLfixed *coords);


#endif /* DRAWTEX_H */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "glheader.h"


void GLAPIENTRY
_mesa_GetnMinmaxARB(GLenum target, GLboolean reset, GLenum format,
                    GLenum type, GLsizei bufSize, GLvoid *values);
void GLAPIENTRY
_mesa_GetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type,
                GLvoid *values);
void GLAPIENTRY
_mesa_GetnHistogramARB(GLenum target, GLboolean reset, GLenum format,
                       GLenum type, GLsizei bufSize, GLvoid *values);
void GLAPIENTRY
_mesa_GetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type,
                   GLvoid *values);
void GLAPIENTRY
_mesa_GetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params);
void GLAPIENTRY
_mesa_GetHistogramParameteriv(GLenum target, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params);
void GLAPIENTRY
_mesa_GetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_Histogram(GLenum target, GLsizei width, GLenum internalFormat,
                GLboolean sink);
void GLAPIENTRY
_mesa_Minmax(GLenum target, GLenum internalFormat, GLboolean sink);
void GLAPIENTRY
_mesa_ResetHistogram(GLenum target);
void GLAPIENTRY
_mesa_ResetMinmax(GLenum target);


#endif /* HISTOGRAM_H */

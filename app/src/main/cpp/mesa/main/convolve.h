#ifndef CONVOLVE_H
#define CONVOLVE_H


#include "glheader.h"

struct _glapi_table;

void GLAPIENTRY
_mesa_ConvolutionFilter1D(GLenum target, GLenum internalFormat, GLsizei width,
                          GLenum format, GLenum type, const GLvoid *image);
void GLAPIENTRY
_mesa_ConvolutionFilter2D(GLenum target, GLenum internalFormat, GLsizei width,
                          GLsizei height, GLenum format, GLenum type,
                          const GLvoid *image);
void GLAPIENTRY
_mesa_ConvolutionParameterf(GLenum target, GLenum pname, GLfloat param);
void GLAPIENTRY
_mesa_ConvolutionParameterfv(GLenum target, GLenum pname,
                             const GLfloat *params);
void GLAPIENTRY
_mesa_ConvolutionParameteri(GLenum target, GLenum pname, GLint param);
void GLAPIENTRY
_mesa_ConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params);
void GLAPIENTRY
_mesa_CopyConvolutionFilter1D(GLenum target, GLenum internalFormat, GLint x,
                              GLint y, GLsizei width);
void GLAPIENTRY
_mesa_CopyConvolutionFilter2D(GLenum target, GLenum internalFormat, GLint x,
                              GLint y, GLsizei width, GLsizei height);
void GLAPIENTRY
_mesa_GetnConvolutionFilterARB(GLenum target, GLenum format, GLenum type,
                               GLsizei bufSize, GLvoid *image);
void GLAPIENTRY
_mesa_GetConvolutionFilter(GLenum target, GLenum format, GLenum type,
                           GLvoid *image);
void GLAPIENTRY
_mesa_GetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params);
void GLAPIENTRY
_mesa_GetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetnSeparableFilterARB(GLenum target, GLenum format, GLenum type,
                             GLsizei rowBufSize, GLvoid *row,
                             GLsizei columnBufSize,  GLvoid *column,
                             GLvoid *span);
void GLAPIENTRY
_mesa_GetSeparableFilter(GLenum target, GLenum format, GLenum type,
                         GLvoid *row, GLvoid *column, GLvoid *span);
void GLAPIENTRY
_mesa_SeparableFilter2D(GLenum target, GLenum internalFormat, GLsizei width,
                        GLsizei height, GLenum format, GLenum type,
                        const GLvoid *row, const GLvoid *column);

#endif /* CONVOLVE_H */

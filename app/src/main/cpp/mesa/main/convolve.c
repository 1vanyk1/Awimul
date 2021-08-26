#include "glheader.h"
#include "context.h"
#include "convolve.h"


void GLAPIENTRY
_mesa_ConvolutionFilter1D(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *image)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionFilter1D");
}

void GLAPIENTRY
_mesa_ConvolutionFilter2D(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionFilter2D");
}


void GLAPIENTRY
_mesa_ConvolutionParameterf(GLenum target, GLenum pname, GLfloat param)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionParameterf");
}


void GLAPIENTRY
_mesa_ConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionParameterfv");
}


void GLAPIENTRY
_mesa_ConvolutionParameteri(GLenum target, GLenum pname, GLint param)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionParameteri");
}


void GLAPIENTRY
_mesa_ConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glConvolutionParameteriv");
}


void GLAPIENTRY
_mesa_CopyConvolutionFilter1D(GLenum target, GLenum internalFormat, GLint x, GLint y, GLsizei width)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glCopyConvolutionFilter1D");
}


void GLAPIENTRY
_mesa_CopyConvolutionFilter2D(GLenum target, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glCopyConvolutionFilter2D");
}


void GLAPIENTRY
_mesa_GetnConvolutionFilterARB(GLenum target, GLenum format, GLenum type,
                               GLsizei bufSize, GLvoid *image)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetConvolutionFilter");
}


void GLAPIENTRY
_mesa_GetConvolutionFilter(GLenum target, GLenum format, GLenum type,
                           GLvoid *image)
{
    _mesa_GetnConvolutionFilterARB(target, format, type, INT_MAX, image);
}


void GLAPIENTRY
_mesa_GetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetConvolutionParameterfv");
}


void GLAPIENTRY
_mesa_GetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetConvolutionParameteriv");
}


void GLAPIENTRY
_mesa_GetnSeparableFilterARB(GLenum target, GLenum format, GLenum type,
                             GLsizei rowBufSize, GLvoid *row,
                             GLsizei columnBufSize,  GLvoid *column,
                             GLvoid *span)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetSeparableFilter");
}


void GLAPIENTRY
_mesa_GetSeparableFilter(GLenum target, GLenum format, GLenum type,
                         GLvoid *row, GLvoid *column, GLvoid *span)
{
    _mesa_GetnSeparableFilterARB(target, format, type, INT_MAX, row,
                                 INT_MAX, column, span);
}


void GLAPIENTRY
_mesa_SeparableFilter2D(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glSeparableFilter2D");
}

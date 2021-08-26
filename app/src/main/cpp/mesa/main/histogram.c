#include "glheader.h"
#include "context.h"
#include "histogram.h"


/**********************************************************************
 * API functions
 */


void GLAPIENTRY
_mesa_GetnMinmaxARB(GLenum target, GLboolean reset, GLenum format,
                    GLenum type, GLsizei bufSize, GLvoid *values)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetMinmax");
}


void GLAPIENTRY
_mesa_GetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type,
                GLvoid *values)
{
    _mesa_GetnMinmaxARB(target, reset, format, type, INT_MAX, values);
}


void GLAPIENTRY
_mesa_GetnHistogramARB(GLenum target, GLboolean reset, GLenum format,
                       GLenum type, GLsizei bufSize, GLvoid *values)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetHistogram");
}


void GLAPIENTRY
_mesa_GetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type,
                   GLvoid *values)
{
    _mesa_GetnHistogramARB(target, reset, format, type, INT_MAX, values);
}


void GLAPIENTRY
_mesa_GetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetHistogramParameterfv");
}


void GLAPIENTRY
_mesa_GetHistogramParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetHistogramParameteriv");
}


void GLAPIENTRY
_mesa_GetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetMinmaxParameterfv");
}


void GLAPIENTRY
_mesa_GetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetMinmaxParameteriv");
}


void GLAPIENTRY
_mesa_Histogram(GLenum target, GLsizei width, GLenum internalFormat, GLboolean sink)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glHistogram");
}


void GLAPIENTRY
_mesa_Minmax(GLenum target, GLenum internalFormat, GLboolean sink)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glMinmax");
}


void GLAPIENTRY
_mesa_ResetHistogram(GLenum target)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glResetHistogram");
}


void GLAPIENTRY
_mesa_ResetMinmax(GLenum target)
{
    GET_CURRENT_CONTEXT(ctx);

    _mesa_error(ctx, GL_INVALID_OPERATION, "glResetMinmax");
}

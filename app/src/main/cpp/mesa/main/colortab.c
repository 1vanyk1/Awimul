#include "glheader.h"
#include "bufferobj.h"
#include "colortab.h"
#include "context.h"
#include "image.h"
#include "macros.h"
#include "mtypes.h"
#include "pack.h"
#include "pbo.h"
#include "state.h"
#include "teximage.h"
#include "texstate.h"


void GLAPIENTRY
_mesa_ColorTable( GLenum target, GLenum internalFormat,
                  GLsizei width, GLenum format, GLenum type,
                  const GLvoid *data )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glColorTable");
}



void GLAPIENTRY
_mesa_ColorSubTable( GLenum target, GLsizei start,
                     GLsizei count, GLenum format, GLenum type,
                     const GLvoid *data )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glColorSubTable");
}



void GLAPIENTRY
_mesa_CopyColorTable(GLenum target, GLenum internalformat,
                     GLint x, GLint y, GLsizei width)
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glCopyColorTable");
}



void GLAPIENTRY
_mesa_CopyColorSubTable(GLenum target, GLsizei start,
                        GLint x, GLint y, GLsizei width)
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glCopyColorSubTable");
}



void GLAPIENTRY
_mesa_GetnColorTableARB( GLenum target, GLenum format, GLenum type,
                         GLsizei bufSize, GLvoid *data )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetnColorTableARB");
}


void GLAPIENTRY
_mesa_GetColorTable( GLenum target, GLenum format,
                     GLenum type, GLvoid *data )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetColorTable");
}


void GLAPIENTRY
_mesa_ColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    /* no extensions use this function */
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glColorTableParameterfv");
}



void GLAPIENTRY
_mesa_ColorTableParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    /* no extensions use this function */
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glColorTableParameteriv");
}



void GLAPIENTRY
_mesa_GetColorTableParameterfv( GLenum target, GLenum pname, GLfloat *params )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetColorTableParameterfv");
}



void GLAPIENTRY
_mesa_GetColorTableParameteriv( GLenum target, GLenum pname, GLint *params )
{
    GET_CURRENT_CONTEXT(ctx);
    _mesa_error(ctx, GL_INVALID_OPERATION, "glGetColorTableParameteriv");
}

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "glxbyteorder.h"
#include "glxext.h"
#include "singlesize.h"
#include "unpack.h"
#include "indirect_size_get.h"
#include "indirect_dispatch.h"

int
__glXDisp_GetCompressedTexImage(struct __GLXclientStateRec *cl, GLbyte * pc)
{
    xGLXSingleReq *const req = (xGLXSingleReq *) pc;
    int error;
    __GLXcontext *const cx = __glXForceCurrent(cl, req->contextTag, &error);
    ClientPtr client = cl->client;

    REQUEST_FIXED_SIZE(xGLXSingleReq, 8);

    pc += __GLX_SINGLE_HDR_SIZE;
    if (cx != NULL) {
        const GLenum target = *(GLenum *) (pc + 0);
        const GLint level = *(GLint *) (pc + 4);
        GLint compsize = 0;
        char *answer = NULL, answerBuffer[200];
        xGLXSingleReply reply = { 0, };

        glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
                                 &compsize);

        if (compsize != 0) {
            PFNGLGETCOMPRESSEDTEXIMAGEARBPROC GetCompressedTexImageARB =
                    __glGetProcAddress("glGetCompressedTexImageARB");
            __GLX_GET_ANSWER_BUFFER(answer, cl, compsize, 1);
            __glXClearErrorOccured();
            GetCompressedTexImageARB(target, level, answer);
        }

        if (__glXErrorOccured()) {
            __GLX_BEGIN_REPLY(0);
            __GLX_SEND_HEADER();
        }
        else {
            __GLX_BEGIN_REPLY(compsize);
            ((xGLXGetTexImageReply *) &reply)->width = compsize;
            __GLX_SEND_HEADER();
            __GLX_SEND_VOID_ARRAY(compsize);
        }

        error = Success;
    }

    return error;
}

int
__glXDispSwap_GetCompressedTexImage(struct __GLXclientStateRec *cl, GLbyte * pc)
{
    xGLXSingleReq *const req = (xGLXSingleReq *) pc;
    int error;
    __GLXcontext *const cx =
            __glXForceCurrent(cl, bswap_32(req->contextTag), &error);
    ClientPtr client = cl->client;

    REQUEST_FIXED_SIZE(xGLXSingleReq, 8);

    pc += __GLX_SINGLE_HDR_SIZE;
    if (cx != NULL) {
        const GLenum target = (GLenum) bswap_32(*(int *) (pc + 0));
        const GLint level = (GLint) bswap_32(*(int *) (pc + 4));
        GLint compsize = 0;
        char *answer = NULL, answerBuffer[200];
        xGLXSingleReply reply = { 0, };

        glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
                                 &compsize);

        if (compsize != 0) {
            PFNGLGETCOMPRESSEDTEXIMAGEARBPROC GetCompressedTexImageARB =
                    __glGetProcAddress("glGetCompressedTexImageARB");
            __GLX_GET_ANSWER_BUFFER(answer, cl, compsize, 1);
            __glXClearErrorOccured();
            GetCompressedTexImageARB(target, level, answer);
        }

        if (__glXErrorOccured()) {
            __GLX_BEGIN_REPLY(0);
            __GLX_SEND_HEADER();
        }
        else {
            __GLX_BEGIN_REPLY(compsize);
            ((xGLXGetTexImageReply *) &reply)->width = compsize;
            __GLX_SEND_HEADER();
            __GLX_SEND_VOID_ARRAY(compsize);
        }

        error = Success;
    }

    return error;
}

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

/**
 * Handle both types of glGetProgramString calls.
 */
static int
DoGetProgramString(struct __GLXclientStateRec *cl, GLbyte * pc,
                   PFNGLGETPROGRAMIVARBPROC get_programiv,
                   PFNGLGETPROGRAMSTRINGARBPROC get_program_string,
                   Bool do_swap)
{
    xGLXVendorPrivateWithReplyReq *const req =
            (xGLXVendorPrivateWithReplyReq *) pc;
    int error;
    __GLXcontext *const cx = __glXForceCurrent(cl, req->contextTag, &error);
    ClientPtr client = cl->client;

    REQUEST_FIXED_SIZE(xGLXVendorPrivateWithReplyReq, 8);

    pc += __GLX_VENDPRIV_HDR_SIZE;
    if (cx != NULL) {
        GLenum target;
        GLenum pname;
        GLint compsize = 0;
        char *answer = NULL, answerBuffer[200];
        xGLXSingleReply reply = { 0, };

        if (do_swap) {
            target = (GLenum) bswap_32(*(int *) (pc + 0));
            pname = (GLenum) bswap_32(*(int *) (pc + 4));
        }
        else {
            target = *(GLenum *) (pc + 0);
            pname = *(GLuint *) (pc + 4);
        }

        /* The value of the GL_PROGRAM_LENGTH_ARB and GL_PROGRAM_LENGTH_NV
         * enumerants is the same.
         */
        get_programiv(target, GL_PROGRAM_LENGTH_ARB, &compsize);

        if (compsize != 0) {
            __GLX_GET_ANSWER_BUFFER(answer, cl, compsize, 1);
            __glXClearErrorOccured();

            get_program_string(target, pname, (GLubyte *) answer);
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
__glXDisp_GetProgramStringARB(struct __GLXclientStateRec *cl, GLbyte * pc)
{
    PFNGLGETPROGRAMIVARBPROC get_program =
            __glGetProcAddress("glGetProgramivARB");
    PFNGLGETPROGRAMSTRINGARBPROC get_program_string =
            __glGetProcAddress("glGetProgramStringARB");

    return DoGetProgramString(cl, pc, get_program, get_program_string, FALSE);
}

int
__glXDispSwap_GetProgramStringARB(struct __GLXclientStateRec *cl, GLbyte * pc)
{
    PFNGLGETPROGRAMIVARBPROC get_program =
            __glGetProcAddress("glGetProgramivARB");
    PFNGLGETPROGRAMSTRINGARBPROC get_program_string =
            __glGetProcAddress("glGetProgramStringARB");

    return DoGetProgramString(cl, pc, get_program, get_program_string, TRUE);
}

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "glxutil.h"
#include "glxext.h"
#include "singlesize.h"
#include "unpack.h"
#include "indirect_size_get.h"
#include "indirect_dispatch.h"
#include "glxbyteorder.h"

static int DoSwapInterval(__GLXclientState * cl, GLbyte * pc, int do_swap);

int
DoSwapInterval(__GLXclientState * cl, GLbyte * pc, int do_swap)
{
    xGLXVendorPrivateReq *const req = (xGLXVendorPrivateReq *) pc;
    ClientPtr client = cl->client;
    const GLXContextTag tag = req->contextTag;
    __GLXcontext *cx;
    GLint interval;

    REQUEST_FIXED_SIZE(xGLXVendorPrivateReq, 4);

    cx = __glXLookupContextByTag(cl, tag);

    if ((cx == NULL) || (cx->pGlxScreen == NULL)) {
        client->errorValue = tag;
        return __glXError(GLXBadContext);
    }

    if (cx->pGlxScreen->swapInterval == NULL) {
        LogMessage(X_ERROR, "AIGLX: cx->pGlxScreen->swapInterval == NULL\n");
        client->errorValue = tag;
        return __glXError(GLXUnsupportedPrivateRequest);
    }

    if (cx->drawPriv == NULL) {
        client->errorValue = tag;
        return BadValue;
    }

    pc += __GLX_VENDPRIV_HDR_SIZE;
    interval = (do_swap)
               ? bswap_32(*(int *) (pc + 0))
               : *(int *) (pc + 0);

    if (interval <= 0)
        return BadValue;

    (void) (*cx->pGlxScreen->swapInterval) (cx->drawPriv, interval);
    return Success;
}

int
__glXDisp_SwapIntervalSGI(__GLXclientState * cl, GLbyte * pc)
{
    return DoSwapInterval(cl, pc, 0);
}

int
__glXDispSwap_SwapIntervalSGI(__GLXclientState * cl, GLbyte * pc)
{
    return DoSwapInterval(cl, pc, 1);
}

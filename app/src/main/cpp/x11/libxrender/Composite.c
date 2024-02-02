#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

void
XRenderComposite (Display   *dpy,
                  int	    op,
                  Picture   src,
                  Picture   mask,
                  Picture   dst,
                  int	    src_x,
                  int	    src_y,
                  int	    mask_x,
                  int	    mask_y,
                  int	    dst_x,
                  int	    dst_y,
                  unsigned int	width,
                  unsigned int	height)
{
    XRenderExtDisplayInfo         *info = XRenderFindDisplay (dpy);
    xRenderCompositeReq	    *req;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);
    GetReq(RenderComposite, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderComposite;
    req->op = (CARD8) op;
    req->src = src;
    req->mask = mask;
    req->dst = dst;
    req->xSrc = src_x;
    req->ySrc = src_y;
    req->xMask = mask_x;
    req->yMask = mask_y;
    req->xDst = dst_x;
    req->yDst = dst_y;
    req->width = width;
    req->height = height;
    UnlockDisplay(dpy);
    SyncHandle();
}

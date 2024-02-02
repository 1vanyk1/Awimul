#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

void
XRenderCompositeTrapezoids (Display		*dpy,
                            int			op,
                            Picture		src,
                            Picture		dst,
                            _Xconst XRenderPictFormat	*maskFormat,
                            int			xSrc,
                            int			ySrc,
                            _Xconst XTrapezoid	*traps,
                            int			ntrap)
{
    XRenderExtDisplayInfo         *info = XRenderFindDisplay (dpy);
    xRenderTrapezoidsReq    *req;
    int			    n;
    long    		    len;
    unsigned long	    max_req = dpy->bigreq_size ? dpy->bigreq_size : dpy->max_request_size;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);
    while (ntrap)
    {
        GetReq(RenderTrapezoids, req);
        req->reqType = info->codes->major_opcode;
        req->renderReqType = X_RenderTrapezoids;
        req->op = (CARD8) op;
        req->src = src;
        req->dst = dst;
        req->maskFormat = maskFormat ? maskFormat->id : 0;
        req->xSrc = xSrc;
        req->ySrc = ySrc;
        n = ntrap;
        len = ((long) n) * (SIZEOF (xTrapezoid) >> 2);
        if (len > (max_req - req->length)) {
            n = (max_req - req->length) / (SIZEOF (xTrapezoid) >> 2);
            len = ((long)n) * (SIZEOF (xTrapezoid) >> 2);
        }
        SetReqLen (req, len, len);
        len <<= 2;
        DataInt32 (dpy, (int *) traps, len);
        ntrap -= n;
        traps += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}
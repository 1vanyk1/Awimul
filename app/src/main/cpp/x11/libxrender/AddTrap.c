#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

#define NLOCAL	256

void
XRenderAddTraps (Display	    *dpy,
                 Picture	    picture,
                 int		    xOff,
                 int		    yOff,
                 _Xconst XTrap	    *traps,
                 int		    ntrap)
{
    XRenderExtDisplayInfo   *info = XRenderFindDisplay (dpy);
    xRenderAddTrapsReq	    *req;
    int			    n;
    long    		    len;
    unsigned long	    max_req = dpy->bigreq_size ? dpy->bigreq_size : dpy->max_request_size;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);
    while (ntrap)
    {
        GetReq(RenderAddTraps, req);
        req->reqType = info->codes->major_opcode;
        req->renderReqType = X_RenderAddTraps;
        req->picture = picture;
        req->xOff = xOff;
        req->yOff = yOff;
        n = ntrap;
        len = ((long) n) * (SIZEOF (xTrap) >> 2);
        if (len > (max_req - req->length)) {
            n = (max_req - req->length) / (SIZEOF (xTrap) >> 2);
            len = ((long)n) * (SIZEOF (xTrap) >> 2);
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

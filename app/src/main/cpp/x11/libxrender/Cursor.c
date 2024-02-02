#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

Cursor
XRenderCreateCursor (Display	    *dpy,
                     Picture	    source,
                     unsigned int   x,
                     unsigned int   y)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    Cursor			cid;
    xRenderCreateCursorReq	*req;

    RenderCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RenderCreateCursor, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderCreateCursor;
    req->cid = cid = XAllocID (dpy);
    req->src = source;
    req->x = x;
    req->y = y;

    UnlockDisplay(dpy);
    SyncHandle();
    return cid;
}

Cursor
XRenderCreateAnimCursor (Display	*dpy,
                         int		ncursor,
                         XAnimCursor	*cursors)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    Cursor			cid;
    xRenderCreateAnimCursorReq	*req;
    long			len;

    RenderCheckExtension (dpy, info, 0);
    LockDisplay(dpy);
    GetReq(RenderCreateAnimCursor, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderCreateAnimCursor;
    req->cid = cid = XAllocID (dpy);

    len = (long) ncursor * SIZEOF (xAnimCursorElt) >> 2;
    SetReqLen (req, len, len);
    len <<= 2;
    Data32 (dpy, (long *) cursors, len);

    UnlockDisplay(dpy);
    SyncHandle();
    return cid;
}

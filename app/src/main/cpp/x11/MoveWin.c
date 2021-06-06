#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XMoveWindow (
        register Display *dpy,
        Window w,
        int x,
        int y)
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 8, req);

    req->window = w;
    req->mask = CWX | CWY;

    {
        CARD32 *valuePtr = (CARD32 *) NEXTPTR(req,xConfigureWindowReq);
        *valuePtr++ = x;
        *valuePtr = y;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
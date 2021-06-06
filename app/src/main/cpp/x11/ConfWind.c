#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XMoveResizeWindow(
        register Display *dpy,
        Window w,
        int x,
        int y,
        unsigned int width,
        unsigned int height)
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 16, req);
    req->window = w;
    req->mask = CWX | CWY | CWWidth | CWHeight;
    {
        register CARD32 *valuePtr =
                (CARD32 *) NEXTPTR(req,xConfigureWindowReq);
        *valuePtr++ = x;
        *valuePtr++ = y;
        *valuePtr++ = width;
        *valuePtr   = height;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int XResizeWindow(
        register Display *dpy,
        Window w,
        unsigned int width,
        unsigned int height)
{
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 8, req); /* 2 4-byte quantities */

    req->window = w;
    req->mask = CWWidth | CWHeight;
    {
        CARD32 *valuePtr = (CARD32 *) NEXTPTR(req,xConfigureWindowReq);
        *valuePtr++ = width;
        *valuePtr = height;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
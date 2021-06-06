#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSelectInput (
        register Display *dpy,
        Window w,
        long mask)
{
    register xChangeWindowAttributesReq *req;

    LockDisplay(dpy);
    GetReqExtra (ChangeWindowAttributes, 4, req);
    req->window = w;
    req->valueMask = CWEventMask;
    OneDataCard32 (dpy, NEXTPTR(req,xChangeWindowAttributesReq), mask);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
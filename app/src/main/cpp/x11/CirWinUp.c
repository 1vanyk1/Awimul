#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XCirculateSubwindowsUp(
        register Display *dpy,
        Window w)
{
    register xCirculateWindowReq *req;

    LockDisplay(dpy);
    GetReq(CirculateWindow, req);
    req->window = w;
    req->direction = RaiseLowest;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
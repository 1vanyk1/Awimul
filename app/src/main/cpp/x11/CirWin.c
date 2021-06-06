#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XCirculateSubwindows(
        register Display *dpy,
        Window w,
        int direction)
{
    register xCirculateWindowReq *req;

    LockDisplay(dpy);
    GetReq(CirculateWindow, req);
    req->window = w;
    req->direction = direction;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
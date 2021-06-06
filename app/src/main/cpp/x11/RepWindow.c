#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XReparentWindow(
        register Display *dpy,
        Window w,
        Window p,
        int x,
        int y)
{
    register xReparentWindowReq *req;

    LockDisplay(dpy);
    GetReq(ReparentWindow, req);
    req->window = w;
    req->parent = p;
    req->x = x;
    req->y = y;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
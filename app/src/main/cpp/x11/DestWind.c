#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDestroyWindow (
        register Display *dpy,
        Window w)
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(DestroyWindow, w, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XMapWindow (
        register Display *dpy,
        Window w)
{
    register xResourceReq *req;
    LockDisplay (dpy);
    GetResReq(MapWindow, w, req);
    UnlockDisplay (dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XUnmapWindow (
        register Display *dpy,
        Window w)
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(UnmapWindow, w, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XBell(
        register Display *dpy,
        int percent)
{
    register xBellReq *req;

    LockDisplay(dpy);
    GetReq(Bell,req);
    req->percent = percent;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
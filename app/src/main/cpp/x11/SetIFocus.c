#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetInputFocus(
        register Display *dpy,
        Window focus,
        int revert_to,
        Time time)
{
    register xSetInputFocusReq *req;

    LockDisplay(dpy);
    GetReq(SetInputFocus, req);
    req->focus = focus;
    req->revertTo = revert_to;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XRaiseWindow (
        register Display *dpy,
        Window w)
{
    register xConfigureWindowReq *req;
    unsigned long val = Above;		/* needed for macro below */

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 4, req);
    req->window = w;
    req->mask = CWStackMode;
    OneDataCard32 (dpy, NEXTPTR(req,xConfigureWindowReq), val);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
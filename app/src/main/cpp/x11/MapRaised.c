#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XMapRaised (
        register Display *dpy,
        Window w)
{
    register xConfigureWindowReq *req;
    register xResourceReq *req2;
    unsigned long val = Above;		/* needed for macro */

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 4, req);
    req->window = w;
    req->mask = CWStackMode;
    OneDataCard32 (dpy, NEXTPTR(req,xConfigureWindowReq), val);
    GetResReq (MapWindow, w, req2);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


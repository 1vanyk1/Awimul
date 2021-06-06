#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XLowerWindow(
        Display *dpy,
        Window w)
{
    register xConfigureWindowReq *req;
    unsigned long val = Below;		/* needed for macro */

    LockDisplay(dpy);
    GetReqExtra(ConfigureWindow, 4, req);
    req->window = w;
    req->mask = CWStackMode;
    OneDataCard32 (dpy, NEXTPTR(req,xConfigureWindowReq), val);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XKillClient(
        register Display *dpy,
        XID resource)
{
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(KillClient, resource, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

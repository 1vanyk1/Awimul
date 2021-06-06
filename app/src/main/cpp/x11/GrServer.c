#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XGrabServer (register Display *dpy)
{
    _X_UNUSED register xReq *req;
    LockDisplay(dpy);
    GetEmptyReq(GrabServer, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
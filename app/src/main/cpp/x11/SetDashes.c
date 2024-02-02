#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetDashes (
        register Display *dpy,
        GC gc,
        int dash_offset,
        _Xconst char *list,
        int n)
{
    register xSetDashesReq *req;

    LockDisplay(dpy);
    GetReq (SetDashes,req);
    req->gc = gc->gid;
    req->dashOffset = gc->values.dash_offset = dash_offset;
    req->nDashes = n;
    req->length += (n+3)>>2;
    gc->dashes = 1;
    gc->dirty &= ~(GCDashList | GCDashOffset);
    Data (dpy, list, (long)n);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XUndefineCursor (
        register Display *dpy,
        Window w)
{
    register xChangeWindowAttributesReq *req;
    unsigned long defcurs = None;

    LockDisplay(dpy);
    GetReqExtra (ChangeWindowAttributes, 4, req);
    req->window = w;
    req->valueMask = CWCursor;
    OneDataCard32 (dpy, NEXTPTR(req,xChangeWindowAttributesReq), defcurs);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


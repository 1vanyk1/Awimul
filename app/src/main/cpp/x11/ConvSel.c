#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XConvertSelection(
        register Display *dpy,
        Atom selection,
        Atom target,
        Atom property,
        Window requestor,
        Time time)
{
    register xConvertSelectionReq *req;

    LockDisplay(dpy);
    GetReq(ConvertSelection, req);
    req->selection = selection;
    req->target = target;
    req->property = property;
    req->requestor = requestor;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
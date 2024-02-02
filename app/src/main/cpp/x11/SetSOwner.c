#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetSelectionOwner(
        register Display *dpy,
        Atom selection,
        Window owner,
        Time time)
{
    register xSetSelectionOwnerReq *req;

    LockDisplay(dpy);
    GetReq(SetSelectionOwner,req);
    req->selection = selection;
    req->window = owner;
    req->time = time;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

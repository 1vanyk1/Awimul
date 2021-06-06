#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Window XGetSelectionOwner(
        register Display *dpy,
        Atom selection)
{
    xGetSelectionOwnerReply rep;
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(GetSelectionOwner, selection, req);

    if (_XReply(dpy, (xReply *)&rep, 0, xTrue) == 0) rep.owner = None;
    UnlockDisplay(dpy);
    SyncHandle();
    return(rep.owner);
}
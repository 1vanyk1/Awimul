#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XGetInputFocus(
        register Display *dpy,
        Window *focus,
        int *revert_to)
{
    xGetInputFocusReply rep;
    _X_UNUSED register xReq *req;
    LockDisplay(dpy);
    GetEmptyReq(GetInputFocus, req);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *focus = rep.focus;
    *revert_to = rep.revertTo;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
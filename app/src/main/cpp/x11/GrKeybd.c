#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
int XGrabKeyboard (
        register Display *dpy,
        Window window,
        Bool ownerEvents,
        int pointerMode,
        int keyboardMode,
        Time time)
{
    xGrabKeyboardReply rep;
    register xGrabKeyboardReq *req;
    register int status;
    LockDisplay(dpy);
    GetReq(GrabKeyboard, req);
    req->grabWindow = window;
    req->ownerEvents = ownerEvents;
    req->pointerMode = pointerMode;
    req->keyboardMode = keyboardMode;
    req->time = time;

    /* if we ever return, suppress the error */
    if (_XReply (dpy, (xReply *) &rep, 0, xTrue) == 0)
        rep.status = GrabSuccess;
    status = rep.status;
    UnlockDisplay(dpy);
    SyncHandle();
    return (status);
}
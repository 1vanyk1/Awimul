#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XChangeSaveSet(
        register Display *dpy,
        Window win,
        int mode)
{
    register xChangeSaveSetReq *req;

    LockDisplay(dpy);
    GetReq(ChangeSaveSet, req);
    req->window = win;
    req->mode = mode;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

int
XAddToSaveSet(
        register Display *dpy,
        Window win)
{
    return XChangeSaveSet(dpy,win,SetModeInsert);
}

int
XRemoveFromSaveSet (
        register Display *dpy,
        Window win)
{
    return XChangeSaveSet(dpy,win,SetModeDelete);
}
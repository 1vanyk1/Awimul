#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XUngrabKeyboard (
        register Display *dpy,
        Time time)
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(UngrabKeyboard, time, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


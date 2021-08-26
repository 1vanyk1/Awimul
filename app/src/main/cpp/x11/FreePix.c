#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XFreePixmap(
        register Display *dpy,
        Pixmap pixmap)
{
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(FreePixmap, pixmap, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


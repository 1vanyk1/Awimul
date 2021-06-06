#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XUnloadFont(
        register Display *dpy,
        Font font)
{
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(CloseFont, font, req);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
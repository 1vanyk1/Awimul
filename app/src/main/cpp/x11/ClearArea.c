#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XClearArea (
        register Display *dpy,
        Window w,
        int x,
        int y,
        unsigned int width,
        unsigned int height,
        Bool exposures)
{
    register xClearAreaReq *req;

    LockDisplay(dpy);
    GetReq(ClearArea, req);
    req->window = w;
    req->x = x;
    req->y = y;
    req->width = width;
    req->height = height;
    req->exposures = exposures;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


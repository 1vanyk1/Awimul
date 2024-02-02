#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XClearWindow(
        register Display *dpy,
        Window w)
{
    register xClearAreaReq *req;

    LockDisplay(dpy);
    GetReq(ClearArea, req);
    req->window = w;
    req->x = req->y = req->width = req->height = 0;
    /* these values mean "clear the entire window" */
    req->exposures = xFalse;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

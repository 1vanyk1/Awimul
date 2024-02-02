#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDeleteProperty(
        register Display *dpy,
        Window window,
        Atom property)
{
    register xDeletePropertyReq *req;

    LockDisplay(dpy);
    GetReq(DeleteProperty, req);
    req->window = window;
    req->property = property;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

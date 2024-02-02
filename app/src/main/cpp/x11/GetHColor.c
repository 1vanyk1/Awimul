#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Status XAllocColor(
        register Display *dpy,
        Colormap cmap,
        XColor *def)
{
    Status status;
    xAllocColorReply rep;
    register xAllocColorReq *req;
    LockDisplay(dpy);
    GetReq(AllocColor, req);

    req->cmap = cmap;
    req->red = def->red;
    req->green = def->green;
    req->blue = def->blue;

    status = _XReply(dpy, (xReply *) &rep, 0, xTrue);
    if (status) {
        def->pixel = rep.pixel;
        def->red = rep.red;
        def->green = rep.green;
        def->blue = rep.blue;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}

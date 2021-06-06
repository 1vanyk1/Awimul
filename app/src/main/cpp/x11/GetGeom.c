#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Status XGetGeometry (
        register Display *dpy,
        Drawable d,
        Window *root, /* RETURN */
        int *x,
        int *y,  /* RETURN */
        unsigned int *width,
        unsigned int *height,
        unsigned int *borderWidth,
        unsigned int *depth)  /* RETURN */
{
    xGetGeometryReply rep;
    register xResourceReq *req;
    LockDisplay(dpy);
    GetResReq(GetGeometry, d, req);
    if (!_XReply (dpy, (xReply *)&rep, 0, xTrue)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (0);
    }
    *root = rep.root;
    *x = cvtINT16toInt (rep.x);
    *y = cvtINT16toInt (rep.y);
    *width = rep.width;
    *height = rep.height;
    *borderWidth = rep.borderWidth;
    *depth = rep.depth;
    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XFillPolygon(
        register Display *dpy,
        Drawable d,
        GC gc,
        XPoint *points,
        int n_points,
        int shape,
        int mode)
{
    register xFillPolyReq *req;
    register long nbytes;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq(FillPoly, req);

    req->drawable = d;
    req->gc = gc->gid;
    req->shape = shape;
    req->coordMode = mode;

    SetReqLen(req, n_points, 65535 - req->length);

    /* shift (mult. by 4) before passing to the (possible) macro */

    nbytes = n_points << 2;

    Data16 (dpy, (short *) points, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

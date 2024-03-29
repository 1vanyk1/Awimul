#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDrawPoints(
        register Display *dpy,
        Drawable d,
        GC gc,
        XPoint *points,
        int n_points,
        int mode) /* CoordMode */
{
    register xPolyPointReq *req;
    register long nbytes;
    int n;
    int xoff, yoff;
    XPoint pt;

    xoff = yoff = 0;
    LockDisplay(dpy);
    FlushGC(dpy, gc);
    while (n_points) {
        GetReq(PolyPoint, req);
        req->drawable = d;
        req->gc = gc->gid;
        req->coordMode = mode;
        n = n_points;
        if (!dpy->bigreq_size && n > (dpy->max_request_size - req->length))
            n = dpy->max_request_size - req->length;
        SetReqLen(req, n, n);
        nbytes = ((long)n) << 2; /* watch out for macros... */
        if (xoff || yoff) {
            pt.x = xoff + points->x;
            pt.y = yoff + points->y;
            Data16 (dpy, (short *) &pt, 4);
            if (nbytes > 4) {
                Data16 (dpy, (short *) (points + 1), nbytes - 4);
            }
        } else {
            Data16 (dpy, (short *) points, nbytes);
        }
        n_points -= n;
        if (n_points && (mode == CoordModePrevious)) {
            register XPoint *pptr = points;
            points += n;
            while (pptr != points) {
                xoff += pptr->x;
                yoff += pptr->y;
                pptr++;
            }
        } else
            points += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

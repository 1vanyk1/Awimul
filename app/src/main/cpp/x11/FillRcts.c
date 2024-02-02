#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XFillRectangles(
        register Display *dpy,
        Drawable d,
        GC gc,
        XRectangle *rectangles,
        int n_rects)
{
    register xPolyFillRectangleReq *req;
    long len;
    int n;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    while (n_rects) {
        GetReq(PolyFillRectangle, req);
        req->drawable = d;
        req->gc = gc->gid;
        n = n_rects;
        len = ((long)n) << 1;
        if (!dpy->bigreq_size && len > (dpy->max_request_size - req->length)) {
            n = (dpy->max_request_size - req->length) >> 1;
            len = ((long)n) << 1;
        }
        SetReqLen(req, len, len);
        len <<= 2; /* watch out for macros... */
        Data16 (dpy, (short *) rectangles, len);
        n_rects -= n;
        rectangles += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


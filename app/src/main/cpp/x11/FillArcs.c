#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#define arc_scale (SIZEOF(xArc) / 4)

int
XFillArcs(
        register Display *dpy,
        Drawable d,
        GC gc,
        XArc *arcs,
        int n_arcs)
{
    register xPolyFillArcReq *req;
    long len;
    int n;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    while (n_arcs) {
        GetReq(PolyFillArc, req);
        req->drawable = d;
        req->gc = gc->gid;
        n = n_arcs;
        len = ((long)n) * arc_scale;
        if (!dpy->bigreq_size && len > (dpy->max_request_size - req->length)) {
            n = (dpy->max_request_size - req->length) / arc_scale;
            len = ((long)n) * arc_scale;
        }
        SetReqLen(req, len, len);
        len <<= 2; /* watch out for macros... */
        Data16 (dpy, (short *) arcs, len);
        n_arcs -= n;
        arcs += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

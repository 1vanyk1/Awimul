#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#define arc_scale (SIZEOF(xArc) / 4)

int
XDrawArcs(
        register Display *dpy,
        Drawable d,
        GC gc,
        XArc *arcs,
        int n_arcs)
{
    register xPolyArcReq *req;
    register long len;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq(PolyArc,req);
    req->drawable = d;
    req->gc = gc->gid;
    len = ((long)n_arcs) * arc_scale;
    SetReqLen(req, len, 1);
    len <<= 2; /* watch out for macros... */
    Data16 (dpy, (short *) arcs, len);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

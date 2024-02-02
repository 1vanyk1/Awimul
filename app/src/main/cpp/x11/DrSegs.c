#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDrawSegments (
        register Display *dpy,
        Drawable d,
        GC gc,
        XSegment *segments,
        int nsegments)
{
    register xPolySegmentReq *req;
    long len;
    int n;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    while (nsegments) {
        GetReq (PolySegment, req);
        req->drawable = d;
        req->gc = gc->gid;
        n = nsegments;
        len = ((long)n) << 1;
        if (!dpy->bigreq_size && len > (dpy->max_request_size - req->length)) {
            n = (dpy->max_request_size - req->length) >> 1;
            len = ((long)n) << 1;
        }
        SetReqLen(req, len, len);
        len <<= 2; /* watch out for macros... */
        Data16 (dpy, (short *) segments, len);
        nsegments -= n;
        segments += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


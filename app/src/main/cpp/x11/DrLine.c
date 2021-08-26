#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* precompute the maximum size of batching request allowed */

#define wsize (SIZEOF(xPolySegmentReq) + WLNSPERBATCH * SIZEOF(xSegment))
#define zsize (SIZEOF(xPolySegmentReq) + ZLNSPERBATCH * SIZEOF(xSegment))

int
XDrawLine (
        register Display *dpy,
        Drawable d,
        GC gc,
        int x1,
        int y1,
        int x2,
        int y2)
{
    register xSegment *segment;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
        register xPolySegmentReq *req = (xPolySegmentReq *) dpy->last_req;

        /* if same as previous request, with same drawable, batch requests */
        if (
                (req->reqType == X_PolySegment)
                && (req->drawable == d)
                && (req->gc == gc->gid)
                && ((dpy->bufptr + SIZEOF(xSegment)) <= dpy->bufmax)
                && (((char *)dpy->bufptr - (char *)req) < (gc->values.line_width ?
                                                           wsize : zsize)) ) {
            req->length += SIZEOF(xSegment) >> 2;
            segment = (xSegment *) dpy->bufptr;
            dpy->bufptr += SIZEOF(xSegment);
        }

        else {
            GetReqExtra (PolySegment, SIZEOF(xSegment), req);
            req->drawable = d;
            req->gc = gc->gid;
            segment = (xSegment *) NEXTPTR(req,xPolySegmentReq);
        }

        segment->x1 = x1;
        segment->y1 = y1;
        segment->x2 = x2;
        segment->y2 = y2;

        UnlockDisplay(dpy);
        SyncHandle();
    }
    return 1;
}


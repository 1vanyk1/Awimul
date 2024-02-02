#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* precompute the maximum size of batching request allowed */

#define size (SIZEOF(xPolyFillArcReq) + FARCSPERBATCH * SIZEOF(xArc))

int
XFillArc(
        register Display *dpy,
        Drawable d,
        GC gc,
        int x,
        int y, /* INT16 */
        unsigned int width,
        unsigned int height, /* CARD16 */
        int angle1,
        int angle2) /* INT16 */
{
    xArc *arc;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
        register xPolyFillArcReq *req = (xPolyFillArcReq *) dpy->last_req;

        /* if same as previous request, with same drawable, batch requests */
        if (
                (req->reqType == X_PolyFillArc)
                && (req->drawable == d)
                && (req->gc == gc->gid)
                && ((dpy->bufptr + SIZEOF(xArc)) <= dpy->bufmax)
                && (((char *)dpy->bufptr - (char *)req) < size) ) {
            req->length += SIZEOF(xArc) >> 2;
            arc = (xArc *) dpy->bufptr;
            dpy->bufptr += SIZEOF(xArc);
        }

        else {
            GetReqExtra(PolyFillArc, SIZEOF(xArc), req);

            req->drawable = d;
            req->gc = gc->gid;
            arc = (xArc *) NEXTPTR(req,xPolyFillArcReq);
        }
        arc->x = x;
        arc->y = y;
        arc->width = width;
        arc->height = height;
        arc->angle1 = angle1;
        arc->angle2 = angle2;

    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* precompute the maximum size of batching request allowed */

#define size (SIZEOF(xPolyPointReq) + PTSPERBATCH * SIZEOF(xPoint))

int
XDrawPoint(
        register Display *dpy,
        Drawable d,
        GC gc,
        int x,
        int y) /* INT16 */
{
    xPoint *point;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
        register xPolyPointReq *req = (xPolyPointReq *) dpy->last_req;


        /* if same as previous request, with same drawable, batch requests */
        if (
                (req->reqType == X_PolyPoint)
                && (req->drawable == d)
                && (req->gc == gc->gid)
                && (req->coordMode == CoordModeOrigin)
                && ((dpy->bufptr + SIZEOF(xPoint)) <= dpy->bufmax)
                && (((char *)dpy->bufptr - (char *)req) < size) ) {
            req->length += SIZEOF(xPoint) >> 2;
            point = (xPoint *) dpy->bufptr;
            dpy->bufptr += SIZEOF(xPoint);
        }

        else {
            GetReqExtra(PolyPoint, 4, req); /* 1 point = 4 bytes */
            req->drawable = d;
            req->gc = gc->gid;
            req->coordMode = CoordModeOrigin;
            point = (xPoint *) NEXTPTR(req,xPolyPointReq);
        }

        point->x = x;
        point->y = y;

    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


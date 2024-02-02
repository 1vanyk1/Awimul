#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* precompute the maximum size of batching request allowed */

#define wsize (SIZEOF(xPolyRectangleReq) + WRCTSPERBATCH * SIZEOF(xRectangle))
#define zsize (SIZEOF(xPolyRectangleReq) + ZRCTSPERBATCH * SIZEOF(xRectangle))

int
XDrawRectangle(
        register Display *dpy,
        Drawable d,
        GC gc,
        int x,
        int y, /* INT16 */
        unsigned int width,
        unsigned int height) /* CARD16 */
{
    xRectangle *rect;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    {
        register xPolyRectangleReq *req = (xPolyRectangleReq *) dpy->last_req;

        /* if same as previous request, with same drawable, batch requests */
        if (
                (req->reqType == X_PolyRectangle)
                && (req->drawable == d)
                && (req->gc == gc->gid)
                && ((dpy->bufptr + SIZEOF(xRectangle)) <= dpy->bufmax)
                && (((char *)dpy->bufptr - (char *)req) < (gc->values.line_width ?
                                                           wsize : zsize)) ) {
            req->length += SIZEOF(xRectangle) >> 2;
            rect = (xRectangle *) dpy->bufptr;
            dpy->bufptr += SIZEOF(xRectangle);
        }

        else {
            GetReqExtra(PolyRectangle, SIZEOF(xRectangle), req);
            req->drawable = d;
            req->gc = gc->gid;
            rect = (xRectangle *) NEXTPTR(req,xPolyRectangleReq);
        }

        rect->x = x;
        rect->y = y;
        rect->width = width;
        rect->height = height;

    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

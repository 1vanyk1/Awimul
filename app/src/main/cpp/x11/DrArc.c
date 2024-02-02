#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDrawArc(
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
    register xPolyArcReq *req;
    register xArc *arc;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReqExtra (PolyArc, SIZEOF(xArc), req);

    req->drawable = d;
    req->gc = gc->gid;

    arc = (xArc *) NEXTPTR(req,xPolyArcReq);

    arc->x = x;
    arc->y = y;
    arc->width = width;
    arc->height = height;
    arc->angle1 = angle1;
    arc->angle2 = angle2;


    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

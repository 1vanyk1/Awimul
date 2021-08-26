#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XCopyArea(
        register Display *dpy,
        Drawable src_drawable,
        Drawable dst_drawable,
        GC gc,
        int src_x,
        int src_y,
        unsigned int width,
        unsigned int height,
        int dst_x,
        int dst_y)

{
    register xCopyAreaReq *req;

    LockDisplay(dpy);
    FlushGC(dpy, gc);
    GetReq(CopyArea, req);
    req->srcDrawable = src_drawable;
    req->dstDrawable = dst_drawable;
    req->gc = gc->gid;
    req->srcX = src_x;
    req->srcY = src_y;
    req->dstX = dst_x;
    req->dstY = dst_y;
    req->width = width;
    req->height = height;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


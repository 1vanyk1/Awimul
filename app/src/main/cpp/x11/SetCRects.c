#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* can only call when display is locked. */
void _XSetClipRectangles (
        Display *dpy,
        GC gc,
        int clip_x_origin, int clip_y_origin,
        XRectangle *rectangles,
        int n,
        int ordering)
{
    register xSetClipRectanglesReq *req;
    register long len;
    unsigned long dirty;
    register _XExtension *ext;

    GetReq (SetClipRectangles, req);
    req->gc = gc->gid;
    req->xOrigin = gc->values.clip_x_origin = clip_x_origin;
    req->yOrigin = gc->values.clip_y_origin = clip_y_origin;
    req->ordering = ordering;
    len = ((long)n) << 1;
    SetReqLen(req, len, 1);
    len <<= 2;
    Data16 (dpy, (short *) rectangles, len);
    gc->rects = 1;
    dirty = gc->dirty & ~(GCClipMask | GCClipXOrigin | GCClipYOrigin);
    gc->dirty = GCClipMask | GCClipXOrigin | GCClipYOrigin;
    /* call out to any extensions interested */
    for (ext = dpy->ext_procs; ext; ext = ext->next)
        if (ext->flush_GC) (*ext->flush_GC)(dpy, gc, &ext->codes);
    gc->dirty = dirty;
}

int
XSetClipRectangles (
        register Display *dpy,
        GC gc,
        int clip_x_origin,
        int clip_y_origin,
        XRectangle *rectangles,
        int n,
        int ordering)
{
    LockDisplay(dpy);
    _XSetClipRectangles (dpy, gc, clip_x_origin, clip_y_origin, rectangles, n,
                         ordering);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


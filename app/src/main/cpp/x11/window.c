#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

void _XProcessWindowAttributes (
        Display *dpy,
        xChangeWindowAttributesReq *req,
        unsigned long valuemask,
        XSetWindowAttributes *attributes)
{
    unsigned long values[32];
    register unsigned long *value = values;
    unsigned int nvalues;

    if (valuemask & CWBackPixmap)
        *value++ = attributes->background_pixmap;

    if (valuemask & CWBackPixel)
        *value++ = attributes->background_pixel;

    if (valuemask & CWBorderPixmap)
        *value++ = attributes->border_pixmap;

    if (valuemask & CWBorderPixel)
        *value++ = attributes->border_pixel;

    if (valuemask & CWBitGravity)
        *value++ = attributes->bit_gravity;

    if (valuemask & CWWinGravity)
        *value++ = attributes->win_gravity;

    if (valuemask & CWBackingStore)
        *value++ = attributes->backing_store;

    if (valuemask & CWBackingPlanes)
        *value++ = attributes->backing_planes;

    if (valuemask & CWBackingPixel)
        *value++ = attributes->backing_pixel;

    if (valuemask & CWOverrideRedirect)
        *value++ = attributes->override_redirect;

    if (valuemask & CWSaveUnder)
        *value++ = attributes->save_under;

    if (valuemask & CWEventMask)
        *value++ = attributes->event_mask;

    if (valuemask & CWDontPropagate)
        *value++ = attributes->do_not_propagate_mask;

    if (valuemask & CWColormap)
        *value++ = attributes->colormap;

    if (valuemask & CWCursor)
        *value++ = attributes->cursor;

    req->length += (nvalues = value - values);

    nvalues <<= 2;			    /* watch out for macros... */
    Data32 (dpy, (long *) values, (long)nvalues);

}

#define AllMaskBits (CWBackPixmap|CWBackPixel|CWBorderPixmap|\
		     CWBorderPixel|CWBitGravity|CWWinGravity|\
		     CWBackingStore|CWBackingPlanes|CWBackingPixel|\
		     CWOverrideRedirect|CWSaveUnder|CWEventMask|\
		     CWDontPropagate|CWColormap|CWCursor)

Window XCreateWindow(
        register Display *dpy,
        Window parent,
        int x,
        int y,
        unsigned int width,
        unsigned int height,
        unsigned int borderWidth,
        int depth,
        unsigned int class,
        Visual *visual,
        unsigned long valuemask,
        XSetWindowAttributes *attributes)
{
    Window wid;
    register xCreateWindowReq *req;

    LockDisplay(dpy);
    GetReq(CreateWindow, req);
    req->parent = parent;
    req->x = x;
    req->y = y;
    req->width = width;
    req->height = height;
    req->borderWidth = borderWidth;
    req->depth = depth;
    req->class = class;
    if (visual == CopyFromParent)
        req->visual = CopyFromParent;
    else
        req->visual = visual->visualid;
    wid = req->wid = XAllocID(dpy);
    valuemask &= AllMaskBits;
    if ((req->mask = valuemask))
        _XProcessWindowAttributes (dpy, (xChangeWindowAttributesReq *)req,
                                   valuemask, attributes);
    UnlockDisplay(dpy);
    SyncHandle();
    return (wid);
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../x11/headers/xlibint.h"
#include "../x11/XomGeneric.h"

#define GET_VALUE_MASK	(GCFunction | GCForeground | GCBackground | GCFillStyle)
#define SET_VALUE_MASK	(GCFunction | GCForeground | GCFillStyle)

static void
_XomGenericDrawImageString(
        Display *dpy,
        Drawable d,
        XOC oc,
        GC gc,
        int x, int y,
        XOMTextType type,
        XPointer text,
        int length)
{
    XGCValues values;
    XRectangle extent;

    XGetGCValues(dpy, gc, GET_VALUE_MASK, &values);

    XSetFunction(dpy, gc, GXcopy);
    XSetForeground(dpy, gc, values.background);
    XSetFillStyle(dpy, gc, FillSolid);

    _XomGenericTextExtents(oc, type, text, length, 0, &extent);
    XFillRectangle(dpy, d, gc, x + extent.x, y + extent.y, extent.width,
                   extent.height);

    XChangeGC(dpy, gc, SET_VALUE_MASK, &values);

    _XomGenericDrawString(dpy, d, oc, gc, x, y, type, text, length);
}

void
_XmbGenericDrawImageString(Display *dpy, Drawable d, XOC oc, GC gc, int x,
                           int y, _Xconst char *text, int length)
{
    _XomGenericDrawImageString(dpy, d, oc, gc, x, y, XOMMultiByte,
                               (XPointer) text, length);
}

void
_XwcGenericDrawImageString(Display *dpy, Drawable d, XOC oc, GC gc, int x,
                           int y, _Xconst wchar_t *text, int length)
{
_XomGenericDrawImageString(dpy, d, oc, gc, x, y, XOMWideChar,
(XPointer) text, length);
}

void
_Xutf8GenericDrawImageString(Display *dpy, Drawable d, XOC oc, GC gc, int x,
                             int y, _Xconst char *text, int length)
{
    _XomGenericDrawImageString(dpy, d, oc, gc, x, y, XOMUtf8String,
                               (XPointer) text, length);
}
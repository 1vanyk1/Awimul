#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"
#include "headers/xutil.h"
#include "headers/xos.h"
#include <stdio.h>

void XSetTextProperty (
        Display *dpy,
        Window w,
        XTextProperty *tp,
        Atom property)
{
    XChangeProperty (dpy, w, property, tp->encoding, tp->format,
                     PropModeReplace, tp->value, tp->nitems);
}

void XSetWMName (
        Display *dpy,
        Window w,
        XTextProperty *tp)
{
    XSetTextProperty (dpy, w, tp, XA_WM_NAME);
}

void XSetWMIconName (
        Display *dpy,
        Window w,
        XTextProperty *tp)
{
    XSetTextProperty (dpy, w, tp, XA_WM_ICON_NAME);
}

void XSetWMClientMachine (
        Display *dpy,
        Window w,
        XTextProperty *tp)
{
    XSetTextProperty (dpy, w, tp, XA_WM_CLIENT_MACHINE);
}


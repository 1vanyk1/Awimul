#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetArcMode (
        register Display *dpy,
        register GC gc,
        int arc_mode)
{
    LockDisplay(dpy);
    if (gc->values.arc_mode != arc_mode) {
        gc->values.arc_mode = arc_mode;
        gc->dirty |= GCArcMode;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

int
XSetFillRule (
        register Display *dpy,
        register GC gc,
        int fill_rule)
{
    LockDisplay(dpy);
    if (gc->values.fill_rule != fill_rule) {
        gc->values.fill_rule = fill_rule;
        gc->dirty |= GCFillRule;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

int
XSetFillStyle (
        register Display *dpy,
        register GC gc,
        int fill_style)
{
    LockDisplay(dpy);
    if (gc->values.fill_style != fill_style) {
        gc->values.fill_style = fill_style;
        gc->dirty |= GCFillStyle;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

int
XSetGraphicsExposures (
        register Display *dpy,
        register GC gc,
        Bool graphics_exposures)
{
    LockDisplay(dpy);
    if (gc->values.graphics_exposures != graphics_exposures) {
        gc->values.graphics_exposures = graphics_exposures;
        gc->dirty |= GCGraphicsExposures;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

int
XSetSubwindowMode (
        register Display *dpy,
        register GC gc,
        int subwindow_mode)
{
    LockDisplay(dpy);
    if (gc->values.subwindow_mode != subwindow_mode) {
        gc->values.subwindow_mode = subwindow_mode;
        gc->dirty |= GCSubwindowMode;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
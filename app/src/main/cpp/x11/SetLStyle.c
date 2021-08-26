#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetLineAttributes(
        register Display *dpy,
        GC gc,
        unsigned int linewidth, /* CARD16 */
        int linestyle,
        int capstyle,
        int joinstyle)
{
    XGCValues *gv = &gc->values;

    LockDisplay(dpy);
    if (linewidth != gv->line_width) {
        gv->line_width = linewidth;
        gc->dirty |= GCLineWidth;
    }
    if (linestyle != gv->line_style) {
        gv->line_style = linestyle;
        gc->dirty |= GCLineStyle;
    }
    if (capstyle != gv->cap_style) {
        gv->cap_style = capstyle;
        gc->dirty |= GCCapStyle;
    }
    if (joinstyle != gv->join_style) {
        gv->join_style = joinstyle;
        gc->dirty |= GCJoinStyle;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

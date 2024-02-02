#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetTSOrigin (
        register Display *dpy,
        GC gc,
        int x,
        int y)
{
    XGCValues *gv = &gc->values;

    LockDisplay(dpy);
    if (x != gv->ts_x_origin) {
        gv->ts_x_origin = x;
        gc->dirty |= GCTileStipXOrigin;
    }
    if (y != gv->ts_y_origin) {
        gv->ts_y_origin = y;
        gc->dirty |= GCTileStipYOrigin;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

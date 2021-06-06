#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "Cr.h"

int
XChangeGC (
        register Display *dpy,
        GC gc,
        unsigned long valuemask,
        XGCValues *values)
{
    LockDisplay(dpy);

    valuemask &= (1L << (GCLastBit + 1)) - 1;
    if (valuemask) _XUpdateGCCache (gc, valuemask, values);

    /* if any Resource ID changed, must flush */
    if (gc->dirty & (GCFont | GCTile | GCStipple))
        _XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
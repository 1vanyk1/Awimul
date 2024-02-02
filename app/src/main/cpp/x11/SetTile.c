#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetTile (
        register Display *dpy,
        GC gc,
        Pixmap tile)
{
    LockDisplay(dpy);
    /* always update, since client may have changed pixmap contents */
    gc->values.tile = tile;
    gc->dirty |= GCTile;
    _XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

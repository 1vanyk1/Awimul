#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetClipMask (
        register Display *dpy,
        GC gc,
        Pixmap mask)
{
    LockDisplay(dpy);
    /* always update, since client may have changed pixmap contents */
    gc->values.clip_mask = mask;
    gc->dirty |= GCClipMask;
    gc->rects = 0;
    _XFlushGCCache(dpy, gc);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

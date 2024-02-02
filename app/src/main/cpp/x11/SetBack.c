#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetBackground (
        register Display *dpy,
        GC gc,
        unsigned long background) /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.background != background) {
        gc->values.background = background;
        gc->dirty |= GCBackground;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

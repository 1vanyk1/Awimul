#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetPlaneMask (
        register Display *dpy,
        GC gc,
        unsigned long planemask) /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.plane_mask != planemask) {
        gc->values.plane_mask = planemask;
        gc->dirty |= GCPlaneMask;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

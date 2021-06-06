#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetForeground (
        register Display *dpy,
        GC gc,
        unsigned long foreground) /* CARD32 */
{
    LockDisplay(dpy);
    if (gc->values.foreground != foreground) {
        gc->values.foreground = foreground;
        gc->dirty |= GCForeground;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XSetFunction (
        register Display *dpy,
        GC gc,
        int function)
{
    LockDisplay(dpy);
    if (gc->values.function != function) {
        gc->values.function = function;
        gc->dirty |= GCFunction;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
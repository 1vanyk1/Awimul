#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* Flush all buffered output requests. */
/* NOTE: NOT necessary when calling any of the Xlib routines. */

int
XFlush (register Display *dpy)
{
    LockDisplay(dpy);
    _XFlush (dpy);
    UnlockDisplay(dpy);
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* Read in pending events if needed and return the number of queued events. */

int XEventsQueued (
        register Display *dpy,
        int mode)
{
    int ret_val;
    LockDisplay(dpy);
    if (dpy->qlen || (mode == QueuedAlready))
        ret_val = dpy->qlen;
    else
        ret_val = _XEventsQueued (dpy, mode);
    UnlockDisplay(dpy);
    return ret_val;
}

int XPending (register Display *dpy)
{
    int ret_val;
    LockDisplay(dpy);
    if (dpy->qlen)
        ret_val = dpy->qlen;
    else
        ret_val = _XEventsQueued (dpy, QueuedAfterFlush);
    UnlockDisplay(dpy);
    return ret_val;
}

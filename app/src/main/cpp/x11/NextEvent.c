#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * Return next event in queue, or if none, flush output and wait for
 * events.
 */

int
XNextEvent (
        register Display *dpy,
        register XEvent *event)
{
    register _XQEvent *qelt;

    LockDisplay(dpy);

    /* Delete unclaimed cookies */
    _XFreeEventCookies(dpy);

    if (dpy->head == NULL)
        _XReadEvents(dpy);
    qelt = dpy->head;
    *event = qelt->event;
    _XDeq(dpy, NULL, qelt);
    _XStoreEventCookie(dpy, event);
    UnlockDisplay(dpy);
    return 0;
}
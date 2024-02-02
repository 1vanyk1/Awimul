#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * Return the next event in the queue,
 * BUT do not remove it from the queue.
 * If none found, flush and wait until there is an event to peek.
 */

int
XPeekEvent (
        register Display *dpy,
        register XEvent *event)
{
    XEvent copy;
    LockDisplay(dpy);
    if (dpy->head == NULL)
        _XReadEvents(dpy);
    *event = (dpy->head)->event;
    if (_XCopyEventCookie(dpy, &event->xcookie, &copy.xcookie)) {
        _XStoreEventCookie(dpy, &copy);
        *event = copy;
    }
    UnlockDisplay(dpy);
    return 1;
}


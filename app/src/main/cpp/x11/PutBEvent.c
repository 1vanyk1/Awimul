#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
_XPutBackEvent (
        Display *dpy,
        XEvent *event)
{
    register _XQEvent *qelt;
    XEvent store = *event;

    if (!dpy->qfree) {
        if ((dpy->qfree = Xmalloc (sizeof (_XQEvent))) == NULL) {
            return 0;
        }
        dpy->qfree->next = NULL;
    }

    /* unclaimed cookie? */
    if (_XIsEventCookie(dpy, event))
    {
        XEvent copy = {0};
        /* if not claimed, then just fetch and store again */
        if (!event->xcookie.data) {
            _XFetchEventCookie(dpy, &event->xcookie);
            store = *event;
        } else { /* if claimed, copy, client must free */
            _XCopyEventCookie(dpy, &event->xcookie, &copy.xcookie);
            store = copy;
        }
    }

    qelt = dpy->qfree;
    dpy->qfree = qelt->next;
    qelt->qserial_num = dpy->next_event_serial_num++;
    qelt->next = dpy->head;
    qelt->event = store;
    dpy->head = qelt;
    if (dpy->tail == NULL)
        dpy->tail = qelt;
    dpy->qlen++;
    return 0;
}

int
XPutBackEvent (
        register Display * dpy,
        register XEvent *event)
{
    int ret;

    LockDisplay(dpy);
    ret = _XPutBackEvent(dpy, event);
    UnlockDisplay(dpy);
    return ret;
}
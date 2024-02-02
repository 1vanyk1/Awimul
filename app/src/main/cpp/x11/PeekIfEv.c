#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * return the next event in the queue that satisfies the predicate.
 * BUT do not remove it from the queue.
 * If none found, flush, and then wait until one satisfies the predicate.
 */

int
XPeekIfEvent (
        register Display *dpy,
        register XEvent *event,
        Bool (*predicate)(
                Display*			/* display */,
                XEvent*			/* event */,
                char*				/* arg */
        ),
        char *arg)
{
    register _XQEvent *prev, *qelt;
    unsigned long qe_serial = 0;

    LockDisplay(dpy);
    prev = NULL;
    while (1) {
        for (qelt = prev ? prev->next : dpy->head;
             qelt;
             prev = qelt, qelt = qelt->next) {
            if(qelt->qserial_num > qe_serial
               && (*predicate)(dpy, &qelt->event, arg)) {
                XEvent copy;
                *event = qelt->event;
                if (_XCopyEventCookie(dpy, &event->xcookie, &copy.xcookie)) {
                    _XStoreEventCookie(dpy, &copy);
                    *event = copy;
                }
                UnlockDisplay(dpy);
                return 0;
            }
        }
        if (prev)
            qe_serial = prev->qserial_num;
        _XReadEvents(dpy);
        if (prev && prev->qserial_num != qe_serial)
            /* another thread has snatched this event */
            prev = NULL;
    }
}


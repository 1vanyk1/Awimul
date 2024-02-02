#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * Check existing events in queue to find if any match.  If so, return.
 * If not, flush buffer and see if any more events are readable. If one
 * matches, return.  If all else fails, tell the user no events found.
 */

Bool XCheckIfEvent (
        register Display *dpy,
        register XEvent *event,		/* XEvent to be filled in. */
        Bool (*predicate)(
                Display*			/* display */,
                XEvent*			/* event */,
                char*				/* arg */
        ),		/* function to call */
        char *arg)
{
    register _XQEvent *prev, *qelt;
    unsigned long qe_serial = 0;
    int n;			/* time through count */

    LockDisplay(dpy);
    prev = NULL;
    for (n = 3; --n >= 0;) {
        for (qelt = prev ? prev->next : dpy->head;
             qelt;
             prev = qelt, qelt = qelt->next) {
            if(qelt->qserial_num > qe_serial
               && (*predicate)(dpy, &qelt->event, arg)) {
                *event = qelt->event;
                _XDeq(dpy, prev, qelt);
                _XStoreEventCookie(dpy, event);
                UnlockDisplay(dpy);
                return True;
            }
        }
        if (prev)
            qe_serial = prev->qserial_num;
        switch (n) {
            case 2:
                _XEventsQueued(dpy, QueuedAfterReading);
                break;
            case 1:
                _XFlush(dpy);
                break;
        }
        if (prev && prev->qserial_num != qe_serial)
            /* another thread has snatched this event */
            prev = NULL;
    }
    UnlockDisplay(dpy);
    return False;
}

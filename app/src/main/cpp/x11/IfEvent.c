#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * Flush output and (wait for and) return the next event matching the
 * predicate in the queue.
 */

int
XIfEvent (
        register Display *dpy,
        register XEvent *event,
        Bool (*predicate)(
                Display*			/* display */,
                XEvent*			/* event */,
                char*				/* arg */
        ),		/* function to call */
        char *arg)
{
    register _XQEvent *qelt, *prev;
    unsigned long qe_serial = 0;

    LockDisplay(dpy);
    prev = NULL;
    while (1) {
        for (qelt = prev ? prev->next : dpy->head;
             qelt;
             prev = qelt, qelt = qelt->next) {
            if(qelt->qserial_num > qe_serial
               && (*predicate)(dpy, &qelt->event, arg)) {
                *event = qelt->event;
                _XDeq(dpy, prev, qelt);
                _XStoreEventCookie(dpy, event);
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
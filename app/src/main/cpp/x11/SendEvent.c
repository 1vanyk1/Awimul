#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * In order to avoid all images requiring _XEventToWire, we install the
 * event converter here if it has never been installed.
 */
Status
XSendEvent(
        register Display *dpy,
        Window w,
        Bool propagate,
        long event_mask,
        XEvent *event)
{
    register xSendEventReq *req;
    xEvent ev;
    register Status (**fp)(
            Display *       /* dpy */,
            XEvent *        /* re */,
            xEvent *        /* event */);
    Status status;

    /* initialize all of the event's fields first, before setting
     * the meaningful ones later.
     */
    memset (&ev, 0, sizeof (ev));

    LockDisplay (dpy);

    /* call through display to find proper conversion routine */

    fp = &dpy->wire_vec[event->type & 0177];
    if (*fp == NULL) *fp = _XEventToWire;
    status = (**fp)(dpy, event, &ev);

    if (status) {
        GetReq(SendEvent, req);
        req->destination = w;
        req->propagate = propagate;
        req->eventMask = event_mask;
        req->event = ev;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}
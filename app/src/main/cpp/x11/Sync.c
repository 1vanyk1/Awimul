#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/* Synchronize with errors and events, optionally discarding pending events */

int
XSync (
        register Display *dpy,
        Bool discard)
{
    xGetInputFocusReply rep;
    _X_UNUSED register xReq *req;

    LockDisplay(dpy);
    GetEmptyReq(GetInputFocus, req);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);

    if (discard && dpy->head) {
        _XQEvent *qelt;

        for (qelt=dpy->head; qelt; qelt=qelt->next)
            qelt->qserial_num = 0;

        ((_XQEvent *)dpy->tail)->next = dpy->qfree;
        dpy->qfree = (_XQEvent *)dpy->head;
        dpy->head = dpy->tail = NULL;
        dpy->qlen = 0;
    }
    UnlockDisplay(dpy);
    return 1;
}
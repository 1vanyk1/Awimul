#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#ifdef XTHREADS
#include "locking.h"
#endif

void
XLockDisplay(
        register Display* dpy)
{
#ifdef XTHREADS
    LockDisplay(dpy);
    if (dpy->lock)
	(*dpy->lock->user_lock_display)(dpy);
    /*
     * We want the threads in the reply queue to all get out before
     * XLockDisplay returns, in case they have any side effects the
     * caller of XLockDisplay was trying to protect against.
     * XLockDisplay puts itself at the head of the event waiters queue
     * to wait for all the replies to come in.
     */
    if (dpy->lock && dpy->lock->reply_awaiters) {
	struct _XCVList *cvl;

	cvl = (*dpy->lock->create_cvl)(dpy);

	/* stuff ourselves on the head of the queue */
	cvl->next = dpy->lock->event_awaiters;
	dpy->lock->event_awaiters = cvl;

	while (dpy->lock->reply_awaiters)
	    ConditionWait(dpy, cvl->cv);
	UnlockNextEventReader(dpy); /* pass the signal on */
    }
    UnlockDisplay(dpy);
#endif
}

void
XUnlockDisplay(
        register Display* dpy)
{
#ifdef XTHREADS
    LockDisplay(dpy);
    if (dpy->lock)
	(*dpy->lock->user_unlock_display)(dpy);
    UnlockDisplay(dpy);
#endif
}
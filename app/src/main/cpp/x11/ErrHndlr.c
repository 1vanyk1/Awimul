#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

/*
 * XErrorHandler - This procedure sets the X non-fatal error handler
 * (_XErrorFunction) to be the specified routine.  If NULL is passed in
 * the original error handler is restored.
 */

XErrorHandler
XSetErrorHandler(XErrorHandler handler)
{
    int (*oldhandler)(Display *dpy, XErrorEvent *event);

    _XLockMutex(_Xglobal_lock);
    oldhandler = _XErrorFunction;

    if (!oldhandler)
        oldhandler = _XDefaultError;

    if (handler != NULL) {
        _XErrorFunction = handler;
    }
    else {
        _XErrorFunction = _XDefaultError;
    }
    _XUnlockMutex(_Xglobal_lock);

    return (XErrorHandler) oldhandler;
}

/*
 * XIOErrorHandler - This procedure sets the X fatal I/O error handler
 * (_XIOErrorFunction) to be the specified routine.  If NULL is passed in
 * the original error handler is restored.
 */

XIOErrorHandler
XSetIOErrorHandler(XIOErrorHandler handler)
{
    int (*oldhandler)(Display *dpy);

    _XLockMutex(_Xglobal_lock);
    oldhandler = _XIOErrorFunction;

    if (!oldhandler)
        oldhandler = _XDefaultIOError;

    if (handler != NULL) {
        _XIOErrorFunction = handler;
    }
    else {
        _XIOErrorFunction = _XDefaultIOError;
    }
    _XUnlockMutex(_Xglobal_lock);

    return (XIOErrorHandler) oldhandler;
}

/*
 * XSetIOErrorExitHandler - This procedure sets the X fatal I/O error
 * exit function to be the specified routine. If NULL is passed in
 * the original error exit function is restored. The default routine
 * calls exit(3).
 */
void
XSetIOErrorExitHandler(
        Display *dpy,
        XIOErrorExitHandler handler,
        void *user_data)
{
    LockDisplay(dpy);

    if (handler != NULL) {
        dpy->exit_handler = handler;
        dpy->exit_handler_data = user_data;
    }
    else {
        dpy->exit_handler = _XDefaultIOErrorExit;
        dpy->exit_handler_data = NULL;
    }
    UnlockDisplay(dpy);
}
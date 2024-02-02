#ifndef _XtThreadsI_h
#define _XtThreadsI_h

#include "../headers/xlibconf.h"
#include "IntrinsicI.h"

#ifdef XTHREADS

typedef struct _LockRec *LockPtr;

typedef void (*ThreadAppProc)(
    XtAppContext /* app */
);

typedef void (*ThreadAppYieldLockProc)(
    XtAppContext, /* app */
    Boolean*, /* push_thread */
    Boolean*, /* pushed_thread */
    int* /* level */
);

typedef void (*ThreadAppRestoreLockProc)(
    XtAppContext /* app */,
    int, /* level */
    Boolean* /* pushed_thread */
);

_XFUNCPROTOBEGIN

extern void (*_XtProcessLock)(
    void
);

extern void (*_XtProcessUnlock)(
    void
);

extern void (*_XtInitAppLock)(
    XtAppContext /* app */
);

_XFUNCPROTOEND

#define INIT_APP_LOCK(app) if(_XtInitAppLock) (*_XtInitAppLock)(app)
#define FREE_APP_LOCK(app) if(app && app->free_lock)(*app->free_lock)(app)

#define LOCK_PROCESS if(_XtProcessLock)(*_XtProcessLock)()
#define UNLOCK_PROCESS if(_XtProcessUnlock)(*_XtProcessUnlock)()
#define LOCK_APP(app) if(app && app->lock)(*app->lock)(app)
#define UNLOCK_APP(app) if(app && app->unlock)(*app->unlock)(app)

#define YIELD_APP_LOCK(app,push,pushed,level)\
	 if(app && app->yield_lock) (*app->yield_lock)(app,push,pushed,level)
#define RESTORE_APP_LOCK(app,level,pushed)\
	 if(app && app->restore_lock) (*app->restore_lock)(app,level,pushed)

#define WIDGET_TO_APPCON(w) \
    XtAppContext app = (w && _XtProcessLock ? \
	XtWidgetToApplicationContext(w) : NULL)

#define DPY_TO_APPCON(d) \
    XtAppContext app = (_XtProcessLock ? XtDisplayToApplicationContext(d): NULL)

#else /* defined(XTHREADS) */

#define LOCK_PROCESS
#define UNLOCK_PROCESS
#define LOCK_APP(app)
#define UNLOCK_APP(app)

#define INIT_APP_LOCK(app)
#define FREE_APP_LOCK(app)

#define WIDGET_TO_APPCON(w)
#define DPY_TO_APPCON(d)

#endif /* !defined(XTHREADS) */
#endif /* _XtThreadsI_h */
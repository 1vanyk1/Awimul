#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"

#ifdef XTHREADS

#define xmalloc __XtMalloc
#define xfree XtFree
#include "../headers/xthreads.h"

#ifndef NDEBUG
#define NDEBUG
#endif
#include <assert.h>
#include <stdio.h>

typedef struct _ThreadStack {
    unsigned int size;
    int sp;
    struct _Tstack {
        xthread_t t;
        xcondition_t c;
    } *st;
} ThreadStack;

typedef struct _LockRec {
    xmutex_t mutex;
    int level;
    ThreadStack stack;
#ifndef _XMUTEX_NESTS
    xthread_t holder;
    xcondition_t cond;
#endif
} LockRec;

#define STACK_INCR 16

static LockPtr process_lock = NULL;

static void
InitProcessLock(void)
{
    if (!process_lock) {
        process_lock = XtNew(LockRec);
        process_lock->mutex = xmutex_malloc();
        xmutex_init(process_lock->mutex);
        process_lock->level = 0;
#ifndef _XMUTEX_NESTS
        process_lock->cond = xcondition_malloc();
        xcondition_init(process_lock->cond);
        xthread_clear_id(process_lock->holder);
#endif
    }
}

static void
ProcessLock(void)
{
#ifdef _XMUTEX_NESTS
    xmutex_lock(process_lock->mutex);
    process_lock->level++;
#else
    xthread_t this_thread = xthread_self();

    xmutex_lock(process_lock->mutex);

    if (!xthread_have_id(process_lock->holder)) {
        process_lock->holder = this_thread;
        xmutex_unlock(process_lock->mutex);
        return;
    }

    if (xthread_equal(process_lock->holder, this_thread)) {
        process_lock->level++;
        xmutex_unlock(process_lock->mutex);
        return;
    }

    while (xthread_have_id(process_lock->holder))
        xcondition_wait(process_lock->cond, process_lock->mutex);

    process_lock->holder = this_thread;
    assert(xthread_equal(process_lock->holder, this_thread));
    xmutex_unlock(process_lock->mutex);
#endif
}

static void
ProcessUnlock(void)
{
#ifdef _XMUTEX_NESTS
    process_lock->level--;
    xmutex_unlock(process_lock->mutex);
#else
    xmutex_lock(process_lock->mutex);
    assert(xthread_equal(process_lock->holder, xthread_self()));
    if (process_lock->level != 0) {
        process_lock->level--;
        xmutex_unlock(process_lock->mutex);
        return;
    }

    xthread_clear_id(process_lock->holder);
    xcondition_signal(process_lock->cond);

    xmutex_unlock(process_lock->mutex);
#endif
}

static void
AppLock(XtAppContext app)
{
    LockPtr app_lock = app->lock_info;

#ifdef _XMUTEX_NESTS
    xmutex_lock(app_lock->mutex);
    app_lock->level++;
#else
    xthread_t self = xthread_self();

    xmutex_lock(app_lock->mutex);
    if (!xthread_have_id(app_lock->holder)) {
        app_lock->holder = self;
        assert(xthread_equal(app_lock->holder, self));
        xmutex_unlock(app_lock->mutex);
        return;
    }
    if (xthread_equal(app_lock->holder, self)) {
        app_lock->level++;
        xmutex_unlock(app_lock->mutex);
        return;
    }
    while (xthread_have_id(app_lock->holder)) {
        xcondition_wait(app_lock->cond, app_lock->mutex);
    }
    app_lock->holder = self;
    assert(xthread_equal(app_lock->holder, self));
    xmutex_unlock(app_lock->mutex);
#endif
}

static void
AppUnlock(XtAppContext app)
{
    LockPtr app_lock = app->lock_info;

#ifdef _XMUTEX_NESTS
    app_lock->level--;
    xmutex_unlock(app_lock->mutex);
#else
    xthread_t self;

    self = xthread_self();
    (void) self;

    xmutex_lock(app_lock->mutex);
    assert(xthread_equal(app_lock->holder, self));
    if (app_lock->level != 0) {
        app_lock->level--;
        xmutex_unlock(app_lock->mutex);
        return;
    }
    xthread_clear_id(app_lock->holder);
    xcondition_signal(app_lock->cond);
    xmutex_unlock(app_lock->mutex);
#endif
}

static void
YieldAppLock(XtAppContext app,
             Boolean *push_thread,
             Boolean *pushed_thread,
             int *level)
{
    LockPtr app_lock = app->lock_info;
    xthread_t self = xthread_self();

#ifndef _XMUTEX_NESTS
    xmutex_lock(app_lock->mutex);
    assert(xthread_equal(app_lock->holder, self));
#endif
    *level = app_lock->level;
    if (*push_thread) {
        *push_thread = FALSE;
        *pushed_thread = TRUE;

        if (app_lock->stack.sp == (int) app_lock->stack.size - 1) {
            unsigned ii;

            app_lock->stack.st = (struct _Tstack *)
                    XtRealloc((char *) app_lock->stack.st,
                              (Cardinal) ((app_lock->stack.size +
                                           STACK_INCR) * sizeof(struct _Tstack)));
            ii = app_lock->stack.size;
            app_lock->stack.size += STACK_INCR;
            for (; ii < app_lock->stack.size; ii++) {
                app_lock->stack.st[ii].c = xcondition_malloc();
                xcondition_init(app_lock->stack.st[ii].c);
            }
        }
        app_lock->stack.st[++(app_lock->stack.sp)].t = self;
    }
#ifdef _XMUTEX_NESTS
    while (app_lock->level > 0) {
        app_lock->level--;
        xmutex_unlock(app_lock->mutex);
    }
#else
    xcondition_signal(app_lock->cond);
    app_lock->level = 0;
    xthread_clear_id(app_lock->holder);
    xmutex_unlock(app_lock->mutex);
#endif
}

static void
RestoreAppLock(XtAppContext app, int level, Boolean *pushed_thread)
{
    LockPtr app_lock = app->lock_info;
    xthread_t self = xthread_self();

    xmutex_lock(app_lock->mutex);
#ifdef _XMUTEX_NESTS
    app_lock->level++;
#else
    while (xthread_have_id(app_lock->holder)) {
        xcondition_wait(app_lock->cond, app_lock->mutex);
    }
#endif
    if (!xthread_equal(app_lock->stack.st[app_lock->stack.sp].t, self)) {
        int ii;

        for (ii = app_lock->stack.sp - 1; ii >= 0; ii--) {
            if (xthread_equal(app_lock->stack.st[ii].t, self)) {
                xcondition_wait(app_lock->stack.st[ii].c, app_lock->mutex);
                break;
            }
        }
#ifndef _XMUTEX_NESTS
        while (xthread_have_id(app_lock->holder)) {
            xcondition_wait(app_lock->cond, app_lock->mutex);
        }
#endif
    }
#ifdef _XMUTEX_NESTS
    while (app_lock->level < level) {
        xmutex_lock(app_lock->mutex);
        app_lock->level++;
    }
#else
    app_lock->holder = self;
    app_lock->level = level;
    assert(xthread_equal(app_lock->holder, self));
#endif
    if (*pushed_thread) {
        *pushed_thread = FALSE;
        (app_lock->stack.sp)--;
        if (app_lock->stack.sp >= 0) {
            xcondition_signal(app_lock->stack.st[app_lock->stack.sp].c);
        }
    }
#ifndef _XMUTEX_NESTS
    xmutex_unlock(app_lock->mutex);
#endif
}

static void
FreeAppLock(XtAppContext app)
{
    unsigned ii;
    LockPtr app_lock = app->lock_info;

    if (app_lock) {
        xmutex_clear(app_lock->mutex);
        xmutex_free(app_lock->mutex);
#ifndef _XMUTEX_NESTS
        xcondition_clear(app_lock->cond);
        xcondition_free(app_lock->cond);
#endif
        if (app_lock->stack.st != (struct _Tstack *) NULL) {
            for (ii = 0; ii < app_lock->stack.size; ii++) {
                xcondition_clear(app_lock->stack.st[ii].c);
                xcondition_free(app_lock->stack.st[ii].c);
            }
            XtFree((char *) app_lock->stack.st);
        }
        XtFree((char *) app_lock);
        app->lock_info = NULL;
    }
}

static void
InitAppLock(XtAppContext app)
{
    int ii;
    LockPtr app_lock;

    app->lock = AppLock;
    app->unlock = AppUnlock;
    app->yield_lock = YieldAppLock;
    app->restore_lock = RestoreAppLock;
    app->free_lock = FreeAppLock;

    app_lock = app->lock_info = XtNew(LockRec);
    app_lock->mutex = xmutex_malloc();
    xmutex_init(app_lock->mutex);
    app_lock->level = 0;
#ifndef _XMUTEX_NESTS
    app_lock->cond = xcondition_malloc();
    xcondition_init(app_lock->cond);
    xthread_clear_id(app_lock->holder);
#endif
    app_lock->stack.size = STACK_INCR;
    app_lock->stack.sp = -1;
    app_lock->stack.st =
            (struct _Tstack *) __XtMalloc(sizeof(struct _Tstack) * STACK_INCR);
    for (ii = 0; ii < STACK_INCR; ii++) {
        app_lock->stack.st[ii].c = xcondition_malloc();
        xcondition_init(app_lock->stack.st[ii].c);
    }
}

#endif                          /* defined(XTHREADS) */

void
XtAppLock(XtAppContext app)
{
#ifdef XTHREADS
    if (app->lock)
        (*app->lock) (app);
#endif
}

void
XtAppUnlock(XtAppContext app)
{
#ifdef XTHREADS
    if (app->unlock)
        (*app->unlock) (app);
#endif
}

void
XtProcessLock(void)
{
#ifdef XTHREADS
    if (_XtProcessLock)
        (*_XtProcessLock) ();
#endif
}

void
XtProcessUnlock(void)
{
#ifdef XTHREADS
    if (_XtProcessUnlock)
        (*_XtProcessUnlock) ();
#endif
}

Boolean
XtToolkitThreadInitialize(void)
{
#ifdef XTHREADS
    if (_XtProcessLock == NULL) {
#ifdef xthread_init
        xthread_init();
#endif
        InitProcessLock();
        _XtProcessLock = ProcessLock;
        _XtProcessUnlock = ProcessUnlock;
        _XtInitAppLock = InitAppLock;
    }
    return True;
#else
    return False;
#endif
}

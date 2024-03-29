#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "../headers/xlib.h"
#include <stdlib.h>
#include "DisplayQue.h"

/*
 * Prototypes
 */
static int _DQCloseDisplay(Display*, XPointer);

#define CallCloseCallback(q,e) (void) (*((q)->closefunc)) ((q), (e))
#define CallFreeCallback(q) (void) (*((q)->freefunc)) ((q))

/*
 * XmuDQCreate - create a display queue
 */
XmuDisplayQueue *
XmuDQCreate(XmuCloseDisplayQueueProc closefunc,
            XmuFreeDisplayQueueProc freefunc,
            XPointer data)
{
    XmuDisplayQueue *q = (XmuDisplayQueue *) malloc (sizeof (XmuDisplayQueue));
    if (q) {
        q->nentries = 0;
        q->head = q->tail = NULL;
        q->closefunc = closefunc;
        q->freefunc = freefunc;
        q->data = data;
    }
    return q;
}


/*
 * XmuDQDestroy - free all storage associated with this display queue,
 * optionally invoking the close callbacks.
 */

Bool
XmuDQDestroy(XmuDisplayQueue *q, Bool docallbacks)
{
    XmuDisplayQueueEntry *e = q->head;

    while (e) {
        XmuDisplayQueueEntry *nexte = e->next;
        if (docallbacks && q->closefunc) CallCloseCallback (q, e);
        free ((char *) e);
        e = nexte;
    }
    free ((char *) q);
    return True;
}


/*
 * XmuDQLookupDisplay - finds the indicated display on the given queue
 */
XmuDisplayQueueEntry *
XmuDQLookupDisplay(XmuDisplayQueue *q, Display *dpy)
{
    XmuDisplayQueueEntry *e;

    for (e = q->head; e; e = e->next) {
        if (e->display == dpy) return e;
    }
    return NULL;
}


/*
 * XmuDQAddDisplay - add the specified display to the queue; set data as a
 * convenience.  Does not ensure that dpy hasn't already been added.
 */
XmuDisplayQueueEntry *
XmuDQAddDisplay(XmuDisplayQueue *q, Display *dpy, XPointer data)
{
    XmuDisplayQueueEntry *e;

    if (!(e = (XmuDisplayQueueEntry *) malloc (sizeof (XmuDisplayQueueEntry)))) {
        return NULL;
    }
    if (!(e->closehook = XmuAddCloseDisplayHook (dpy, _DQCloseDisplay,
                                                 (XPointer) q))) {
        free ((char *) e);
        return NULL;
    }

    e->display = dpy;
    e->next = NULL;
    e->data = data;

    if (q->tail) {
        q->tail->next = e;
        e->prev = q->tail;
    } else {
        q->head = e;
        e->prev = NULL;
    }
    q->tail = e;
    q->nentries++;
    return e;
}


/*
 * XmuDQRemoveDisplay - remove the specified display from the queue
 */
Bool
XmuDQRemoveDisplay(XmuDisplayQueue *q, Display *dpy)
{
    XmuDisplayQueueEntry *e;

    for (e = q->head; e; e = e->next) {
        if (e->display == dpy) {
            if (q->head == e)
                q->head = e->next;	/* if at head, then bump head */
            else
                e->prev->next = e->next;	/* else splice out */
            if (q->tail == e)
                q->tail = e->prev;	/* if at tail, then bump tail */
            else
                e->next->prev = e->prev;	/* else splice out */
            (void) XmuRemoveCloseDisplayHook (dpy, e->closehook,
                                              _DQCloseDisplay, (XPointer) q);
            free ((char *) e);
            q->nentries--;
            return True;
        }
    }
    return False;
}


/*****************************************************************************
 *			       private functions                             *
 *****************************************************************************/

/*
 * _DQCloseDisplay - upcalled from CloseHook to notify this queue; remove the
 * display when finished
 */
static int
_DQCloseDisplay(Display *dpy, XPointer arg)
{
    XmuDisplayQueue *q = (XmuDisplayQueue *) arg;
    XmuDisplayQueueEntry *e;

    for (e = q->head; e; e = e->next) {
        if (e->display == dpy) {
            if (q->closefunc) CallCloseCallback (q, e);
            (void) XmuDQRemoveDisplay (q, dpy);
            if (q->nentries == 0 && q->freefunc) CallFreeCallback (q);
            return 1;
        }
    }

    return 0;
}

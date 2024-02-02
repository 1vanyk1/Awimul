#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "../headers/xlib.h"
#include "../headers/xos.h"
#include "CvtCache.h"
#include <stdlib.h>

/*
 * Prototypes
 */
static int _CloseDisplay(XmuDisplayQueue*, XmuDisplayQueueEntry*);
static int _FreeCCDQ(XmuDisplayQueue*);
static void _InitializeCvtCache(XmuCvtCache*);

/*
 * Initialization
 */
static XmuDisplayQueue *dq = NULL;


/*
 * internal utility callbacks
 */

static int
_FreeCCDQ(XmuDisplayQueue *q)
{
    XmuDQDestroy (dq, False);
    dq = NULL;
    return (0);
}


static int
_CloseDisplay(XmuDisplayQueue *q, XmuDisplayQueueEntry *e)
{
    XmuCvtCache *c;

    if (e && (c = (XmuCvtCache *)(e->data))) {
        _XmuStringToBitmapFreeCache (c);
        /* insert calls to free any cached memory */

    }
    return 0;
}

static void
_InitializeCvtCache(register XmuCvtCache *c)
{
    _XmuStringToBitmapInitCache (c);
    /* insert calls to init any cached memory */
}


/*
 * XmuCCLookupDisplay - return the cache entry for the indicated display;
 * initialize the cache if necessary
 */
XmuCvtCache *
_XmuCCLookupDisplay(Display *dpy)
{
    XmuDisplayQueueEntry *e;

    /*
     * If no displays have been added before this, create the display queue.
     */
    if (!dq) {
        dq = XmuDQCreate (_CloseDisplay, _FreeCCDQ, NULL);
        if (!dq) return NULL;
    }

    /*
     * See if the display is already there
     */
    e = XmuDQLookupDisplay (dq, dpy);	/* see if it's there */
    if (!e) {				/* else create it */
        XmuCvtCache *c = (XmuCvtCache *) malloc (sizeof (XmuCvtCache));
        if (!c) return NULL;

        /*
         * Add the display to the queue
         */
        e = XmuDQAddDisplay (dq, dpy, (XPointer) c);
        if (!e) {
            free ((char *) c);
            return NULL;
        }

        /*
         * initialize fields in cache
         */
        _InitializeCvtCache (c);
    }

    /*
     * got it
     */
    return (XmuCvtCache *)(e->data);
}



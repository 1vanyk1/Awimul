#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "reallocarray.h"
#include <stdio.h>

/*
 * XListDepths - return info from connection setup
 */
int *XListDepths (
        Display *dpy,
        int scrnum,
        int *countp)
{
    Screen *scr;
    int count;
    int *depths;

    if (scrnum < 0 || scrnum >= dpy->nscreens) return NULL;

    scr = &dpy->screens[scrnum];
    if ((count = scr->ndepths) > 0) {
        register Depth *dp;
        register int i;

        depths = Xmallocarray (count, sizeof(int));
        if (!depths) return NULL;
        for (i = 0, dp = scr->depths; i < count; i++, dp++)
            depths[i] = dp->depth;
    } else {
        /* a screen must have a depth */
        return NULL;
    }
    *countp = count;
    return depths;
}

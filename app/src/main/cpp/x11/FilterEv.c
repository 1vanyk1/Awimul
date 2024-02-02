#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#define XLOCALE 1
#endif
#include "headers/xlibint.h"
#if XLOCALE
#include "xlc/xlcint.h"
#endif

extern long const _Xevent_to_mask[];

/*
 * Look up if there is a specified filter for the event.
 */
Bool
XFilterEvent(
        XEvent *ev,
        Window window)
{
#if XLOCALE
    XFilterEventList	p;
    Window		win;
    long		mask;
    Bool		ret;

    if (window)
        win = window;
    else
        win = ev->xany.window;
    if (ev->type >= LASTEvent)
        mask = 0;
    else
        mask = _Xevent_to_mask[ev->type];

    LockDisplay(ev->xany.display);
    for (p = ev->xany.display->im_filters; p != NULL; p = p->next) {
        if (win == p->window) {
            if ((mask & p->event_mask) ||
                (ev->type >= p->start_type && ev->type <= p->end_type)) {
                UnlockDisplay(ev->xany.display);
                ret = (*(p->filter))(ev->xany.display, p->window, ev,
                                     p->client_data);
                return(ret);
            }
        }
    }
    UnlockDisplay(ev->xany.display);
#endif
    return(False);
}

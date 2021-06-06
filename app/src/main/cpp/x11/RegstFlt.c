#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xlc/xlcint.h"

static void
_XFreeIMFilters(
        Display *display)
{
    register XFilterEventList fl;

    while ((fl = display->im_filters)) {
        display->im_filters = fl->next;
        Xfree(fl);
    }
    display->im_filters = NULL;
}

/*
 * Register a filter with the filter machinery by event mask.
 */
void
_XRegisterFilterByMask(
        Display *display,
        Window window,
        unsigned long event_mask,
        Bool (*filter)(
                Display*, Window, XEvent*, XPointer
        ),
        XPointer client_data)
{
    XFilterEventRec		*rec;

    rec = Xmalloc(sizeof(XFilterEventRec));
    if (!rec)
        return;
    rec->window = window;
    rec->event_mask = event_mask;
    rec->start_type = 0;
    rec->end_type = 0;
    rec->filter = filter;
    rec->client_data = client_data;
    LockDisplay(display);
    rec->next = display->im_filters;
    display->im_filters = rec;
    display->free_funcs->im_filters = _XFreeIMFilters;
    UnlockDisplay(display);
}

/*
 * Register a filter with the filter machinery by type code.
 */
void
_XRegisterFilterByType(
        Display *display,
        Window window,
        int start_type,
        int end_type,
        Bool (*filter)(
                Display*, Window, XEvent*, XPointer
        ),
        XPointer client_data)
{
    XFilterEventRec		*rec;

    rec = Xmalloc(sizeof(XFilterEventRec));
    if (!rec)
        return;
    rec->window = window;
    rec->event_mask = 0;
    rec->start_type = start_type;
    rec->end_type = end_type;
    rec->filter = filter;
    rec->client_data = client_data;
    LockDisplay(display);
    rec->next = display->im_filters;
    display->im_filters = rec;
    display->free_funcs->im_filters = _XFreeIMFilters;
    UnlockDisplay(display);
}

void
_XUnregisterFilter(
        Display *display,
        Window window,
        Bool (*filter)(
                Display*, Window, XEvent*, XPointer
        ),
        XPointer client_data)
{
    register XFilterEventList	*prev, fl;

    for (prev = &display->im_filters; (fl = *prev); ) {
        if (fl->window == window &&
            fl->filter == filter && fl->client_data == client_data) {
            *prev = fl->next;
            Xfree(fl);
        } else
            prev = &fl->next;
    }
}
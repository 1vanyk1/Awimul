#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "IntrinsicI.h"

void
XtSetMultiClickTime(Display *dpy, int time)
{
    DPY_TO_APPCON(dpy);

    LOCK_APP(app);
    _XtGetPerDisplay(dpy)->multi_click_time = time;
    UNLOCK_APP(app);
}

int
XtGetMultiClickTime(Display *dpy)
{
    int retval;

    DPY_TO_APPCON(dpy);

    LOCK_APP(app);
    retval = _XtGetPerDisplay(dpy)->multi_click_time;
    UNLOCK_APP(app);
    return retval;
}

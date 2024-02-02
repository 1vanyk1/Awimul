#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

long XMaxRequestSize(Display *dpy)
{
    return dpy->max_request_size;
}

char *XResourceManagerString(Display *dpy)
{
    return dpy->xdefaults;
}

unsigned long XDisplayMotionBufferSize(Display *dpy)
{
    return dpy->motion_buffer;
}

int
XDisplayKeycodes(
        Display *dpy,
        int *min_keycode_return,
        int *max_keycode_return)
{
    *min_keycode_return = dpy->min_keycode;
    *max_keycode_return = dpy->max_keycode;
    return 1;
}

VisualID XVisualIDFromVisual(Visual *visual)
{
    return visual->visualid;
}

long XExtendedMaxRequestSize(Display *dpy)
{
    return dpy->bigreq_size;
}

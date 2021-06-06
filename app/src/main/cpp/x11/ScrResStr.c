#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"

char *XScreenResourceString(Screen *screen)
{
    Atom prop_name;
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;
    char *val = NULL;

    prop_name = XInternAtom(screen->display, "SCREEN_RESOURCES", True);
    if (prop_name &&
        XGetWindowProperty(screen->display, screen->root, prop_name,
                           0L, 100000000L, False,
                           XA_STRING, &actual_type, &actual_format,
                           &nitems, &leftover,
                           (unsigned char **) &val) == Success) {
        if ((actual_type == XA_STRING) && (actual_format == 8))
            return val;
        Xfree(val);
    }
    return (char *)NULL;
}
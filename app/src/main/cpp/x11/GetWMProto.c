#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"
#include <stdio.h>

Status XGetWMProtocols (
        Display *dpy,
        Window w,
        Atom **protocols,
        int *countReturn)
{
    Atom *data = NULL;
    Atom actual_type;
    Atom prop;
    int actual_format;
    unsigned long leftover, nitems;

    prop =  XInternAtom(dpy, "WM_PROTOCOLS", False);
    if (prop == None) return False;

    /* get the property */
    if (XGetWindowProperty (dpy, w, prop,
                            0L, 1000000L, False,
                            XA_ATOM, &actual_type, &actual_format,
                            &nitems, &leftover, (unsigned char **) &data)
        != Success)
        return False;

    if (actual_type != XA_ATOM || actual_format != 32) {
        Xfree (data);
        return False;
    }

    *protocols = (Atom *) data;
    *countReturn = (int) nitems;
    return True;
}
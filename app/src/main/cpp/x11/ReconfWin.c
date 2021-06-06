#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#define AllMaskBits (CWX|CWY|CWWidth|CWHeight|\
		     CWBorderWidth|CWSibling|CWStackMode)

int
XConfigureWindow(
        register Display *dpy,
        Window w,
        unsigned int mask,
        XWindowChanges *changes)
{
    unsigned long values[7];
    register unsigned long *value = values;
    long nvalues;
    register xConfigureWindowReq *req;

    LockDisplay(dpy);
    GetReq(ConfigureWindow, req);
    req->window = w;
    mask &= AllMaskBits;
    req->mask = mask;

    if (mask & CWX)
        *value++ = changes->x;

    if (mask & CWY)
        *value++ = changes->y;

    if (mask & CWWidth)
        *value++ = changes->width;

    if (mask & CWHeight)
        *value++ = changes->height;

    if (mask & CWBorderWidth)
        *value++ = changes->border_width;

    if (mask & CWSibling)
        *value++ = changes->sibling;

    if (mask & CWStackMode)
        *value++ = changes->stack_mode;

    req->length += (nvalues = value - values);

    nvalues <<= 2;			/* watch out for macros... */
    Data32 (dpy, (long *) values, nvalues);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
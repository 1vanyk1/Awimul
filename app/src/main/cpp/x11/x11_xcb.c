#include "headers/xlibint.h"
#include "Xxcbint.h"

xcb_connection_t *XGetXCBConnection(Display *dpy)
{
    return dpy->xcb->connection;
}

void XSetEventQueueOwner(Display *dpy, enum XEventQueueOwner owner)
{
    dpy->xcb->event_owner = owner;
}
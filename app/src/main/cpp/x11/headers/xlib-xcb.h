#ifndef _X11_XLIB_XCB_H_
#define _X11_XLIB_XCB_H_
#include "/../../xcb/xcb.h"
#include "xlib.h"
#include "xfuncproto.h"
_XFUNCPROTOBEGIN
        xcb_connection_t *XGetXCBConnection(Display *dpy);
enum XEventQueueOwner { XlibOwnsEventQueue = 0, XCBOwnsEventQueue };
void XSetEventQueueOwner(Display *dpy, enum XEventQueueOwner owner);
_XFUNCPROTOEND
#endif /* _X11_XLIB_XCB_H_ */
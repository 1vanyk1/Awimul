#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XGrabKey(
        register Display *dpy,
        int key,
        unsigned int modifiers,
        Window grab_window,
        Bool owner_events,
        int pointer_mode,
        int keyboard_mode)

{
    register xGrabKeyReq *req;
    LockDisplay(dpy);
    GetReq(GrabKey, req);
    req->ownerEvents = owner_events;
    req->grabWindow = grab_window;
    req->modifiers = modifiers;
    req->key = key;
    req->pointerMode = pointer_mode;
    req->keyboardMode = keyboard_mode;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
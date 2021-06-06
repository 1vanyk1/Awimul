#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XGrabButton(
        register Display *dpy,
        unsigned int button, /* CARD8 */
        unsigned int modifiers, /* CARD16 */
        Window grab_window,
        Bool owner_events,
        unsigned int event_mask, /* CARD16 */
        int pointer_mode,
        int keyboard_mode,
        Window confine_to,
        Cursor curs)
{
    register xGrabButtonReq *req;
    LockDisplay(dpy);
    GetReq(GrabButton, req);
    req->modifiers = modifiers;
    req->button = button;
    req->grabWindow = grab_window;
    req->ownerEvents = owner_events;
    req->eventMask = event_mask;
    req->pointerMode = pointer_mode;
    req->keyboardMode = keyboard_mode;
    req->confineTo = confine_to;
    req->cursor = curs;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XUngrabButton(
        register Display *dpy,
        unsigned int button, /* CARD8 */
        unsigned int modifiers, /* CARD16 */
        Window grab_window)
{
    register xUngrabButtonReq *req;

    LockDisplay(dpy);
    GetReq(UngrabButton, req);
    req->button = button;
    req->modifiers = modifiers;
    req->grabWindow = grab_window;
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

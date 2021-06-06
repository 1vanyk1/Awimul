#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
/* returns either  DeviceMappingSuccess or DeviceMappingBusy  */

int
XSetPointerMapping (
        register Display *dpy,
        _Xconst unsigned char *map,
        int nmaps)
{
    register xSetPointerMappingReq *req;
    xSetPointerMappingReply rep;

    LockDisplay(dpy);
    GetReq (SetPointerMapping, req);
    req->nElts = nmaps;
    req->length += (nmaps + 3)>>2;
    Data (dpy, (_Xconst char *)map, (long) nmaps);
    if (_XReply (dpy, (xReply *)&rep, 0, xFalse) == 0)
        rep.success = MappingSuccess;
    UnlockDisplay(dpy);
    SyncHandle();
    return ((int) rep.success);
}

int
XChangeKeyboardMapping (
        register Display *dpy,
        int first_keycode,
        int keysyms_per_keycode,
        KeySym *keysyms,
        int nkeycodes)
{
    register long nbytes;
    register xChangeKeyboardMappingReq *req;

    LockDisplay(dpy);
    GetReq (ChangeKeyboardMapping, req);
    req->firstKeyCode = first_keycode;
    req->keyCodes = nkeycodes;
    req->keySymsPerKeyCode = keysyms_per_keycode;
    req->length += nkeycodes * keysyms_per_keycode;
    nbytes = keysyms_per_keycode * nkeycodes * 4;
    Data32 (dpy, (long *)keysyms, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
    return 0;
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "reallocarray.h"
#include <limits.h>

#ifdef MIN		/* some systems define this in <sys/param.h> */
#undef MIN
#endif
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int XGetPointerMapping (
        register Display *dpy,
        unsigned char *map,	/* RETURN */
        int nmaps)

{
    unsigned char mapping[256];	/* known fixed size */
    unsigned long nbytes, remainder = 0;
    xGetPointerMappingReply rep;
    _X_UNUSED register xReq *req;

    LockDisplay(dpy);
    GetEmptyReq(GetPointerMapping, req);
    if (! _XReply(dpy, (xReply *)&rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return 0;
    }

    /* Don't count on the server returning a valid value */
    if (rep.length >= (INT_MAX >> 2)) {
        _XEatDataWords(dpy, rep.length);
        UnlockDisplay(dpy);
        SyncHandle();
        return 0;
    }

    nbytes = (unsigned long) rep.length << 2;
    if (nbytes > sizeof mapping) {
        remainder = nbytes - sizeof mapping;
        nbytes = sizeof mapping;
    }
    _XRead (dpy, (char *)mapping, nbytes);
    /* don't return more data than the user asked for. */
    if (rep.nElts) {
        memcpy ((char *) map, (char *) mapping,
                MIN((int)rep.nElts, nmaps) );
    }

    if (remainder)
        _XEatData(dpy, remainder);

    UnlockDisplay(dpy);
    SyncHandle();
    return ((int) rep.nElts);
}

KeySym *
XGetKeyboardMapping (Display *dpy,
#if NeedWidePrototypes
                     unsigned int first_keycode,
#else
        KeyCode first_keycode,
#endif
                     int count,
                     int *keysyms_per_keycode)
{
    unsigned long nbytes;
    CARD32 nkeysyms;
    register KeySym *mapping = NULL;
    xGetKeyboardMappingReply rep;
    register xGetKeyboardMappingReq *req;

    LockDisplay(dpy);
    GetReq(GetKeyboardMapping, req);
    req->firstKeyCode = first_keycode;
    req->count = count;
    if (! _XReply(dpy, (xReply *)&rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (KeySym *) NULL;
    }

    nkeysyms = rep.length;
    if (nkeysyms > 0) {
        if (nkeysyms < (INT_MAX / sizeof (KeySym))) {
            mapping = Xmallocarray (nkeysyms, sizeof (KeySym));
        }
        if (! mapping) {
            _XEatDataWords(dpy, rep.length);
            UnlockDisplay(dpy);
            SyncHandle();
            return (KeySym *) NULL;
        }
        nbytes = nkeysyms << 2;
        _XRead32 (dpy, (long *) mapping, nbytes);
    }
    *keysyms_per_keycode = rep.keySymsPerKeyCode;
    UnlockDisplay(dpy);
    SyncHandle();
    return (mapping);
}

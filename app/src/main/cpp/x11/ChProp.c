#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XChangeProperty (
        register Display *dpy,
        Window w,
        Atom property,
        Atom type,
        int format,  /* 8, 16, or 32 */
        int mode,  /* PropModeReplace, PropModePrepend, PropModeAppend */
        _Xconst unsigned char *data,
        int nelements)
{
    register xChangePropertyReq *req;
    register long len;

    LockDisplay(dpy);
    GetReq (ChangeProperty, req);
    req->window = w;
    req->property = property;
    req->type = type;
    req->mode = mode;
    if (nelements < 0) {
        req->nUnits = 0;
        req->format = 0; /* ask for garbage, get garbage */
    } else {
        req->nUnits = nelements;
        req->format = format;
    }

    switch (req->format) {
        case 8:
            len = ((long)nelements + 3)>>2;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                Data (dpy, (_Xconst char *)data, nelements);
            } /* else force BadLength */
            break;

        case 16:
            len = ((long)nelements + 1)>>1;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                len = (long)nelements << 1;
                Data16 (dpy, (_Xconst short *) data, len);
            } /* else force BadLength */
            break;

        case 32:
            len = nelements;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                len = (long)nelements << 2;
                Data32 (dpy, (_Xconst long *) data, len);
            } /* else force BadLength */
            break;

        default:
            /* BadValue will be generated */ ;
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}
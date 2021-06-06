#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include "headers/xlibint.h"

Font
XLoadFont (
        register Display *dpy,
        _Xconst char *name)
{
    register long nbytes;
    Font fid;
    register xOpenFontReq *req;

    if (name != NULL && strlen(name) >= USHRT_MAX)
        return (0);

    if (_XF86LoadQueryLocaleFont(dpy, name, (XFontStruct **)0, &fid))
        return fid;

    LockDisplay(dpy);
    GetReq(OpenFont, req);
    nbytes = req->nbytes = name ? (CARD16) strlen(name) : 0;
    req->fid = fid = XAllocID(dpy);
    req->length += (nbytes+3)>>2;
    Data (dpy, name, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
    return (fid);
    /* can't return (req->fid) since request may have already been sent */
}

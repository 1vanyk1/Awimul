#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Status XAllocColorPlanes(
        register Display *dpy,
        Colormap cmap,
        Bool contig,
        unsigned long *pixels, /* LISTofCARD32 */ /* RETURN */
        int ncolors,
        int nreds,
        int ngreens,
        int nblues,
        unsigned long *rmask,
        unsigned long *gmask,
        unsigned long *bmask) /* CARD32 */ /* RETURN */
{
    xAllocColorPlanesReply rep;
    Status status;
    register xAllocColorPlanesReq *req;

    LockDisplay(dpy);
    GetReq(AllocColorPlanes,req);

    req->cmap = cmap;
    req->colors = ncolors;
    req->red = nreds;
    req->green = ngreens;
    req->blue = nblues;
    req->contiguous = contig;

    status = _XReply(dpy, (xReply *)&rep, 0, xFalse);


    if (status) {
        *rmask = rep.redMask;
        *gmask = rep.greenMask;
        *bmask = rep.blueMask;

        /* sizeof(CARD32) = 4 */
        _XRead32 (dpy, (long *) pixels, (long)(ncolors * 4));
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}

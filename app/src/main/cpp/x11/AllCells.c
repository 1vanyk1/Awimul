#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Status XAllocColorCells(
        register Display *dpy,
        Colormap cmap,
        Bool contig,
        unsigned long *masks, /* LISTofCARD32 */ /* RETURN */
        unsigned int nplanes, /* CARD16 */
        unsigned long *pixels, /* LISTofCARD32 */ /* RETURN */
        unsigned int ncolors) /* CARD16 */
{

    Status status;
    xAllocColorCellsReply rep;
    register xAllocColorCellsReq *req;
    LockDisplay(dpy);
    GetReq(AllocColorCells, req);

    req->cmap = cmap;
    req->colors = ncolors;
    req->planes = nplanes;
    req->contiguous = contig;

    status = _XReply(dpy, (xReply *)&rep, 0, xFalse);

    if (status) {
        if ((rep.nPixels > ncolors) || (rep.nMasks > nplanes)) {
            _XEatDataWords(dpy, rep.length);
            status = 0; /* Failure */
        } else {
            _XRead32 (dpy, (long *) pixels, 4L * (long) (rep.nPixels));
            _XRead32 (dpy, (long *) masks, 4L * (long) (rep.nMasks));
        }
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return(status);
}

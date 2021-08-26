#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XFreeColors(
        register Display *dpy,
        Colormap cmap,
        unsigned long *pixels, /* LISTofCARD32 */
        int npixels,
        unsigned long planes) /* CARD32 */
{
    register xFreeColorsReq *req;
    register long nbytes;

    LockDisplay(dpy);
    GetReq(FreeColors, req);
    req->cmap = cmap;
    req->planeMask = planes;

    /* on the VAX, each pixel is a 32-bit (unsigned) integer */
    req->length += npixels;

    nbytes = npixels << 2;		/* watch out for macros... */
    Data32 (dpy, (long *) pixels, nbytes);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}


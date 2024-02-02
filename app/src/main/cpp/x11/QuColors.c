#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "reallocarray.h"

static void
_XQueryColors(
        register Display *dpy,
        Colormap cmap,
        XColor *defs, 		/* RETURN */
        int ncolors)
{
    register int i;
    xQueryColorsReply rep;
    register xQueryColorsReq *req;

    GetReq(QueryColors, req);

    req->cmap = cmap;
    SetReqLen(req, ncolors, ncolors); /* each pixel is a CARD32 */

    for (i = 0; i < ncolors; i++)
        Data32 (dpy, (long *)&defs[i].pixel, 4L);
    /* XXX this isn't very efficient */

    if (_XReply(dpy, (xReply *) &rep, 0, xFalse) != 0) {
        xrgb *color = Xmallocarray(ncolors, sizeof(xrgb));
        if (color != NULL) {
            unsigned long nbytes = (long) ncolors * SIZEOF(xrgb);

            _XRead(dpy, (char *) color, nbytes);

            for (i = 0; i < ncolors; i++) {
                register XColor *def = &defs[i];
                register xrgb *rgb = &color[i];
                def->red = rgb->red;
                def->green = rgb->green;
                def->blue = rgb->blue;
                def->flags = DoRed | DoGreen | DoBlue;
            }
            Xfree(color);
        }
        else
            _XEatDataWords(dpy, rep.length);
    }
}

int
XQueryColors(
        register Display * const dpy,
        const Colormap cmap,
        XColor *defs, 		/* RETURN */
        int ncolors)
{
    int n;

    if (dpy->bigreq_size > 0)
        n = dpy->bigreq_size - (sizeof (xQueryColorsReq) >> 2) - 1;
    else
        n = dpy->max_request_size - (sizeof (xQueryColorsReq) >> 2);

    LockDisplay(dpy);
    while (ncolors >= n) {
        _XQueryColors(dpy, cmap, defs, n);
        defs += n;
        ncolors -= n;
    }
    if (ncolors > 0)
        _XQueryColors(dpy, cmap, defs, ncolors);
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XQueryColor(
        register Display *dpy,
        Colormap cmap,
        XColor *def)	/* RETURN */
{
    xrgb color;
    xQueryColorsReply rep;
    register xQueryColorsReq *req;
    unsigned long val = def->pixel;	/* needed for macro below */

    LockDisplay(dpy);
    GetReqExtra(QueryColors, 4, req); /* a pixel (CARD32) is 4 bytes */
    req->cmap = cmap;

    OneDataCard32 (dpy, NEXTPTR(req,xQueryColorsReq), val);

    if (_XReply(dpy, (xReply *) &rep, 0, xFalse) != 0) {

        _XRead(dpy, (char *)&color, (long) SIZEOF(xrgb));

        def->red = color.red;
        def->blue = color.blue;
        def->green = color.green;
        def->flags = DoRed | DoGreen | DoBlue;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}

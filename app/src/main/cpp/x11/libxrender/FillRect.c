#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

/* precompute the maximum size of batching request allowed */

#define size (SIZEOF(xRenderFillRectanglesReq) + FRCTSPERBATCH * SIZEOF(xRectangle))

void
XRenderFillRectangle (Display	    *dpy,
                      int	    op,
                      Picture	    dst,
                      _Xconst XRenderColor  *color,
                      int	    x,
                      int	    y,
                      unsigned int  width,
                      unsigned int  height)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    xRectangle			*rect;
    xRenderFillRectanglesReq	*req;
#ifdef MUSTCOPY
    xRectangle			rectdata;
    long			len = SIZEOF(xRectangle);

    rect = &rectdata;
#endif /* MUSTCOPY */

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);

    req = (xRenderFillRectanglesReq *) dpy->last_req;
    /* if same as previous request, with same drawable, batch requests */
    if (req->reqType == info->codes->major_opcode &&
        req->renderReqType == X_RenderFillRectangles &&
        req->op == op &&
        req->dst == dst &&
        req->color.red == color->red &&
        req->color.green == color->green &&
        req->color.blue == color->blue &&
        req->color.alpha == color->alpha &&
        dpy->bufptr + SIZEOF(xRectangle) <= dpy->bufmax &&
        (char *)dpy->bufptr - (char *)req < size)
    {
        req->length += SIZEOF(xRectangle) >> 2;
#ifndef MUSTCOPY
        rect = (xRectangle *) dpy->bufptr;
        dpy->bufptr += SIZEOF(xRectangle);
#endif /* not MUSTCOPY */
    }
    else
    {
        GetReqExtra(RenderFillRectangles, SIZEOF(xRectangle), req);

        req->reqType = info->codes->major_opcode;
        req->renderReqType = X_RenderFillRectangles;
        req->op = op;
        req->dst = dst;
        req->color.red = color->red;
        req->color.green = color->green;
        req->color.blue = color->blue;
        req->color.alpha = color->alpha;

#ifdef MUSTCOPY
        dpy->bufptr -= SIZEOF(xRectangle);
#else
        rect = (xRectangle *) NEXTPTR(req,xRenderFillRectanglesReq);
#endif /* MUSTCOPY */
    }
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;

#ifdef MUSTCOPY
    Data (dpy, (char *) rect, len);
#endif /* MUSTCOPY */
    UnlockDisplay(dpy);
    SyncHandle();
}
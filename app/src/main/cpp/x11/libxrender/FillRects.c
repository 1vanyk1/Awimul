#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xrenderint.h"

/* precompute the maximum size of batching request allowed */

#define size (SIZEOF(xRenderFillRectanglesReq) + FRCTSPERBATCH * SIZEOF(xRectangle))

void
XRenderFillRectangles (Display		    *dpy,
                       int		    op,
                       Picture		    dst,
                       _Xconst XRenderColor *color,
                       _Xconst XRectangle   *rectangles,
                       int		    n_rects)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    xRenderFillRectanglesReq	*req;
    long			len;
    int				n;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);

    while (n_rects)
    {
        GetReq(RenderFillRectangles, req);

        req->reqType = info->codes->major_opcode;
        req->renderReqType = X_RenderFillRectangles;
        req->op = op;
        req->dst = dst;
        req->color.red = color->red;
        req->color.green = color->green;
        req->color.blue = color->blue;
        req->color.alpha = color->alpha;

        n = n_rects;
        len = ((long)n) << 1;
        if (!dpy->bigreq_size && len > (dpy->max_request_size - req->length))
        {
            n = (dpy->max_request_size - req->length) >> 1;
            len = ((long)n) << 1;
        }
        SetReqLen(req, len, len);
        len <<= 2; /* watch out for macros... */
        Data16 (dpy, (short *) rectangles, len);
        n_rects -= n;
        rectangles += n;
    }
    UnlockDisplay(dpy);
    SyncHandle();
}

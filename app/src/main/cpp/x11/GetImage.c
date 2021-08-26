#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xutil.h"		/* for XDestroyImage */
#include "headers/ImUtil.h"
#include <limits.h>

#define ROUNDUP(nbytes, pad) (((((nbytes) - 1) + (pad)) / (pad)) * (pad))

#ifdef HAVE___BUILTIN_POPCOUNTL
# define Ones __builtin_popcountl
#else
/*
 * Count the number of bits set to 1 in a 32-bit word.
 * Algorithm from MIT AI Lab Memo 239: "HAKMEM", ITEM 169.
 * http://dspace.mit.edu/handle/1721.1/6086
 */
static inline unsigned int
Ones(unsigned long mask)
{
    register unsigned long y;

    y = (mask >> 1) & 033333333333;
    y = mask - y - ((y >>1) & 033333333333);
    return ((unsigned int) (((y + (y >> 3)) & 030707070707) % 077));
}
#endif

XImage *XGetImage (
        register Display *dpy,
        Drawable d,
        int x,
        int y,
        unsigned int width,
        unsigned int height,
        unsigned long plane_mask,
        int format)	/* either XYPixmap or ZPixmap */
{
    xGetImageReply rep;
    register xGetImageReq *req;
    char *data;
    unsigned long nbytes;
    XImage *image;
    int planes;
    LockDisplay(dpy);
    GetReq (GetImage, req);
    /*
     * first set up the standard stuff in the request
     */
    req->drawable = d;
    req->x = x;
    req->y = y;
    req->width = width;
    req->height = height;
    req->planeMask = plane_mask;
    req->format = format;

    if (_XReply (dpy, (xReply *) &rep, 0, xFalse) == 0 ||
        rep.length == 0) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (XImage *)NULL;
    }

    if (rep.length < (INT_MAX >> 2)) {
        nbytes = (unsigned long)rep.length << 2;
        data = Xmalloc(nbytes);
    } else
        data = NULL;
    if (! data) {
        _XEatDataWords(dpy, rep.length);
        UnlockDisplay(dpy);
        SyncHandle();
        return (XImage *) NULL;
    }
    _XReadPad (dpy, data, nbytes);
    if (format == XYPixmap) {
        image = XCreateImage(dpy, _XVIDtoVisual(dpy, rep.visual),
                             Ones (plane_mask &
                                   (((unsigned long)0xFFFFFFFF) >> (32 - rep.depth))),
                             format, 0, data, width, height, dpy->bitmap_pad, 0);
        planes = image->depth;
    } else { /* format == ZPixmap */
        image = XCreateImage (dpy, _XVIDtoVisual(dpy, rep.visual),
                              rep.depth, ZPixmap, 0, data, width, height,
                              _XGetScanlinePad(dpy, (int) rep.depth), 0);
        planes = 1;
    }

    if (!image) {
        Xfree(data);
    } else {
        if (planes < 1 || image->height < 1 || image->bytes_per_line < 1 ||
            INT_MAX / image->height <= image->bytes_per_line ||
            INT_MAX / planes <= image->height * image->bytes_per_line ||
            nbytes < planes * image->height * image->bytes_per_line) {
            XDestroyImage(image);
            image = NULL;
        }
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return (image);
}

XImage *XGetSubImage(
        register Display *dpy,
        Drawable d,
        int x,
        int y,
        unsigned int width,
        unsigned int height,
        unsigned long plane_mask,
        int format,	/* either XYPixmap or ZPixmap */
        XImage *dest_image,
        int dest_x,
        int dest_y)
{
    XImage *temp_image;
    temp_image = XGetImage(dpy, d, x, y, width, height,
                           plane_mask, format);
    if (!temp_image)
        return (XImage *)NULL;
    _XSetImage(temp_image, dest_image, dest_x, dest_y);
    XDestroyImage(temp_image);
    return (dest_image);
}

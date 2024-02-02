#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlib.h"

/*
 * XCreateBitmapFromData: Routine to make a pixmap of depth 1 from user
 *	                  supplied data.
 *	D is any drawable on the same screen that the pixmap will be used in.
 *	Data is a pointer to the bit data, and
 *	width & height give the size in bits of the pixmap.
 *
 * The following format is assumed for data:
 *
 *    format=XYPixmap
 *    bit_order=LSBFirst
 *    byte_order=LSBFirst
 *    padding=8
 *    bitmap_unit=8
 *    xoffset=0
 *    no extra bytes per line
 */
Pixmap XCreateBitmapFromData(
        Display *display,
        Drawable d,
        _Xconst char *data,
        unsigned int width,
        unsigned int height)
{
    Pixmap pix = XCreatePixmap(display, d, width, height, 1);
    GC gc = XCreateGC(display, pix, (unsigned long) 0, (XGCValues *) 0);
    if (gc == NULL) {
        XFreePixmap(display, pix);
        return (Pixmap) None;
    } else {
        XImage ximage = {
                .height = height,
                .width = width,
                .depth = 1,
                .bits_per_pixel = 1,
                .xoffset = 0,
                .format = XYPixmap,
                .data = (char *) data,
                .byte_order = LSBFirst,
                .bitmap_unit = 8,
                .bitmap_bit_order = LSBFirst,
                .bitmap_pad = 8,
                .bytes_per_line = (width + 7) / 8,
        };
        XPutImage(display, pix, gc, &ximage, 0, 0, 0, 0, width, height);
        XFreeGC(display, gc);
        return(pix);
    }
}

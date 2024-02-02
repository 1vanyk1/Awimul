#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlib.h"
#include <stdio.h>

/*
 * XCreatePixmapFromBitmapData: Routine to make a pixmap from user supplied bitmap data.
 *             D is any drawable on the same screen that the pixmap will be used in.
 *             Data is a pointer to the bit data, and
 *             width & height give the size in bits of the pixmap.
 *             Fg and Bg are the pixel values to use for the two colors.
 *             Depth is the depth of the pixmap to create.
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
Pixmap XCreatePixmapFromBitmapData(
        Display *display,
        Drawable d,
        char *data,
        unsigned int width,
        unsigned int height,
        unsigned long fg,
        unsigned long bg,
        unsigned int depth)
{
    Pixmap pix = XCreatePixmap(display, d, width, height, depth);
    XGCValues gcv = {
            .foreground = fg,
            .background = bg
    };
    GC gc = XCreateGC(display, pix, GCForeground|GCBackground, &gcv);
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
                .format = XYBitmap,
                .data = data,
                .byte_order = LSBFirst,
                .bitmap_unit = 8,
                .bitmap_bit_order = LSBFirst,
                .bitmap_pad = 8,
                .bytes_per_line = (width + 7) / 8
        };
        XPutImage(display, pix, gc, &ximage, 0, 0, 0, 0, width, height);
        XFreeGC(display, gc);
        return(pix);
    }
}

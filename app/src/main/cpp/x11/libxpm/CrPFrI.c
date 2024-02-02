#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "XpmI.h"

void
xpmCreatePixmapFromImage(
        Display	*display,
        Drawable	 d,
        XImage	*ximage,
        Pixmap	*pixmap_return)
{
    GC gc;
    XGCValues values;

    *pixmap_return = XCreatePixmap(display, d, ximage->width,
                                   ximage->height, ximage->depth);
    /* set fg and bg in case we have an XYBitmap */
    values.foreground = 1;
    values.background = 0;
    gc = XCreateGC(display, *pixmap_return,
                   GCForeground | GCBackground, &values);

    XPutImage(display, *pixmap_return, gc, ximage, 0, 0, 0, 0,
              ximage->width, ximage->height);

    XFreeGC(display, gc);
}

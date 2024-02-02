#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "XpmI.h"

int
XpmReadFileToPixmap(
        Display		*display,
        Drawable		 d,
        const char		*filename,
        Pixmap		*pixmap_return,
        Pixmap		*shapemask_return,
        XpmAttributes	*attributes)
{
    XImage *ximage, *shapeimage;
    int ErrorStatus;

    /* initialize return values */
    if (pixmap_return)
        *pixmap_return = 0;
    if (shapemask_return)
        *shapemask_return = 0;

    /* create the images */
    ErrorStatus = XpmReadFileToImage(display, filename,
                                     (pixmap_return ? &ximage : NULL),
                                     (shapemask_return ? &shapeimage : NULL),
                                     attributes);

    if (ErrorStatus < 0)		/* fatal error */
        return (ErrorStatus);

    /* create the pixmaps and destroy images */
    if (pixmap_return && ximage) {
        xpmCreatePixmapFromImage(display, d, ximage, pixmap_return);
        XDestroyImage(ximage);
    }
    if (shapemask_return && shapeimage) {
        xpmCreatePixmapFromImage(display, d, shapeimage, shapemask_return);
        XDestroyImage(shapeimage);
    }
    return (ErrorStatus);
}

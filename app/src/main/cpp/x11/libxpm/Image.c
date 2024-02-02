#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "XpmI.h"

/*
 * Init returned data to free safely later on
 */
void
xpmInitXpmImage(XpmImage *image)
{
    image->ncolors = 0;
    image->colorTable = NULL;
    image->data = NULL;
}

/*
 * Free the XpmImage data which have been allocated
 */
void
XpmFreeXpmImage(XpmImage *image)
{
    if (image->colorTable)
        xpmFreeColorTable(image->colorTable, image->ncolors);
    if (image->data)
        XpmFree(image->data);
    image->data = NULL;
}

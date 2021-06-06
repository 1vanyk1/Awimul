#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xutil.h"

int
XGeometry (
        Display *dpy,			/* user's display connection */
        int screen,			/* screen on which to do computation */
        _Xconst char *pos,			/* user provided geometry spec */
        _Xconst char *def,			/* default geometry spec for window */
        unsigned int bwidth,		/* border width */
        unsigned int fwidth,		/* size of position units */
        unsigned int fheight,
        int xadd,				/* any additional interior space */
        int yadd,
        register int *x,			/* always set on successful RETURN */
        register int *y,			/* always set on successful RETURN */
        register int *width,		/* always set on successful RETURN */
        register int *height)		/* always set on successful RETURN */
{
    int px, py;			/* returned values from parse */
    unsigned int pwidth, pheight;	/* returned values from parse */
    int dx, dy;			/* default values from parse */
    unsigned int dwidth, dheight;	/* default values from parse */
    int pmask, dmask;		/* values back from parse */

    pmask = XParseGeometry(pos, &px, &py, &pwidth, &pheight);
    dmask = XParseGeometry(def, &dx, &dy, &dwidth, &dheight);

    /* set default values */
    *x = (dmask & XNegative) ?
         DisplayWidth(dpy, screen)  + dx - dwidth * fwidth -
         2 * bwidth - xadd : dx;
    *y = (dmask & YNegative) ?
         DisplayHeight(dpy, screen) + dy - dheight * fheight -
         2 * bwidth - yadd : dy;
    *width  = dwidth;
    *height = dheight;

    if (pmask & WidthValue)  *width  = pwidth;
    if (pmask & HeightValue) *height = pheight;

    if (pmask & XValue)
        *x = (pmask & XNegative) ?
             DisplayWidth(dpy, screen) + px - *width * fwidth -
             2 * bwidth - xadd : px;
    if (pmask & YValue)
        *y = (pmask & YNegative) ?
             DisplayHeight(dpy, screen) + py - *height * fheight -
             2 * bwidth - yadd : py;
    return (pmask);
}
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xutil.h"
#include "Xatomtype.h"
#include "headers/xatom.h"
#include "headers/xos.h"

void XSetWMSizeHints (
        Display *dpy,
        Window w,
        XSizeHints *hints,
        Atom prop)
{
    xPropSizeHints data;

    memset(&data, 0, sizeof(data));
    data.flags = (hints->flags &
                  (USPosition|USSize|PPosition|PSize|PMinSize|PMaxSize|
                   PResizeInc|PAspect|PBaseSize|PWinGravity));

    /*
     * The x, y, width, and height fields are obsolete; but, applications
     * that want to work with old window managers might set them.
     */
    if (hints->flags & (USPosition|PPosition)) {
        data.x = hints->x;
        data.y = hints->y;
    }
    if (hints->flags & (USSize|PSize)) {
        data.width = hints->width;
        data.height = hints->height;
    }

    if (hints->flags & PMinSize) {
        data.minWidth = hints->min_width;
        data.minHeight = hints->min_height;
    }
    if (hints->flags & PMaxSize) {
        data.maxWidth  = hints->max_width;
        data.maxHeight = hints->max_height;
    }
    if (hints->flags & PResizeInc) {
        data.widthInc = hints->width_inc;
        data.heightInc = hints->height_inc;
    }
    if (hints->flags & PAspect) {
        data.minAspectX = hints->min_aspect.x;
        data.minAspectY = hints->min_aspect.y;
        data.maxAspectX = hints->max_aspect.x;
        data.maxAspectY = hints->max_aspect.y;
    }
    if (hints->flags & PBaseSize) {
        data.baseWidth = hints->base_width;
        data.baseHeight = hints->base_height;
    }
    if (hints->flags & PWinGravity) {
        data.winGravity = hints->win_gravity;
    }

    XChangeProperty (dpy, w, prop, XA_WM_SIZE_HINTS, 32,
                     PropModeReplace, (unsigned char *) &data,
                     NumPropSizeElements);
}


void XSetWMNormalHints (
        Display *dpy,
        Window w,
        XSizeHints *hints)
{
    XSetWMSizeHints (dpy, w, hints, XA_WM_NORMAL_HINTS);
}


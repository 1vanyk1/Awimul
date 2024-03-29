#ifndef XLIB_SW_WINSYS_H
#define XLIB_SW_WINSYS_H

#include "sw_winsys.h"
#include "../../../x11/headers/xlib.h"


/* This is what the xlib software winsys expects to find in the
 * "private" field of flush_frontbuffers().
 *
 * Xlib-based gallium frontends somehow need to know this.
 */
struct xlib_drawable {
    Visual *visual;
    int depth;
    Drawable drawable;
};

#endif

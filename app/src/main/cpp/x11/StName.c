#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include "headers/xlibint.h"
#include "headers/xatom.h"

int
XStoreName (
        register Display *dpy,
        Window w,
        _Xconst char *name)
{
    if (name != NULL && strlen(name) >= USHRT_MAX)
        return 0;
    return XChangeProperty(dpy, w, XA_WM_NAME, XA_STRING, /*  */
                           8, PropModeReplace, (_Xconst unsigned char *)name,
                           name ? (int) strlen(name) : 0);
}

int
XSetIconName (
        register Display *dpy,
        Window w,
        _Xconst char *icon_name)
{
    if (icon_name != NULL && strlen(icon_name) >= USHRT_MAX)
        return 0;
    return XChangeProperty(dpy, w, XA_WM_ICON_NAME, XA_STRING, 8,
                           PropModeReplace, (_Xconst unsigned char *)icon_name,
                           icon_name ? (int) strlen(icon_name) : 0);
}

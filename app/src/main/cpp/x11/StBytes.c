#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"

/* insulate predefined atom numbers from cut routines */
static const Atom n_to_atom[8] = {
        XA_CUT_BUFFER0,
        XA_CUT_BUFFER1,
        XA_CUT_BUFFER2,
        XA_CUT_BUFFER3,
        XA_CUT_BUFFER4,
        XA_CUT_BUFFER5,
        XA_CUT_BUFFER6,
        XA_CUT_BUFFER7};

int
XRotateBuffers (
        register Display *dpy,
        int rotate)
{
    /* XRotateWindowProperties wants a non-const Atom*, but it doesn't
     * modify it, so this is safe.
     */
    return XRotateWindowProperties(dpy, RootWindow(dpy, 0), (Atom *)n_to_atom, 8, rotate);
}

char *XFetchBuffer (
        register Display *dpy,
        int *nbytes,
        register int buffer)
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;
    unsigned char *data;
    *nbytes = 0;
    if ((buffer < 0) || (buffer > 7)) return (NULL);
/* XXX should be (sizeof (maxint) - 1)/4 */
    if (XGetWindowProperty(dpy, RootWindow(dpy, 0), n_to_atom[buffer],
                           0L, 10000000L, False, XA_STRING,
                           &actual_type, &actual_format, &nitems, &leftover, &data) != Success) {
        return (NULL);
    }
    if ( (actual_type == XA_STRING) &&  (actual_format != 32) ) {
        *nbytes = nitems;
        return((char *)data);
    }
    Xfree (data);
    return(NULL);
}

char *XFetchBytes (
        register Display *dpy,
        int *nbytes)
{
    return (XFetchBuffer (dpy, nbytes, 0));
}

int
XStoreBuffer (
        register Display *dpy,
        _Xconst char *bytes,
        int nbytes,
        register int buffer)
{
    if ((buffer < 0) || (buffer > 7)) return 0;
    return XChangeProperty(dpy, RootWindow(dpy, 0), n_to_atom[buffer],
                           XA_STRING, 8, PropModeReplace, (_Xconst unsigned char *) bytes, nbytes);
}

int
XStoreBytes (
        register Display *dpy,
        _Xconst char *bytes,
        int nbytes)
{
    return XStoreBuffer (dpy, bytes, nbytes, 0);
}

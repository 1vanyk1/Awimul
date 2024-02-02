#ifndef _XTEST_H_
#define _XTEST_H_

#include "../xfuncproto.h"
#include "xtestconst.h"
#include "XInput.h"

_XFUNCPROTOBEGIN

        Bool XTestQueryExtension(
        Display*		/* dpy */,
int*		/* event_basep */,
int*		/* error_basep */,
int*		/* majorp */,
int*		/* minorp */
);

Bool XTestCompareCursorWithWindow(
        Display*		/* dpy */,
        Window		/* window */,
        Cursor		/* cursor */
);

Bool XTestCompareCurrentCursorWithWindow(
        Display*		/* dpy */,
        Window		/* window */
);

extern int XTestFakeKeyEvent(
        Display*		/* dpy */,
        unsigned int	/* keycode */,
        Bool		/* is_press */,
        unsigned long	/* delay */
);

extern int XTestFakeButtonEvent(
        Display*		/* dpy */,
        unsigned int	/* button */,
        Bool		/* is_press */,
        unsigned long	/* delay */
);

extern int XTestFakeMotionEvent(
        Display*		/* dpy */,
        int			/* screen */,
        int			/* x */,
        int			/* y */,
        unsigned long	/* delay */
);

extern int XTestFakeRelativeMotionEvent(
        Display*		/* dpy */,
        int			/* x */,
        int			/* y */,
        unsigned long	/* delay */
);

extern int XTestFakeDeviceKeyEvent(
        Display*		/* dpy */,
        XDevice*		/* dev */,
        unsigned int	/* keycode */,
        Bool		/* is_press */,
        int*		/* axes */,
        int			/* n_axes */,
        unsigned long	/* delay */
);

extern int XTestFakeDeviceButtonEvent(
        Display*		/* dpy */,
        XDevice*		/* dev */,
        unsigned int	/* button */,
        Bool		/* is_press */,
        int*		/* axes */,
        int			/* n_axes */,
        unsigned long	/* delay */
);

extern int XTestFakeProximityEvent(
        Display*		/* dpy */,
        XDevice*		/* dev */,
        Bool		/* in_prox */,
        int*		/* axes */,
        int			/* n_axes */,
        unsigned long	/* delay */
);

extern int XTestFakeDeviceMotionEvent(
        Display*		/* dpy */,
        XDevice*		/* dev */,
        Bool		/* is_relative */,
        int			/* first_axis */,
        int*		/* axes */,
        int			/* n_axes */,
        unsigned long	/* delay */
);

extern int XTestGrabControl(
        Display*		/* dpy */,
        Bool		/* impervious */
);

void XTestSetGContextOfGC(
        GC			/* gc */,
        GContext		/* gid */
);

void XTestSetVisualIDOfVisual(
        Visual*		/* visual */,
        VisualID		/* visualid */
);

Status XTestDiscard(
        Display*		/* dpy */
);

_XFUNCPROTOEND

#endif

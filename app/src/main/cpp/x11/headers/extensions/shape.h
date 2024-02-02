#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "../xfuncproto.h"
#include "shapeconst.h"

#ifndef _SHAPE_SERVER_
#include "../xutil.h"

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    Bool send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Window window;	    /* window of event */
    int kind;		    /* ShapeBounding or ShapeClip */
    int x, y;		    /* extents of new region */
    unsigned width, height;
    Time time;		    /* server timestamp when region changed */
    Bool shaped;	    /* true if the region exists */
} XShapeEvent;

_XFUNCPROTOBEGIN

extern Bool XShapeQueryExtension (
        Display*	/* display */,
int*	/* event_base */,
int*	/* error_base */
);

extern Status XShapeQueryVersion (
        Display*	/* display */,
        int*	/* major_version */,
        int*	/* minor_version */
);

extern void XShapeCombineRegion (
        Display*	/* display */,
        Window	/* dest */,
        int		/* dest_kind */,
        int		/* x_off */,
        int		/* y_off */,
        Region	/* region */,
        int		/* op */
);

extern void XShapeCombineRectangles (
        Display*	/* display */,
        Window	/* dest */,
        int		/* dest_kind */,
        int		/* x_off */,
        int		/* y_off */,
        XRectangle*	/* rectangles */,
        int		/* n_rects */,
        int		/* op */,
        int		/* ordering */
);

extern void XShapeCombineMask (
        Display*	/* display */,
        Window	/* dest */,
        int		/* dest_kind */,
        int		/* x_off */,
        int		/* y_off */,
        Pixmap	/* src */,
        int		/* op */
);

extern void XShapeCombineShape (
        Display*	/* display */,
        Window	/* dest */,
        int		/* dest_kind */,
        int		/* x_off */,
        int		/* y_off */,
        Window	/* src */,
        int		/* src_kind */,
        int		/* op */
);

extern void XShapeOffsetShape (
        Display*	/* display */,
        Window	/* dest */,
        int		/* dest_kind */,
        int		/* x_off */,
        int		/* y_off */
);

extern Status XShapeQueryExtents (
        Display*		/* display */,
        Window		/* window */,
        Bool*		/* bounding_shaped */,
        int*		/* x_bounding */,
        int*		/* y_bounding */,
        unsigned int*	/* w_bounding */,
        unsigned int*	/* h_bounding */,
        Bool*		/* clip_shaped */,
        int*		/* x_clip */,
        int*		/* y_clip */,
        unsigned int*	/* w_clip */,
        unsigned int*	/* h_clip */
);

extern void XShapeSelectInput (
        Display*		/* display */,
        Window		/* window */,
        unsigned long	/* mask */
);

extern unsigned long XShapeInputSelected (
        Display*	/* display */,
        Window	/* window */
);

extern XRectangle *XShapeGetRectangles (
        Display*	/* display */,
        Window	/* window */,
        int		/* kind */,
        int*	/* count */,
        int*	/* ordering */
);

_XFUNCPROTOEND

#endif /* !_SHAPE_SERVER_ */

#endif /* _SHAPE_H_ */

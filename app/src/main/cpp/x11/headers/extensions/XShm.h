#ifndef _XSHM_H_
#define _XSHM_H_

#include "../xfuncproto.h"
#include "shm.h"

#ifndef _XSHM_SERVER_
typedef unsigned long ShmSeg;

typedef struct {
    int	type;		    /* of event */
    unsigned long serial;   /* # of last request processed by server */
    Bool send_event;	    /* true if this came frome a SendEvent request */
    Display *display;	    /* Display the event was read from */
    Drawable drawable;	    /* drawable of request */
    int major_code;	    /* ShmReqCode */
    int minor_code;	    /* X_ShmPutImage */
    ShmSeg shmseg;	    /* the ShmSeg used in the request */
    unsigned long offset;   /* the offset into ShmSeg used in the request */
} XShmCompletionEvent;

typedef struct {
    ShmSeg shmseg;	/* resource id */
    int shmid;		/* kernel id */
    char *shmaddr;	/* address in client */
    Bool readOnly;	/* how the server should attach it */
} XShmSegmentInfo;

_XFUNCPROTOBEGIN

        Bool XShmQueryExtension(
        Display*		/* dpy */
);

int XShmGetEventBase(
        Display* 		/* dpy */
);

Bool XShmQueryVersion(
        Display*		/* dpy */,
        int*		/* majorVersion */,
        int*		/* minorVersion */,
        Bool*		/* sharedPixmaps */
);

int XShmPixmapFormat(
        Display*		/* dpy */
);

Bool XShmAttach(
        Display*		/* dpy */,
        XShmSegmentInfo*	/* shminfo */
);

Bool XShmDetach(
        Display*		/* dpy */,
        XShmSegmentInfo*	/* shminfo */
);

Bool XShmPutImage(
        Display*		/* dpy */,
        Drawable		/* d */,
        GC			/* gc */,
        XImage*		/* image */,
        int			/* src_x */,
        int			/* src_y */,
        int			/* dst_x */,
        int			/* dst_y */,
        unsigned int	/* src_width */,
        unsigned int	/* src_height */,
        Bool		/* send_event */
);

Bool XShmGetImage(
        Display*		/* dpy */,
        Drawable		/* d */,
        XImage*		/* image */,
        int			/* x */,
        int			/* y */,
        unsigned long	/* plane_mask */
);

XImage *XShmCreateImage(
        Display*		/* dpy */,
        Visual*		/* visual */,
        unsigned int	/* depth */,
        int			/* format */,
        char*		/* data */,
        XShmSegmentInfo*	/* shminfo */,
        unsigned int	/* width */,
        unsigned int	/* height */
);

Pixmap XShmCreatePixmap(
        Display*		/* dpy */,
        Drawable		/* d */,
        char*		/* data */,
        XShmSegmentInfo*	/* shminfo */,
        unsigned int	/* width */,
        unsigned int	/* height */,
        unsigned int	/* depth */
);

_XFUNCPROTOEND
#endif /* _XSHM_SERVER_ */

#endif

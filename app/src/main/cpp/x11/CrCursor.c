#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

#ifdef USE_DYNAMIC_XCURSOR
Cursor
_XTryShapeBitmapCursor (Display		*dpy,
			Pixmap		source,
			Pixmap		mask,
			XColor		*foreground,
			XColor		*background,
			unsigned int	x,
			unsigned int	y);
#endif

Cursor XCreatePixmapCursor(
        register Display *dpy,
        Pixmap source,
        Pixmap mask,
        XColor *foreground,
        XColor *background,
        unsigned int x,
        unsigned int y)

{
    register xCreateCursorReq *req;
    Cursor cid;

#ifdef USE_DYNAMIC_XCURSOR
    cid = _XTryShapeBitmapCursor (dpy, source, mask,
				  foreground, background, x, y);
    if (cid)
	return cid;
#endif
    LockDisplay(dpy);
    GetReq(CreateCursor, req);
    req->cid = cid = XAllocID(dpy);
    req->source = source;
    req->mask = mask;
    req->foreRed = foreground->red;
    req->foreGreen = foreground->green;
    req->foreBlue = foreground->blue;
    req->backRed = background->red;
    req->backGreen = background->green;
    req->backBlue = background->blue;
    req->x = x;
    req->y = y;
    UnlockDisplay(dpy);
    SyncHandle();
    return (cid);
}


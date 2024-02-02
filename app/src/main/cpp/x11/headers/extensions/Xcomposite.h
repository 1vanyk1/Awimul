#ifndef _XCOMPOSITE_H_
#define _XCOMPOSITE_H_

#include "composite.h"
#include "Xfixes.h"
#include "../xfuncproto.h"

/*
 * This revision number also appears in configure.ac, they have
 * to be manually synchronized
 */
#define XCOMPOSITE_MAJOR	COMPOSITE_MAJOR
#define XCOMPOSITE_MINOR	COMPOSITE_MINOR
#define XCOMPOSITE_REVISION	2
#define XCOMPOSITE_VERSION	((XCOMPOSITE_MAJOR * 10000) + (XCOMPOSITE_MINOR * 100) + (XCOMPOSITE_REVISION))

_XFUNCPROTOBEGIN

        Bool XCompositeQueryExtension (Display *dpy,
int *event_base_return,
int *error_base_return);

Status XCompositeQueryVersion (Display *dpy,
                               int     *major_version_return,
                               int     *minor_version_return);

int XCompositeVersion (void);

void
        XCompositeRedirectWindow (Display *dpy, Window window, int update);

void
        XCompositeRedirectSubwindows (Display *dpy, Window window, int update);

void
        XCompositeUnredirectWindow (Display *dpy, Window window, int update);

void
        XCompositeUnredirectSubwindows (Display *dpy, Window window, int update);

XserverRegion
        XCompositeCreateRegionFromBorderClip (Display *dpy, Window window);

Pixmap
        XCompositeNameWindowPixmap (Display *dpy, Window window);

Window
        XCompositeGetOverlayWindow (Display *dpy, Window window);

void
        XCompositeReleaseOverlayWindow (Display *dpy, Window window);

_XFUNCPROTOEND

#endif /* _XCOMPOSITE_H_ */

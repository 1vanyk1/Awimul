#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include "xfixeswire.h"

#define COMPOSITE_NAME				"Composite"
#define COMPOSITE_MAJOR				0
#define COMPOSITE_MINOR				4

#define CompositeRedirectAutomatic		0
#define CompositeRedirectManual			1

#define X_CompositeQueryVersion			0
#define X_CompositeRedirectWindow		1
#define X_CompositeRedirectSubwindows		2
#define X_CompositeUnredirectWindow		3
#define X_CompositeUnredirectSubwindows		4
#define X_CompositeCreateRegionFromBorderClip	5
#define X_CompositeNameWindowPixmap		6
#define X_CompositeGetOverlayWindow             7
#define X_CompositeReleaseOverlayWindow         8

#define CompositeNumberRequests	    (X_CompositeReleaseOverlayWindow + 1)

#define CompositeNumberEvents			0

#endif /* _COMPOSITE_H_ */

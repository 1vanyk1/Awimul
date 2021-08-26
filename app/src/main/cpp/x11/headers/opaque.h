#ifndef OPAQUE_H
#define OPAQUE_H

#include "xmd.h"

#include "globals.h"
#include "xlocale.h"
#include "xdefs.h"

extern _X_EXPORT const char *defaultTextFont;
extern _X_EXPORT const char *defaultCursorFont;
extern _X_EXPORT int MaxClients;
extern _X_EXPORT int LimitClients;
extern _X_EXPORT volatile char isItTimeToYield;
extern _X_EXPORT volatile char dispatchException;

/* bit values for dispatchException */
#define DE_RESET     1
#define DE_TERMINATE 2
#define DE_PRIORITYCHANGE 4     /* set when a client's priority changes */

extern _X_EXPORT CARD32 TimeOutValue;
extern _X_EXPORT int ScreenSaverBlanking;
extern _X_EXPORT int ScreenSaverAllowExposures;
extern _X_EXPORT int defaultScreenSaverBlanking;
extern _X_EXPORT int defaultScreenSaverAllowExposures;
extern _X_EXPORT const char *display;
extern _X_EXPORT int displayfd;
extern _X_EXPORT Bool explicit_display;

extern _X_EXPORT int defaultBackingStore;
extern _X_EXPORT Bool disableBackingStore;
extern _X_EXPORT Bool enableBackingStore;
extern _X_EXPORT Bool enableIndirectGLX;
extern _X_EXPORT Bool PartialNetwork;
extern _X_EXPORT Bool RunFromSigStopParent;

#ifdef RLIMIT_DATA
extern _X_EXPORT int limitDataSpace;
#endif
#ifdef RLIMIT_STACK
extern _X_EXPORT int limitStackSpace;
#endif
#ifdef RLIMIT_NOFILE
extern _X_EXPORT int limitNoFile;
#endif
extern _X_EXPORT Bool defeatAccessControl;
extern _X_EXPORT long maxBigRequestSize;
extern _X_EXPORT Bool party_like_its_1989;
extern _X_EXPORT Bool whiteRoot;
extern _X_EXPORT Bool bgNoneRoot;

extern _X_EXPORT Bool CoreDump;
extern _X_EXPORT Bool NoListenAll;

#endif                          /* OPAQUE_H */
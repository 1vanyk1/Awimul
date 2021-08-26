#ifndef XV_H
#define XV_H
/*
** File: 
**
**   Xv.h --- Xv shared library and server header file
**
** Author: 
**
**   David Carver (Digital Workstation Engineering/Project Athena)
**
** Revisions:
**
**   05.15.91 Carver
**     - version 2.0 upgrade
**
**   01.24.91 Carver
**     - version 1.4 upgrade
**
*/

#include "../x.h"

#define XvName "XVideo"
#define XvVersion 2
#define XvRevision 2

/* Symbols */

typedef XID XvPortID;
typedef XID XvEncodingID;

#define XvNone 0

#define XvInput          0
#define XvOutput         1

#define XvInputMask      (1L<<XvInput)
#define XvOutputMask     (1L<<XvOutput)
#define XvVideoMask	 0x00000004
#define XvStillMask	 0x00000008
#define XvImageMask	 0x00000010

/* These two are not client viewable */
#define XvPixmapMask	 0x00010000
#define XvWindowMask	 0x00020000


#define XvGettable	0x01
#define XvSettable	0x02

#define XvRGB		0
#define XvYUV		1

#define XvPacked	0
#define XvPlanar	1

#define XvTopToBottom	0
#define XvBottomToTop	1


/* Events */

#define XvVideoNotify 0
#define XvPortNotify 1
#define XvNumEvents 2

/* Video Notify Reasons */

#define XvStarted 0
#define XvStopped 1
#define XvBusy 2
#define XvPreempted 3
#define XvHardError 4
#define XvLastReason 4

#define XvNumReasons (XvLastReason + 1)

#define XvStartedMask     (1L<<XvStarted)
#define XvStoppedMask     (1L<<XvStopped)
#define XvBusyMask        (1L<<XvBusy)
#define XvPreemptedMask   (1L<<XvPreempted)
#define XvHardErrorMask   (1L<<XvHardError)

#define XvAnyReasonMask   ((1L<<XvNumReasons) - 1)
#define XvNoReasonMask    0

/* Errors */

#define XvBadPort 0
#define XvBadEncoding 1
#define XvBadControl 2
#define XvNumErrors 3

/* Status */

#define XvBadExtension 1
#define XvAlreadyGrabbed 2
#define XvInvalidTime 3
#define XvBadReply 4
#define XvBadAlloc 5

#endif /* XV_H */
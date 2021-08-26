#ifndef _FS_H_
#define	_FS_H_

#include "../xdefs.h"
#include "fsmasks.h"

#define	FS_PROTOCOL		2
#define	FS_PROTOCOL_MINOR	0

#ifndef X_PROTOCOL
/* protocol families */
#define FamilyInternet          0
#define FamilyDECnet            1
#define FamilyChaos             2
#define FamilyInternet6         6


typedef unsigned int    FSDrawDirection;
#endif

#ifndef None
#define	None		0L
#endif

#define	LeftToRightDrawDirection	0
#define	RightToLeftDrawDirection	1

/* font info flags */
#define	FontInfoAllCharsExist		(1L << 0)
#define	FontInfoInkInside		(1L << 1)
#define	FontInfoHorizontalOverlap	(1L << 2)

/* auth status flags */
#define	AuthSuccess	0
#define	AuthContinue	1
#define	AuthBusy	2
#define	AuthDenied	3

/* property types */
#define	PropTypeString		0
#define	PropTypeUnsigned	1
#define	PropTypeSigned		2

#ifndef LSBFirst
/* byte order */
#define LSBFirst                0
#define MSBFirst                1
#endif

/* event masks */
#define	CatalogueChangeNotifyMask	(1L << 0)
#define	FontChangeNotifyMask		(1L << 1)

/* errors */
#define	FSSuccess		-1
#define	FSBadRequest		0
#define	FSBadFormat		1
#define	FSBadFont		2
#define	FSBadRange		3
#define	FSBadEventMask		4
#define	FSBadAccessContext	5
#define	FSBadIDChoice		6
#define	FSBadName		7
#define	FSBadResolution		8
#define	FSBadAlloc		9
#define	FSBadLength		10
#define	FSBadImplementation	11

#define	FirstExtensionError	128
#define	LastExtensionError	255

/* events */
#define	KeepAlive		0
#define	CatalogueChangeNotify	1
#define	FontChangeNotify	2
#define FSLASTEvent		3

#endif				/* _FS_H_ */
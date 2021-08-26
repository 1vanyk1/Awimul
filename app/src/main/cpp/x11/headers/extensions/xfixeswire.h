#ifndef _XFIXESWIRE_H_
#define _XFIXESWIRE_H_

#define XFIXES_NAME	"XFIXES"
#define XFIXES_MAJOR	6
#define XFIXES_MINOR	0

/*************** Version 1 ******************/
#define X_XFixesQueryVersion		    0
#define X_XFixesChangeSaveSet		    1
#define X_XFixesSelectSelectionInput	    2
#define X_XFixesSelectCursorInput	    3
#define X_XFixesGetCursorImage		    4
/*************** Version 2 ******************/
#define X_XFixesCreateRegion		    5
#define X_XFixesCreateRegionFromBitmap	    6
#define X_XFixesCreateRegionFromWindow	    7
#define X_XFixesCreateRegionFromGC	    8
#define X_XFixesCreateRegionFromPicture	    9
#define X_XFixesDestroyRegion		    10
#define X_XFixesSetRegion		    11
#define X_XFixesCopyRegion		    12
#define X_XFixesUnionRegion		    13
#define X_XFixesIntersectRegion		    14
#define X_XFixesSubtractRegion		    15
#define X_XFixesInvertRegion		    16
#define X_XFixesTranslateRegion		    17
#define X_XFixesRegionExtents		    18
#define X_XFixesFetchRegion		    19
#define X_XFixesSetGCClipRegion		    20
#define X_XFixesSetWindowShapeRegion	    21
#define X_XFixesSetPictureClipRegion	    22
#define X_XFixesSetCursorName		    23
#define X_XFixesGetCursorName		    24
#define X_XFixesGetCursorImageAndName	    25
#define X_XFixesChangeCursor		    26
#define X_XFixesChangeCursorByName	    27
/*************** Version 3 ******************/
#define X_XFixesExpandRegion		    28
/*************** Version 4 ******************/
#define X_XFixesHideCursor		    29
#define X_XFixesShowCursor		    30
/*************** Version 5 ******************/
#define X_XFixesCreatePointerBarrier	    31
#define X_XFixesDestroyPointerBarrier	    32
/*************** Version 6 ******************/
#define X_XFixesSetClientDisconnectMode	    33
#define X_XFixesGetClientDisconnectMode	    34

#define XFixesNumberRequests		    (X_XFixesGetClientDisconnectMode+1)

/* Selection events share one event number */
#define XFixesSelectionNotify		    0

/* Within the selection, the 'subtype' field distinguishes */
#define XFixesSetSelectionOwnerNotify	    0
#define XFixesSelectionWindowDestroyNotify  1
#define XFixesSelectionClientCloseNotify    2

#define XFixesSetSelectionOwnerNotifyMask	(1L << 0)
#define XFixesSelectionWindowDestroyNotifyMask	(1L << 1)
#define XFixesSelectionClientCloseNotifyMask	(1L << 2)

/* There's only one cursor event so far */
#define XFixesCursorNotify		    1

#define XFixesDisplayCursorNotify	    0

#define XFixesDisplayCursorNotifyMask	    (1L << 0)

#define XFixesNumberEvents		    (2)

/* errors */
#define BadRegion			    0
#define BadBarrier			    1
#define XFixesNumberErrors		    (BadBarrier+1)

#define SaveSetNearest			    0
#define SaveSetRoot			    1

#define SaveSetMap			    0
#define SaveSetUnmap			    1

/*************** Version 2 ******************/

#define WindowRegionBounding		    0
#define WindowRegionClip		    1

/*************** Version 5 ******************/

#define BarrierPositiveX		    (1L << 0)
#define BarrierPositiveY		    (1L << 1)
#define BarrierNegativeX		    (1L << 2)
#define BarrierNegativeY		    (1L << 3)

/*************** Version 6 ******************/

/* The default server behaviour */
#define XFixesClientDisconnectFlagDefault   0
/* The server may disconnect this client to shut down */
#define XFixesClientDisconnectFlagTerminate  (1L << 0)

#endif	/* _XFIXESWIRE_H_ */

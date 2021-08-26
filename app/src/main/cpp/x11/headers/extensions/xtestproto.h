#ifndef _XTESTPROTO_H_
#define _XTESTPROTO_H_

#include "xtestconst.h"
#include "../xmd.h"

#define Window CARD32
#define Time CARD32
#define Cursor CARD32

#define X_XTestGetVersion	0
#define X_XTestCompareCursor	1
#define X_XTestFakeInput	2
#define X_XTestGrabControl	3

typedef struct {
    CARD8	reqType;	/* always XTestReqCode */
    CARD8	xtReqType;	/* always X_XTestGetVersion */
    CARD16	length B16;
    CARD8	majorVersion;
    CARD8	pad;
    CARD16	minorVersion B16;
} xXTestGetVersionReq;
#define sz_xXTestGetVersionReq 8

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	majorVersion;
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD16	minorVersion B16;
    CARD16	pad0 B16;
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
    CARD32	pad5 B32;
} xXTestGetVersionReply;
#define sz_xXTestGetVersionReply 32

typedef struct {
    CARD8	reqType;	/* always XTestReqCode */
    CARD8	xtReqType;	/* always X_XTestCompareCursor */
    CARD16	length B16;
    Window	window B32;
    Cursor	cursor B32;
} xXTestCompareCursorReq;
#define sz_xXTestCompareCursorReq 12

typedef struct {
    BYTE	type;			/* X_Reply */
    BOOL	same;
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD32	pad0 B32;
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
    CARD32	pad5 B32;
} xXTestCompareCursorReply;
#define sz_xXTestCompareCursorReply 32

/* used only on the client side */
typedef struct {
    CARD8	reqType;	/* always XTestReqCode */
    CARD8	xtReqType;	/* always X_XTestFakeInput */
    CARD16	length B16;
    BYTE	type;
    BYTE	detail;
    CARD16	pad0 B16;
    Time	time B32;
    Window	root B32;
    CARD32	pad1 B32;
    CARD32	pad2 B32;
    INT16	rootX B16, rootY B16;
    CARD32	pad3 B32;
    CARD16	pad4 B16;
    CARD8	pad5;
    CARD8	deviceid;
} xXTestFakeInputReq;
#define sz_xXTestFakeInputReq 36

typedef struct {
    CARD8	reqType;	/* always XTestReqCode */
    CARD8	xtReqType;	/* always X_XTestGrabControl */
    CARD16	length B16;
    BOOL	impervious;
    CARD8	pad0;
    CARD8	pad1;
    CARD8	pad2;
} xXTestGrabControlReq;
#define sz_xXTestGrabControlReq 8

#undef Window
#undef Time
#undef Cursor

#endif /* _XTESTPROTO_H_ */
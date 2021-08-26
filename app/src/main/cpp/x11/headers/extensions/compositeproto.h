
#ifndef _COMPOSITEPROTO_H_
#define _COMPOSITEPROTO_H_

#include "../xmd.h"
#include "composite.h"

#define Window CARD32
#define Region CARD32
#define Pixmap CARD32

/*
 * requests and replies
 */
typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    CARD32  majorVersion;
    CARD32  minorVersion;
} xCompositeQueryVersionReq;

#define sz_xCompositeQueryVersionReq   12

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber;
    CARD32  length;
    CARD32  majorVersion;
    CARD32  minorVersion;
    CARD32  pad2;
    CARD32  pad3;
    CARD32  pad4;
    CARD32  pad5;
} xCompositeQueryVersionReply;

#define sz_xCompositeQueryVersionReply	32

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
    CARD8   update;
    CARD8   pad1;
    CARD16  pad2;
} xCompositeRedirectWindowReq;

#define sz_xCompositeRedirectWindowReq	12

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
    CARD8   update;
    CARD8   pad1;
    CARD16  pad2;
} xCompositeRedirectSubwindowsReq;

#define sz_xCompositeRedirectSubwindowsReq	    12

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
    CARD8   update;
    CARD8   pad1;
    CARD16  pad2;
} xCompositeUnredirectWindowReq;

#define sz_xCompositeUnredirectWindowReq    12

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
    CARD8   update;
    CARD8   pad1;
    CARD16  pad2;
} xCompositeUnredirectSubwindowsReq;

#define sz_xCompositeUnredirectSubwindowsReq   12

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Region  region;
    Window  window;
} xCompositeCreateRegionFromBorderClipReq;

#define sz_xCompositeCreateRegionFromBorderClipReq  12

/* Version 0.2 additions */

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
    Pixmap  pixmap;
} xCompositeNameWindowPixmapReq;

#define sz_xCompositeNameWindowPixmapReq	    12

/* Version 0.3 additions */

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
} xCompositeGetOverlayWindowReq;

#define sz_xCompositeGetOverlayWindowReq sizeof(xCompositeGetOverlayWindowReq)

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber;
    CARD32  length;
    Window  overlayWin;
    CARD32  pad2;
    CARD32  pad3;
    CARD32  pad4;
    CARD32  pad5;
    CARD32  pad6;
} xCompositeGetOverlayWindowReply;

#define sz_xCompositeGetOverlayWindowReply sizeof(xCompositeGetOverlayWindowReply)

typedef struct {
    CARD8   reqType;
    CARD8   compositeReqType;
    CARD16  length;
    Window  window;
} xCompositeReleaseOverlayWindowReq;

#define sz_xCompositeReleaseOverlayWindowReq sizeof(xCompositeReleaseOverlayWindowReq)

#undef Window
#undef Region
#undef Pixmap

#endif /* _COMPOSITEPROTO_H_ */

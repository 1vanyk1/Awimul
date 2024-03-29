#ifndef _DRI3_PROTO_H_
#define _DRI3_PROTO_H_

#include "../xmd.h"

#define DRI3_NAME			"DRI3"
#define DRI3_MAJOR			1
#define DRI3_MINOR			0

#define DRI3NumberErrors		0
#define DRI3NumberEvents		0

#define X_DRI3QueryVersion		0
#define X_DRI3Open			1
#define X_DRI3PixmapFromBuffer          2
#define X_DRI3BufferFromPixmap          3
#define X_DRI3FenceFromFD               4
#define X_DRI3FDFromFence               5

#define DRI3NumberRequests		6

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  majorVersion B32;
    CARD32  minorVersion B32;
} xDRI3QueryVersionReq;
#define sz_xDRI3QueryVersionReq   12

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  majorVersion B32;
    CARD32  minorVersion B32;
    CARD32  pad2 B32;
    CARD32  pad3 B32;
    CARD32  pad4 B32;
    CARD32  pad5 B32;
} xDRI3QueryVersionReply;
#define sz_xDRI3QueryVersionReply	32

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  drawable B32;
    CARD32  provider B32;
} xDRI3OpenReq;
#define sz_xDRI3OpenReq	12

typedef struct {
    BYTE    type;   /* X_Reply */
    CARD8   nfd;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  pad2 B32;
    CARD32  pad3 B32;
    CARD32  pad4 B32;
    CARD32  pad5 B32;
    CARD32  pad6 B32;
    CARD32  pad7 B32;
} xDRI3OpenReply;
#define sz_xDRI3OpenReply	32

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  pixmap B32;
    CARD32  drawable B32;
    CARD32  size B32;
    CARD16  width B16;
    CARD16  height B16;
    CARD16  stride B16;
    CARD8   depth;
    CARD8   bpp;
} xDRI3PixmapFromBufferReq;

#define sz_xDRI3PixmapFromBufferReq     24

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  pixmap B32;
} xDRI3BufferFromPixmapReq;
#define sz_xDRI3BufferFromPixmapReq     8

typedef struct {
    BYTE    type;   /* X_Reply */
    CARD8   nfd;    /* Number of file descriptors returned (1) */
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  size B32;
    CARD16  width B16;
    CARD16  height B16;
    CARD16  stride B16;
    CARD8   depth;
    CARD8   bpp;
    CARD32  pad20 B32;
    CARD32  pad24 B32;
    CARD32  pad28 B32;
} xDRI3BufferFromPixmapReply;
#define sz_xDRI3BufferFromPixmapReply   32

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  drawable B32;
    CARD32  fence B32;
    BOOL    initially_triggered;
    CARD8   pad13;
    CARD16  pad14 B16;
} xDRI3FenceFromFDReq;

#define sz_xDRI3FenceFromFDReq  16

typedef struct {
    CARD8   reqType;
    CARD8   dri3ReqType;
    CARD16  length B16;
    CARD32  drawable B32;
    CARD32  fence B32;
} xDRI3FDFromFenceReq;

#define sz_xDRI3FDFromFenceReq  12

typedef struct {
    BYTE    type;   /* X_Reply */
    CARD8   nfd;    /* Number of file descriptors returned (1) */
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  pad08 B32;
    CARD32  pad12 B32;
    CARD32  pad16 B32;
    CARD32  pad20 B32;
    CARD32  pad24 B32;
    CARD32  pad28 B32;
} xDRI3FDFromFenceReply;

#define sz_xDRI3FDFromFenceReply   32

#endif

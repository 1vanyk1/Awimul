#ifndef _BIGREQSPROTO_H_
#define _BIGREQSPROTO_H_

#include "../xmd.h"

#define X_BigReqEnable		0

#define XBigReqNumberEvents	0

#define XBigReqNumberErrors	0

#define XBigReqExtensionName	"BIG-REQUESTS"

typedef struct {
    CARD8	reqType;	/* always XBigReqCode */
    CARD8	brReqType;	/* always X_BigReqEnable */
    CARD16	length;
} xBigReqEnableReq;
#define sz_xBigReqEnableReq 4

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad0;
    CARD16	sequenceNumber;
    CARD32	length;
    CARD32	max_request_size;
    CARD32	pad1;
    CARD32	pad2;
    CARD32	pad3;
    CARD32	pad4;
    CARD32	pad5;
} xBigReqEnableReply;
#define sz_xBigReqEnableReply 32


typedef struct {
    CARD8 reqType;
    CARD8 data;
    CARD16 zero;
    CARD32 length;
} xBigReq;

#endif /* _BIGREQSPROTO_H_ */

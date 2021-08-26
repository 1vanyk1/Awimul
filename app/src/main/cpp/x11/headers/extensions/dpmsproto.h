#ifndef _DPMSPROTO_H_
#define _DPMSPROTO_H_

#include "dpmsconst.h"
#include "../xmd.h"

#define X_DPMSGetVersion	0
#define X_DPMSCapable		1
#define X_DPMSGetTimeouts	2
#define X_DPMSSetTimeouts	3
#define X_DPMSEnable		4
#define X_DPMSDisable		5
#define X_DPMSForceLevel       	6
#define X_DPMSInfo       	7
#define X_DPMSSelectInput	8

#define DPMSNumberEvents	0

#define DPMSNumberErrors	0


typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSGetVersion */
    CARD16	length;
    CARD16	majorVersion;
    CARD16	minorVersion;
} xDPMSGetVersionReq;
#define sz_xDPMSGetVersionReq 8

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad0;
    CARD16	sequenceNumber;
    CARD32	length;
    CARD16	majorVersion;
    CARD16	minorVersion;
    CARD32	pad1;
    CARD32	pad2;
    CARD32	pad3;
    CARD32	pad4;
    CARD32	pad5;
} xDPMSGetVersionReply;
#define sz_xDPMSGetVersionReply 32

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSCapable */
    CARD16	length;
} xDPMSCapableReq;
#define sz_xDPMSCapableReq 4

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad0;
    CARD16	sequenceNumber;
    CARD32	length;
    BOOL	capable;
    CARD8	pad1;
    CARD16	pad2;
    CARD32	pad3;
    CARD32	pad4;
    CARD32	pad5;
    CARD32	pad6;
    CARD32	pad7;
} xDPMSCapableReply;
#define sz_xDPMSCapableReply 32

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSGetTimeouts */
    CARD16	length;
} xDPMSGetTimeoutsReq;
#define sz_xDPMSGetTimeoutsReq 4

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad0;
    CARD16	sequenceNumber;
    CARD32	length;
    CARD16	standby;
    CARD16	suspend;
    CARD16	off;
    CARD16	pad1;
    CARD32	pad2;
    CARD32	pad3;
    CARD32	pad4;
    CARD32	pad5;
} xDPMSGetTimeoutsReply;
#define sz_xDPMSGetTimeoutsReply 32

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSSetTimeouts */
    CARD16	length;
    CARD16	standby;
    CARD16	suspend;
    CARD16	off;
    CARD16	pad0;
} xDPMSSetTimeoutsReq;
#define sz_xDPMSSetTimeoutsReq 12

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSEnable */
    CARD16	length;
} xDPMSEnableReq;
#define sz_xDPMSEnableReq 4

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSDisable */
    CARD16	length;
} xDPMSDisableReq;
#define sz_xDPMSDisableReq 4

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSForceLevel */
    CARD16	length;
    CARD16	level;		/* power level requested */
    CARD16	pad0;
} xDPMSForceLevelReq;
#define sz_xDPMSForceLevelReq 8

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSInfo */
    CARD16	length;
} xDPMSInfoReq;
#define sz_xDPMSInfoReq 4

typedef struct {
    BYTE	type;			/* X_Reply */
    CARD8	pad0;
    CARD16	sequenceNumber;
    CARD32	length;
    CARD16	power_level;
    BOOL	state;
    CARD8	pad1;
    CARD32	pad2;
    CARD32	pad3;
    CARD32	pad4;
    CARD32	pad5;
    CARD32	pad6;
} xDPMSInfoReply;
#define sz_xDPMSInfoReply 32

typedef struct {
    CARD8	reqType;	/* always DPMSCode */
    CARD8	dpmsReqType;	/* always X_DPMSSelectInput */
    CARD16	length B16;
    CARD32	eventMask B32;
} xDPMSSelectInputReq;
#define sz_xDPMSSelectInputReq 8

typedef struct {
    CARD8	type;
    CARD8	extension;
    CARD16	sequenceNumber B16;
    CARD32	length;
    CARD16	evtype B16;
    CARD16	pad0 B16;
    Time	timestamp B32;
    CARD16	power_level B16;
    BOOL	state;
    CARD8	pad1;
    CARD32	pad2 B32;
    CARD32	pad3 B32;
    CARD32	pad4 B32;
} xDPMSInfoNotifyEvent;
#define sz_xDPMSInfoNotifyEvent 32

#endif /* _DPMSPROTO_H_ */

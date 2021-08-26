#ifndef _DAMAGEPROTO_H_
#define _DAMAGEPROTO_H_

#include "../xmd.h"
#include "xfixesproto.h"
#include "damagewire.h"

#define Window CARD32
#define Drawable CARD32
#define Font CARD32
#define Pixmap CARD32
#define Cursor CARD32
#define Colormap CARD32
#define GContext CARD32
#define Atom CARD32
#define VisualID CARD32
#define Time CARD32
#define KeyCode CARD8
#define KeySym CARD32
#define Picture CARD32
#define Region CARD32
#define Damage CARD32

/************** Version 0 ******************/

typedef struct {
    CARD8   reqType;
    CARD8   damageReqType;
    CARD16  length;
} xDamageReq;

/*
 * requests and replies
 */

typedef struct {
    CARD8   reqType;
    CARD8   damageReqType;
    CARD16  length;
    CARD32  majorVersion;
    CARD32  minorVersion;
} xDamageQueryVersionReq;

#define sz_xDamageQueryVersionReq   12

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
} xDamageQueryVersionReply;

#define sz_xDamageQueryVersionReply	32

typedef struct {
    CARD8	reqType;
    CARD8	damageReqType;
    CARD16	length;
    Damage	damage;
    Drawable	drawable;
    CARD8	level;
    CARD8	pad1;
    CARD16	pad2;
} xDamageCreateReq;

#define sz_xDamageCreateReq		16

typedef struct {
    CARD8	reqType;
    CARD8	damageReqType;
    CARD16	length;
    Damage	damage;
} xDamageDestroyReq;

#define sz_xDamageDestroyReq		8

typedef struct {
    CARD8	reqType;
    CARD8	damageReqType;
    CARD16	length;
    Damage	damage;
    Region	repair;
    Region	parts;
} xDamageSubtractReq;

#define sz_xDamageSubtractReq		16

typedef struct {
    CARD8	reqType;
    CARD8	damageReqType;
    CARD16	length;
    Drawable	drawable;
    Region	region;
} xDamageAddReq;

#define sz_xDamageAddReq		12

/* Events */

#define DamageNotifyMore    0x80

typedef struct {
    CARD8	type;
    CARD8	level;
    CARD16	sequenceNumber;
    Drawable	drawable;
    Damage	damage;
    Time	timestamp;
    xRectangle	area;
    xRectangle	geometry;
} xDamageNotifyEvent;

#undef Damage
#undef Region
#undef Picture
#undef Window
#undef Drawable
#undef Font
#undef Pixmap
#undef Cursor
#undef Colormap
#undef GContext
#undef Atom
#undef VisualID
#undef Time
#undef KeyCode
#undef KeySym

#endif /* _DAMAGEPROTO_H_ */

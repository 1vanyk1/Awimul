#ifndef _ICEPROTO_H_
#define _ICEPROTO_H_

#include "../headers/xmd.h"

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	data[2];
    CARD32	length;
} iceMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD16	errorClass;
    CARD32	length;
    CARD8	offendingMinorOpcode;
    CARD8	severity;
    CARD16	unused;
    CARD32	offendingSequenceNum;
    /* n	varying values */
    /* p	p = pad (n, 8) */
} iceErrorMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	byteOrder;
    CARD8	unused;
    CARD32	length;
} iceByteOrderMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	versionCount;
    CARD8	authCount;
    CARD32	length;
    CARD8	mustAuthenticate;
    CARD8	unused[7];
    /* i	STRING		vendor */
    /* j	STRING		release */
    /* k	LIST of STRING	authentication-protocol-names */
    /* m	LIST of VERSION version-list */
    /* p	p = pad (i+j+k+m, 8) */
} iceConnectionSetupMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	authIndex;
    CARD8	unused1;
    CARD32	length;
    CARD16	authDataLength;
    CARD8	unused2[6];
    /* n	varying data */
    /* p	p = pad (n, 8) */
} iceAuthRequiredMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused1[2];
    CARD32	length;
    CARD16	authDataLength;
    CARD8	unused2[6];
    /* n	varying data */
    /* p	p = pad (n, 8) */
} iceAuthReplyMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused1[2];
    CARD32	length;
    CARD16	authDataLength;
    CARD8	unused2[6];
    /* n	varying data */
    /* p	p = pad (n, 8) */
} iceAuthNextPhaseMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	versionIndex;
    CARD8	unused;
    CARD32	length;
    /* i	STRING		vendor */
    /* j	STRING		release */
    /* p	p = pad (i+j, 8) */
} iceConnectionReplyMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	protocolOpcode;
    CARD8	mustAuthenticate;
    CARD32	length;
    CARD8	versionCount;
    CARD8	authCount;
    CARD8	unused[6];
    /* i	STRING		protocol-name */
    /* j	STRING		vendor */
    /* k	STRING		release */
    /* m	LIST of STRING	authentication-protocol-names */
    /* n	LIST of VERSION version-list */
    /* p        p = pad (i+j+k+m+n, 8) */
} iceProtocolSetupMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	versionIndex;
    CARD8	protocolOpcode;
    CARD32	length;
    /* i	STRING		vendor */
    /* j	STRING		release */
    /* p	p = pad (i+j, 8) */
} iceProtocolReplyMsg;

typedef iceMsg  icePingMsg;
typedef iceMsg  icePingReplyMsg;
typedef iceMsg  iceWantToCloseMsg;
typedef iceMsg  iceNoCloseMsg;


/*
 * SIZEOF values.  These better be multiples of 8.
 */

#define sz_iceMsg			8
#define sz_iceErrorMsg			16
#define sz_iceByteOrderMsg		8
#define sz_iceConnectionSetupMsg        16
#define sz_iceAuthRequiredMsg		16
#define sz_iceAuthReplyMsg		16
#define sz_iceAuthNextPhaseMsg		16
#define sz_iceConnectionReplyMsg	8
#define sz_iceProtocolSetupMsg		16
#define sz_iceProtocolReplyMsg		8
#define sz_icePingMsg			8
#define sz_icePingReplyMsg		8
#define sz_iceWantToCloseMsg		8
#define sz_iceNoCloseMsg		8

#endif /* _ICEPROTO_H_ */

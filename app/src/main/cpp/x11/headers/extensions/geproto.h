#ifndef _GEPROTO_H_
#define _GEPROTO_H_

#include "../xproto.h"
#include "../x.h"
#include "ge.h"


/*********************************************************
 *
 * Protocol request constants
 *
 */

#define X_GEGetExtensionVersion 1

/*********************************************************
 *
 * XGE protocol requests/replies
 *
 */

/* generic request */
typedef struct {
    CARD8   reqType;
    CARD8   ReqType;
    CARD16  length B16;
} xGEReq;


/* QueryVersion */
typedef struct {
    CARD8	reqType;       /* input extension major code   */
    CARD8	ReqType;       /* always X_GEQueryVersion */
    CARD16	length B16;
    CARD16      majorVersion B16;
    CARD16      minorVersion B16;
} xGEQueryVersionReq;

#define sz_xGEQueryVersionReq    8

typedef struct {
    CARD8	repType;	/* X_Reply			*/
    CARD8	RepType;	/* always X_GEQueryVersion */
    CARD16	sequenceNumber B16;
    CARD32	length B32;
    CARD16	majorVersion B16;
    CARD16	minorVersion B16;
    CARD32	pad00 B32;
    CARD32	pad01 B32;
    CARD32	pad02 B32;
    CARD32	pad03 B32;
    CARD32	pad04 B32;
} xGEQueryVersionReply;

#define sz_xGEQueryVersionReply    32

#endif /* _GEPROTO_H_ */

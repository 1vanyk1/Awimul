#ifndef _SYNCSDK_H_
#define _SYNCSDK_H_

#include "../mi/misync.h"

extern _X_EXPORT int
SyncVerifyFence(SyncFence ** ppFence, XID fid, ClientPtr client, Mask mode);

extern _X_EXPORT SyncObject*
SyncCreate(ClientPtr client, XID id, unsigned char type);

#define VERIFY_SYNC_FENCE(pFence, fid, client, mode)			\
    do {								\
	int rc;								\
	rc = SyncVerifyFence(&(pFence), (fid), (client), (mode));	\
	if (Success != rc) return rc;					\
    } while (0)

#define VERIFY_SYNC_FENCE_OR_NONE(pFence, fid, client, mode)		\
    do {								\
        pFence = 0;							\
        if (None != fid)						\
	    VERIFY_SYNC_FENCE((pFence), (fid), (client), (mode));	\
    } while (0)

#endif                          /* _SYNCSDK_H_ */

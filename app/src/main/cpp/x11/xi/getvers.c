#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "getvers.h"

XExtensionVersion XIVersion;

/***********************************************************************
 *
 * Handle a request from a client with a different byte order than us.
 *
 */

int _X_COLD
SProcXGetExtensionVersion(ClientPtr client)
{
    REQUEST(xGetExtensionVersionReq);
    swaps(&stuff->length);
    REQUEST_AT_LEAST_SIZE(xGetExtensionVersionReq);
    swaps(&stuff->nbytes);
    return (ProcXGetExtensionVersion(client));
}

/***********************************************************************
 *
 * This procedure returns the major/minor version of the X Input extension.
 *
 */

int
ProcXGetExtensionVersion(ClientPtr client)
{
    xGetExtensionVersionReply rep;

    REQUEST(xGetExtensionVersionReq);
    REQUEST_AT_LEAST_SIZE(xGetExtensionVersionReq);

    if (stuff->length != bytes_to_int32(sizeof(xGetExtensionVersionReq) +
                                        stuff->nbytes))
        return BadLength;

    rep = (xGetExtensionVersionReply) {
            .repType = X_Reply,
            .RepType = X_GetExtensionVersion,
            .sequenceNumber = client->sequence,
            .length = 0,
            .major_version = XIVersion.major_version,
            .minor_version = XIVersion.minor_version,
            .present = TRUE
    };

    WriteReplyToClient(client, sizeof(xGetExtensionVersionReply), &rep);

    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetExtensionVersion function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetExtensionVersion(ClientPtr client, int size,
                         xGetExtensionVersionReply * rep)
{
    swaps(&rep->sequenceNumber);
    swapl(&rep->length);
    swaps(&rep->major_version);
    swaps(&rep->minor_version);
    WriteToClient(client, size, rep);
}

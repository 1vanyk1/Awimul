#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"
#include "../headers/swaprep.h"
#include "../headers/xkbsrv.h"
#include "../headers/xkbstr.h"

#include "getkmap.h"

/***********************************************************************
 *
 * This procedure gets the key mapping for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int _X_COLD
SProcXGetDeviceKeyMapping(ClientPtr client)
{
REQUEST(xGetDeviceKeyMappingReq);
swaps(&stuff->length);
return (ProcXGetDeviceKeyMapping(client));
}

/***********************************************************************
 *
 * Get the device key mapping.
 *
 */

int
ProcXGetDeviceKeyMapping(ClientPtr client)
{
    xGetDeviceKeyMappingReply rep;
    DeviceIntPtr dev;
    XkbDescPtr xkb;
    KeySymsPtr syms;
    int rc;

    REQUEST(xGetDeviceKeyMappingReq);
    REQUEST_SIZE_MATCH(xGetDeviceKeyMappingReq);

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    if (dev->key == NULL)
        return BadMatch;
    xkb = dev->key->xkbInfo->desc;

    if (stuff->firstKeyCode < xkb->min_key_code ||
        stuff->firstKeyCode > xkb->max_key_code) {
        client->errorValue = stuff->firstKeyCode;
        return BadValue;
    }

    if (stuff->firstKeyCode + stuff->count > xkb->max_key_code + 1) {
        client->errorValue = stuff->count;
        return BadValue;
    }

    syms = XkbGetCoreMap(dev);
    if (!syms)
        return BadAlloc;

    rep = (xGetDeviceKeyMappingReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceKeyMapping,
            .sequenceNumber = client->sequence,
            .keySymsPerKeyCode = syms->mapWidth,
            .length = (syms->mapWidth * stuff->count) /* KeySyms are 4 bytes */
    };
    WriteReplyToClient(client, sizeof(xGetDeviceKeyMappingReply), &rep);

    client->pSwapReplyFunc = (ReplySwapPtr) CopySwap32Write;
    WriteSwappedDataToClient(client,
                             syms->mapWidth * stuff->count * sizeof(KeySym),
                             &syms->map[syms->mapWidth * (stuff->firstKeyCode -
                                                          syms->minKeyCode)]);
    free(syms->map);
    free(syms);

    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceKeyMapping function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceKeyMapping(ClientPtr client, int size,
        xGetDeviceKeyMappingReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}

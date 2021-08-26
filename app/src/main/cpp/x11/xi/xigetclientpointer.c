#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"              /* for inputstr.h    */
#include "../headers/xproto.h"         /* Request macro     */
#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structure  */
#include "../headers/scrnintstr.h"         /* screen structure  */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XI2proto.h"
#include "../headers/extnsionst.h"
#include "../headers/extinit.h"            /* LookupDeviceIntRec */
#include "../headers/exevents.h"
#include "../headers/exglobals.h"

#include "xigetclientpointer.h"

/***********************************************************************
 * This procedure allows a client to query another client's client pointer
 * setting.
 */

int _X_COLD
SProcXIGetClientPointer(ClientPtr client)
{
REQUEST(xXIGetClientPointerReq);
REQUEST_SIZE_MATCH(xXIGetClientPointerReq);

swaps(&stuff->length);
swapl(&stuff->win);
return ProcXIGetClientPointer(client);
}

int
ProcXIGetClientPointer(ClientPtr client)
{
    int rc;
    ClientPtr winclient;
    xXIGetClientPointerReply rep;

    REQUEST(xXIGetClientPointerReq);
    REQUEST_SIZE_MATCH(xXIGetClientPointerReq);

    if (stuff->win != None) {
        rc = dixLookupClient(&winclient, stuff->win, client, DixGetAttrAccess);

        if (rc != Success)
            return BadWindow;
    }
    else
        winclient = client;

    rep = (xXIGetClientPointerReply) {
            .repType = X_Reply,
            .RepType = X_XIGetClientPointer,
            .sequenceNumber = client->sequence,
            .length = 0,
            .set = (winclient->clientPtr != NULL),
            .deviceid = (winclient->clientPtr) ? winclient->clientPtr->id : 0
    };

    WriteReplyToClient(client, sizeof(xXIGetClientPointerReply), &rep);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetClientPointer function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXIGetClientPointer(ClientPtr client, int size,
        xXIGetClientPointerReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
swaps(&rep->deviceid);
WriteToClient(client, size, rep);
}

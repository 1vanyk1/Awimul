#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window struct     */
#include "../headers/exglobals.h"
#include "../headers/swaprep.h"

#include "getprop.h"
#include "getselev.h"

/***********************************************************************
 *
 * This procedure gets the current selected extension events.
 *
 */

int _X_COLD
SProcXGetSelectedExtensionEvents(ClientPtr client)
{
REQUEST(xGetSelectedExtensionEventsReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xGetSelectedExtensionEventsReq);
swapl(&stuff->window);
return (ProcXGetSelectedExtensionEvents(client));
}

/***********************************************************************
 *
 * This procedure gets the current device select mask,
 * if the client and server have a different byte ordering.
 *
 */

int
ProcXGetSelectedExtensionEvents(ClientPtr client)
{
    int i, rc, total_length = 0;
    xGetSelectedExtensionEventsReply rep;
    WindowPtr pWin;
    XEventClass *buf = NULL;
    XEventClass *tclient;
    XEventClass *aclient;
    OtherInputMasks *pOthers;
    InputClientsPtr others;

    REQUEST(xGetSelectedExtensionEventsReq);
    REQUEST_SIZE_MATCH(xGetSelectedExtensionEventsReq);

    rep = (xGetSelectedExtensionEventsReply) {
            .repType = X_Reply,
            .RepType = X_GetSelectedExtensionEvents,
            .sequenceNumber = client->sequence,
            .length = 0,
            .this_client_count = 0,
            .all_clients_count = 0
    };

    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    if ((pOthers = wOtherInputMasks(pWin)) != 0) {
        for (others = pOthers->inputClients; others; others = others->next)
            for (i = 0; i < EMASKSIZE; i++)
                ClassFromMask(NULL, others->mask[i], i,
                              &rep.all_clients_count, COUNT);

        for (others = pOthers->inputClients; others; others = others->next)
            if (SameClient(others, client)) {
                for (i = 0; i < EMASKSIZE; i++)
                    ClassFromMask(NULL, others->mask[i], i,
                                  &rep.this_client_count, COUNT);
                break;
            }

        total_length = (rep.all_clients_count + rep.this_client_count) *
                       sizeof(XEventClass);
        rep.length = bytes_to_int32(total_length);
        buf = (XEventClass *) malloc(total_length);

        tclient = buf;
        aclient = buf + rep.this_client_count;
        if (others)
            for (i = 0; i < EMASKSIZE; i++)
                tclient =
                        ClassFromMask(tclient, others->mask[i], i, NULL, CREATE);

        for (others = pOthers->inputClients; others; others = others->next)
            for (i = 0; i < EMASKSIZE; i++)
                aclient =
                        ClassFromMask(aclient, others->mask[i], i, NULL, CREATE);
    }

    WriteReplyToClient(client, sizeof(xGetSelectedExtensionEventsReply), &rep);

    if (total_length) {
        client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
        WriteSwappedDataToClient(client, total_length, buf);
    }
    free(buf);
    return Success;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetSelectedExtensionEvents function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetSelectedExtensionEvents(ClientPtr client, int size,
        xGetSelectedExtensionEventsReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
swaps(&rep->this_client_count);
swaps(&rep->all_clients_count);
WriteToClient(client, size, rep);
}

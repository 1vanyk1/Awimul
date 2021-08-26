#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/windowstr.h"          /* window structs    */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"
#include "../headers/swaprep.h"

#include "getprop.h"

extern XExtEventInfo EventInfo[];
extern int ExtEventIndex;

/***********************************************************************
 *
 * Handle a request from a client with a different byte order.
 *
 */

int _X_COLD
SProcXGetDeviceDontPropagateList(ClientPtr client)
{
REQUEST(xGetDeviceDontPropagateListReq);
swaps(&stuff->length);
REQUEST_SIZE_MATCH(xGetDeviceDontPropagateListReq);
swapl(&stuff->window);
return (ProcXGetDeviceDontPropagateList(client));
}

/***********************************************************************
 *
 * This procedure lists the input devices available to the server.
 *
 */

int
ProcXGetDeviceDontPropagateList(ClientPtr client)
{
    CARD16 count = 0;
    int i, rc;
    XEventClass *buf = NULL, *tbuf;
    WindowPtr pWin;
    xGetDeviceDontPropagateListReply rep;
    OtherInputMasks *others;

    REQUEST(xGetDeviceDontPropagateListReq);
    REQUEST_SIZE_MATCH(xGetDeviceDontPropagateListReq);

    rep = (xGetDeviceDontPropagateListReply) {
            .repType = X_Reply,
            .RepType = X_GetDeviceDontPropagateList,
            .sequenceNumber = client->sequence,
            .length = 0,
            .count = 0
    };

    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    if ((others = wOtherInputMasks(pWin)) != 0) {
        for (i = 0; i < EMASKSIZE; i++)
            ClassFromMask(NULL, others->dontPropagateMask[i], i, &count, COUNT);
        if (count) {
            rep.count = count;
            buf = xallocarray(rep.count, sizeof(XEventClass));
            rep.length = bytes_to_int32(rep.count * sizeof(XEventClass));

            tbuf = buf;
            for (i = 0; i < EMASKSIZE; i++)
                tbuf = ClassFromMask(tbuf, others->dontPropagateMask[i], i,
                                     NULL, CREATE);
        }
    }

    WriteReplyToClient(client, sizeof(xGetDeviceDontPropagateListReply), &rep);

    if (count) {
        client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
        WriteSwappedDataToClient(client, count * sizeof(XEventClass), buf);
        free(buf);
    }
    return Success;
}

/***********************************************************************
 *
 * This procedure gets a list of event classes from a mask word.
 * A single mask may translate to more than one event class.
 *
 */

XEventClass
* ClassFromMask(XEventClass * buf, Mask mask, int maskndx, CARD16 *count,
                int mode)
{
    int i, j;
    int id = maskndx;
    Mask tmask = 0x80000000;

    for (i = 0; i < 32; i++, tmask >>= 1)
        if (tmask & mask) {
            for (j = 0; j < ExtEventIndex; j++)
                if (EventInfo[j].mask == tmask) {
                    if (mode == COUNT)
                        (*count)++;
                    else
                        *buf++ = (id << 8) | EventInfo[j].type;
                }
        }
    return buf;
}

/***********************************************************************
 *
 * This procedure writes the reply for the XGetDeviceDontPropagateList function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXGetDeviceDontPropagateList(ClientPtr client, int size,
        xGetDeviceDontPropagateListReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
swaps(&rep->count);
WriteToClient(client, size, rep);
}

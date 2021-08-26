#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/swaprep.h"
#include "../headers/extensions/xcmiscproto.h"
#include "../headers/extinit.h"

#include <stdint.h>

static int
ProcXCMiscGetVersion(ClientPtr client)
{
    xXCMiscGetVersionReply rep = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .majorVersion = XCMiscMajorVersion,
            .minorVersion = XCMiscMinorVersion
    };

    REQUEST_SIZE_MATCH(xXCMiscGetVersionReq);

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swaps(&rep.majorVersion);
        swaps(&rep.minorVersion);
    }
    WriteToClient(client, sizeof(xXCMiscGetVersionReply), &rep);
    return Success;
}

static int
ProcXCMiscGetXIDRange(ClientPtr client)
{
    xXCMiscGetXIDRangeReply rep;
    XID min_id, max_id;

    REQUEST_SIZE_MATCH(xXCMiscGetXIDRangeReq);
    GetXIDRange(client->index, FALSE, &min_id, &max_id);
    rep = (xXCMiscGetXIDRangeReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .start_id = min_id,
            .count = max_id - min_id + 1
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.start_id);
        swapl(&rep.count);
    }
    WriteToClient(client, sizeof(xXCMiscGetXIDRangeReply), &rep);
    return Success;
}

static int
ProcXCMiscGetXIDList(ClientPtr client)
{
    REQUEST(xXCMiscGetXIDListReq);
    xXCMiscGetXIDListReply rep;
    XID *pids;
    unsigned int count;

    REQUEST_SIZE_MATCH(xXCMiscGetXIDListReq);

    if (stuff->count > UINT32_MAX / sizeof(XID))
        return BadAlloc;

    pids = xallocarray(stuff->count, sizeof(XID));
    if (!pids) {
        return BadAlloc;
    }
    count = GetXIDList(client, stuff->count, pids);
    rep = (xXCMiscGetXIDListReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = count,
            .count = count
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.count);
    }
    WriteToClient(client, sizeof(xXCMiscGetXIDListReply), &rep);
    if (count) {
        client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
        WriteSwappedDataToClient(client, count * sizeof(XID), pids);
    }
    free(pids);
    return Success;
}

static int
ProcXCMiscDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
        case X_XCMiscGetVersion:
            return ProcXCMiscGetVersion(client);
        case X_XCMiscGetXIDRange:
            return ProcXCMiscGetXIDRange(client);
        case X_XCMiscGetXIDList:
            return ProcXCMiscGetXIDList(client);
        default:
            return BadRequest;
    }
}

static int _X_COLD
SProcXCMiscGetVersion(ClientPtr client)
{
REQUEST(xXCMiscGetVersionReq);

swaps(&stuff->length);
REQUEST_SIZE_MATCH(xXCMiscGetVersionReq);
swaps(&stuff->majorVersion);
swaps(&stuff->minorVersion);
return ProcXCMiscGetVersion(client);
}

static int _X_COLD
SProcXCMiscGetXIDRange(ClientPtr client)
{
REQUEST(xReq);

swaps(&stuff->length);
return ProcXCMiscGetXIDRange(client);
}

static int _X_COLD
SProcXCMiscGetXIDList(ClientPtr client)
{
REQUEST(xXCMiscGetXIDListReq);
REQUEST_SIZE_MATCH(xXCMiscGetXIDListReq);

swaps(&stuff->length);
swapl(&stuff->count);
return ProcXCMiscGetXIDList(client);
}

static int _X_COLD
SProcXCMiscDispatch(ClientPtr client)
{
REQUEST(xReq);
switch (stuff->data) {
case X_XCMiscGetVersion:
return SProcXCMiscGetVersion(client);
case X_XCMiscGetXIDRange:
return SProcXCMiscGetXIDRange(client);
case X_XCMiscGetXIDList:
return SProcXCMiscGetXIDList(client);
default:
return BadRequest;
}
}

void
XCMiscExtensionInit(void)
{
    AddExtension(XCMiscExtensionName, 0, 0,
                 ProcXCMiscDispatch, SProcXCMiscDispatch,
                 NULL, StandardMinorOpcode);
}

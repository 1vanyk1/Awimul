#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xproto.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/dixstruct.h"
#include "../headers/extnsionst.h"
#include "../headers/extensions/bigreqsproto.h"
#include "../headers/opaque.h"
#include "../headers/extinit.h"

static int
ProcBigReqDispatch(ClientPtr client)
{
    REQUEST(xBigReqEnableReq);
    xBigReqEnableReply rep;

    if (client->swapped) {
        swaps(&stuff->length);
    }
    if (stuff->brReqType != X_BigReqEnable)
        return BadRequest;
    REQUEST_SIZE_MATCH(xBigReqEnableReq);
    client->big_requests = TRUE;
    rep = (xBigReqEnableReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .max_request_size = maxBigRequestSize
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.max_request_size);
    }
    WriteToClient(client, sizeof(xBigReqEnableReply), &rep);
    return Success;
}

void
BigReqExtensionInit(void)
{
    AddExtension(XBigReqExtensionName, 0, 0,
                 ProcBigReqDispatch, ProcBigReqDispatch,
                 NULL, StandardMinorOpcode);
}

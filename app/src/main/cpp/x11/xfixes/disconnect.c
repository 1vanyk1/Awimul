#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "xfixesint.h"
#include "../headers/opaque.h"

static DevPrivateKeyRec ClientDisconnectPrivateKeyRec;

#define ClientDisconnectPrivateKey (&ClientDisconnectPrivateKeyRec)

typedef struct _ClientDisconnect {
    int disconnect_mode;
} ClientDisconnectRec, *ClientDisconnectPtr;

#define GetClientDisconnect(s) \
    ((ClientDisconnectPtr) dixLookupPrivate(&(s)->devPrivates, \
                                            ClientDisconnectPrivateKey))

int
ProcXFixesSetClientDisconnectMode(ClientPtr client)
{
    ClientDisconnectPtr pDisconnect = GetClientDisconnect(client);

    REQUEST(xXFixesSetClientDisconnectModeReq);

    pDisconnect->disconnect_mode = stuff->disconnect_mode;

    return Success;
}

int _X_COLD
SProcXFixesSetClientDisconnectMode(ClientPtr client)
{
    REQUEST(xXFixesSetClientDisconnectModeReq);

    swaps(&stuff->length);

    REQUEST_AT_LEAST_SIZE(xXFixesSetClientDisconnectModeReq);

    swapl(&stuff->disconnect_mode);

    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}

int
ProcXFixesGetClientDisconnectMode(ClientPtr client)
{
    ClientDisconnectPtr pDisconnect = GetClientDisconnect(client);
    xXFixesGetClientDisconnectModeReply reply;

    REQUEST_SIZE_MATCH(xXFixesGetClientDisconnectModeReq);

    reply = (xXFixesGetClientDisconnectModeReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .disconnect_mode = pDisconnect->disconnect_mode,
    };
    if (client->swapped) {
        swaps(&reply.sequenceNumber);
        swapl(&reply.disconnect_mode);
    }
    WriteToClient(client, sizeof(xXFixesGetClientDisconnectModeReply), &reply);

    return Success;
}

int _X_COLD
SProcXFixesGetClientDisconnectMode(ClientPtr client)
{
    REQUEST(xXFixesGetClientDisconnectModeReq);

    swaps(&stuff->length);

    REQUEST_SIZE_MATCH(xXFixesGetClientDisconnectModeReq);

    return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
}

Bool
XFixesShouldDisconnectClient(ClientPtr client)
{
    ClientDisconnectPtr pDisconnect = GetClientDisconnect(client);

    if (!pDisconnect)
        return FALSE;

    if (dispatchExceptionAtReset & DE_TERMINATE)
        return (pDisconnect->disconnect_mode & XFixesClientDisconnectFlagTerminate);

    return FALSE;
}

Bool
XFixesClientDisconnectInit(void)
{
    if (!dixRegisterPrivateKey(&ClientDisconnectPrivateKeyRec,
                               PRIVATE_CLIENT, sizeof(ClientDisconnectRec)))
        return FALSE;

    return TRUE;
}

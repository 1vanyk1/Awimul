#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ICElib.h"
#include "ICElibint.h"

Status
IcePing (
        IceConn		 iceConn,
        IcePingReplyProc pingReplyProc,
        IcePointer	 clientData
)
{
    _IcePingWait *newping = malloc (sizeof (_IcePingWait));
    _IcePingWait *ptr = iceConn->ping_waits;

    if (newping == NULL)
        return (0);

    newping->ping_reply_proc = pingReplyProc;
    newping->client_data = clientData;
    newping->next = NULL;

    while (ptr && ptr->next)
        ptr = ptr->next;

    if (ptr == NULL)
        iceConn->ping_waits = newping;
    else
        ptr->next = newping;

    IceSimpleMessage (iceConn, 0, ICE_Ping);
    IceFlush (iceConn);

    return (1);
}

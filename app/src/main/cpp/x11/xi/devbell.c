#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/exglobals.h"

#include "devbell.h"

/***********************************************************************
 *
 * This procedure is invoked to swap the request bytes if the server and
 * client have a different byte order.
 *
 */

int _X_COLD
SProcXDeviceBell(ClientPtr client)
{
REQUEST(xDeviceBellReq);
swaps(&stuff->length);
return (ProcXDeviceBell(client));
}

/***********************************************************************
 *
 * This procedure rings a bell on an extension device.
 *
 */

int
ProcXDeviceBell(ClientPtr client)
{
    DeviceIntPtr dev;
    KbdFeedbackPtr k;
    BellFeedbackPtr b;
    int rc, base;
    int newpercent;
    CARD8 class;
    void *ctrl;
    BellProcPtr proc;

    REQUEST(xDeviceBellReq);
    REQUEST_SIZE_MATCH(xDeviceBellReq);

    rc = dixLookupDevice(&dev, stuff->deviceid, client, DixBellAccess);
    if (rc != Success) {
        client->errorValue = stuff->deviceid;
        return rc;
    }

    if (stuff->percent < -100 || stuff->percent > 100) {
        client->errorValue = stuff->percent;
        return BadValue;
    }
    if (stuff->feedbackclass == KbdFeedbackClass) {
        for (k = dev->kbdfeed; k; k = k->next)
            if (k->ctrl.id == stuff->feedbackid)
                break;
        if (!k) {
            client->errorValue = stuff->feedbackid;
            return BadValue;
        }
        base = k->ctrl.bell;
        proc = k->BellProc;
        ctrl = (void *) &(k->ctrl);
        class = KbdFeedbackClass;
    }
    else if (stuff->feedbackclass == BellFeedbackClass) {
        for (b = dev->bell; b; b = b->next)
            if (b->ctrl.id == stuff->feedbackid)
                break;
        if (!b) {
            client->errorValue = stuff->feedbackid;
            return BadValue;
        }
        base = b->ctrl.percent;
        proc = b->BellProc;
        ctrl = (void *) &(b->ctrl);
        class = BellFeedbackClass;
    }
    else {
        client->errorValue = stuff->feedbackclass;
        return BadValue;
    }
    newpercent = (base * stuff->percent) / 100;
    if (stuff->percent < 0)
        newpercent = base + newpercent;
    else
        newpercent = base - newpercent + stuff->percent;
    if (proc == NULL)
        return BadValue;
    (*proc) (newpercent, dev, ctrl, class);
    return Success;
}

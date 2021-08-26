#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/inputstr.h"           /* DeviceIntPtr      */
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"     /* control constants */
#include "../headers/XIstubs.h"

#include "../headers/exglobals.h"
#include "../headers/exevents.h"

#include "chgdctl.h"

/***********************************************************************
 *
 * This procedure changes the control attributes for an extension device,
 * for clients on machines with a different byte ordering than the server.
 *
 */

int _X_COLD
SProcXChangeDeviceControl(ClientPtr client)
{
xDeviceCtl *ctl;

REQUEST(xChangeDeviceControlReq);
swaps(&stuff->length);
REQUEST_AT_LEAST_EXTRA_SIZE(xChangeDeviceControlReq, sizeof(xDeviceCtl));
swaps(&stuff->control);
ctl = (xDeviceCtl *) &stuff[1];
swaps(&ctl->control);
swaps(&ctl->length);
switch (stuff->control) {
case DEVICE_ABS_CALIB:
case DEVICE_ABS_AREA:
case DEVICE_CORE:
case DEVICE_ENABLE:
case DEVICE_RESOLUTION:
/* hmm. beer. *drool* */
break;

}
return (ProcXChangeDeviceControl(client));
}

/***********************************************************************
 *
 * Change the control attributes.
 *
 */

int
ProcXChangeDeviceControl(ClientPtr client)
{
    unsigned len;
    int i, status, ret = BadValue;
    DeviceIntPtr dev;
    xDeviceResolutionCtl *r;
    xChangeDeviceControlReply rep;
    AxisInfoPtr a;
    CARD32 *resolution;
    xDeviceEnableCtl *e;

    REQUEST(xChangeDeviceControlReq);
    REQUEST_AT_LEAST_EXTRA_SIZE(xChangeDeviceControlReq, sizeof(xDeviceCtl));

    len = stuff->length - bytes_to_int32(sizeof(xChangeDeviceControlReq));
    ret = dixLookupDevice(&dev, stuff->deviceid, client, DixManageAccess);
    if (ret != Success)
        goto out;

    /* XTest devices are special, none of the below apply to them anyway */
    if (IsXTestDevice(dev, NULL)) {
        ret = BadMatch;
        goto out;
    }

    rep = (xChangeDeviceControlReply) {
            .repType = X_Reply,
            .RepType = X_ChangeDeviceControl,
            .sequenceNumber = client->sequence,
            .length = 0,
            .status = Success,
    };

    switch (stuff->control) {
        case DEVICE_RESOLUTION:
            r = (xDeviceResolutionCtl *) &stuff[1];
            if ((len < bytes_to_int32(sizeof(xDeviceResolutionCtl))) ||
                (len !=
                 bytes_to_int32(sizeof(xDeviceResolutionCtl)) + r->num_valuators)) {
                ret = BadLength;
                goto out;
            }
            if (!dev->valuator) {
                ret = BadMatch;
                goto out;
            }
            if ((dev->deviceGrab.grab) && !SameClient(dev->deviceGrab.grab, client)) {
                rep.status = AlreadyGrabbed;
                ret = Success;
                goto out;
            }
            resolution = (CARD32 *) (r + 1);
            if (r->first_valuator + r->num_valuators > dev->valuator->numAxes) {
                ret = BadValue;
                goto out;
            }
            status = ChangeDeviceControl(client, dev, (xDeviceCtl *) r);
            if (status == Success) {
                a = &dev->valuator->axes[r->first_valuator];
                for (i = 0; i < r->num_valuators; i++)
                    if (*(resolution + i) < (a + i)->min_resolution ||
                        *(resolution + i) > (a + i)->max_resolution)
                        return BadValue;
                for (i = 0; i < r->num_valuators; i++)
                    (a++)->resolution = *resolution++;

                ret = Success;
            }
            else if (status == DeviceBusy) {
                rep.status = DeviceBusy;
                ret = Success;
            }
            else {
                ret = BadMatch;
            }
            break;
        case DEVICE_ABS_CALIB:
        case DEVICE_ABS_AREA:
            /* Calibration is now done through properties, and never had any effect
             * on anything (in the open-source world). Thus, be honest. */
            ret = BadMatch;
            break;
        case DEVICE_CORE:
            /* Sorry, no device core switching no more. If you want a device to
             * send core events, attach it to a master device */
            ret = BadMatch;
            break;
        case DEVICE_ENABLE:
            e = (xDeviceEnableCtl *) &stuff[1];
            if ((len != bytes_to_int32(sizeof(xDeviceEnableCtl)))) {
                ret = BadLength;
                goto out;
            }

            if (IsXTestDevice(dev, NULL))
                status = !Success;
            else
                status = ChangeDeviceControl(client, dev, (xDeviceCtl *) e);

            if (status == Success) {
                if (e->enable)
                    EnableDevice(dev, TRUE);
                else
                    DisableDevice(dev, TRUE);
                ret = Success;
            }
            else if (status == DeviceBusy) {
                rep.status = DeviceBusy;
                ret = Success;
            }
            else {
                ret = BadMatch;
            }

            break;
        default:
            ret = BadValue;
    }

    out:
    if (ret == Success) {
        devicePresenceNotify dpn = {
                .type = DevicePresenceNotify,
                .time = currentTime.milliseconds,
                .devchange = DeviceControlChanged,
                .deviceid = dev->id,
                .control = stuff->control
        };
        SendEventToAllWindows(dev, DevicePresenceNotifyMask,
                              (xEvent *) &dpn, 1);

        WriteReplyToClient(client, sizeof(xChangeDeviceControlReply), &rep);
    }

    return ret;
}

/***********************************************************************
 *
 * This procedure writes the reply for the xChangeDeviceControl function,
 * if the client and server have a different byte ordering.
 *
 */

void _X_COLD
SRepXChangeDeviceControl(ClientPtr client, int size,
        xChangeDeviceControlReply * rep)
{
swaps(&rep->sequenceNumber);
swapl(&rep->length);
WriteToClient(client, size, rep);
}

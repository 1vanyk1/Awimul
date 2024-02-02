#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XSetDeviceValuators(
        register Display	*dpy,
        XDevice		*dev,
        int			*valuators,
        int			 first_valuator,
        int			 num_valuators)
{
    xSetDeviceValuatorsReq *req;
    xSetDeviceValuatorsReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Add_XSetDeviceValuators, info) == -1)
        return (NoSuchExtension);

    GetReq(SetDeviceValuators, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_SetDeviceValuators;
    req->deviceid = dev->device_id;
    req->first_valuator = first_valuator;
    req->num_valuators = num_valuators;
    req->length += num_valuators;

    /* note: Data is a macro that uses its arguments multiple
     * times, so "nvalues" is changed in a separate assignment
     * statement */

    num_valuators <<= 2;
    Data(dpy, (char *)valuators, num_valuators);

    (void)_XReply(dpy, (xReply *) & rep, 0, xTrue);
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.status);
}

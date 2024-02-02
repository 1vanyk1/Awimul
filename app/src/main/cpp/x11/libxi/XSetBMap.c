#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

/* returns either  DeviceMappingSuccess or DeviceMappingBusy  */

int
XSetDeviceButtonMapping(
        register Display	*dpy,
        XDevice		*device,
        unsigned char	 map[],
        int			 nmap)
{
    register xSetDeviceButtonMappingReq *req;
    xSetDeviceButtonMappingReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);
    GetReq(SetDeviceButtonMapping, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_SetDeviceButtonMapping;
    req->map_length = nmap;
    req->length += (nmap + 3) >> 2;
    req->deviceid = device->device_id;

    Data(dpy, (char *)map, (long)nmap);	/* note that map is char[] */
    if (_XReply(dpy, (xReply *) & rep, 0, xFalse) == 0)	/* suppress error   */
        rep.status = MappingSuccess;
    UnlockDisplay(dpy);
    SyncHandle();
    return ((int)rep.status);
}

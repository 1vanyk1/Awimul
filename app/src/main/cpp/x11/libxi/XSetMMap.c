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
XSetDeviceModifierMapping(
        register Display	*dpy,
        XDevice		*dev,
        XModifierKeymap	*modmap)
{
    int mapSize = modmap->max_keypermod << 3;	/* 8 modifiers */
    xSetDeviceModifierMappingReq *req;
    xSetDeviceModifierMappingReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReqExtra(SetDeviceModifierMapping, mapSize, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_SetDeviceModifierMapping;
    req->deviceid = dev->device_id;
    req->numKeyPerModifier = modmap->max_keypermod;
    memcpy((char *)&req[1], modmap->modifiermap, mapSize);

    (void)_XReply(dpy, (xReply *) & rep,
                  (sizeof(xSetDeviceModifierMappingReply) -
                   sizeof(xReply)) >> 2, xTrue);

    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.success);
}

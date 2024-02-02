#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"
#include <limits.h>

#ifdef MIN	/* some systems define this in <sys/param.h> */
#undef MIN
#endif
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int
XGetDeviceButtonMapping(
        register Display	*dpy,
        XDevice		*device,
        unsigned char	 map[],
        unsigned int	 nmap)
{
    int status = 0;
    unsigned char mapping[256];	/* known fixed size */
    XExtDisplayInfo *info = XInput_find_display(dpy);

    register xGetDeviceButtonMappingReq *req;
    xGetDeviceButtonMappingReply rep;

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);
    GetReq(GetDeviceButtonMapping, req);

    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceButtonMapping;
    req->deviceid = device->device_id;

    status = _XReply(dpy, (xReply *) & rep, 0, xFalse);
    if (status == 1) {
        if (rep.length <= (sizeof(mapping) >> 2) &&
            rep.nElts <= (rep.length << 2)) {
            unsigned long nbytes = rep.length << 2;
            _XRead(dpy, (char *)mapping, nbytes);

            /* don't return more data than the user asked for. */
            if (rep.nElts)
                memcpy(map, mapping, MIN((int)rep.nElts, nmap));
            status = rep.nElts;
        } else {
            _XEatDataWords(dpy, rep.length);
            status = 0;
        }
    } else
        status = 0;
    UnlockDisplay(dpy);
    SyncHandle();
    return (status);
}

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/xlibint.h"
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

void
XChangeDeviceProperty(Display* dpy, XDevice* dev,
                      Atom property, Atom type,
                      int format, int mode,
                      _Xconst unsigned char *data, int nelements)
{
    xChangeDevicePropertyReq   *req;
    int                         len;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return;

    GetReq(ChangeDeviceProperty, req);
    req->reqType    = info->codes->major_opcode;
    req->ReqType    = X_ChangeDeviceProperty;
    req->deviceid   = dev->device_id;
    req->property   = property;
    req->type       = type;
    req->mode       = mode;
    if (nelements < 0) {
        req->nUnits = 0;
        req->format = 0; /* ask for garbage, get garbage */
    } else {
        req->nUnits = nelements;
        req->format = format;
    }

    switch (req->format) {
        case 8:
            len = ((long)nelements + 3) >> 2;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                Data (dpy, (_Xconst char *)data, nelements);
            } /* else force BadLength */
            break;

        case 16:
            len = ((long)nelements + 1) >> 1;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                len = (long)nelements << 1;
                Data16 (dpy, (_Xconst short *) data, len);
            } /* else force BadLength */
            break;

        case 32:
            len = nelements;
            if (dpy->bigreq_size || req->length + len <= (unsigned) 65535) {
                SetReqLen(req, len, len);
                len = (long)nelements << 2;
                Data32 (dpy, (_Xconst long *) data, len);
            } /* else force BadLength */
            break;

        default:
            /* BadValue will be generated */ ;
    }

    UnlockDisplay(dpy);
    SyncHandle();
}


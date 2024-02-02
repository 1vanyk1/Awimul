#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/xlibint.h"
#include "../headers/extensions/XI.h"
#include "../headers/extensions/XIproto.h"
#include "../headers/extensions/XInput.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

Atom*
XListDeviceProperties(Display* dpy, XDevice* dev, int *nprops_return)
{
    xListDevicePropertiesReq    *req;
    xListDevicePropertiesReply  rep;
    XExtDisplayInfo             *info = XInput_find_display(dpy);
    Atom                        *props = NULL;

    LockDisplay(dpy);
    *nprops_return = 0;
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(ListDeviceProperties, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_ListDeviceProperties;
    req->deviceid = dev->device_id;

    if (!_XReply(dpy, (xReply*)&rep, 0, xFalse))
        goto cleanup;

    if (rep.nAtoms) {
        props = (Atom*)Xmalloc(rep.nAtoms * sizeof(Atom));
        if (!props)
        {
            _XEatDataWords(dpy, rep.length);
            goto cleanup;
        }

        _XRead32(dpy, (long*)props, rep.nAtoms << 2);
    }

    *nprops_return = rep.nAtoms;

    cleanup:
    UnlockDisplay(dpy);
    SyncHandle();
    return props;
}


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

XEventClass *
XGetDeviceDontPropagateList(
        register Display	*dpy,
        Window		 window,
        int			*count)
{
    XEventClass *list = NULL;
    xGetDeviceDontPropagateListReq *req;
    xGetDeviceDontPropagateListReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return NULL;

    GetReq(GetDeviceDontPropagateList, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetDeviceDontPropagateList;
    req->window = window;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return (XEventClass *) NULL;
    }
    *count = rep.count;

    if (rep.length != 0) {
        if ((rep.count != 0) && (rep.length < (INT_MAX / sizeof(XEventClass))))
            list = Xmalloc(rep.length * sizeof(XEventClass));
        if (list) {
            unsigned int i;
            CARD32 ec;

            /* read and assign each XEventClass separately because
             * the library representation may not be the same size
             * as the wire representation (64 bit machines)
             */
            for (i = 0; i < rep.length; i++) {
                _XRead(dpy, (char *)(&ec), sizeof(CARD32));
                list[i] = (XEventClass) ec;
            }
        } else {
            *count = 0;
            _XEatDataWords(dpy, rep.length);
        }
    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (list);
}

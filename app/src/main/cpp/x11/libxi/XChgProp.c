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
XChangeDeviceDontPropagateList(
        register Display	*dpy,
        Window		 window,
        int			 count,
        XEventClass		*events,
        int			 mode)
{
    xChangeDeviceDontPropagateListReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);

    GetReq(ChangeDeviceDontPropagateList, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_ChangeDeviceDontPropagateList;
    req->window = window;
    req->count = count;
    req->mode = mode;
    req->length += count;

    /* note: Data is a macro that uses its arguments multiple
     * times, so "nvalues" is changed in a separate assignment
     * statement */

    count <<= 2;
    Data32(dpy, (long *)events, count);

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}

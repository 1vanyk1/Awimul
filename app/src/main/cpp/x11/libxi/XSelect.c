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
XSelectExtensionEvent(
        register Display	*dpy,
        Window		 w,
        XEventClass		*event_list,
        int			 count)
{
    register xSelectExtensionEventReq *req;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);
    GetReq(SelectExtensionEvent, req);

    req->reqType = info->codes->major_opcode;
    req->ReqType = X_SelectExtensionEvent;
    req->window = w;
    req->count = count;
    req->length += count;

    /* note: Data is a macro that uses its arguments multiple
     * times, so "nvalues" is changed in a separate assignment
     * statement */

    count <<= 2;
    Data32(dpy, (long *)event_list, count);

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}

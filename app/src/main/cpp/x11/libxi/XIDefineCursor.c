#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"


int XIDefineCursor(Display *dpy, int deviceid, Window w, Cursor cursor)
{
    xXIChangeCursorReq *req;

    XExtDisplayInfo *info = XInput_find_display(dpy);
    LockDisplay(dpy);

    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return (NoSuchExtension);

    GetReq(XIChangeCursor, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIChangeCursor;
    req->deviceid = deviceid;
    req->win = w;
    req->cursor = cursor;
    UnlockDisplay(dpy);
    SyncHandle();
    return Success;
}

int XIUndefineCursor(Display *dpy, int deviceid, Window w)
{
    return XIDefineCursor(dpy, deviceid, w, None);
}


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include "../headers/extensions/XI2proto.h"
#include "../headers/xlibint.h"
#include "../headers/extensions/XInput2.h"
#include "../headers/extensions/extutil.h"
#include "XIint.h"

int
XIWarpPointer(Display      *dpy,
              int          deviceid,
              Window       src_win,
              Window       dst_win,
              double       src_x,
              double       src_y,
              unsigned int src_width,
              unsigned int src_height,
              double       dst_x,
              double       dst_y)
{
    xXIWarpPointerReq *req;

    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_2_0, info) == -1)
        return (NoSuchExtension);

    GetReq(XIWarpPointer, req);
    req->reqType = info->codes->major_opcode;
    req->ReqType = X_XIWarpPointer;
    req->deviceid = deviceid;
    req->src_win = src_win;
    req->dst_win = dst_win;
    req->src_x = (int)(src_x * 65536.0);
    req->src_y = (int)(src_y * 65536.0);
    req->src_width = src_width;
    req->src_height = src_height;
    req->dst_x = (int)(dst_x * 65536.0);
    req->dst_y = (int)(dst_y * 65536.0);


    UnlockDisplay(dpy);
    SyncHandle();
    return Success;
}

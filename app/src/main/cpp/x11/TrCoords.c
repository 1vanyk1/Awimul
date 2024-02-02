#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Bool XTranslateCoordinates(
        register Display *dpy,
        Window src_win,
        Window dest_win,
        int src_x,
        int src_y,
        int *dst_x,
        int *dst_y,
        Window *child)
{
    register xTranslateCoordsReq *req;
    xTranslateCoordsReply rep;

    LockDisplay(dpy);
    GetReq(TranslateCoords, req);
    req->srcWid = src_win;
    req->dstWid = dest_win;
    req->srcX = src_x;
    req->srcY = src_y;
    if (_XReply (dpy, (xReply *)&rep, 0, xTrue) == 0) {
        UnlockDisplay(dpy);
        SyncHandle();
        return(False);
    }

    *child = rep.child;
    *dst_x = cvtINT16toInt (rep.dstX);
    *dst_y = cvtINT16toInt (rep.dstY);
    UnlockDisplay(dpy);
    SyncHandle();
    return ((int)rep.sameScreen);
}

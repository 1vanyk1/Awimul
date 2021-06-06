#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDrawImageString(
        register Display *dpy,
        Drawable d,
        GC gc,
        int x,
        int y,
        _Xconst char *string,
        int length)
{
    register xImageText8Req *req;
    char *CharacterOffset = (char *)string;
    int FirstTimeThrough = True;
    int lastX = 0;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    while (length > 0)
    {
        int Unit;

        if (length > 255) Unit = 255;
        else Unit = length;

        if (FirstTimeThrough)
        {
            FirstTimeThrough = False;
        }
        else
        {
            char buf[512];
            char *ptr, *str;
            xQueryTextExtentsReq *qreq;
            xQueryTextExtentsReply rep;
            int i;

            GetReq(QueryTextExtents, qreq);
            qreq->fid = gc->gid;
            qreq->length += (510 + 3)>>2;
            qreq->oddLength = 1;
            str = CharacterOffset - 255;
            for (ptr = buf, i = 255; --i >= 0; ) {
                *ptr++ = 0;
                *ptr++ = *str++;
            }
            Data (dpy, buf, 510);
            if (!_XReply (dpy, (xReply *)&rep, 0, xTrue))
                break;

            x = lastX + cvtINT32toInt (rep.overallWidth);
        }

        GetReq (ImageText8, req);
        req->length += (Unit + 3) >> 2;
        req->nChars = Unit;
        req->drawable = d;
        req->gc = gc->gid;
        req->y = y;

        lastX = req->x = x;
        Data (dpy, CharacterOffset, (long)Unit);
        CharacterOffset += Unit;
        length -= Unit;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 0;
}
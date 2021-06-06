#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XDrawImageString16(
        register Display *dpy,
        Drawable d,
        GC gc,
        int x,
        int y,
        _Xconst XChar2b *string,
        int length)
{
    register xImageText16Req *req;
    XChar2b *CharacterOffset = (XChar2b *)string;
    int FirstTimeThrough = True;
    int lastX = 0;

    LockDisplay(dpy);
    FlushGC(dpy, gc);

    while (length > 0)
    {
        int Unit, Datalength;

        if (length > 255) Unit = 255;
        else Unit = length;

        if (FirstTimeThrough)
        {
            FirstTimeThrough = False;
        }
        else
        {
            char buf[512];
            xQueryTextExtentsReq *qreq;
            xQueryTextExtentsReply rep;
            unsigned char *ptr;
            XChar2b *str;
            int i;

            GetReq(QueryTextExtents, qreq);
            qreq->fid = gc->gid;
            qreq->length += (510 + 3)>>2;
            qreq->oddLength = 1;
            str = CharacterOffset - 255;
            for (ptr = (unsigned char *)buf, i = 255; --i >= 0; str++) {
            *ptr++ = str->byte1;
            *ptr++ = str->byte2;
            }
            Data (dpy, buf, 510);
            if (!_XReply (dpy, (xReply *)&rep, 0, xTrue))
            break;

            x = lastX + cvtINT32toInt (rep.overallWidth);
        }

        GetReq (ImageText16, req);
        req->length += ((Unit << 1) + 3) >> 2;
        req->nChars = Unit;
        req->drawable = d;
        req->gc = gc->gid;
        req->y = y;

        lastX = req->x = x;
        Datalength = Unit << 1;
        Data (dpy, (char *)CharacterOffset, (long)Datalength);
        CharacterOffset += Unit;
        length -= Unit;
    }
    UnlockDisplay(dpy);
    SyncHandle();
    return 0;
}
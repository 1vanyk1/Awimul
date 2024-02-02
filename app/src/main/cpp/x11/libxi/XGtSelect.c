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
XGetSelectedExtensionEvents(
        register Display	 *dpy,
        Window		  w,
        int			 *this_client_count,
        XEventClass		**this_client_list,
        int			 *all_clients_count,
        XEventClass		**all_clients_list)
{
    int tlen, alen;
    register xGetSelectedExtensionEventsReq *req;
    xGetSelectedExtensionEventsReply rep;
    XExtDisplayInfo *info = XInput_find_display(dpy);

    LockDisplay(dpy);
    if (_XiCheckExtInit(dpy, XInput_Initial_Release, info) == -1)
        return (NoSuchExtension);
    GetReq(GetSelectedExtensionEvents, req);

    req->reqType = info->codes->major_opcode;
    req->ReqType = X_GetSelectedExtensionEvents;
    req->window = w;

    if (!_XReply(dpy, (xReply *) & rep, 0, xFalse)) {
        UnlockDisplay(dpy);
        SyncHandle();
        return Success;
    }

    *this_client_count = rep.this_client_count;
    *all_clients_count = rep.all_clients_count;

    if (rep.length) {
        int i;
        CARD32 ec;

        tlen = (*this_client_count) * sizeof(CARD32);
        alen = (rep.length << 2) - tlen;

        if (tlen) {
            *this_client_list =
                    (XEventClass *) Xmalloc(*this_client_count *
                                            sizeof(XEventClass));
            if (!*this_client_list) {
                _XEatDataWords(dpy, rep.length);
                UnlockDisplay(dpy);
                SyncHandle();
                return (Success);
            }
            for (i = 0; i < *this_client_count; i++) {
                _XRead(dpy, (char *)(&ec), sizeof(CARD32));
                (*this_client_list)[i] = (XEventClass) ec;
            }
        } else
            *this_client_list = (XEventClass *) NULL;
        if (alen) {
            *all_clients_list =
                    (XEventClass *) Xmalloc(*all_clients_count *
                                            sizeof(XEventClass));
            if (!*all_clients_list) {
                Xfree((char *)*this_client_list);
                *this_client_list = NULL;
                _XEatData(dpy, (unsigned long)alen);
                UnlockDisplay(dpy);
                SyncHandle();
                return (Success);
            }
            for (i = 0; i < *all_clients_count; i++) {
                _XRead(dpy, (char *)(&ec), sizeof(CARD32));
                (*all_clients_list)[i] = (XEventClass) ec;
            }
        } else
            *all_clients_list = (XEventClass *) NULL;

    }

    UnlockDisplay(dpy);
    SyncHandle();
    return (Success);
}

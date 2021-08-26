#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include <stdbool.h>
#include "headers/xlibint.h"

Bool
XQueryExtension(
        register Display *dpy,
        _Xconst char *name,
        int *major_opcode,  /* RETURN */
        int *first_event,   /* RETURN */
        int *first_error)	/* RETURN */
{
    xQueryExtensionReply rep;
    register xQueryExtensionReq *req;

    if (name != NULL && strlen(name) >= USHRT_MAX)
        return false;

    LockDisplay(dpy);
    GetReq(QueryExtension, req);
    req->nbytes = name ? (CARD16) strlen(name) : 0;
    req->length += (req->nbytes+(unsigned)3)>>2;
    _XSend(dpy, name, (long)req->nbytes);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *major_opcode = rep.major_opcode;
    *first_event = rep.first_event;
    *first_error = rep.first_error;
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.present);
}


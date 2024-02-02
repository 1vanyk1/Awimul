#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xfixesint.h"

void
XFixesSelectSelectionInput (Display	    *dpy,
                            Window	    win,
                            Atom	    selection,
                            unsigned long   eventMask)
{
    XFixesExtDisplayInfo	    *info = XFixesFindDisplay (dpy);
    xXFixesSelectSelectionInputReq  *req;

    XFixesSimpleCheckExtension (dpy, info);

    LockDisplay (dpy);
    GetReq (XFixesSelectSelectionInput, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSelectSelectionInput;
    req->window = win;
    req->selection = selection;
    req->eventMask = eventMask;
    UnlockDisplay (dpy);
    SyncHandle ();
}

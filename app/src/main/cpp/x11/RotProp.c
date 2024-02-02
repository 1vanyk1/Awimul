#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
XRotateWindowProperties(
        register Display *dpy,
        Window w,
        Atom *properties,
        register int nprops,
        int npositions)
{
    register long nbytes;
    register xRotatePropertiesReq *req;

    LockDisplay(dpy);
    GetReq (RotateProperties, req);
    req->window = w;
    req->nAtoms = nprops;
    req->nPositions = npositions;

    req->length += nprops;
    nbytes = nprops << 2;
/* XXX Cray needs packing here.... */
    Data32 (dpy, (long *) properties, nbytes);


    UnlockDisplay(dpy);
    SyncHandle();
    return 1;
}



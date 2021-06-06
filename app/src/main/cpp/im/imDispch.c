#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../x11/headers/xlib.h"
#include "../x11/headers/xlibint.h"
#include "../x11/headers/xutil.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/Ximint.h"


Bool
_XimRegProtoIntrCallback(
        Xim		 im,
        CARD16	 major_code,
        CARD16	 minor_code,
        Bool	(*proc)(
                Xim, INT16, XPointer, XPointer
        ),

        XPointer	 call_data)
{
    XimProtoIntrRec    *rec;

    if (!(rec = Xmalloc(sizeof(XimProtoIntrRec))))
        return False;
    rec->func       = proc;
    rec->major_code = major_code;
    rec->minor_code = minor_code;
    rec->call_data  = call_data;
    rec->next       = im->private.proto.intrproto;
    im->private.proto.intrproto = rec;
    return True;
}

void
_XimFreeProtoIntrCallback(Xim im)
{
    register XimProtoIntrRec *rec, *next;

    for (rec = im->private.proto.intrproto; rec;) {
        next = rec->next;
        Xfree(rec);
        rec = next;
    }
    im->private.proto.intrproto = NULL;
    return;
}

static Bool
_XimTransportIntr(
        Xim		 im,
        INT16	 len,
        XPointer	 data,
        XPointer	 call_data)
{
    Xim			 call_im = (Xim)call_data;
    XimProtoIntrRec	*rec = call_im->private.proto.intrproto;
    CARD8		 major_opcode = *((CARD8 *)data);
    CARD8		 minor_opcode = *((CARD8 *)data + 1);

    for (; rec; rec = rec->next) {
        if ((major_opcode == (CARD8)rec->major_code)
            && (minor_opcode == (CARD8)rec->minor_code))
            if ((*rec->func)(call_im, len, data, rec->call_data))
                return True;
    }
    return False;
}

Bool
_XimDispatchInit(Xim im)
{
    if (_XimRegisterDispatcher(im, _XimTransportIntr, (XPointer)im))
        return True;
    return False;
}
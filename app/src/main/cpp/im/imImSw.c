#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../x11/headers/xlibint.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/Ximint.h"
#include "../x11/xlc/XimImSw.h"

static Bool
_XimCheckIfDefault(
        Xim		im)
{
    return(True);
}

XimImsportSW _XimImSportRec[] = {
        { _XimCheckIfLocalProcessing, _XimLocalOpenIM, _XimLocalIMFree },
        { _XimCheckIfThaiProcessing,	_XimThaiOpenIM,	 _XimThaiIMFree },
        { _XimCheckIfDefault,         _XimProtoOpenIM, _XimProtoIMFree },
        { NULL,                       NULL,		 NULL },
};
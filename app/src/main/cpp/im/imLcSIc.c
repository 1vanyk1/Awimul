#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "../x11/headers/xlib.h"
#include "../x11/headers/xmd.h"
#include "../x11/headers/xutil.h"
#include "../x11/headers/xlibint.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/Ximint.h"

char *
_XimLocalSetICValues(XIC xic, XIMArg *values)
{
    XimDefICValues	 ic_values;
    Xic			 ic = (Xic)xic;
    char		*name;

    _XimGetCurrentICValues(ic, &ic_values);
    name = _XimSetICValueData(ic, (XPointer)&ic_values,
                              ic->private.local.ic_resources,
                              ic->private.local.ic_num_resources,
                              values, XIM_SETICVALUES, True);
    _XimSetCurrentICValues(ic, &ic_values);
    return(name);
}
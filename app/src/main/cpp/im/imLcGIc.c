#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../x11/headers/xlibint.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/Ximint.h"

char *
_XimLocalGetICValues(XIC xic, XIMArg *values)
{
    Xic			 ic = (Xic)xic;
    XimDefICValues	 ic_values;

    _XimGetCurrentICValues(ic, &ic_values);
    return(_XimGetICValueData(ic, (XPointer)&ic_values,
                              ic->private.local.ic_resources,
                              ic->private.local.ic_num_resources,
                              values, XIM_GETICVALUES));
}
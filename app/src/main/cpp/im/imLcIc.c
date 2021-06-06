#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "../x11/headers/xlib.h"
#include "../x11/headers/xmd.h"
#include "../x11/headers/xlibint.h"
#include "../x11/xlc/xlcint.h"
#include "../x11/xlc/Ximint.h"

static void
_XimLocalUnSetFocus(
        XIC	 xic)
{
    Xic  ic = (Xic)xic;
    ((Xim)ic->core.im)->private.local.current_ic = (XIC)NULL;

    if (ic->core.focus_window)
        _XUnregisterFilter(ic->core.im->core.display,
                           ic->core.focus_window, _XimLocalFilter, (XPointer)ic);
    return;
}

static void
_XimLocalDestroyIC(
        XIC	 xic)
{
    Xic	 ic = (Xic)xic;

    if(((Xim)ic->core.im)->private.local.current_ic == (XIC)ic) {
        ((Xim)ic->core.im)->private.local.current_ic = (XIC)NULL;
    }
    if (ic->core.focus_window)
        _XUnregisterFilter(ic->core.im->core.display,
                           ic->core.focus_window, _XimLocalFilter, (XPointer)ic);
    if(ic->private.local.ic_resources) {
        Xfree(ic->private.local.ic_resources);
        ic->private.local.ic_resources = NULL;
    }
    return;
}

static void
_XimLocalSetFocus(
        XIC	 xic)
{
    Xic	 ic = (Xic)xic;
    XIC	 current_ic = ((Xim)ic->core.im)->private.local.current_ic;

    if (current_ic == (XIC)ic)
        return;

    if (current_ic != (XIC)NULL) {
        _XimLocalUnSetFocus(current_ic);
    }
    ((Xim)ic->core.im)->private.local.current_ic = (XIC)ic;

    if (ic->core.focus_window)
        _XRegisterFilterByType(ic->core.im->core.display,
                               ic->core.focus_window, KeyPress, KeyRelease,
                               _XimLocalFilter, (XPointer)ic);
    return;
}

static void
_XimLocalReset(
        XIC	 xic)
{
    Xic	 ic = (Xic)xic;
    ic->private.local.composed       = 0;
    ic->private.local.context        = ((Xim)ic->core.im)->private.local.top;
    ic->private.local.brl_pressed    = 0;
    ic->private.local.brl_committing = 0;
    ic->private.local.brl_committed  = 0;
}

static char *
_XimLocalMbReset(
        XIC	 xic)
{
    _XimLocalReset(xic);
    return (char *)NULL;
}

static wchar_t *
_XimLocalWcReset(
        XIC	 xic)
{
    _XimLocalReset(xic);
    return (wchar_t *)NULL;
}

static XICMethodsRec Local_ic_methods = {
        _XimLocalDestroyIC, 	/* destroy */
        _XimLocalSetFocus,  	/* set_focus */
        _XimLocalUnSetFocus,	/* unset_focus */
        _XimLocalSetICValues,	/* set_values */
        _XimLocalGetICValues,	/* get_values */
        _XimLocalMbReset,		/* mb_reset */
        _XimLocalWcReset,		/* wc_reset */
        _XimLocalMbReset,		/* utf8_reset */
        _XimLocalMbLookupString,	/* mb_lookup_string */
        _XimLocalWcLookupString,	/* wc_lookup_string */
        _XimLocalUtf8LookupString	/* utf8_lookup_string */
};

XIC
_XimLocalCreateIC(
        XIM			 im,
        XIMArg		*values)
{
    Xic			 ic;
    XimDefICValues	 ic_values;
    XIMResourceList	 res;
    unsigned int	 num;
    int			 len;

    if((ic = Xcalloc(1, sizeof(XicRec))) == (Xic)NULL) {
        return ((XIC)NULL);
    }

    ic->methods = &Local_ic_methods;
    ic->core.im = im;
    ic->private.local.base           = ((Xim)im)->private.local.base;
    ic->private.local.context        = ((Xim)im)->private.local.top;
    ic->private.local.composed       = 0;
    ic->private.local.brl_pressed    = 0;
    ic->private.local.brl_committing = 0;
    ic->private.local.brl_committed  = 0;

    num = im->core.ic_num_resources;
    len = sizeof(XIMResource) * num;
    if((res = Xmalloc(len)) == (XIMResourceList)NULL) {
        goto Set_Error;
    }
    (void)memcpy((char *)res, (char *)im->core.ic_resources, len);
    ic->private.local.ic_resources     = res;
    ic->private.local.ic_num_resources = num;

    bzero((char *)&ic_values, sizeof(XimDefICValues));
    if(_XimCheckLocalInputStyle(ic, (XPointer)&ic_values, values,
                                im->core.styles, res, num) == False) {
        goto Set_Error;
    }

    _XimSetICMode(res, num, ic_values.input_style);

    if(_XimSetICValueData(ic, (XPointer)&ic_values,
                          ic->private.local.ic_resources,
                          ic->private.local.ic_num_resources,
                          values, XIM_CREATEIC, True)) {
        goto Set_Error;
    }
    ic_values.filter_events = KeyPressMask | KeyReleaseMask;
    _XimSetCurrentICValues(ic, &ic_values);
    if(_XimSetICDefaults(ic, (XPointer)&ic_values,
                         XIM_SETICDEFAULTS, res, num) == False) {
        goto Set_Error;
    }
    _XimSetCurrentICValues(ic, &ic_values);

    return((XIC)ic);

    Set_Error :
    if (ic->private.local.ic_resources) {
        Xfree(ic->private.local.ic_resources);
        ic->private.local.ic_resources = NULL;
    }
    Xfree(ic);
    return((XIC)NULL);
}
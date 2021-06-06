#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xlc/xlcint.h"

XOM
XOpenOM(Display *dpy, XrmDatabase rdb, _Xconst char *res_name,
        _Xconst char *res_class)
{
    XLCd lcd = _XOpenLC((char *) NULL);

    if (lcd == NULL)
        return (XOM) NULL;

    if (lcd->methods->open_om)
        return (*lcd->methods->open_om)(lcd, dpy, rdb, res_name, res_class);

    return (XOM) NULL;
}

Status
XCloseOM(XOM om)
{
    XOC oc, next;
    XLCd lcd = om->core.lcd;

    next = om->core.oc_list;

    while ((oc = next)) {
        next = oc->core.next;
        (*oc->methods->destroy)(oc);
    }

    om->core.oc_list = NULL;

    _XCloseLC(lcd);

    return (*om->methods->close)(om);
}

char *
XSetOMValues(XOM om, ...)
{
    va_list var;
    XlcArgList args;
    char *ret;
    int num_args;

    va_start(var, om);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, om);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (char *) NULL;

    ret = (*om->methods->set_values)(om, args, num_args);

    Xfree(args);

    return ret;
}

char *
XGetOMValues(XOM om, ...)
{
    va_list var;
    XlcArgList args;
    char *ret;
    int num_args;

    va_start(var, om);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, om);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (char *) NULL;

    ret = (*om->methods->get_values)(om, args, num_args);

    Xfree(args);

    return ret;
}

Display *
XDisplayOfOM(XOM om)
{
    return om->core.display;
}

char *
XLocaleOfOM(XOM om)
{
    return om->core.lcd->core->name;
}
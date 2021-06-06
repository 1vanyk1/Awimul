#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xlc/xlcint.h"

XOC
XCreateOC(XOM om, ...)
{
    va_list var;
    XlcArgList args;
    XOC oc;
    int num_args;

    va_start(var, om);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, om);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (XOC) NULL;

    oc = (*om->methods->create_oc)(om, args, num_args);

    Xfree(args);

    if (oc) {
        oc->core.next = om->core.oc_list;
        om->core.oc_list = oc;
    }

    return oc;
}

void
XDestroyOC(XOC oc)
{
    XOC prev, oc_list;

    prev = oc_list = oc->core.om->core.oc_list;
    if (oc_list == oc)
        oc->core.om->core.oc_list = oc_list->core.next;
    else {
        while ((oc_list = oc_list->core.next)) {
            if (oc_list == oc) {
                prev->core.next = oc_list->core.next;
                break;
            }
            prev = oc_list;
        }
    }

    (*oc->methods->destroy)(oc);
}

XOM
XOMOfOC(XOC oc)
{
    return oc->core.om;
}

char *
XSetOCValues(XOC oc, ...)
{
    va_list var;
    XlcArgList args;
    char *ret;
    int num_args;

    va_start(var, oc);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, oc);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (char *) NULL;

    ret = (*oc->methods->set_values)(oc, args, num_args);

    Xfree(args);

    return ret;
}

char *
XGetOCValues(XOC oc, ...)
{
    va_list var;
    XlcArgList args;
    char *ret;
    int num_args;

    va_start(var, oc);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, oc);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (char *) NULL;

    ret = (*oc->methods->get_values)(oc, args, num_args);

    Xfree(args);

    return ret;
}
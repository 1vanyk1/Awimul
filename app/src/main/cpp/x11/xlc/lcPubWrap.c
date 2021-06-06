#include "../headers/xlibint.h"
#include "xlcPubI.h"

char *
_XGetLCValues(XLCd lcd, ...)
{
    va_list var;
    XlcArgList args;
    char *ret;
    int num_args;
    XLCdPublicMethodsPart *methods = XLC_PUBLIC_METHODS(lcd);

    va_start(var, lcd);
    _XlcCountVaList(var, &num_args);
    va_end(var);

    va_start(var, lcd);
    _XlcVaToArgList(var, num_args, &args);
    va_end(var);

    if (args == (XlcArgList) NULL)
        return (char *) NULL;

    ret = (*methods->get_values)(lcd, args, num_args);

    Xfree(args);

    return ret;
}

void
_XlcDestroyLC(
        XLCd lcd)
{
    XLCdPublicMethods methods = (XLCdPublicMethods) lcd->methods;

    (*methods->pub.destroy)(lcd);
}

XLCd
_XlcCreateLC(
        const char *name,
        XLCdMethods methods)
{
    XLCdPublicMethods pub_methods = (XLCdPublicMethods) methods;
    XLCd lcd;

    lcd = (*pub_methods->pub.create)(name, methods);
    if (lcd == NULL)
        return (XLCd) NULL;

    if (lcd->core->name == NULL) {
        lcd->core->name = (char*) Xmalloc(strlen(name) + 1);
        if (lcd->core->name == NULL)
            goto err;
        strcpy(lcd->core->name, name);
    }

    if (lcd->methods == NULL)
        lcd->methods = methods;

    if ((*pub_methods->pub.initialize)(lcd) == False)
        goto err;

    return lcd;

    err:
    _XlcDestroyLC(lcd);

    return (XLCd) NULL;
}
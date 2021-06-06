#include <stdio.h>
#include "../headers/xlibint.h"
#include "xlcPubI.h"
#include "XlcGeneric.h"

XLCd
_XlcUtf8Loader(
        const char *name)
{
    XLCd lcd;

    lcd = _XlcCreateLC(name, _XlcGenericMethods);
    if (lcd == (XLCd) NULL)
        return lcd;

    /* The official IANA name for UTF-8 is "UTF-8" in upper case with a dash. */
    if (!XLC_PUBLIC_PART(lcd)->codeset ||
        (_XlcCompareISOLatin1(XLC_PUBLIC_PART(lcd)->codeset, "UTF-8"))) {
        _XlcDestroyLC(lcd);
        return (XLCd) NULL;
    }

    _XlcAddUtf8LocaleConverters(lcd);
    _XlcAddUtf8Converters(lcd);

    return lcd;
}
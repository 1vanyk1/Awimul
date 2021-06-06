#include "../headers/xlibint.h"
#include "xlcint.h"

#ifdef USE_DYNAMIC_LC
#undef USE_DEFAULT_LOADER
#undef USE_GENERIC_LOADER
#undef USE_UTF8_LOADER
#else
#define USE_GENERIC_LOADER
#define USE_DEFAULT_LOADER
#define USE_UTF8_LOADER
#ifdef X_LOCALE
# define USE_EUC_LOADER
# define USE_SJIS_LOADER
# define USE_JIS_LOADER
#endif
#endif

/*
 * The _XlcInitLoader function initializes the locale object loader list
 * with vendor specific manner.
 */

void
_XlcInitLoader()
{

#ifdef USE_DYNAMIC_LC
    _XlcAddLoader(_XlcDynamicLoad, XlcHead);
#else /* USE_DYNAMIC_LC */

#ifdef USE_GENERIC_LOADER
    _XlcAddLoader(_XlcGenericLoader, XlcHead);
#endif

#ifdef USE_DEFAULT_LOADER
    _XlcAddLoader(_XlcDefaultLoader, XlcHead);
#endif

#ifdef USE_UTF8_LOADER
    _XlcAddLoader(_XlcUtf8Loader, XlcHead);
#endif

#ifdef USE_EUC_LOADER
    _XlcAddLoader(_XlcEucLoader, XlcHead);
#endif

#ifdef USE_SJIS_LOADER
    _XlcAddLoader(_XlcSjisLoader, XlcHead);
#endif

#ifdef USE_JIS_LOADER
    _XlcAddLoader(_XlcJisLoader, XlcHead);
#endif

#ifdef USE_DYNAMIC_LOADER
    _XlcAddLoader(_XlcDynamicLoader, XlcHead);
#endif

#endif /* USE_DYNAMIC_LC */
}

void
_XlcDeInitLoader()
{

#ifdef USE_DYNAMIC_LC
    _XlcRemoveLoader(_XlcDynamicLoad);
#else /* USE_DYNAMIC_LC */

#ifdef USE_GENERIC_LOADER
    _XlcRemoveLoader(_XlcGenericLoader);
#endif

#ifdef USE_DEFAULT_LOADER
    _XlcRemoveLoader(_XlcDefaultLoader);
#endif

#ifdef USE_UTF8_LOADER
    _XlcRemoveLoader(_XlcUtf8Loader);
#endif

#ifdef USE_EUC_LOADER
    _XlcRemoveLoader(_XlcEucLoader);
#endif

#ifdef USE_SJIS_LOADER
    _XlcRemoveLoader(_XlcSjisLoader);
#endif

#ifdef USE_JIS_LOADER
    _XlcRemoveLoader(_XlcJisLoader);
#endif

#ifdef USE_DYNAMIC_LOADER
    _XlcRemoveLoader(_XlcDynamicLoader);
#endif

#endif /* USE_DYNAMIC_LC */
}
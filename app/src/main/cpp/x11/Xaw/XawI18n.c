#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits.h>
#include "../libXt/IntrinsicP.h"
#include "XawI18n.h"

wchar_t
#if NeedWidePrototypes
_Xaw_atowc(int c)
#else
_Xaw_atowc(unsigned char c)
#endif
{
    wchar_t  wc;
    char str[2];

    str[0] = (char)c;
    str[1] = '\0';

    mbtowc(&wc, str, 1);

    return (wc);
}

#ifdef NCR
int
_Xaw_iswspace(wchar_t w)
{
    int ret = 0;
    wchar_t s = _Xaw_atowc(' ');

    if (s == w)
	ret = 1;

  return (ret);
}
#endif

int
_Xaw_iswalnum(wchar_t ch)
{
#ifdef HAVE_ISWALNUM
    return iswalnum((wint_t)ch);
#else
    unsigned char mb[MB_LEN_MAX];

    wctomb((char*)mb, ch);

    return (isalnum(*mb));
#endif
}

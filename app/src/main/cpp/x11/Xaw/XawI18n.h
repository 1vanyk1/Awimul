#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#ifdef HAVE_WIDEC_H
#include <widec.h>
#define wcslen(c) wslen(c)
#define wcscpy(d, s)		wscpy(d, s)
#define wcsncpy(d, s, l)	wsncpy(d, s, l)
#endif
#endif

#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#if defined(AIXV3) || defined(__SCO__)
#include <ctype.h>
#endif

#ifdef NCR
#define iswspace(c) _Xaw_iswspace(c)
int _Xaw_iswspace
(
 wchar_t		c
 );
#endif

#ifdef sony
#ifndef SVR4
#include <jctype.h>
#define iswspace(c) jisspace(c)
#endif
#endif

#ifdef QNX4
#define toascii( c ) ((unsigned)(c) & 0x007f)
#endif

#include <stdlib.h>

#ifdef USE_XWCHAR_STRING
int _Xwcslen
(
 wchar_t		*wstr
 );

#define wcslen(c) _Xwcslen(c)

wchar_t *_Xwcscpy
(
 wchar_t		*wstr1,
 wchar_t		*wstr2
 );

#define wcscpy(d,s) _Xwcscpy(d,s)

wchar_t *_Xwcsncpy
(
 wchar_t		*wstr1,
 wchar_t		*wstr2,
 int			len
 );

#define wcsncpy(d, s, l)	_Xwcsncpy(d, s, l)

#ifdef USE_XMBTOWC
#define mbtowc(wc, s, l)	_Xmbtowc(wc, s, l)
#endif
#endif

wchar_t _Xaw_atowc
        (
#if NeedWidePrototypes
        int			c
#else
        unsigned char		c
#endif
);

#ifndef HAS_ISW_FUNCS
#include <ctype.h>
#ifndef iswspace
#define iswspace(c) (isascii(c) && isspace(toascii(c)))
#endif
#endif

#if !defined(iswalnum) && !defined(HAVE_ISWALNUM)
#define iswalnum(c) _Xaw_iswalnum(c)
#endif
int _Xaw_iswalnum
        (
                wchar_t		c
        );

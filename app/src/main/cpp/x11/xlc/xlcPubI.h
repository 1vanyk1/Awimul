#ifndef _XLCPUBLICI_H_
#define _XLCPUBLICI_H_

#include "xlcPublic.h"

#define XLC_PUBLIC(lcd, x)	(((XLCdPublic) lcd->core)->pub.x)
#define XLC_PUBLIC_PART(lcd)	(&(((XLCdPublic) lcd->core)->pub))
#define XLC_PUBLIC_METHODS(lcd)	(&(((XLCdPublicMethods) lcd->methods)->pub))

/*
 * XLCd public methods
 */

typedef struct _XLCdPublicMethodsRec *XLCdPublicMethods;

typedef XLCd (*XlcPubCreateProc)(
        const char*		name,
        XLCdMethods		methods
);

typedef Bool (*XlcPubInitializeProc)(
        XLCd		lcd
);

typedef void (*XlcPubDestroyProc)(
        XLCd		lcd
);

typedef char* (*XlcPubGetValuesProc)(
        XLCd		lcd,
        XlcArgList		args,
        int			num_args
);

typedef void (*XlcPubGetResourceProc)(
        XLCd		lcd,
        const char*		category,
        const char*		_class,
        char***		value,
        int*		count
);

typedef struct _XLCdPublicMethodsPart {
    XLCdPublicMethods superclass;
    XlcPubCreateProc create;
    XlcPubInitializeProc initialize;
    XlcPubDestroyProc destroy;
    XlcPubGetValuesProc get_values;
    XlcPubGetResourceProc get_resource;
} XLCdPublicMethodsPart;

typedef struct _XLCdPublicMethodsRec {
    XLCdMethodsRec core;
    XLCdPublicMethodsPart pub;
} XLCdPublicMethodsRec;

/*
 * XLCd public data
 */

typedef struct _XLCdPublicPart {
    char *siname;			/* for _XlcMapOSLocaleName() */
    char *language;			/* language part of locale name */
    char *territory;			/* territory part of locale name */
    char *codeset;			/* codeset part of locale name */
    char *encoding_name;		/* encoding name */
    int mb_cur_max;			/* ANSI C MB_CUR_MAX */
    Bool is_state_depend;		/* state-depend encoding */
    const char *default_string;		/* for XDefaultString() */
    XPointer xlocale_db;
} XLCdPublicPart;

typedef struct _XLCdPublicRec {
    XLCdCoreRec core;
    XLCdPublicPart pub;
} XLCdPublicRec, *XLCdPublic;

extern XLCdMethods _XlcPublicMethods;

_XFUNCPROTOBEGIN

extern XLCd _XlcCreateLC(
        const char*		name,
        XLCdMethods		methods
);

extern void _XlcDestroyLC(
        XLCd		lcd
);

/* Fills into a freshly created XlcCharSet the fields that can be inferred
   from the ESC sequence. These are side, char_size, set_size. */
extern Bool _XlcParseCharSet(
        XlcCharSet		charset
);

/* Creates a new XlcCharSet, given its name (including side suffix) and
   Compound Text ESC sequence (normally at most 4 bytes). */
extern XlcCharSet _XlcCreateDefaultCharSet(
        const char*		name,
        const char*		ct_sequence
);

extern XlcCharSet _XlcAddCT(
        const char*		name,
        const char*		ct_sequence
);

extern Bool _XlcInitCTInfo (void);

extern XrmMethods _XrmDefaultInitParseInfo(
        XLCd		lcd,
XPointer*		state
);

extern int _XmbTextPropertyToTextList(
        XLCd		lcd,
Display*		dpy,
const XTextProperty* text_prop,
char***		list_ret,
int*		count_ret
);

extern int _XwcTextPropertyToTextList(
        XLCd		lcd,
Display*		dpy,
const XTextProperty* text_prop,
wchar_t***		list_ret,
int*		count_ret
);

extern int _Xutf8TextPropertyToTextList(
        XLCd		lcd,
Display*		dpy,
const XTextProperty* text_prop,
char***		list_ret,
int*		count_ret
);

extern int _XmbTextListToTextProperty(
        XLCd		/* lcd */,
        Display*		/* dpy */,
        char**		/* list */,
        int			/* count */,
        XICCEncodingStyle	/* style */,
        XTextProperty*	/* text_prop */
);

extern int _XwcTextListToTextProperty(
        XLCd		/* lcd */,
        Display*		/* dpy */,
        wchar_t**		/* list */,
        int			/* count */,
        XICCEncodingStyle	/* style */,
        XTextProperty*	/* text_prop */
);

extern int _Xutf8TextListToTextProperty(
        XLCd		/* lcd */,
        Display*		/* dpy */,
        char**		/* list */,
        int			/* count */,
        XICCEncodingStyle	/* style */,
        XTextProperty*	/* text_prop */
);

extern void _XwcFreeStringList(
        XLCd		/* lcd */,
        wchar_t**		/* list */
);

extern int _XlcResolveLocaleName(
        const char*		lc_name,
        XLCdPublicPart*	pub
);

extern int _XlcResolveI18NPath(
        char*		buf,
        int			buf_len
);

extern char *_XlcLocaleDirName(
        char*             /* dir_name */,
        size_t,	       /* dir_len */
        char*             /* lc_name */
);

extern XPointer _XlcCreateLocaleDataBase(
        XLCd		lcd
);

extern void _XlcDestroyLocaleDataBase(
        XLCd		lcd
);

extern void _XlcGetLocaleDataBase(
        XLCd		/* lcd */,
const char*		/* category */,
const char*		/* name */,
char***		/* value */,
int*		/* count */
);

#ifdef X_LOCALE
extern char *
_Xsetlocale(
    int           category,
    _Xconst char  *name);
#else
#ifdef __DARWIN__
extern char *
_Xsetlocale(
    int           category,
    _Xconst char  *name);
#endif
extern char *_XlcMapOSLocaleName(
        char *osname,
        char *siname);
#endif

extern int
_Xmbstoutf8(
        char *ustr,
        const char *str,
        int len);
extern int
        _Xlcmbstoutf8(
        XLCd lcd,
char *ustr,
const char *str,
int len);
extern int
_Xmbstowcs(
        wchar_t *wstr,
        char *str,
        int len);
extern int
        _Xlcwcstombs(
        XLCd lcd,
char *str,
wchar_t *wstr,
int len);
extern int
        _Xlcmbstowcs(
        XLCd lcd,
wchar_t *wstr,
char *str,
int len);
extern int
_Xwcstombs(
        char *str,
        wchar_t *wstr,
        int len);
extern int
        _Xlcmbtowc(
        XLCd lcd,
wchar_t *wstr,
char *str,
int len);
extern int
        _Xlcwctomb(
        XLCd lcd,
char *str,
wchar_t wc);



extern XPointer
_Utf8GetConvByName(
        const char *name);

_XFUNCPROTOEND

#endif  /* _XLCPUBLICI_H_ */

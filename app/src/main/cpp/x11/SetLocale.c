#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "xlc/xlcint.h"
#include "headers/xlocale.h"
#include "headers/xos.h"
#include "xlc/xlcPubI.h"

#define MAXLOCALE	64	/* buffer size of locale name */


#if defined(__APPLE__) || defined(__CYGWIN__)
char *
_Xsetlocale(
    int           category,
    _Xconst char  *name
)
{
    return setlocale(category, name);
}
#endif /* __APPLE__ || __CYGWIN__ */

/*
 * _XlcMapOSLocaleName is an implementation dependent routine that derives
 * the LC_CTYPE locale name as used in the sample implementation from that
 * returned by setlocale.
 *
 * Should match the code in Xt ExtractLocaleName.
 *
 * This function name is a bit of a misnomer. Even the siname parameter
 * name is a misnomer. On most modern operating systems this function is
 * a no-op, simply returning the osname; but on older operating systems
 * like Ultrix, or HPUX 9.x and earlier, when you set LANG=german.88591
 * then the string returned by setlocale(LC_ALL, "") will look something
 * like: "german.88591 german.88591 ... german.88591". Then this function
 * will pick out the LC_CTYPE component and return a pointer to that.
 */

char *
_XlcMapOSLocaleName(
        char *osname,
        char *siname)
{
#if defined(hpux) || defined(CSRG_BASED) || defined(sun) || defined(SVR4) || defined(sgi) || defined(__osf__) || defined(AIXV3) || defined(ultrix) || defined(WIN32) || defined(__UNIXOS2__) || defined(linux)
    # ifdef hpux
#  ifndef _LastCategory
   /* HPUX 9 and earlier */
#   define SKIPCOUNT 2
#   define STARTCHAR ':'
#   define ENDCHAR ';'
#  else
   /* HPUX 10 */
#   define ENDCHAR ' '
#  endif
# else
#  ifdef ultrix
#   define SKIPCOUNT 2
#   define STARTCHAR '\001'
#   define ENDCHAR '\001'
#  else
#   if defined(WIN32) || defined(__UNIXOS2__)
#    define SKIPCOUNT 1
#    define STARTCHAR '='
#    define ENDCHAR ';'
#    define WHITEFILL
#   else
#    if defined(__osf__) || (defined(AIXV3) && !defined(AIXV4))
#     define STARTCHAR ' '
#     define ENDCHAR ' '
#    else
#     if defined(linux)
#      define STARTSTR "LC_CTYPE="
#      define ENDCHAR ';'
#     else
#      if !defined(sun) || defined(SVR4)
#       define STARTCHAR '/'
#       define ENDCHAR '/'
#      endif
#     endif
#    endif
#   endif
#  endif
# endif

    char           *start;
    char           *end;
    int             len;
# ifdef SKIPCOUNT
    int		    n;
# endif

    start = osname;
# ifdef SKIPCOUNT
    for (n = SKIPCOUNT;
	 --n >= 0 && start && (start = strchr (start, STARTCHAR));
	 start++)
	;
    if (!start)
	start = osname;
# endif
# ifdef STARTCHAR
    if (start && (start = strchr (start, STARTCHAR)))
# elif  defined (STARTSTR)
    if (start && (start = strstr (start,STARTSTR)))
# endif
    {
# ifdef STARTCHAR
	start++;
# elif defined (STARTSTR)
	start += strlen(STARTSTR);
# endif
	if ((end = strchr (start, ENDCHAR))) {
	    len = end - start;
	    if (len >= MAXLOCALE)
		len = MAXLOCALE - 1;
	    strncpy(siname, start, (size_t) len);
	    *(siname + len) = '\0';
# ifdef WHITEFILL
	    for (start = siname; start = strchr(start, ' '); )
		*start++ = '-';
# endif
	    return siname;
	} else  /* if no ENDCHAR is found we are at the end of the line */
	    return start;
    }
# ifdef WHITEFILL
    if (strchr(osname, ' ')) {
	len = strlen(osname);
	if (len >= MAXLOCALE - 1)
	    len = MAXLOCALE - 1;
	strncpy(siname, osname, len);
	*(siname + len) = '\0';
	for (start = siname; start = strchr(start, ' '); )
	    *start++ = '-';
	return siname;
    }
# endif
# undef STARTCHAR
# undef ENDCHAR
# undef WHITEFILL
#endif
    return osname;
}

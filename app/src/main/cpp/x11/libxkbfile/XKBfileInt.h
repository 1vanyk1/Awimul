#ifndef _XKBFILEINT_H_
#define	_XKBFILEINT_H_ 1

#include "../headers/extensions/XKBfile.h"
#include <string.h>

#ifdef DEBUG
#define	_XkbLibError(c,l,d) \
	{ fprintf(stderr,"xkbfile: %s in %s\n",_XkbErrMessages[c],(l)); \
	 _XkbErrCode= (c); _XkbErrLocation= (l); _XkbErrData= (d); }
#else
#define	_XkbLibError(c,l,d) \
	{ _XkbErrCode= (c); _XkbErrLocation= (l); _XkbErrData= (d); }
#endif


#define	_XkbAlloc(s)		malloc((s))
#define	_XkbCalloc(n,s)		calloc((n),(s))
#define	_XkbRealloc(o,s)	realloc((o),(s))
#define	_XkbTypedAlloc(t)	((t *)malloc(sizeof(t)))
#define	_XkbTypedCalloc(n,t)	((t *)calloc((n),sizeof(t)))
#define	_XkbTypedRealloc(o,n,t) \
	((o)?(t *)realloc((o),(n)*sizeof(t)):_XkbTypedCalloc(n,t))
#define	_XkbClearElems(a,f,l,t)	bzero(&(a)[f],((l)-(f)+1)*sizeof(t))
#define	_XkbFree(p)		free(p)

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif


_XFUNCPROTOBEGIN

static inline char *
_XkbDupString(const char *s)
{
    return s ? strdup(s) : NULL;
}

#define _XkbStrCaseEqual(s1,s2)	(_XkbStrCaseCmp(s1,s2)==0)

#ifndef HAVE_STRCASECMP
extern int _XkbStrCaseCmp(char *s1, char *s2);
#else
#define _XkbStrCaseCmp strcasecmp
#include <strings.h>
#endif

_XFUNCPROTOEND
#endif                          /* _XKBFILEINT_H_ */

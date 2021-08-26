#ifndef XPRINTF_H
#define XPRINTF_H

#include <stdio.h>
#include <stdarg.h>
#include "xfuncproto.h"

#ifndef _X_RESTRICT_KYWD
#if defined(restrict) /* assume autoconf set it correctly */ || \
   (defined(__STDC__) && (__STDC_VERSION__ - 0 >= 199901L))     /* C99 */
#define _X_RESTRICT_KYWD  restrict
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)    /* gcc w/C89+extensions */
#define _X_RESTRICT_KYWD __restrict__
#else
#define _X_RESTRICT_KYWD
#endif
#endif

/*
 * These functions provide a portable implementation of the common (but not
 * yet universal) asprintf & vasprintf routines to allocate a buffer big
 * enough to sprintf the arguments to.  The XNF variants terminate the server
 * if the allocation fails.
 * The buffer allocated is returned in the pointer provided in the first
 * argument.   The return value is the size of the allocated buffer, or -1
 * on failure.
 */
extern _X_EXPORT int
Xasprintf(char **ret, const char *_X_RESTRICT_KYWD fmt, ...)
_X_ATTRIBUTE_PRINTF(2, 3);
extern _X_EXPORT int
Xvasprintf(char **ret, const char *_X_RESTRICT_KYWD fmt, va_list va)
_X_ATTRIBUTE_PRINTF(2, 0);
extern _X_EXPORT int
XNFasprintf(char **ret, const char *_X_RESTRICT_KYWD fmt, ...)
_X_ATTRIBUTE_PRINTF(2, 3);
extern _X_EXPORT int
XNFvasprintf(char **ret, const char *_X_RESTRICT_KYWD fmt, va_list va)
_X_ATTRIBUTE_PRINTF(2, 0);

#if !defined(HAVE_ASPRINTF) && !defined(HAVE_VASPRINTF)
#define asprintf  Xasprintf
#define vasprintf Xvasprintf
#endif

/*
 * These functions provide a portable implementation of the linux kernel
 * scnprintf & vscnprintf routines that return the number of bytes actually
 * copied during a snprintf, (excluding the final '\0').
 */
extern _X_EXPORT int
Xscnprintf(char *s, int n, const char * _X_RESTRICT_KYWD fmt, ...)
_X_ATTRIBUTE_PRINTF(3,4);
extern _X_EXPORT int
Xvscnprintf(char *s, int n, const char * _X_RESTRICT_KYWD fmt, va_list va)
_X_ATTRIBUTE_PRINTF(3,0);

#endif                          /* XPRINTF_H */
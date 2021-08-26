#ifndef U_STRING_H_
#define U_STRING_H_

#if !defined(XF86_LIBC_H)
#include <stdio.h>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

#include "macros.h" // PRINTFLIKE


#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_GNU_SOURCE) || defined(__APPLE__)

#define strchrnul util_strchrnul
static inline char *
util_strchrnul(const char *s, char c)
{
    for (; *s && *s != c; ++s);

    return (char *)s;
}

#endif

#ifdef _WIN32

#define sprintf util_sprintf
static inline void
   PRINTFLIKE(2, 3)
util_sprintf(char *str, const char *format, ...)
{
   va_list ap;
   va_start(ap, format);
   vsnprintf(str, INT_MAX, format, ap);
   va_end(ap);
}

#define vasprintf util_vasprintf
static inline int
util_vasprintf(char **ret, const char *format, va_list ap)
{
   va_list ap_copy;

   /* Compute length of output string first */
   va_copy(ap_copy, ap);
   int r = vsnprintf(NULL, 0, format, ap_copy);
   va_end(ap_copy);

   if (r < 0)
      return -1;

   *ret = (char *) malloc(r + 1);
   if (!*ret)
      return -1;

   /* Print to buffer */
   return vsnprintf(*ret, r + 1, format, ap);
}

#define asprintf util_asprintf
static inline int
util_asprintf(char **str, const char *fmt, ...)
{
   int ret;
   va_list args;
   va_start(args, fmt);
   ret = vasprintf(str, fmt, args);
   va_end(args);
   return ret;
}

#ifndef strcasecmp
#define strcasecmp stricmp
#endif

#define strdup _strdup

#if defined(_WIN32) && !defined(HAVE_STRTOK_R)
#define strtok_r strtok_s
#endif

#endif


#ifdef __cplusplus
}
#endif

#endif /* U_STRING_H_ */

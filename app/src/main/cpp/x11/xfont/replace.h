#ifndef XFONT_REPLACE_H
#define XFONT_REPLACE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../headers/xfuncproto.h"

#include <stdlib.h>
#if defined(HAVE_LIBBSD) && defined(HAVE_REALLOCARRAY)
#include <bsd/stdlib.h>       /* for reallocarray */
#endif

#ifndef HAVE_REALLOCARRAY
extern _X_HIDDEN void *
reallocarray(void *optr, size_t nmemb, size_t size);
#endif

#ifndef mallocarray
#define mallocarray(n, s)	reallocarray(NULL, n, s)
#endif

#include <string.h>
#if defined(HAVE_LIBBSD) && defined(HAVE_STRLCPY)
#include <bsd/string.h>       /* for strlcpy, strlcat */
#endif

#ifndef HAVE_STRLCPY
extern _X_HIDDEN size_t
strlcpy(char *dst, const char *src, size_t siz);
extern _X_HIDDEN size_t
strlcat(char *dst, const char *src, size_t siz);
#endif

#ifndef HAVE_ERR_H
#define err(eval, ...) do { \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(eval); \
  } while (0)
#define vwarn(...) do { \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  } while (0)
#endif

#ifndef HAVE_REALPATH
extern _X_HIDDEN char *
realpath(const char *path, char *resolved_path);
#endif

#endif /* XFONT_REPLACE_H */

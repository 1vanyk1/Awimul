#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include "headers/xfuncproto.h"

#ifndef HAVE_REALLOCARRAY
extern _X_HIDDEN void *xreallocarray(void *optr, size_t nmemb, size_t size);
# define reallocarray(ptr, n, size)	xreallocarray((ptr), (size_t)(n), (size_t)(size))
#endif

#if defined(MALLOC_0_RETURNS_NULL) || defined(__clang_analyzer__)
# define Xreallocarray(ptr, n, size) \
    reallocarray((ptr), ((n) == 0 ? 1 : (n)), size)
#else
# define Xreallocarray(ptr, n, size)	reallocarray((ptr), (n), (size))
#endif

#define Xmallocarray(n, size)		Xreallocarray(NULL, (n), (size))
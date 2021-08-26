#ifndef STRNDUP_H
#define STRNDUP_H

#if defined(_WIN32)

#include <stdlib.h> // size_t
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline char *
strndup(const char *str, size_t max)
{
   size_t n;
   char *ptr;

   if (!str)
      return NULL;

   n = strnlen(str, max);
   ptr = (char *) calloc(n + 1, sizeof(char));
   if (!ptr)
      return NULL;

   memcpy(ptr, str, n);
   return ptr;
}

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* STRNDUP_H */

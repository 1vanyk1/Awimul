#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "../x11/headers/os.h"

/*
 * This is sqrt(SIZE_MAX+1), as s1*s2 <= SIZE_MAX
 * if both s1 < MUL_NO_OVERFLOW and s2 < MUL_NO_OVERFLOW
 */
#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))

//void *
//reallocarray(void *optr, size_t nmemb, size_t size)
//{
//    if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) &&
//        nmemb > 0 && SIZE_MAX / nmemb < size) {
//        errno = ENOMEM;
//        return NULL;
//    }
//    return realloc(optr, size * nmemb);
//}

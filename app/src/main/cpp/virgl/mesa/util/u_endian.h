#ifndef U_ENDIAN_H
#define U_ENDIAN_H

#include "../../config.h"

/* (virglrenderer) Detected by meson */

#if !defined(UTIL_ARCH_LITTLE_ENDIAN) || !defined(UTIL_ARCH_BIG_ENDIAN)
# error "UTIL_ARCH_LITTLE_ENDIAN and/or UTIL_ARCH_BIG_ENDIAN were unset."
#elif UTIL_ARCH_LITTLE_ENDIAN == UTIL_ARCH_BIG_ENDIAN
# error "UTIL_ARCH_LITTLE_ENDIAN and UTIL_ARCH_BIG_ENDIAN must not both be 1 or 0."
#endif

#endif

#ifndef COMPILER_H
#define COMPILER_H


#include <assert.h>

#include "macros.h"

#include "../compat/c99_compat.h" /* inline, __func__, etc. */


/**
 * Either define MESA_BIG_ENDIAN or MESA_LITTLE_ENDIAN, and CPU_TO_LE32.
 * Do not use these unless absolutely necessary!
 * Try to use a runtime test instead.
 * For now, only used by some DRI hardware drivers for color/texel packing.
 */
#if defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN
#if defined(__linux__)
#include <byteswap.h>
#define CPU_TO_LE32( x )	bswap_32( x )
#elif defined(__APPLE__)
#include <CoreFoundation/CFByteOrder.h>
#define CPU_TO_LE32( x )	CFSwapInt32HostToLittle( x )
#elif defined(__OpenBSD__)
#include <sys/types.h>
#define CPU_TO_LE32( x )	htole32( x )
#else /*__linux__ */
#include <sys/endian.h>
#define CPU_TO_LE32( x )	bswap32( x )
#endif /*__linux__*/
#define MESA_BIG_ENDIAN 1
#else
#define CPU_TO_LE32( x )	( x )
#define MESA_LITTLE_ENDIAN 1
#endif
#define LE32_TO_CPU( x )	CPU_TO_LE32( x )



#define IEEE_ONE 0x3f800000

#ifndef __has_attribute
#  define __has_attribute(x) 0
#endif

#if __cplusplus >= 201703L || __STDC_VERSION__ > 201710L
/* Standard C++17/C23 attribute */
#define FALLTHROUGH [[fallthrough]]
#elif __has_attribute(fallthrough)
/* Non-standard but supported by at least gcc and clang */
#define FALLTHROUGH __attribute__((fallthrough))
#else
#define FALLTHROUGH do { } while(0)
#endif

#endif /* COMPILER_H */

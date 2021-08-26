#ifndef P_CONFIG_H_
#define P_CONFIG_H_

#include <limits.h>
/*
 * Compiler
 */

#if defined(__GNUC__)
#define PIPE_CC_GCC
#define PIPE_CC_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

/*
 * Meaning of _MSC_VER value:
 * - 1800: Visual Studio 2013
 * - 1700: Visual Studio 2012
 * - 1600: Visual Studio 2010
 * - 1500: Visual Studio 2008
 * - 1400: Visual C++ 2005
 * - 1310: Visual C++ .NET 2003
 * - 1300: Visual C++ .NET 2002
 *
 * __MSC__ seems to be an old macro -- it is not pre-defined on recent MSVC
 * versions.
 */
#if defined(_MSC_VER) || defined(__MSC__)
#define PIPE_CC_MSVC
#endif

#if defined(__ICL)
#define PIPE_CC_ICL
#endif


/*
 * Processor architecture
 */

#if defined(__i386__) /* gcc */ || defined(_M_IX86) /* msvc */ || defined(_X86_) || defined(__386__) || defined(i386) || defined(__i386) /* Sun cc */
#define PIPE_ARCH_X86
#endif

#if defined(__x86_64__) /* gcc */ || defined(_M_X64) /* msvc */ || defined(_M_AMD64) /* msvc */ || defined(__x86_64) /* Sun cc */
#define PIPE_ARCH_X86_64
#endif

#if defined(PIPE_ARCH_X86) || defined(PIPE_ARCH_X86_64)
#if defined(PIPE_CC_GCC) && !defined(__SSE2__)
/* #warning SSE2 support requires -msse -msse2 compiler options */
#else
#define PIPE_ARCH_SSE
#endif
#if defined(PIPE_CC_GCC) && (__GNUC__ * 100 + __GNUC_MINOR__) < 409 && !defined(__SSSE3__)
/* #warning SSE3 support requires -msse3 compiler options before GCC 4.9 */
#else
#define PIPE_ARCH_SSSE3
#endif
#endif

#if defined(__ppc__) || defined(__ppc64__) || defined(__PPC__)
#define PIPE_ARCH_PPC
#if defined(__ppc64__) || defined(__PPC64__)
#define PIPE_ARCH_PPC_64
#endif
#endif

#if defined(__s390x__)
#define PIPE_ARCH_S390
#endif

#if defined(__arm__)
#define PIPE_ARCH_ARM
#endif

#if defined(__aarch64__)
#define PIPE_ARCH_AARCH64
#endif

/*
 * Endian detection.
 */

#include "../../util/u_endian.h"

/*
 * Auto-detect the operating system family.
 */
#include "../../util/detect_os.h"

#if DETECT_OS_LINUX
#define PIPE_OS_LINUX
#endif

#if DETECT_OS_UNIX
#define PIPE_OS_UNIX
#endif

#if DETECT_OS_ANDROID
#define PIPE_OS_ANDROID
#endif

#if DETECT_OS_FREEBSD
#define PIPE_OS_FREEBSD
#endif

#if DETECT_OS_BSD
#define PIPE_OS_BSD
#endif

#if DETECT_OS_OPENBSD
#define PIPE_OS_OPENBSD
#endif

#if DETECT_OS_NETBSD
#define PIPE_OS_NETBSD
#endif

#if DETECT_OS_DRAGONFLY
#define PIPE_OS_DRAGONFLY
#endif

#if DETECT_OS_HURD
#define PIPE_OS_HURD
#endif

#if DETECT_OS_SOLARIS
#define PIPE_OS_SOLARIS
#endif

#if DETECT_OS_APPLE
#define PIPE_OS_APPLE
#endif

#if DETECT_OS_WINDOWS
#define PIPE_OS_WINDOWS
#endif

#if DETECT_OS_HAIKU
#define PIPE_OS_HAIKU
#endif

#if DETECT_OS_CYGWIN
#define PIPE_OS_CYGWIN
#endif

#endif /* P_CONFIG_H_ */

#ifndef _DIX_CONFIG_H_
#define _DIX_CONFIG_H_

/* Support BigRequests extension */
#undef BIGREQS

/* Builder address */
#undef BUILDERADDR

/* Builder string */
#undef BUILDERSTRING

/* Default font path */
#undef COMPILEDDEFAULTFONTPATH

/* Miscellaneous server configuration files path */
#undef SERVER_MISC_CONFIG_PATH

/* Support Composite Extension */
#undef COMPOSITE

/* Support Damage extension */
#undef DAMAGE

/* Use OsVendorVErrorF */
#undef DDXOSVERRORF

/* Use ddxBeforeReset */
#undef DDXBEFORERESET

/* Build DPMS extension */
#undef DPMSExtension

/* Build DRI3 extension */
#undef DRI3

/* Build GLX extension */
#undef GLXEXT

/* Build GLX DRI loader */
#undef GLX_DRI

/* Path to DRI drivers */
#undef DRI_DRIVER_PATH

/* Support XDM-AUTH*-1 */
#undef HASXDMAUTH

/* Support SHM */
#undef HAS_SHM

/* Has backtrace support */
#undef HAVE_BACKTRACE

/* Has libunwind support */
#undef HAVE_LIBUNWIND

/* Define to 1 if you have the `cbrt' function. */
#undef HAVE_CBRT

/* Define to 1 if you have the declaration of `program_invocation_short_name', and
   to 0 if you don't. */
#undef HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#undef HAVE_DIRENT_H

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Have execinfo.h */
#undef HAVE_EXECINFO_H

/* Define to 1 if you have the <fcntl.h> header file. */
#undef HAVE_FCNTL_H

/* Define to 1 if you have the `getdtablesize' function. */
#undef HAVE_GETDTABLESIZE

/* Define to 1 if you have the `getifaddrs' function. */
#undef HAVE_GETIFADDRS

/* Define to 1 if you have the `getpeereid' function. */
#undef HAVE_GETPEEREID

/* Define to 1 if you have the `getpeerucred' function. */
#undef HAVE_GETPEERUCRED

/* Define to 1 if you have the `getprogname' function. */
#undef HAVE_GETPROGNAME

/* Define to 1 if you have the `getzoneid' function. */
#undef HAVE_GETZONEID

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Have Quartz */
#undef XQUARTZ

/* Support application updating through sparkle. */
#undef XQUARTZ_SPARKLE

/* Prefix to use for bundle identifiers */
#undef BUNDLE_ID_PREFIX

/* Build a standalone xpbproxy */
#undef STANDALONE_XPBPROXY

/* Define to 1 if you have the `bsd' library (-lbsd). */
#undef HAVE_LIBBSD

/* Define to 1 if you have the `m' library (-lm). */
#undef HAVE_LIBM

/* Define to 1 if you have the <linux/agpgart.h> header file. */
#undef HAVE_LINUX_AGPGART_H

/* Define to 1 if you have the <linux/apm_bios.h> header file. */
#undef HAVE_LINUX_APM_BIOS_H

/* Define to 1 if you have the <linux/fb.h> header file. */
#undef HAVE_LINUX_FB_H

/* Define to 1 if you have the `memfd_create' function. */
#undef HAVE_MEMFD_CREATE

/* Define to 1 if you have the `mkostemp' function. */
#undef HAVE_MKOSTEMP

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define to 1 if you have the function pthread_setname_np(const char*) */
#undef HAVE_PTHREAD_SETNAME_NP_WITHOUT_TID

/* Define to 1 if you have the function pthread_setname_np(pthread_t, const char*) */
#undef HAVE_PTHREAD_SETNAME_NP_WITH_TID

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#undef HAVE_NDIR_H

/* Define to 1 if you have the `reallocarray' function. */
#undef HAVE_REALLOCARRAY

/* Define to 1 if you have the `arc4random_buf' function. */
#undef HAVE_ARC4RANDOM_BUF

/* Define to use libc SHA1 functions */
#undef HAVE_SHA1_IN_LIBC

/* Define to use CommonCrypto SHA1 functions */
#undef HAVE_SHA1_IN_COMMONCRYPTO

/* Define to use CryptoAPI SHA1 functions */
#undef HAVE_SHA1_IN_CRYPTOAPI

/* Define to use libmd SHA1 functions */
#undef HAVE_SHA1_IN_LIBMD

/* Define to use libgcrypt SHA1 functions */
#undef HAVE_SHA1_IN_LIBGCRYPT

/* Define to use libnettle SHA1 functions */
#undef HAVE_SHA1_IN_LIBNETTLE

/* Define to use libsha1 for SHA1 */
#undef HAVE_SHA1_IN_LIBSHA1

/* Define to 1 if you have the `shmctl64' function. */
#undef HAVE_SHMCTL64

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the `strcasecmp' function. */
#undef HAVE_STRCASECMP

/* Define to 1 if you have the `strcasestr' function. */
#undef HAVE_STRCASESTR

/* Define to 1 if you have the `strncasecmp' function. */
#undef HAVE_STRNCASECMP

/* Define to 1 if you have the `strlcat' function. */
#undef HAVE_STRLCAT

/* Define to 1 if you have the `strlcpy' function. */
#undef HAVE_STRLCPY

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the `strndup' function. */
#undef HAVE_STRNDUP

/* Define to 1 if libsystemd-daemon is available */
#undef HAVE_SYSTEMD_DAEMON

/* Define to 1 if SYSV IPC is available */
#undef HAVE_SYSV_IPC

/* Define to 1 if you have the <sys/agpio.h> header file. */
#undef HAVE_SYS_AGPIO_H

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_DIR_H

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#undef HAVE_SYS_NDIR_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <sys/utsname.h> header file. */
#undef HAVE_SYS_UTSNAME_H

/* Define to 1 if you have the `timingsafe_memcmp' function. */
#undef HAVE_TIMINGSAFE_MEMCMP

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the <fnmatch.h> header file. */
#undef HAVE_FNMATCH_H

/* Have /dev/urandom */
#undef HAVE_URANDOM

/* Define to 1 if you have the `vasprintf' function. */
#undef HAVE_VASPRINTF

/* Support IPv6 for TCP connections */
#undef IPv6

/* Support os-specific local connections */
#undef LOCALCONN

/* Support MIT-SHM Extension */
#undef MITSHM

/* Enable some debugging code */
#undef DEBUG

/* Name of package */
#undef PACKAGE

/* Internal define for Xinerama */
#undef PANORAMIX

/* Support Present extension */
#undef PRESENT

/* Overall prefix */
#undef PROJECTROOT

/* Support RANDR extension */
#undef RANDR

/* Support Record extension */
#undef XRECORD

/* Support RENDER extension */
#undef RENDER

/* Support X resource extension */
#undef RES

/* Support client ID tracking in X resource extension */
#undef CLIENTIDS

/* Support MIT-SCREEN-SAVER extension */
#undef SCREENSAVER

/* Support Secure RPC ("SUN-DES-1") authentication for X11 clients */
#undef SECURE_RPC

/* Support SHAPE extension */
#undef SHAPE

/* Where to install Xorg.bin and Xorg.wrap */
#undef SUID_WRAPPER_DIR

/* Define to 1 on systems derived from System V Release 4 */
#undef SVR4

/* sysconfdir */
#undef SYSCONFDIR

/* Support TCP socket connections */
#undef TCPCONN

/* Support UNIX socket connections */
#define UNIXCONN 1

/* Build X string registry */
#undef XREGISTRY

/* Build X-ACE extension */
#undef XACE

/* Build SELinux extension */
#undef XSELINUX

/* Support XCMisc extension */
#undef XCMISC

/* Build Security extension */
#undef XCSECURITY

/* Support Xdmcp */
#undef XDMCP

/* Build XFree86 BigFont extension */
#undef XF86BIGFONT

/* Support XFree86 Video Mode extension */
#undef XF86VIDMODE

/* Support XFixes extension */
#undef XFIXES

/* Build XDGA support */
#undef XFreeXDGA

/* Support Xinerama extension */
#undef XINERAMA

/* Current Xorg version */
#undef XORG_VERSION_CURRENT

/* Build Xv Extension */
#undef XvExtension

/* Build XvMC Extension */
#undef XvMCExtension

/* Support XSync extension */
#undef XSYNC

/* Support XTest extension */
#undef XTEST

/* Support Xv extension */
#undef XV

/* Support DRI extension */
#undef XF86DRI

/* Build DRI2 extension */
#undef DRI2

/* Build DBE support */
#undef DBE

/* Vendor name */
#undef XVENDORNAME

/* Number of bits in a file offset, on hosts where this is settable. */
#undef _FILE_OFFSET_BITS

/* Enable GNU and other extensions to the C environment for GLIBC */
#undef _GNU_SOURCE

/* Define for large files, on AIX-style hosts. */
#undef _LARGE_FILES

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `int' if <sys/types.h> does not define. */
#undef pid_t

/* Build Rootless code */
#undef ROOTLESS

/* Define to 1 if unsigned long is 64 bits. */
#undef _XSERVER64

/* System is BSD-like */
#undef CSRG_BASED

/* Define to 1 if `struct sockaddr_in' has a `sin_len' member */
#undef BSD44SOCKETS

/* Support D-Bus */
#undef HAVE_DBUS

/* Use libudev for input hotplug */
#undef CONFIG_UDEV

/* Use libudev for kms enumeration */
#undef CONFIG_UDEV_KMS

/* Use udev_monitor_filter_add_match_tag() */
#undef HAVE_UDEV_MONITOR_FILTER_ADD_MATCH_TAG

/* Use udev_enumerate_add_match_tag() */
#undef HAVE_UDEV_ENUMERATE_ADD_MATCH_TAG

/* Enable D-Bus core */
#undef NEED_DBUS

/* Support HAL for hotplug */
#undef CONFIG_HAL

/* Enable systemd-logind integration */
#undef SYSTEMD_LOGIND 1

/* Have a monotonic clock from clock_gettime() */
#undef MONOTONIC_CLOCK

/* Define to 1 if the DTrace Xserver provider probes should be built in */
#undef XSERVER_DTRACE

/* Define to 1 if typeof works with your compiler. */
#undef HAVE_TYPEOF

/* Define to __typeof__ if your compiler spells it that way. */
#undef typeof

/* Correctly set _XSERVER64 for OSX fat binaries */
#ifdef __APPLE__
#include "dix-config-apple-verbatim.h"
#endif

/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# undef __EXTENSIONS__
#endif

/* Defined if needed to expose struct msghdr.msg_control */
#undef _XOPEN_SOURCE

/* Have support for X shared memory fence library (xshmfence) */
#undef HAVE_XSHMFENCE

/* Use XTrans FD passing support */
#undef XTRANS_SEND_FDS

/* Wrap SIGBUS to catch MIT-SHM faults */
#undef BUSFAULT

/* Don't let Xdefs.h define 'pointer' */
#define _XTYPEDEF_POINTER       1

/* Don't let XIproto define 'Pointer' */
#define _XITYPEDEF_POINTER      1

/* Ask fontsproto to make font path element names const */
#define FONT_PATH_ELEMENT_NAME_CONST    1

/* Build GLAMOR */
#undef GLAMOR

/* Build glamor's GBM-based EGL support */
#undef GLAMOR_HAS_GBM

/* Build glamor/gbm has linear support */
#undef GLAMOR_HAS_GBM_LINEAR

/* GBM has modifiers support */
#undef GBM_BO_WITH_MODIFIERS

/* Glamor can use eglQueryDmaBuf* functions */
#undef GLAMOR_HAS_EGL_QUERY_DMABUF

/* Glamor can use EGL_MESA_query_driver functions */
#undef GLAMOR_HAS_EGL_QUERY_DRIVER

/* byte order */
#undef X_BYTE_ORDER

/* Listen on TCP socket */
#undef LISTEN_TCP

/* Listen on Unix socket */
#define LISTEN_UNIX 1

/* Listen on local socket */
#undef LISTEN_LOCAL

/* Define if no local socket credentials interface exists */
#undef NO_LOCAL_CLIENT_CRED

/* Have setitimer support */
#undef HAVE_SETITIMER

/* Have posix_fallocate() */
#undef HAVE_POSIX_FALLOCATE

/* Use input thread */
#undef INPUTTHREAD

/* Have poll() */
#undef HAVE_POLL

/* Have epoll_create1() */
#undef HAVE_EPOLL_CREATE1

/* Have <sys/sysmacros.h> header */
#undef HAVE_SYS_SYSMACROS_H

/* Have sigprocmask */
#undef HAVE_SIGPROCMASK

/* Have isastream */
#undef HAVE_ISASTREAM

#endif /* _DIX_CONFIG_H_ */

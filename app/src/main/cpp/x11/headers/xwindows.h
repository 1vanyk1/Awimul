#undef _XFree86Server
#ifdef XFree86Server
# define _XFree86Server
# undef XFree86Server
#endif

#define NOMINMAX
#define BOOL wBOOL
#define INT32 wINT32
#undef Status
#define Status wStatus
#define ATOM wATOM
#define BYTE wBYTE
#define FreeResource wFreeResource
#include "windows/windows.h"
#undef NOMINMAX
#undef Status
#define Status int
#undef BYTE
#undef BOOL
#undef INT32
#undef ATOM
#undef FreeResource
#undef CreateWindowA

#ifdef RESOURCE_H
# undef RT_FONT
# undef RT_CURSOR
# define RT_FONT         ((RESTYPE)4)
# define RT_CURSOR       ((RESTYPE)5)
#endif

#ifndef __CYGWIN__
#define sleep(x) Sleep((x) * 1000)
#endif

#if defined(WIN32) && (!defined(PATH_MAX) || PATH_MAX < 1024)
# undef PATH_MAX
# define PATH_MAX 1024
#endif

#ifdef _XFree86Server
# define XFree86Server
# undef _XFree86Server
#endif
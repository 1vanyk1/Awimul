#undef _XFree86Server
#ifdef XFree86Server
# define _XFree86Server
# undef XFree86Server
#endif

#define BOOL wBOOL
#define INT32 wINT32
#undef Status
#define Status wStatus
#define ATOM wATOM
#define BYTE wBYTE
#define FreeResource wFreeResource
#include <winsock2.h>
#undef Status
#define Status int
#undef BYTE
#undef BOOL
#undef INT32
#undef ATOM
#undef FreeResource
#undef CreateWindowA
#undef RT_FONT
#undef RT_CURSOR

#ifdef _XFree86Server
# define XFree86Server
# undef _XFree86Server
#endif

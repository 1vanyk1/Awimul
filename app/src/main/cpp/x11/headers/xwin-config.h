#include "dix-config.h"

/* Winsock networking */
#undef HAS_WINSOCK

/* Cygwin has /dev/windows for signaling new win32 messages */
#undef HAS_DEVWINDOWS

/* Switch on debug messages */
#undef CYGDEBUG
#undef CYGWINDOWING_DEBUG
#undef CYGMULTIWINDOW_DEBUG

/* Define to 1 if unsigned long is 64 bits. */
#undef _XSERVER64

/* Short vendor name */
#undef XVENDORNAMESHORT

/* Vendor web address for support */
#undef __VENDORDWEBSUPPORT__

/* Default log location */
#define DEFAULT_LOGDIR ""

/* Whether we should re-locate the root to where the executable lives */
#undef RELOCATE_PROJECTROOT

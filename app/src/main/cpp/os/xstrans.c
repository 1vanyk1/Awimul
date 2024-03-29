#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../x11/headers/xfuncproto.h"

/* ErrorF is used by xtrans */
#ifndef HAVE_DIX_CONFIG_H
extern _X_EXPORT void
ErrorF(const char *f, ...)
_X_ATTRIBUTE_PRINTF(1, 2);
#endif

#define TRANS_REOPEN
#define TRANS_SERVER
#define XSERV_t
#include "../x11/headers/Xtrans/transport.c"

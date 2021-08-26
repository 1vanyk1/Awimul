#ifndef XORG_PROBES_H
#define XORG_PROBES_H

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

/* definitions needed to include Dtrace probes in a source file */

#ifdef XSERVER_DTRACE
#include <sys/types.h>
typedef const char *string;
typedef const uint8_t *const_uint8_p;
typedef const double *const_double_p;
#include "Xserver-dtrace.h"
#endif

#endif /* XORG_PROBES_H */

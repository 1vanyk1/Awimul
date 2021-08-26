#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _ROOTLESSCONFIG_H
#define _ROOTLESSCONFIG_H

#ifdef __APPLE__
#define ROOTLESS_RESIZE_GRAVITY TRUE
#endif

/*# define ROOTLESSDEBUG*/

#define ROOTLESS_PROTECT_ALPHA TRUE
#define ROOTLESS_REDISPLAY_DELAY 10

/* Bit mask for alpha channel with a particular number of bits per
   pixel. Note that we only care for 32bpp data. Mac OS X uses planar
   alpha for 16bpp. */
#define RootlessAlphaMask(bpp) ((bpp) == 32 ? 0xFF000000 : 0)

#endif                          /* _ROOTLESSCONFIG_H */

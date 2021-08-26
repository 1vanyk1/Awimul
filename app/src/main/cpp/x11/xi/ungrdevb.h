#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef UNGRDEVB_H
#define UNGRDEVB_H 1

#include "../headers/xdefs.h"

int SProcXUngrabDeviceButton(ClientPtr  /* client */
);

int ProcXUngrabDeviceButton(ClientPtr   /* client */
);

#endif                          /* UNGRDEVB_H */

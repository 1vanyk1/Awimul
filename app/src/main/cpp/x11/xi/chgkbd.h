#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHGKBD_H
#define CHGKBD_H 1

#include "../headers/xdefs.h"

int SProcXChangeKeyboardDevice(ClientPtr        /* client */
);

int ProcXChangeKeyboardDevice(ClientPtr /* client */
);

#endif                          /* CHGKBD_H */

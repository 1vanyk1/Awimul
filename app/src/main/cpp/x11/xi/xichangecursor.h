#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef CHDEVCUR_H
#define CHDEVCUR_H 1

#include "../headers/xdefs.h"

int SProcXIChangeCursor(ClientPtr /* client */ );
int ProcXIChangeCursor(ClientPtr /* client */ );

#endif                          /* CHDEVCUR_H */

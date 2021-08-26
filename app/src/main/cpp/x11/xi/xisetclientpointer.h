#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef SETCPTR_H
#define SETCPTR_H 1

#include "../headers/xdefs.h"

int SProcXISetClientPointer(ClientPtr /* client */ );
int ProcXISetClientPointer(ClientPtr /* client */ );

#endif                          /* SETCPTR_H */

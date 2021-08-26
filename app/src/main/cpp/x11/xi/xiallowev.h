#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XIALLOWEV_H
#define XIALLOWEV_H 1

#include "../headers/xdefs.h"

int ProcXIAllowEvents(ClientPtr client);
int SProcXIAllowEvents(ClientPtr client);

#endif                          /* XIALLOWEV_H */

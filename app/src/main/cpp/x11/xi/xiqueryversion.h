#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

#ifndef QUERYVERSION_H
#define QUERYVERSION_H 1

int SProcXIQueryVersion(ClientPtr client);
int ProcXIQueryVersion(ClientPtr client);
void SRepXIQueryVersion(ClientPtr client, int size, xXIQueryVersionReply * rep);

#endif                          /* QUERYVERSION_H */

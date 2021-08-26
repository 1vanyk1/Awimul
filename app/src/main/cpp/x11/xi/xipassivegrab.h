#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XIPASSIVEGRAB_H
#define XIPASSIVEGRAB_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int SProcXIPassiveUngrabDevice(ClientPtr client);
int ProcXIPassiveUngrabDevice(ClientPtr client);
void SRepXIPassiveGrabDevice(ClientPtr client, int size,
                             xXIPassiveGrabDeviceReply * rep);
int ProcXIPassiveGrabDevice(ClientPtr client);
int SProcXIPassiveGrabDevice(ClientPtr client);

#endif                          /* XIPASSIVEGRAB_H */

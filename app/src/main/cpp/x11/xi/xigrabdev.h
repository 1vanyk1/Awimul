#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef XIGRABDEV_H
#define XIGRABDEV_H 1

#include "../headers/xdefs.h"
#include "../headers/extensions/XI2proto.h"

int ProcXIGrabDevice(ClientPtr client);
int SProcXIGrabDevice(ClientPtr client);

int ProcXIUngrabDevice(ClientPtr client);
int SProcXIUngrabDevice(ClientPtr client);

void SRepXIGrabDevice(ClientPtr client, int size, xXIGrabDeviceReply * rep);

#endif                          /* XIGRABDEV_H */

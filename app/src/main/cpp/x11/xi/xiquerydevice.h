#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef QUERYDEV_H
#define QUERYDEV_H 1

#include "../headers/extensions/XI2proto.h"
#include "../headers/xdefs.h"
#include "../headers/input.h"
#include <stdint.h>

int SProcXIQueryDevice(ClientPtr client);
int ProcXIQueryDevice(ClientPtr client);
void SRepXIQueryDevice(ClientPtr client, int size, xXIQueryDeviceReply * rep);
int SizeDeviceClasses(DeviceIntPtr dev);
int ListDeviceClasses(ClientPtr client, DeviceIntPtr dev,
                      char *any, uint16_t * nclasses);
int GetDeviceUse(DeviceIntPtr dev, uint16_t * attachment);
int ListButtonInfo(DeviceIntPtr dev, xXIButtonInfo * info, Bool reportState);
int ListKeyInfo(DeviceIntPtr dev, xXIKeyInfo * info);
int ListValuatorInfo(DeviceIntPtr dev, xXIValuatorInfo * info,
                     int axisnumber, Bool reportState);
int ListScrollInfo(DeviceIntPtr dev, xXIScrollInfo * info, int axisnumber);
int ListTouchInfo(DeviceIntPtr dev, xXITouchInfo * info);
#endif                          /* QUERYDEV_H */
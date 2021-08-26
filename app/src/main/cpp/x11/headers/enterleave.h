#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef ENTERLEAVE_H
#define ENTERLEAVE_H

#include "dix.h" /* DoFocusEvents() */

extern void DoEnterLeaveEvents(DeviceIntPtr pDev,
                               int sourceid,
                               WindowPtr fromWin, WindowPtr toWin, int mode);

extern void EnterLeaveEvent(DeviceIntPtr mouse,
                            int type,
                            int mode, int detail, WindowPtr pWin, Window child);

extern void CoreEnterLeaveEvent(DeviceIntPtr mouse,
                                int type,
                                int mode,
                                int detail, WindowPtr pWin, Window child);
extern void DeviceEnterLeaveEvent(DeviceIntPtr mouse,
                                  int sourceid,
                                  int type,
                                  int mode,
                                  int detail, WindowPtr pWin, Window child);
extern void DeviceFocusEvent(DeviceIntPtr dev,
                             int type,
                             int mode,
                             int detail ,
                             WindowPtr pWin);

extern void EnterWindow(DeviceIntPtr dev, WindowPtr win, int mode);

extern void LeaveWindow(DeviceIntPtr dev);

extern void CoreFocusEvent(DeviceIntPtr kbd,
                           int type, int mode, int detail, WindowPtr pWin);

extern void SetFocusIn(DeviceIntPtr kbd, WindowPtr win);

extern void SetFocusOut(DeviceIntPtr dev);
#endif                          /* _ENTERLEAVE_H_ */
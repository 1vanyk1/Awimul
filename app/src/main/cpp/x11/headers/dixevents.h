#ifndef DIXEVENTS_H
#define DIXEVENTS_H

#include "xlocale.h"
#include "xproto.h"
#include "xdefs.h"
#include "window.h"

extern _X_EXPORT void SetCriticalEvent(int /* event */ );

extern _X_EXPORT CursorPtr GetSpriteCursor(DeviceIntPtr /*pDev */ );

extern _X_EXPORT int ProcAllowEvents(ClientPtr /* client */ );

extern _X_EXPORT int MaybeDeliverEventsToClient(WindowPtr /* pWin */ ,
                                                xEvent * /* pEvents */ ,
                                                int /* count */ ,
                                                Mask /* filter */ ,
                                                ClientPtr /* dontClient */ );

extern _X_EXPORT int ProcWarpPointer(ClientPtr /* client */ );

extern _X_EXPORT int EventSelectForWindow(WindowPtr /* pWin */ ,
                                          ClientPtr /* client */ ,
                                          Mask /* mask */ );

extern _X_EXPORT int EventSuppressForWindow(WindowPtr /* pWin */ ,
                                            ClientPtr /* client */ ,
                                            Mask /* mask */ ,
                                            Bool * /* checkOptional */ );

extern _X_EXPORT int ProcSetInputFocus(ClientPtr /* client */ );

extern _X_EXPORT int ProcGetInputFocus(ClientPtr /* client */ );

extern _X_EXPORT int ProcGrabPointer(ClientPtr /* client */ );

extern _X_EXPORT int ProcChangeActivePointerGrab(ClientPtr /* client */ );

extern _X_EXPORT int ProcUngrabPointer(ClientPtr /* client */ );

extern _X_EXPORT int ProcGrabKeyboard(ClientPtr /* client */ );

extern _X_EXPORT int ProcUngrabKeyboard(ClientPtr /* client */ );

extern _X_EXPORT int ProcQueryPointer(ClientPtr /* client */ );

extern _X_EXPORT int ProcSendEvent(ClientPtr /* client */ );

extern _X_EXPORT int ProcUngrabKey(ClientPtr /* client */ );

extern _X_EXPORT int ProcGrabKey(ClientPtr /* client */ );

extern _X_EXPORT int ProcGrabButton(ClientPtr /* client */ );

extern _X_EXPORT int ProcUngrabButton(ClientPtr /* client */ );

extern _X_EXPORT int ProcRecolorCursor(ClientPtr /* client */ );

#endif                          /* DIXEVENTS_H */
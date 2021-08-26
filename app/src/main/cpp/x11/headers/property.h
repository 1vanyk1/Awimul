#ifndef PROPERTY_H
#define PROPERTY_H

#include "window.h"

typedef struct _Property *PropertyPtr;

typedef struct _PropertyStateRec {
    WindowPtr win;
    PropertyPtr prop;
    int state;
} PropertyStateRec;

extern CallbackListPtr PropertyStateCallback;

extern _X_EXPORT int dixLookupProperty(PropertyPtr * /*result */ ,
                                       WindowPtr /*pWin */ ,
                                       Atom /*proprty */ ,
                                       ClientPtr /*pClient */ ,
                                       Mask /*access_mode */ );

extern _X_EXPORT int dixChangeWindowProperty(ClientPtr pClient,
                                             WindowPtr pWin,
                                             Atom property,
                                             Atom type,
                                             int format,
                                             int mode,
                                             unsigned long len,
                                             void *value,
                                             Bool sendevent);

extern _X_EXPORT int DeleteProperty(ClientPtr /*client */ ,
                                    WindowPtr /*pWin */ ,
                                    Atom /*propName */ );

extern _X_EXPORT void DeleteAllWindowProperties(WindowPtr /*pWin */ );

#endif                          /* PROPERTY_H */
#ifndef _XtObject_h
#define _XtObject_h

#include "Intrinsic.h"

_XFUNCPROTOBEGIN

typedef struct _ObjectRec *Object;
typedef struct _ObjectClassRec *ObjectClass;

#ifndef VMS
externalref WidgetClass objectClass;
#endif

_XFUNCPROTOEND

#endif /* _XtObject_h */

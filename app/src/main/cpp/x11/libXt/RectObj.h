#ifndef _XtRect_h
#define _XtRect_h

#include "Intrinsic.h"

_XFUNCPROTOBEGIN

typedef struct _RectObjRec *RectObj;
typedef struct _RectObjClassRec *RectObjClass;

#ifndef VMS
externalref WidgetClass rectObjClass;
#endif

_XFUNCPROTOEND

#endif /* _XtRect_h */

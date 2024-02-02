#ifndef _XtCore_h
#define _XtCore_h

#include "Intrinsic.h"

_XFUNCPROTOBEGIN

typedef struct _WidgetClassRec *CoreWidgetClass;
typedef struct _WidgetRec *CoreWidget;
externalref WidgetClass coreWidgetClass;

#ifndef VMS
externalref WidgetClass widgetClass;

#endif

_XFUNCPROTOEND

#endif /* _XtCore_h */

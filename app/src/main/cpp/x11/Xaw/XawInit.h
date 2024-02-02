#ifndef _XawInit_h
#define _XawInit_h

#define XawVendor	XVENDORNAMESHORT

#ifdef OLDXAW
#define XawVersion	6700002L
#else
#define	XawVersion	7000002L

typedef struct _XawDL XawDisplayList;
#endif /* OLDXAW */

#include "../libXt/Intrinsic.h"
#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

void XawInitializeWidgetSet(void);
#ifndef OLDXAW
void XawInitializeDefaultConverters(void);
#endif

extern Widget XawOpenApplication(
        XtAppContext *app_context_return,
        Display      *dpy,
        Screen       *screen,
        String        application_name,
        String        application_class,
        WidgetClass   widget_class,
        int          *argc,
        _XtString    *argv
);

_XFUNCPROTOEND

#endif /* _XawInit_h */

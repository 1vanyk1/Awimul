#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../libXt/Intrinsic.h"
#include "../libXt/Vendor.h"
#include "XawInit.h"
#include "../libXt/IntrinsicP.h"
#include "../libXt/StringDefs.h"
#include "Private.h"

void
XawInitializeWidgetSet(void)
{
    static Boolean firsttime = True;

    if (firsttime) {
        firsttime = False;
#ifndef OLDXAW
        XawPixmapsInitialize();
        XawInitializeDefaultConverters();
#endif
        XtInitializeWidgetClass(vendorShellWidgetClass);
    }
}

/* XawOpenApplication() - mainly identical to XtOpenApplication() but
 * takes a |Display *| and |Screen *| as arguments, too... */
Widget XawOpenApplication(XtAppContext *app_context_return,
                          Display      *dpy,
                          Screen       *screen,
                          String        application_name,
                          String        application_class,
                          WidgetClass   widget_class,
                          int          *argc,
                          _XtString    *argv)
{
    Widget   toplevel;
    Cardinal n;
    Arg      args[2];

    XtToolkitInitialize();
    *app_context_return = XtCreateApplicationContext();
    if( *app_context_return == NULL )
        return NULL;

    XtDisplayInitialize(*app_context_return, dpy,
                        application_name, application_class,
                        NULL, 0,
                        argc, argv);

    n = 0;
    if (screen) {
        XtSetArg(args[n], XtNscreen, screen); n++;
    }
    toplevel = XtAppCreateShell(application_name,
                                application_class,
                                widget_class,
                                dpy,
                                args, n);

    return toplevel;
}


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../x11/libXt/Intrinsic.h"
#include "../x11/libXt/StringDefs.h"
#include "../x11/libXt/Shell.h"
#include "Eyes.h"
#include <stdio.h>
#include <stdlib.h>
#include "eyes.bit"
#include "eyesmask.bit"

/* Exit with message describing command line format */

static void _X_NORETURN
usage(void)
{
    fprintf(stderr,
            "usage: xeyes\n"
            "       [-display [{host}]:[{vs}]]\n"
            "       [-geometry [{width}][x{height}][{+-}{xoff}[{+-}{yoff}]]]\n"
            "       [-fg {color}] [-bg {color}] [-bd {color}] [-bw {pixels}]\n"
            "       [-shape | +shape] [-outline {color}] [-center {color}]\n"
            "       [-backing {backing-store}] [-distance]\n");
#ifdef XRENDER
    fprintf(stderr,
	    "       [-render | +render]\n");
#endif
#ifdef PRESENT
    fprintf(stderr,
	    "       [-present | +present]\n");
#endif
    exit(1);
}

/* Command line options table.  Only resources are entered here...there is a
   pass over the remaining options after XtParseCommand is let loose. */

static XrmOptionDescRec options[] = {
        {(char *)"-outline",	(char *)"*eyes.outline",	XrmoptionSepArg,	NULL},
        {(char *)"-center",	(char *)"*eyes.center",		XrmoptionSepArg,	NULL},
        {(char *)"-backing",	(char *)"*eyes.backingStore",	XrmoptionSepArg,	NULL},
        {(char *)"-shape",	(char *)"*eyes.shapeWindow",	XrmoptionNoArg,		(char *)"TRUE"},
        {(char *)"+shape",	(char *)"*eyes.shapeWindow",	XrmoptionNoArg,		(char *)"FALSE"},
#ifdef XRENDER
        {(char *)"-render",	(char *)"*eyes.render",		XrmoptionNoArg,		(char *)"TRUE"},
{(char *)"+render",	(char *)"*eyes.render",		XrmoptionNoArg,		(char *)"FALSE"},
#endif
#ifdef PRESENT
        {(char *)"-present",	(char *)"*eyes.present",	XrmoptionNoArg,		(char *)"TRUE"},
{(char *)"+present",	(char *)"*eyes.present",	XrmoptionNoArg,		(char *)"FALSE"},
#endif
        {(char *)"-distance",	(char *)"*eyes.distance",	XrmoptionNoArg,		(char *)"TRUE"},
};

static Atom wm_delete_window;

/*ARGSUSED*/
static void
quit(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    if (event->type == ClientMessage &&
        event->xclient.data.l[0] != wm_delete_window) {
        XBell(XtDisplay(w), 0);
    } else {
        XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
        exit(0);
    }
}

static XtActionsRec actions[] = {
        {(char *) "quit",	quit}
};

int
main_xeyes(int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel;
    Arg arg[2];
    Cardinal i;

    XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);

    toplevel = XtAppInitialize(&app_context, "XEyes",
                               options, XtNumber(options), &argc, argv,
                               NULL, arg, (Cardinal) 0);
    if (argc != 1) usage();

    wm_delete_window = XInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW",
                                   False);
    XtAppAddActions(app_context, actions, XtNumber(actions));
    XtOverrideTranslations
            (toplevel, XtParseTranslationTable ("<Message>WM_PROTOCOLS: quit()"));

    i = 0;
    XtSetArg (arg[i], XtNiconPixmap,
              XCreateBitmapFromData (XtDisplay(toplevel),
                                     XtScreen(toplevel)->root,
                                     (char *)eyes_bits, eyes_width, eyes_height));
    i++;
    XtSetArg (arg[i], XtNiconMask,
              XCreateBitmapFromData (XtDisplay(toplevel),
                                     XtScreen(toplevel)->root,
                                     (char *)eyesmask_bits,
                                     eyesmask_width, eyesmask_height));
    i++;
    XtSetValues (toplevel, arg, i);

    (void) XtCreateManagedWidget ("eyes", eyesWidgetClass, toplevel, NULL, 0);
    XtRealizeWidget (toplevel);
    (void) XSetWMProtocols (XtDisplay(toplevel), XtWindow(toplevel),
                            &wm_delete_window, 1);
    XtAppMainLoop(app_context);

    return 0;
}

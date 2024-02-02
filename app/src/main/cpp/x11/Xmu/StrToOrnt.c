#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../libXt/Intrinsic.h"
#include "../libXt/StringDefs.h"
#include "Converters.h"
#include "CharSet.h"

/*
 * Prototypes
 */
static void InitializeQuarks(void);

/*
 * Initialization
 */
static	XrmQuark Qhorizontal, Qvertical;
static Boolean haveQuarks;

/*
 * Implementation
 */
static void
InitializeQuarks(void)
{
    if (!haveQuarks)
    {
        Qhorizontal = XrmPermStringToQuark(XtEhorizontal);
        Qvertical = XrmPermStringToQuark(XtEvertical);
        haveQuarks = True;
    }
}

/*ARGSUSED*/
void
XmuCvtStringToOrientation(XrmValuePtr args, Cardinal *num_args,
                          XrmValuePtr fromVal, XrmValuePtr toVal)
{
    static XtOrientation orient;
    XrmQuark	q;
    char name[11];

    InitializeQuarks();
    XmuNCopyISOLatin1Lowered(name, (char *)fromVal->addr, sizeof(name));
    q = XrmStringToQuark(name);

    toVal->size = sizeof(XtJustify);
    toVal->addr = (XPointer)&orient;

    if (q == Qhorizontal)
        orient = XtorientHorizontal;
    else if (q == Qvertical)
        orient = XtorientVertical;
    else
    {
        toVal->addr = NULL;
        XtStringConversionWarning((char *)fromVal->addr, XtROrientation);
    }
}

/*ARGSUSED*/
Boolean
XmuCvtOrientationToString(Display *dpy, XrmValuePtr args, Cardinal *num_args,
                          XrmValuePtr fromVal, XrmValuePtr toVal,
                          XtPointer *data)
{
    static String buffer;
    Cardinal size;

    switch (*(XtOrientation *)fromVal->addr)
    {
        case XtorientVertical:
            buffer = XtEvertical;
            break;
        case XtorientHorizontal:
            buffer = XtEhorizontal;
            break;
        default:
            XtWarning("Cannot convert Orientation to String");
            toVal->addr = NULL;
            toVal->size = 0;
            return (False);
    }

    size = strlen(buffer) + 1;
    if (toVal->addr != NULL)
    {
        if (toVal->size < size)
        {
            toVal->size = size;
            return (False);
        }
        strcpy((char *)toVal->addr, buffer);
    }
    else
        toVal->addr = (XPointer)buffer;
    toVal->size = sizeof(String);

    return (True);
}

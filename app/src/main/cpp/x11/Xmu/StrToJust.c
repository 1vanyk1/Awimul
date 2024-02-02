#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string.h>
#include "../libXt/Intrinsic.h"
#include "Converters.h"
#include "CharSet.h"

/*
 * Prototypes
 */
static void InitializeQuarks(void);

/*
 * Initialization
 */
static XrmQuark Qleft, Qcenter, Qright;
static Boolean haveQuarks;

/*
 * Implementation
 */
static void
InitializeQuarks(void)
{
    if (!haveQuarks)
    {
        Qleft = XrmPermStringToQuark(XtEleft);
        Qcenter = XrmPermStringToQuark(XtEcenter);
        Qright = XrmPermStringToQuark(XtEright);
        haveQuarks = True;
    }
}

/*ARGSUSED*/
void
XmuCvtStringToJustify(XrmValuePtr args, Cardinal *num_args,
                      XrmValuePtr fromVal, XrmValuePtr toVal)
{
    static XtJustify	e;
    XrmQuark    q;
    char *s = (char *)fromVal->addr;
    char name[7];

    if (s == NULL)
        return;

    InitializeQuarks();
    XmuNCopyISOLatin1Lowered(name, s, sizeof(name));

    q = XrmStringToQuark(name);

    toVal->size = sizeof(XtJustify);
    toVal->addr = (XPointer)&e;

    if (q == Qleft)
        e = XtJustifyLeft;
    else if (q == Qcenter)
        e = XtJustifyCenter;
    else if (q == Qright)
        e = XtJustifyRight;
    else
    {
        toVal->addr = NULL;
        XtStringConversionWarning((char *)fromVal->addr, XtRJustify);
    }
}

/*ARGSUSED*/
Boolean
XmuCvtJustifyToString(Display *dpy, XrmValue* args, Cardinal *num_args,
                      XrmValue *fromVal, XrmValue *toVal, XtPointer *data)
{
    static String buffer;
    Cardinal size;

    switch (*(XtJustify *)fromVal->addr)
    {
        case XtJustifyLeft:
            buffer = XtEleft;
            break;
        case XtJustifyCenter:
            buffer = XtEcenter;
            break;
        case XtJustifyRight:
            buffer = XtEright;
            break;
        default:
            XtWarning("Cannot convert Justify to String");
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

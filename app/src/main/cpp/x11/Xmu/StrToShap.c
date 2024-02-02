#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string.h>
#include "../libXt/Intrinsic.h"
#include "Converters.h"
#include "CharSet.h"

/* ARGSUSED */
#define	done(type, value) \
	{							\
	    if (toVal->addr != NULL) {				\
		if (toVal->size < sizeof(type)) {		\
		    toVal->size = sizeof(type);			\
		    return False;				\
		}						\
		*(type*)(toVal->addr) = (value);		\
	    }							\
	    else {						\
		static type static_val;				\
		static_val = (value);				\
		toVal->addr = (XtPointer)&static_val;		\
	    }							\
	    toVal->size = sizeof(type);				\
	    return True;					\
	}


/*ARGSUSED*/
Boolean
XmuCvtStringToShapeStyle(Display *dpy, XrmValue *args, Cardinal *num_args,
                         XrmValue *from, XrmValue *toVal, XtPointer *data)
{
    String name = (String)from->addr;

    if (XmuCompareISOLatin1(name, XtERectangle) == 0)
    done(int, XmuShapeRectangle);
    if (XmuCompareISOLatin1(name, XtEOval) == 0)
    done(int, XmuShapeOval);
    if (XmuCompareISOLatin1(name, XtEEllipse) == 0)
    done(int, XmuShapeEllipse);
    if (XmuCompareISOLatin1(name, XtERoundedRectangle) == 0)
    done(int, XmuShapeRoundedRectangle);

    XtDisplayStringConversionWarning(dpy, name, XtRShapeStyle);

    return (False);
}

/*ARGSUSED*/
Boolean
XmuCvtShapeStyleToString(Display *dpy, XrmValue *args, Cardinal *num_args,
                         XrmValue *fromVal, XrmValue *toVal, XtPointer *data)
{
    static char *buffer;
    Cardinal size;

    switch (*(int *)fromVal->addr)
    {
        case XmuShapeRectangle:
            buffer = XtERectangle;
            break;
        case XmuShapeOval:
            buffer = XtEOval;
            break;
        case XmuShapeEllipse:
            buffer = XtEEllipse;
            break;
        case XmuShapeRoundedRectangle:
            buffer = XtERoundedRectangle;
            break;
        default:
            XtAppWarning(XtDisplayToApplicationContext(dpy),
                         "Cannot convert ShapeStyle to String");
            toVal->addr = NULL;
            toVal->size = 0;

            return (False);
    }

    size = strlen(buffer) + 1;
    if (toVal->addr != NULL)
    {
        if (toVal->size <= size)
        {
            toVal->size = size;
            return (False);
        }
        strcpy((char *)toVal->addr, buffer);
    }
    else
        toVal->addr = (XPointer)buffer;
    toVal->size = size;

    return (True);
}

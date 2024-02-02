#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "../libXt/IntrinsicP.h"
#include "../libXt/StringDefs.h"
#include "AsciiTextP.h"
#include "AsciiSrcP.h"
#include "AsciiSink.h"
#include "Cardinals.h"
#include "MultiSinkP.h"
#include "MultiSrc.h"
#include "XawImP.h"
#include "XawInit.h"
#include "Private.h"

#define TAB_COUNT 32

/*
 * Class Methods
 */
static void XawAsciiInitialize(Widget, Widget, ArgList, Cardinal*);
static void XawAsciiDestroy(Widget);

/*
 * From TextSrc.c
 */
void _XawSourceAddText(Widget, Widget);
void _XawSourceRemoveText(Widget, Widget, Bool);

#define Superclass	(&textClassRec)
AsciiTextClassRec asciiTextClassRec = {
        /* core */
        {
                (WidgetClass)Superclass,		/* superclass */
                "Text",				/* class_name */
                sizeof(AsciiRec),			/* widget_size */
                XawInitializeWidgetSet,		/* class_initialize */
                NULL,				/* class_part_init */
                False,				/* class_inited */
                XawAsciiInitialize,			/* initialize */
                NULL,				/* initialize_hook */
                XtInheritRealize,			/* realize */
                NULL,				/* actions */
                0,					/* num_actions */
                NULL,				/* resources */
                0,					/* num_resource */
                NULLQUARK,				/* xrm_class */
                True,				/* compress_motion */
                XtExposeGraphicsExpose |		/* compress_exposure */
                XtExposeNoExpose,
                True,				/* compress_enterleave */
                False,				/* visible_interest */
                XawAsciiDestroy,			/* destroy */
                XtInheritResize,			/* resize */
                XtInheritExpose,			/* expose */
                NULL,				/* set_values */
                NULL,				/* set_values_hook */
                XtInheritSetValuesAlmost,		/* set_values_almost */
                NULL,				/* get_values_hook */
                XtInheritAcceptFocus,		/* accept_focus */
                XtVersion,				/* version */
                NULL,				/* callback_private */
                XtInheritTranslations,		/* tm_table */
                XtInheritQueryGeometry,		/* query_geometry */
        },
        /* simple */
        {
                XtInheritChangeSensitive,		/* change_sensitive */
        },
        /* text */
        {
                NULL,				/* extension */
        },
        /* ascii */
        {
                NULL,				/* extension */
        },
};

WidgetClass asciiTextWidgetClass = (WidgetClass)&asciiTextClassRec;

#ifdef ASCII_STRING
AsciiStringClassRec asciiStringClassRec = {
  /* core */
  {
    (WidgetClass)&asciiTextClassRec,	/* superclass */
    "Text",				/* class_name */
    sizeof(AsciiStringRec),		/* widget_size */
    NULL,				/* class_initialize */
    NULL,				/* class_part_init */
    False,				/* class_inited */
    NULL,				/* initialize */
    NULL,				/* initialize_hook */
    XtInheritRealize,			/* realize */
    NULL,				/* actions */
    0,					/* num_actions */
    NULL,				/* resources */
    0,					/* num_resource */
    NULLQUARK,				/* xrm_class */
    True,				/* compress_motion */
    XtExposeGraphicsExpose |		/* compress_exposure */
	XtExposeNoExpose,
    True,				/* compress_enterleave */
    False,				/* visible_interest */
    NULL,				/* destroy */
    XtInheritResize,			/* resize */
    XtInheritExpose,			/* expose */
    NULL,				/* set_values */
    NULL,				/* set_values_hook */
    XtInheritSetValuesAlmost,		/* set_values_almost */
    NULL,				/* get_values_hook */
    XtInheritAcceptFocus,		/* accept_focus */
    XtVersion,				/* version */
    NULL,				/* callback_private */
    XtInheritTranslations,		/* tm_table */
    XtInheritQueryGeometry,		/* query_geometry */
  },
  /* simple */
  {
    XtInheritChangeSensitive,		/* change_sensitive */
  },
  /* text */
  {
    NULL,				/* extension */
  },
  /* ascii */
  {
    NULL,				/* extension */
  },
  /* string */
  {
    NULL,				/* extension */
  },
};

WidgetClass asciiStringWidgetClass = (WidgetClass)&asciiStringClassRec;
#endif /* ASCII_STRING */

#ifdef ASCII_DISK
AsciiDiskClassRec asciiDiskClassRec = {
  /* core */
  {
    (WidgetClass)&asciiTextClassRec,	/* superclass */
    "Text",				/* class_name */
    sizeof(AsciiDiskRec),		/* widget_size */
    NULL,				/* class_initialize */
    NULL,				/* class_part_init */
    False,				/* class_inited */
    NULL,				/* initialize */
    NULL,				/* initialize_hook */
    XtInheritRealize,			/* realize */
    NULL,				/* actions */
    0,					/* num_actions */
    NULL,				/* resources */
    0,					/* num_resource */
    NULLQUARK,				/* xrm_class */
    True,				/* compress_motion */
    True,				/* compress_exposure */
    XtExposeGraphicsExpose |		/* compress_enterleave */
	XtExposeNoExpose,
    False,				/* visible_interest */
    NULL,				/* destroy */
    XtInheritResize,			/* resize */
    XtInheritExpose,			/* expose */
    NULL,				/* set_values */
    NULL,				/* set_values_hook */
    XtInheritSetValuesAlmost,		/* set_values_almost */
    NULL,				/* get_values_hook */
    XtInheritAcceptFocus,		/* accept_focus */
    XtVersion,				/* version */
    NULL,				/* callback_private */
    XtInheritTranslations,		/* tm_table */
    XtInheritQueryGeometry,		/* query_geometry */
  },
  /* simple */
  {
    XtInheritChangeSensitive,		/* change_sensitive */
  },
  /* text */
  {
    NULL,				/* extension */
  },
  /* ascii */
  {
    NULL,				/* extension */
  },
  /* disk */
  {
    NULL,				/* extension */
  },
};

WidgetClass asciiDiskWidgetClass = (WidgetClass)&asciiDiskClassRec;
#endif /* ASCII_DISK */

/*
 * Implementation
 */
static void
XawAsciiInitialize(Widget request, Widget cnew,
                   ArgList args, Cardinal *num_args)
{
    AsciiWidget w = (AsciiWidget)cnew;
    int i;
    int tabs[TAB_COUNT], tab;

    MultiSinkObject sink;

    /* superclass Initialize can't set the following,
     * as it didn't know the source or sink when it was called
     */
    if (XtHeight(request) == DEFAULT_TEXT_HEIGHT)
        XtHeight(cnew) = DEFAULT_TEXT_HEIGHT;

    /* This is the main change for internationalization  */
    if (w->simple.international == True) { /* The multi* are international */
        if (w->text.sink == NULL)
            w->text.sink = XtCreateWidget("textSink", multiSinkObjectClass,
                                          cnew, args, *num_args);
        else if (!XtIsSubclass(w->text.sink, multiSinkObjectClass))
            XtError("Sink object is not a subclass of multiSink");

        if (w->text.source == NULL)
            w->text.source = XtCreateWidget("textSource", multiSrcObjectClass,
                                            cnew, args, *num_args);
        else if (!XtIsSubclass(w->text.source, multiSrcObjectClass))
            XtError("Source object is not a subclass of multiSrc");
#ifndef OLDXAW
        else
            _XawSourceAddText(w->text.source, cnew);
#endif
    }
    else {
        if (w->text.sink == NULL)
            w->text.sink = XtCreateWidget("textSink", asciiSinkObjectClass,
                                          cnew, args, *num_args);
        else if (!XtIsSubclass(w->text.source, asciiSinkObjectClass))
            XtError("Sink object is not a subclass of asciiSink");

        if (w->text.source == NULL)
            w->text.source = XtCreateWidget("textSource", asciiSrcObjectClass,
                                            cnew, args, *num_args);
        else if (!XtIsSubclass(w->text.source, asciiSrcObjectClass))
            XtError("Source object is not a subclass of asciiSrc");
#ifndef OLDXAW
        else
            _XawSourceAddText(w->text.source, cnew);
#endif
    }

    if (XtHeight(w) == DEFAULT_TEXT_HEIGHT)
        XtHeight(w) = (Dimension)(VMargins(w) + XawTextSinkMaxHeight(w->text.sink, 1));

    for (i = 0, tab = 0; i < TAB_COUNT; i++)
        tabs[i] = (tab += 8);

    XawTextSinkSetTabs(w->text.sink, TAB_COUNT, tabs);

    XawTextDisableRedisplay(cnew);
    XawTextEnableRedisplay(cnew);

    _XawImRegister(cnew);

    /* If we are using a MultiSink we need to tell the input method stuff */
    if (w->simple.international == True) {
        Arg list[4];
        Cardinal ac = 0;

        sink = (MultiSinkObject)w->text.sink;
        XtSetArg(list[ac], XtNfontSet, sink->multi_sink.fontset);	ac++;
        XtSetArg(list[ac], XtNinsertPosition, w->text.insertPos);	ac++;
        XtSetArg(list[ac], XtNforeground, sink->text_sink.foreground);	ac++;
        XtSetArg(list[ac], XtNbackground, sink->text_sink.background);	ac++;
        _XawImSetValues(cnew, list, ac);
    }
}

static void
XawAsciiDestroy(Widget w)
{
    AsciiWidget ascii = (AsciiWidget)w;

    _XawImUnregister(w);

    if (w == XtParent(ascii->text.sink))
        XtDestroyWidget(ascii->text.sink);

#ifdef OLDXAW
    if (w == XtParent(ascii->text.source))
	XtDestroyWidget(ascii->text.source);
#else
    _XawSourceRemoveText(ascii->text.source, w,
                         ascii->text.source &&
                         w == XtParent(ascii->text.source));
#endif
}

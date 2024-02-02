#ifndef _XtintrinsicP_h
#define _XtintrinsicP_h

#include "Intrinsic.h"

/*
 * Field sizes and offsets of XrmResource must match those of XtResource.
 * Type long is used instead of XrmQuark here because XrmQuark and String
 * are not the same size on all systems.
 */
typedef struct {
    XtIntPtr	xrm_name;	  /* Resource name quark		*/
    XtIntPtr	xrm_class;	  /* Resource class quark		*/
    XtIntPtr	xrm_type;	  /* Resource representation type quark */
    Cardinal	xrm_size;	  /* Size in bytes of representation	*/
    int		xrm_offset;	  /* -offset-1				*/
    XtIntPtr	xrm_default_type; /* Default representation type quark	*/
    XtPointer	xrm_default_addr; /* Default resource address		*/
} XrmResource, *XrmResourceList;
#if __STDC_VERSION__ >= 201112L
_Static_assert(XtOffsetOf(XrmResource, xrm_default_addr) ==
                   XtOffsetOf(XtResource, default_addr),
               "Field offset mismatch");
#endif

typedef unsigned long XtVersionType;

#define XT_VERSION 11
#ifndef XT_REVISION
#define XT_REVISION 6
#endif
#define XtVersion (XT_VERSION * 1000 + XT_REVISION)
#define XtVersionDontCheck 0

typedef void (*XtProc)(
        void
);

typedef void (*XtWidgetClassProc)(
        WidgetClass /* class */
);

typedef void (*XtWidgetProc)(
        Widget	/* widget */
);

typedef Boolean (*XtAcceptFocusProc)(
        Widget	/* widget */,
        Time*	/* time */
);

typedef void (*XtArgsProc)(
        Widget	/* widget */,
        ArgList	/* args */,
        Cardinal*	/* num_args */
);

typedef void (*XtInitProc)(
        Widget	/* request */,
        Widget	/* new */,
        ArgList	/* args */,
        Cardinal*	/* num_args */
);

typedef Boolean (*XtSetValuesFunc)(
        Widget 	/* old */,
        Widget 	/* request */,
        Widget 	/* new */,
        ArgList 	/* args */,
        Cardinal*	/* num_args */
);

typedef Boolean (*XtArgsFunc)(
        Widget	/* widget */,
        ArgList	/* args */,
        Cardinal*	/* num_args */
);

typedef void (*XtAlmostProc)(
        Widget		/* old */,
        Widget		/* new */,
        XtWidgetGeometry*	/* request */,
        XtWidgetGeometry*	/* reply */
);

typedef void (*XtExposeProc)(
        Widget	/* widget */,
        XEvent*	/* event */,
        Region	/* region */
);

/* compress_exposure options*/
#define XtExposeNoCompress		((XtEnum)False)
#define XtExposeCompressSeries		((XtEnum)True)
#define XtExposeCompressMultiple	2
#define XtExposeCompressMaximal		3

/* modifiers */
#define XtExposeGraphicsExpose	  	0x10
#define XtExposeGraphicsExposeMerged	0x20
#define XtExposeNoExpose	  	0x40
#define XtExposeNoRegion		0x80

typedef void (*XtRealizeProc)(
        Widget 		  /* widget */,
        XtValueMask* 	  /* mask */,
        XSetWindowAttributes* /* attributes */
);

typedef XtGeometryResult (*XtGeometryHandler)(
        Widget		/* widget */,
        XtWidgetGeometry*	/* request */,
        XtWidgetGeometry*	/* reply */
);

typedef void (*XtStringProc)(
        Widget	/* widget */,
        String	/* str */
);

typedef struct {
    String	name;	/* resource name */
    String	type;	/* representation type name */
    XtArgVal	value;	/* representation */
    int		size;	/* size of representation */
} XtTypedArg, *XtTypedArgList;

typedef void (*XtAllocateProc)(
        WidgetClass		/* widget_class */,
        Cardinal *		/* constraint_size */,
        Cardinal *		/* more_bytes */,
        ArgList		/* args */,
        Cardinal *		/* num_args */,
        XtTypedArgList	/* typed_args */,
        Cardinal *		/* num_typed_args */,
        Widget *		/* widget_return */,
        XtPointer *		/* more_bytes_return */
);

typedef void (*XtDeallocateProc)(
        Widget		/* widget */,
        XtPointer		/* more_bytes */
);

struct _XtStateRec;	/* Forward declare before use for C++ */

typedef struct _XtTMRec {
    XtTranslations  translations;	/* private to Translation Manager    */
    XtBoundActions  proc_table;		/* procedure bindings for actions    */
    struct _XtStateRec *current_state;  /* Translation Manager state ptr     */
    unsigned long   lastEventTime;
} XtTMRec, *XtTM;

#include "CoreP.h"
#include "CompositeP.h"
#include "ConstrainP.h"
#include "ObjectP.h"
#include "RectObjP.h"

#define XtDisplay(widget)	DisplayOfScreen((widget)->core.screen)
#define XtScreen(widget)	((widget)->core.screen)
#define XtWindow(widget)	((widget)->core.window)

#define XtClass(widget)		((widget)->core.widget_class)
#define XtSuperclass(widget)	(XtClass(widget)->core_class.superclass)
#define XtIsRealized(object)	(XtWindowOfObject(object) != None)
#define XtParent(widget)	((widget)->core.parent)

#undef XtIsRectObj
extern Boolean XtIsRectObj(Widget);
#define XtIsRectObj(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x02)

#undef XtIsWidget
extern Boolean XtIsWidget(Widget);
#define XtIsWidget(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x04)

#undef XtIsComposite
extern Boolean XtIsComposite(Widget);
#define XtIsComposite(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x08)

#undef XtIsConstraint
extern Boolean XtIsConstraint(Widget);
#define XtIsConstraint(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x10)

#undef XtIsShell
extern Boolean XtIsShell(Widget);
#define XtIsShell(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x20)

#undef XtIsWMShell
extern Boolean XtIsWMShell(Widget);
#define XtIsWMShell(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x40)

#undef XtIsTopLevelShell
extern Boolean XtIsTopLevelShell(Widget);
#define XtIsTopLevelShell(obj) \
    (((Object)(obj))->object.widget_class->core_class.class_inited & 0x80)

#ifdef DEBUG
#define XtCheckSubclass(w, widget_class_ptr, message)	\
	if (!XtIsSubclass(((Widget)(w)), (widget_class_ptr))) {	\
	    String dbgArgV[3];				\
	    Cardinal dbgArgC = 3;			\
	    dbgArgV[0] = ((Widget)(w))->core.widget_class->core_class.class_name;\
	    dbgArgV[1] = (widget_class_ptr)->core_class.class_name;	     \
	    dbgArgV[2] = (message);					     \
	    XtAppErrorMsg(XtWidgetToApplicationContext((Widget)(w)),	     \
		    "subclassMismatch", "xtCheckSubclass", "XtToolkitError", \
		    "Widget class %s found when subclass of %s expected: %s",\
		    dbgArgV, &dbgArgC);			\
	}
#else
#define XtCheckSubclass(w, widget_class, message)	/* nothing */
#endif

_XFUNCPROTOBEGIN

extern Widget _XtWindowedAncestor( /* internal; implementation-dependent */
        Widget 		/* object */
);

#if (defined(_WIN32) || defined(__CYGWIN__)) && !defined(LIBXT_COMPILATION)
__declspec(dllimport)
#else
extern
#endif
void _XtInherit(
        void
);

extern void _XtHandleFocus(
        Widget		/* widget */,
        XtPointer		/* client_data */,
        XEvent *		/* event */,
        Boolean *		/* cont */);

extern void XtCreateWindow(
        Widget 		/* widget */,
        unsigned int 	/* window_class */,
        Visual*		/* visual */,
        XtValueMask		/* value_mask */,
        XSetWindowAttributes* /* attributes */
);

extern void XtResizeWidget(
        Widget 		/* widget */,
        _XtDimension	/* width */,
        _XtDimension	/* height */,
        _XtDimension	/* border_width */
);

extern void XtMoveWidget(
        Widget 		/* widget */,
        _XtPosition		/* x */,
        _XtPosition		/* y */
);

extern void XtConfigureWidget(
        Widget 		/* widget */,
        _XtPosition		/* x */,
        _XtPosition		/* y */,
        _XtDimension	/* width */,
        _XtDimension	/* height */,
        _XtDimension	/* border_width */
);

extern void XtResizeWindow(
        Widget 		/* widget */
);

extern void XtProcessLock(
        void
);

extern void XtProcessUnlock(
        void
);

_XFUNCPROTOEND

#endif /* _XtIntrinsicP_h */
/* DON'T ADD STUFF AFTER THIS #endif */

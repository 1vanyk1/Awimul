#ifndef _XawForm_h
#define _XawForm_h

#include "../libXt/Intrinsic.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 defaultDistance     Thickness		int		4
 destroyCallback     Callback		Pointer		NULL
 displayList	     DisplayList	XawDisplayList*	NULL
 height		     Height		Dimension	computed at realize
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	computed at realize
 x		     Position		Position	0
 y		     Position		Position	0

*/

/* Constraint parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 bottom		     Edge		XtEdgeType	XtRubber
 fromHoriz	     Widget		Widget		(left edge of form)
 fromVert	     Widget		Widget		(top of form)
 horizDistance	     Thickness		int		defaultDistance
 left		     Edge		XtEdgeType	XtRubber
 resizable	     Boolean		Boolean		False
 right		     Edge		XtEdgeType	XtRubber
 top		     Edge		XtEdgeType	XtRubber
 vertDistance	     Thickness		int		defaultDistance

*/


#ifndef _XtStringDefs_h_
#define XtNtop "top"
#define XtRWidget "Widget"
#endif

#define XtNdefaultDistance "defaultDistance"
#define XtNbottom "bottom"
#define XtNleft "left"
#define XtNright "right"
#define XtNfromHoriz "fromHoriz"
#define XtNfromVert "fromVert"
#define XtNhorizDistance "horizDistance"
#define XtNvertDistance "vertDistance"
#define XtNresizable "resizable"

#define XtCEdge "Edge"
#define XtCWidget "Widget"

typedef enum {
    XawChainTop,		/* Keep this edge a constant distance from
				   the top of the form */
    XawChainBottom,		/* Keep this edge a constant distance from
				   the bottom of the form */
    XawChainLeft,		/* Keep this edge a constant distance from
				   the left of the form */
    XawChainRight,		/* Keep this edge a constant distance from
				   the right of the form */
    XawRubber			/* Keep this edge a proportional distance
				   from the edges of the form */
} XawEdgeType;

#define XtEdgeType XawEdgeType

#define XtChainTop XawChainTop
#define XtChainBottom XawChainBottom
#define XtChainLeft XawChainLeft
#define XtChainRight XawChainRight
#define XtRubber XawRubber

#define XtEchainLeft		"chainLeft"
#define XtEchainRight		"chainRight"
#define XtEchainTop		"chainTop"
#define XtEchainBottom		"chainBottom"
#define XtErubber		"rubber"

#ifndef OLDXAW
#ifndef XawNdisplayList
#define XawNdisplayList "displayList"
#endif

#ifndef XawCDisplayList
#define XawCDisplayList "DisplayList"
#endif

#ifndef XawRDisplayList
#define XawRDisplayList "XawDisplayList"
#endif
#endif

typedef struct _FormClassRec	*FormWidgetClass;
typedef struct _FormRec		*FormWidget;

extern WidgetClass formWidgetClass;

_XFUNCPROTOBEGIN

void XawFormDoLayout
        (
                Widget		w,
#if NeedWidePrototypes
        Bool		do_layout
#else
                Boolean	do_layout
#endif
);

_XFUNCPROTOEND

#endif /* _XawForm_h */

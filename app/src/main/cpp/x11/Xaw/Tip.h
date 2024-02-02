#ifndef _XawTip_h
#define _XawTip_h

/*
 * Tip Widget
 */

#include "Simple.h"

/* Resources:

  Name		    Class		RepType		Default Value
  ----		    -----		-------		-------------
  background	    Background		Pixel		XtDefaultBackground
  backgroundPixmap  BackgroundPixmap	Pixmap		XtUnspecifiedPixmap
  border	    BorderColor		Pixel		XtDefaultForeground
  borderWidth	    BorderWidth		Dimension	1
  bottomMargin	    VerticalMargins	Dimension	2
  destroyCallback   Callback		XtCallbackList	NULL
  displayList	    DisplayList		XawDisplayList*	NULL
  font		    Font		XFontStruct*	XtDefaultFont
  foreground	    Foreground		Pixel		XtDefaultForeground
  height	    Height		Dimension	text height
  leftMargin	    HorizontalMargins	Dimension	6
  rightMargin	    HorizontalMargins	Dimension	6
  timeout	    Timeout		Int		500
  topMargin	    VerticalMargins	Dimension	2
  width		    Width		Dimension	text width
  x		    Position		Position	0
  y		    Position		Position	0

*/

typedef struct _TipClassRec *TipWidgetClass;
typedef struct _TipRec *TipWidget;

extern WidgetClass tipWidgetClass;

#define XtNbottomMargin		"bottomMargin"
#define XawNdisplayList		"displayList"
#define XtNencoding		"encoding"
#define XtNleftMargin		"leftMargin"
#define XtNrightMargin		"rightMargin"
#define XtNtimeout		"timeout"
#define XtNtopMargin		"topMargin"
#define XtNtip			"tip"

#define XawCDisplayList		"DisplayList"
#define XtCHorizontalMargins	"HorizontalMargins"
#define XtCTimeout		"Timeout"
#define XtCVerticalMargins	"VerticalMargins"
#define XtCTip			"Tip"

#define XawRDisplayList		"XawDisplayList"

/*
 * Public Functions
 */
/*
 * Function:
 *	XawTipEnable
 *
 * Parameters:
 *	w - widget
 *
 * Description:
 *	Enables the tip event handler for this widget.
 */
void XawTipEnable
        (
                Widget		w
        );

/*
 * Function:
 *	XawTipEnable
 *
 * Parameters:
 *	w - widget
 *
 * Description:
 *	Disables the tip event handler for this widget.
 */
void XawTipDisable
        (
                Widget		w
        );

#endif /* _XawTip_h */

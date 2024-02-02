#ifndef _XawTextSink_h
#define _XawTextSink_h

#include "Text.h"

/***********************************************************************
 *
 * TextSink Object
 *
 ***********************************************************************/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 font                Font            XFontStruct *      XtDefaultFont
 foreground          Foreground      Pixel              XtDefaultForeground
 background          Background      Pixel              XtDefaultBackground
 cursorColor	     Color	      Pixel		XtDefaultForeground
*/

/* Class record constants */

extern WidgetClass textSinkObjectClass;

typedef struct _TextSinkClassRec *TextSinkObjectClass;
typedef struct _TextSinkRec      *TextSinkObject;

typedef enum {XawisOn, XawisOff} XawTextInsertState;

#ifndef OLDXAW
#ifndef XtNcursorColor
#define XtNcursorColor		"cursorColor"
#endif

#define XawNtextProperties	"textProperties"
#define XawCTextProperties	"TextProperties"
#define XawRTextProperties	"XawTextProperties"
#endif

/*
 * Public Functions
 */
_XFUNCPROTOBEGIN

/*
 * Function:
 *	XawTextSinkDisplayText
 *
 * Parameters:
 *	w	  - the TextSink Object
 *	x	  - location to start drawing text
 *	y	  - ""
 *	pos1	  - location of starting and ending points in the text buffer
 *	pos2	  - ""
 *	highlight - hightlight this text?
 *
 * Description:
 *	Stub function that in subclasses will display text.
 *
 * Note:
 *	  This function doesn't actually display anything, it is only a place
 *	holder.
 */
void XawTextSinkDisplayText
        (
                Widget			w,
#if NeedWidePrototypes
        int			x,
 int			y,
#else
                Position		x,
                Position		y,
#endif
                XawTextPosition	pos1,
                XawTextPosition	pos2,
#if NeedWidePrototypes
        int			highlight
#else
                Boolean		highlight
#endif
);

/*
 * Function:
 *	XawTextSinkInsertCursor
 *
 * Parameters:
 *	w	- the TextSink Object.
 *	x	- location for the cursor.
 *	y	- ""
 *	state	- whether to turn the cursor on, or off
 *
 * Description:
 *	Places the InsertCursor.
 *
 * Note:
 *	  This function doesn't actually display anything, it is only a place
 *	holder.
 */
void XawTextSinkInsertCursor
        (
                Widget			w,
#if NeedWidePrototypes
        int			x,
 int			y,
 int			state
#else
                Position		x,
                Position		y,
                XawTextInsertState	state
#endif
);

/*
 * Function:
 *	XawTextSinkClearToBackground
 *
 * Parameters:
 *	w	- TextSink Object
 *	x	- location of area to clear
 *	y	- ""
 *	width	- size of area to clear
 *	height	- ""
 *
 * Description:
 *	Clears a region of the sink to the background color.
 *
 * Note:
 *	  This function doesn't actually display anything, it is only a place
 *	holder.
 */
void XawTextSinkClearToBackground
        (
                Widget			w,
#if NeedWidePrototypes
        int			x,
 int			y,
 unsigned int		width,
 unsigned int		height
#else
                Position		x,
                Position		y,
                Dimension		width,
                Dimension		height
#endif
);

/*
 * Function:
 *	XawTextSinkFindPosition
 *
 * Parameters:
 *	w		- TextSink Object
 *	fromPos		- reference position
 *	fromX		- reference location
 *	width		- width of section to paint text
 *	stopAtWordBreak - returned position is a word break?
 *	resPos		- Position to return
 *	resWidth	- Width actually used
 *	resHeight	- Height actually used
 *
 * Description:
 *	Finds a position in the text.
 */
void XawTextSinkFindPosition
        (
                Widget			w,
                XawTextPosition	fromPos,
                int			fromX,
                int			width,
#if NeedWidePrototypes
        int			stopAtWordBreak,
#else
                Boolean		stopAtWordBreak,
#endif
                XawTextPosition*	pos_return,
                int			*width_return,
                int			*height_return
);

/*
 * Function:
 *	XawTextSinkFindDistance
 *
 * Parameters:
 *	w		- TextSink Object
 *	fromPos		- starting Position
 *	fromX		- x location of starting Position
 *	toPos		- end Position
 *	resWidth	- Distance between fromPos and toPos
 *	resPos		- Acutal toPos used
 *	resHeight	- Height required by this text
 *
 * Description:
 *	Find the Pixel Distance between two text Positions.
 */
void XawTextSinkFindDistance
        (
                Widget			w,
                XawTextPosition	fromPos,
                int			fromX,
                XawTextPosition	toPos,
                int			*width_return,
                XawTextPosition	*pos_return,
                int			*height_return
        );

/*
 * Function:
 *	XawTextSinkResolve
 *
 * Parameters:
 *	w	- TextSink Object
 *	pos	- reference Position
 *	fromx	- reference Location
 *	width	- width to move
 *	resPos	- resulting position
 *
 * Description:
 *	Resloves a location to a position.
 */
void XawTextSinkResolve
        (
                Widget			w,
                XawTextPosition	fromPos,
                int			fromX,
                int			width,
                XawTextPosition	*pos_return
        );

/*
 * Function:
 *	XawTextSinkMaxLines
 *
 * Parameters:
 *	w	- TextSink Object
 *	height	- height to fit lines into
 *
 * Returns:
 *	Number of lines that will fit
 *
 * Description:
 *	Finds the Maximum number of lines that will fit in a given height.
 */
int XawTextSinkMaxLines
        (
                Widget			w,
#if NeedWidePrototypes
        unsigned int		height
#else
                Dimension		height
#endif
);

/*
 * Function:
 *	XawTextSinkMaxHeight
 *
 * Parameters:
 *	w	- TextSink Object
 *	lines	- number of lines
 *
 * Returns:
 *	Height
 *
 * Description:
 *	Finds the Minium height that will contain a given number lines.
 */
int XawTextSinkMaxHeight
        (
                Widget			w,
                int			lines
        );

/*
 * Function:
 *	XawTextSinkSetTabs
 *
 * Parameters:
 *	w		- TextSink Object
 *	tab_count	- number of tabs in the list
 *	tabs		- text positions of the tabs
 * Description:
 *	Sets the Tab stops.
 */
void XawTextSinkSetTabs
        (
                Widget			w,
                int			tab_count,
                int			*tabs
        );

/*
 * Function:
 *	XawTextSinkGetCursorBounds
 *
 * Parameters:
 *	w	- TextSink Object
 *	rect	- X rectance containing the cursor bounds
 * Description:
 *	Finds the bounding box for the insert curor (caret).
 */
void XawTextSinkGetCursorBounds
        (
                Widget			w,
                XRectangle		*rect_return
        );

_XFUNCPROTOEND

#endif /* _XawTextSink_h */

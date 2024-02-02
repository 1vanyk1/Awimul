#ifndef _XawText_h
#define _XawText_h

#include "Simple.h"

/*

 Class:		textWidgetClass
 Class Name:	Text
 Superclass:	Simple

 Resources added by the Text widget:

 Name		     Class	     RepType		Default Value
 ----		     -----	     -------		-------------
 autoFill	    AutoFill	     Boolean		False
 bottomMargin	    Margin	     Position		2
 displayPosition    TextPosition     XawTextPosition	0
 insertPosition	    TextPosition     XawTextPosition	0
 justify	    JustifyMode	     JustifyMode	left
 leftColumn	    Column	     Column		0
 rightColumn	    Column	     Column		0
 leftMargin	    Margin	     Position		2
 rightMargin	    Margin	     Position		4
 positionCallback   Callback	     Callback		NULL
 scrollHorizontal   Scroll	     Boolean		False
 scrollVertical     Scroll	     Boolean		False
 selectTypes        SelectTypes      Pointer            see documentation
 textSink	    TextSink	     Widget		NULL
 textSource	    TextSource	     Widget		NULL
 topMargin	    Margin	     Position		2
 unrealizeCallback  Callback	     Callback		NULL
 wrap		    Wrap	     XawTextWrapMode	XawTextWrapNever

*/

typedef long XawTextPosition;

#ifndef notdef
typedef enum {
    XawtextScrollNever,
    XawtextScrollWhenNeeded,
    XawtextScrollAlways
} XawTextScrollMode;

typedef enum {
    XawtextResizeNever,
    XawtextResizeWidth,
    XawtextResizeHeight,
    XawtextResizeBoth
} XawTextResizeMode;
#endif

typedef enum {
    XawtextWrapNever,
    XawtextWrapLine,
    XawtextWrapWord
} XawTextWrapMode;

typedef enum {
    XawsdLeft,
    XawsdRight
} XawTextScanDirection;

typedef enum {
    XawtextRead,
    XawtextAppend,
    XawtextEdit
} XawTextEditType;

typedef enum {
    XawselectNull,
    XawselectPosition,
    XawselectChar,
    XawselectWord,
    XawselectLine,
    XawselectParagraph,
    XawselectAll,
    XawselectAlphaNumeric
} XawTextSelectType;

typedef enum {
    XawjustifyLeft,
    XawjustifyRight,
    XawjustifyCenter,
    XawjustifyFull
} XawTextJustifyMode;

typedef struct {
    int  firstPos;
    int  length;
    char *ptr;
    unsigned long format;
} XawTextBlock, *XawTextBlockPtr;

#ifndef OLDXAW
typedef struct {
    int line_number;
    int column_number;
    XawTextPosition insert_position;
    XawTextPosition last_position;
    Boolean overwrite_mode;
} XawTextPositionInfo;

typedef struct {
    XawTextPosition left, right;
    XawTextBlock *block;
} XawTextPropertyInfo;

typedef struct _XawTextAnchor XawTextAnchor;
typedef struct _XawTextEntity XawTextEntity;
typedef struct _XawTextProperty XawTextProperty;
typedef struct _XawTextPropertyList XawTextPropertyList;
#endif

#include "TextSink.h"
#include "TextSrc.h"

#define XtEtextScrollNever "never"
#define XtEtextScrollWhenNeeded "whenneeded"
#define XtEtextScrollAlways "always"
#define XtEtextResizeNever "never"
#define XtEtextResizeWidth "width"
#define XtEtextResizeHeight "height"
#define XtEtextResizeBoth "both"

#define XtEtextWrapNever	"never"
#define XtEtextWrapLine		"line"
#define XtEtextWrapWord		"word"

#define XtNautoFill "autoFill"
#define XtNbottomMargin "bottomMargin"
#define XtNdialogHOffset "dialogHOffset"
#define XtNdialogVOffset "dialogVOffset"
#define XtNdisplayCaret "displayCaret"
#define XtNdisplayPosition "displayPosition"
#define XtNleftMargin "leftMargin"
#define XtNrightMargin "rightMargin"
#define XtNpositionCallback "positionCallback"
#define XtNscrollVertical "scrollVertical"
#define XtNscrollHorizontal "scrollHorizontal"
#define XtNselectTypes "selectTypes"
#define XtNtopMargin "topMargin"
#define XtNwrap "wrap"

#define XtCColumn		"Column"
#define XtNleftColumn		"leftColumn"
#define XtNrightColumn		"rightColumn"

#define XtCJustifyMode		XtCJustify
#define XtNjustifyMode		XtNjustify
#define XtEtextJustifyLeft	"left"
#define XtEtextJustifyRight	"right"
#define XtEtextJustifyCenter	"center"
#define XtEtextJustifyFull	"full"

#define XtCAutoFill "AutoFill"
#define XtCSelectTypes "SelectTypes"
#define XtCWrap "Wrap"
#ifndef notdef
#define XtCScroll		"Scroll"
#endif

#ifndef _XtStringDefs_h_
#define XtNinsertPosition "insertPosition"
#ifndef notdef
#define XtNresize "resize"
#define XtCResize "Resize"
#endif
#define XtNselection		"selection"
#endif

/* return Error code for XawTextSearch */
#define XawTextSearchError      (-12345L)

/* return codes from XawTextReplace */
#define XawReplaceError	       -1
#define XawEditDone		0
#define XawEditError		1
#define XawPositionError	2

/* XrmQuark is not unsigned long */
#define XawTextFormat(tw,fmt) ((unsigned long)_XawTextFormat(tw) == (fmt))

extern unsigned long FMT8BIT;
extern unsigned long XawFmt8Bit;
extern unsigned long XawFmtWide;

extern WidgetClass textWidgetClass;

typedef struct _TextClassRec *TextWidgetClass;
typedef struct _TextRec      *TextWidget;

_XFUNCPROTOBEGIN

        XrmQuark _XawTextFormat
(
        TextWidget		tw
);

void XawTextDisplay
        (
                Widget			w
        );

void XawTextEnableRedisplay
        (
                Widget			w
        );

void XawTextDisableRedisplay
        (
                Widget			w
        );

void XawTextSetSelectionArray
        (
                Widget			w,
                XawTextSelectType	*sarray
        );

void XawTextGetSelectionPos
        (
                Widget			w,
                XawTextPosition	*begin_return,
                XawTextPosition	*end_return
        );

void XawTextSetSource
        (
                Widget			w,
                Widget			source,
                XawTextPosition	top
        );

int XawTextReplace
        (
                Widget			w,
                XawTextPosition	start,
                XawTextPosition	end,
                XawTextBlock		*text
        );

XawTextPosition XawTextTopPosition
        (
                Widget			w
        );

XawTextPosition XawTextLastPosition
        (
                Widget			w
        );

void XawTextSetInsertionPoint
        (
                Widget			w,
                XawTextPosition	position
        );

XawTextPosition XawTextGetInsertionPoint
        (
                Widget			w
        );

void XawTextUnsetSelection
        (
                Widget			w
        );

void XawTextSetSelection
        (
                Widget			w,
                XawTextPosition	left,
                XawTextPosition	right
        );

void XawTextInvalidate
        (
                Widget			w,
                XawTextPosition	from,
                XawTextPosition	to
        );

Widget XawTextGetSource
        (
                Widget			w
        );

Widget XawTextGetSink
        (
                Widget			w
        );

XawTextPosition XawTextSearch
        (
                Widget			w,
#if NeedWidePrototypes
        int			dir,
#else
                XawTextScanDirection	dir,
#endif
                XawTextBlock		*text
);

void XawTextDisplayCaret
        (
                Widget			w,
#if NeedWidePrototypes
        int			visible
#else
                Boolean		visible
#endif
);

_XFUNCPROTOEND

/*
 * For R3 compatability only
 */
#include "AsciiSrc.h"
#include "AsciiSink.h"

#endif /* _XawText_h */

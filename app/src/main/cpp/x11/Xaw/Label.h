#ifndef _XawLabel_h
#define _XawLabel_h

/*
 * Label Widget
 */

#include "Simple.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 bitmap		     Pixmap		Pixmap		None
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 destroyCallback     Callback		XtCallbackList	NULL
 encoding	     Encoding		UnsignedChar	XawTextEncoding8bit
 font		     Font		XFontStruct*	XtDefaultFont
 foreground	     Foreground		Pixel		XtDefaultForeground
 height		     Height		Dimension	text height
 insensitiveBorder   Insensitive	Pixmap		Gray
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	4
 justify	     Justify		XtJustify	XtJustifyCenter
 label		     Label		String		NULL
 leftBitmap	     LeftBitmap		Pixmap		None
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 pointerColor	     Foreground		Pixel		XtDefaultForeground
 pointerColorBackground Background	Pixel		XtDefaultBackground
 resize		     Resize		Boolean		True
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	text width
 x		     Position		Position	0
 y		     Position		Position	0

*/

#define XawTextEncoding8bit	0
#define XawTextEncodingChar2b	1

#define XtNleftBitmap "leftBitmap"
#define XtCLeftBitmap "LeftBitmap"
#define XtNencoding "encoding"
#define XtCEncoding "Encoding"

#ifndef XtNfontSet
#define XtNfontSet "fontSet"
#endif

#ifndef XtCFontSet
#define XtCFontSet "FontSet"
#endif

#ifndef _XtStringDefs_h_
#define XtNbitmap "bitmap"
#define XtNforeground "foreground"
#define XtNlabel "label"
#define XtNfont "font"
#define XtNinternalWidth "internalWidth"
#define XtNinternalHeight "internalHeight"
#define XtNresize "resize"
#define XtCResize "Resize"
#define XtCBitmap "Bitmap"
#endif

#ifndef XtNlabelX
#define XtNlabelX "labelX"
#endif

#ifndef XtNlabelY
#define XtNlabelY "labelY"
#endif

/* Class record constants */

extern WidgetClass labelWidgetClass;

typedef struct _LabelClassRec *LabelWidgetClass;
typedef struct _LabelRec      *LabelWidget;

#endif /* _XawLabel_h */

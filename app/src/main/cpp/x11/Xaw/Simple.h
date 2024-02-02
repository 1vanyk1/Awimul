#ifndef _Simple_h
#define _Simple_h

#include "../Xmu/Converters.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 destroyCallback     Callback		Pointer		NULL
 displayList	     DisplayList	XawDisplayList*	NULL
 height		     Height		Dimension	0
 insensitiveBorder   Insensitive	Pixmap		Gray
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 pointerColor        Foreground         Pixel           XtDefaultForeground
 pointerColorBackground Background      Pixel           XtDefaultBackground
 sensitive	     Sensitive		Boolean		True
 tip		     Tip		String		NULL
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

*/

#define XtNcursor "cursor"
#define XtNcursorName "cursorName"
#define XtNinsensitiveBorder "insensitiveBorder"

#define XtCInsensitive "Insensitive"

#ifndef XtCInternational
#define XtCInternational "International"
#endif

#ifndef XtNinternational
#define XtNinternational "international"
#endif

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

#define XtNtip		"tip"
#define XtCTip		"Tip"
#endif	/* OLDXAW */

typedef struct _SimpleClassRec	*SimpleWidgetClass;
typedef struct _SimpleRec	*SimpleWidget;

extern WidgetClass simpleWidgetClass;

#endif /* _Simple_h */

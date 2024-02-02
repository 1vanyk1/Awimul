#ifndef _XawClock_h
#define _XawClock_h

/***********************************************************************
 *
 * Clock Widget
 *
 ***********************************************************************/

#include "../x11/Xmu/Converters.h"

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 twentyfour	     Boolean		Boolean		True
 analog		     Boolean		Boolean		True
 background	     Background		Pixel		white
 backingStore	     BackingStore	BackingStore	default
 border		     BorderColor	Pixel		Black
 borderWidth	     BorderWidth	Dimension	1
 chime		     Boolean		Boolean		False
 destroyCallback     Callback		Pointer		NULL
 font		     Font		XFontStruct*	fixed
 foreground	     Foreground		Pixel		black
 hand		     Foreground		Pixel		black
 height		     Height		Dimension	164
 highlight	     Foreground		Pixel		black
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 padding	     Margin		int		8
 utime		     Boolean		Boolean		False
 update		     Interval		int		60 (seconds)
 width		     Width		Dimension	164
 x		     Position		Position	0
 y		     Position		Position	0

*/

/* Resource names used to the clock widget */

/* color of hands */
#define XtNhand "hands"


/* Boolean: 24-hour if TRUE */
#define XtNtwentyfour "twentyfour"

/* Boolean: digital if FALSE */
#define XtNanalog "analog"

/* Boolean: only hour/minute if TRUE */
#define XtNbrief  "brief"

/* String: will be used as format arg to
   "strftime" if not empty string */
#define XtNstrftime "strftime"

/* Boolean: show seconds since Epoch if TRUE */
#define XtNutime  "utime"

/* Boolean:  */
#define XtNchime "chime"

/* Int: amount of space around outside of clock */
#define XtNpadding "padding"

/* Boolean: use Render extension if TRUE */
#define XtNrender "render"

/* Boolean: use backing pixmap for double buffering */
#define XtNbuffer "buffer"

/* RenderColor: colors for various clock elements */
#define XtNhourColor "hourColor"
#define XtNminuteColor "minuteColor"
#define XtNsecondColor "secondColor"
#define XtNmajorColor "majorColor"
#define XtNminorColor "minorColor"

#define XtRXftColor "XftColor"

#define XtNface "face"
#define XtCFace "Face"
#define XtRXftFont "XftFont"

/* Boolean: use sharp rendering for Render polygons */
#define XtNsharp "sharp"
#define XtCSharp "Sharp"

typedef struct _ClockRec *ClockWidget;  /* completely defined in ClockP.h */
typedef struct _ClockClassRec *ClockWidgetClass;  /* completely defined in ClockP.h */

extern WidgetClass clockWidgetClass;

#endif /* _XawClock_h */

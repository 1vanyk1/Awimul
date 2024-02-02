#ifndef _ScrollbarP_h
#define _ScrollbarP_h

#include "Scrollbar.h"
#include "SimpleP.h"

typedef struct {
    /* resources */
    Pixel	   foreground;	/* thumb foreground color */
    XtOrientation  orientation;	/* horizontal or vertical */
    XtCallbackList scrollProc;	/* proportional scroll */
    XtCallbackList thumbProc;	/* jump (to position) scroll */
    XtCallbackList jumpProc;	/* same as thumbProc but pass data by ref */
    Pixmap	   thumb;			/* thumb pixmap */
    Cursor	   upCursor;	/* scroll up cursor */
    Cursor	   downCursor;	/* scroll down cursor */
    Cursor	   leftCursor;	/* scroll left cursor */
    Cursor	   rightCursor;	/* scroll right cursor */
    Cursor	   verCursor;	/* scroll vertical cursor */
    Cursor	   horCursor;	/* scroll horizontal cursor */
    float	   top;
    float	   shown;
    Dimension	   length;	/* either height or width */
    Dimension	   thickness;	/* either width or height */
    Dimension	   min_thumb;	/* minium size for the thumb */

    /* private */
    Cursor	   inactiveCursor; /* The normal cursor for scrollbar */
    char	   direction;	/* a scroll has started; which direction */
    GC		   gc;		/* a (shared) gc */
    Position	   topLoc;	/* Pixel that corresponds to top */
    Dimension	   shownLength;	/* Num pixels corresponding to shown */
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} ScrollbarPart;

typedef struct _ScrollbarRec {
    CorePart		core;
    SimplePart		simple;
    ScrollbarPart	scrollbar;
} ScrollbarRec;

typedef struct {
    XtPointer extension;
} ScrollbarClassPart;

typedef struct _ScrollbarClassRec {
    CoreClassPart		core_class;
    SimpleClassPart		simple_class;
    ScrollbarClassPart		scrollbar_class;
} ScrollbarClassRec;

extern ScrollbarClassRec scrollbarClassRec;

#endif /* _ScrollbarP_h */

#ifndef _XawClockP_h
#define _XawClockP_h

#include "../x11/headers/xos.h"		/* Needed for struct tm. */
#include "Clock.h"
#include "../x11/Xaw/SimpleP.h"
#include <time.h>
#ifdef XRENDER
#include <X11/Xft/Xft.h>
#include "../x11/headers/extensions/Xrender.h"
#endif

#define SEG_BUFF_SIZE		128
#ifdef NO_I18N
#define ASCII_TIME_BUFLEN	32	/* big enough for 26 plus slop */
#define STRFTIME_BUFF_SIZE      100     /* buffer for "strftime" option */
#else
#define STRFTIME_BUFF_SIZE	256	/* should handle any locale */
#endif


/* New fields for the clock widget instance record */
typedef struct {
#ifndef RENDER
    Pixel	fgpixel;	/* color index for text */
#endif
    Pixel	Hipixel;	/* color index for Highlighting */
    Pixel	Hdpixel;	/* color index for hands */
    XFontStruct	*font;	/* font for text */
    GC	myGC;		/* pointer to GraphicsContext */
    GC	EraseGC;	/* eraser GC */
    GC	HandGC;		/* Hand GC */
    GC	HighGC;		/* Highlighting GC */
/* start of graph stuff */
    float	update;		/* update period in second */
    Dimension radius;		/* radius factor */
    int	backing_store;	/* backing store type */
    Boolean chime;
    Boolean beeped;
    Boolean analog;
    Boolean brief;
    Boolean twentyfour;
    Boolean utime;
    String strftime;
    Boolean show_second_hand;
    Dimension second_hand_length;
    Dimension minute_hand_length;
    Dimension hour_hand_length;
    Dimension hand_width;
    Dimension second_hand_width;
    Position centerX;
    Position centerY;
    int	numseg;
    int	padding;
    XPoint	segbuff[SEG_BUFF_SIZE];
    XPoint	*segbuffptr;
    XPoint	*hour, *sec;
    struct tm  otm ;
    struct timeval otv ;
    XtIntervalId interval_id;
    char prev_time_string[STRFTIME_BUFF_SIZE];
#ifndef NO_I18N
    XFontSet fontSet;     /* font set for localized text */
    Boolean	utf8;
#endif
#ifdef XRENDER
    XftColor	fg_color;
	 XftColor	hour_color;
	 XftColor	min_color;
	 XftColor	sec_color;
	 XftColor	major_color;
	 XftColor	minor_color;
	 XftFont	*face;
	 XRenderPictFormat  *mask_format;

	 Boolean    render;
	 Boolean    sharp;
	 Boolean    can_polygon;
	 Boolean    buffer;
	 XftDraw    *draw;
	 Picture    picture;
	 Picture    fill_picture;
	 Pixmap	    pixmap;
	 XRectangle damage;
	 XDouble    x_scale;
	 XDouble    x_off;
	 XDouble    y_scale;
	 XDouble    y_off;
#endif
} ClockPart;

#ifdef XRENDER
#define ClockFgPixel(c)	((c)->clock.fg_color.pixel)
#else
#define ClockFgPixel(c)	((c)->clock.fgpixel)
#endif

/* Full instance record declaration */
typedef struct _ClockRec {
    CorePart core;
    SimplePart simple;
    ClockPart clock;
} ClockRec;

/* New fields for the Clock widget class record */
typedef struct {int dummy;} ClockClassPart;

/* Full class record declaration. */
typedef struct _ClockClassRec {
    CoreClassPart core_class;
    SimpleClassPart simple_class;
    ClockClassPart clock_class;
} ClockClassRec;

/* Class pointer. */
extern ClockClassRec clockClassRec;

#endif /* _XawClockP_h */

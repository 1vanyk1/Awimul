#ifndef _SimpleP_h
#define _SimpleP_h

#include "../headers/xfuncproto.h"

#include "Simple.h"

_XFUNCPROTOBEGIN

#include "XawInit.h"
#include "../libXt/CoreP.h"

typedef struct {
    Bool (*change_sensitive)(Widget);
#ifndef OLDXAW
    XtPointer extension;
#endif
} SimpleClassPart;

#define XtInheritChangeSensitive	((Bool (*)(Widget))_XtInherit)

typedef struct _SimpleClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
} SimpleClassRec;

extern SimpleClassRec simpleClassRec;

typedef struct {
    /* resources */
    Cursor cursor;
    Pixmap insensitive_border;
    String cursor_name;			/* cursor specified by name */
    Pixel pointer_fg, pointer_bg;	/* Pointer colors */
    Boolean international;

    /* private */
#ifndef OLDXAW
    XawDisplayList *display_list;
    String tip;
    XtPointer pad[3];	/* for future use and keep binary compatability */
#endif
} SimplePart;

typedef struct _SimpleRec {
    CorePart	core;
    SimplePart	simple;
} SimpleRec;

_XFUNCPROTOEND

#endif /* _SimpleP_h */

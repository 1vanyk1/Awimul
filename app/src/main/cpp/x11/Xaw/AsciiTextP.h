#ifndef _AsciiTextP_h
#define _AsciiTextP_h

#include "TextP.h"
#include "AsciiText.h"
#include "AsciiSrc.h"
#include "MultiSrc.h"

typedef struct {
    XtPointer extension;
} AsciiClassPart;

typedef struct _AsciiTextClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    TextClassPart	text_class;
    AsciiClassPart	ascii_class;
} AsciiTextClassRec;

extern AsciiTextClassRec asciiTextClassRec;

typedef struct {
    int resource;
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} AsciiPart;

typedef struct _AsciiRec {
    CorePart		core;
    SimplePart		simple;
    TextPart		text;
    AsciiPart		ascii;
} AsciiRec;

/*
 * Ascii String Emulation widget
 */
#ifdef ASCII_STRING
typedef struct {
  XtPointer extension;
} AsciiStringClassPart;

typedef struct _AsciiStringClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    TextClassPart	text_class;
    AsciiClassPart	ascii_class;
    AsciiStringClassPart string_class;
} AsciiStringClassRec;

extern AsciiStringClassRec asciiStringClassRec;

typedef struct {
    int resource;
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} AsciiStringPart;

typedef struct _AsciiStringRec {
    CorePart		core;
    SimplePart		simple;
    TextPart		text;
    AsciiPart           ascii;
    AsciiStringPart     ascii_str;
} AsciiStringRec;
#endif /* ASCII_STRING */

#ifdef ASCII_DISK
/*
 * Ascii Disk Emulation widget
 */
typedef struct {
    XtPointer extension;
} AsciiDiskClassPart;

typedef struct _AsciiDiskClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    TextClassPart	text_class;
    AsciiClassPart	ascii_class;
    AsciiDiskClassPart	disk_class;
} AsciiDiskClassRec;

extern AsciiDiskClassRec asciiDiskClassRec;

typedef struct {
    char resource;
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} AsciiDiskPart;

typedef struct _AsciiDiskRec {
    CorePart		core;
    SimplePart		simple;
    TextPart		text;
    AsciiPart           ascii;
    AsciiDiskPart       ascii_disk;
} AsciiDiskRec;
#endif /* ASCII_DISK */

#endif /* _AsciiTextP_h */

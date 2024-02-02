#ifndef _XMU_STRCONVERT_H_
#define _XMU_STRCONVERT_H_

#include "../libXt/Intrinsic.h"
#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

void XmuCvtFunctionToCallback
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

#define XtNbackingStore "backingStore"
#define XtCBackingStore "BackingStore"
#define XtRBackingStore "BackingStore"
#define XtEnotUseful "notUseful"
#define XtEwhenMapped "whenMapped"
#define XtEalways "always"
#define XtEdefault "default"
void XmuCvtStringToBackingStore
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

Boolean XmuCvtBackingStoreToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

void XmuCvtStringToCursor
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

#define XtRColorCursor "ColorCursor"
#define XtNpointerColor "pointerColor"
#define XtNpointerColorBackground "pointerColorBackground"
Boolean XmuCvtStringToColorCursor
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

typedef int XtGravity;

#ifndef XtRGravity
#define XtRGravity "Gravity"
#endif
#define XtEForget "forget"
#define XtENorthWest "northwest"
#define XtENorth "north"
#define XtENorthEast "northeast"
#define XtEWest "west"
#define XtECenter "center"
#define XtEEast "east"
#define XtESouthWest "southwest"
#define XtESouth "south"
#define XtESouthEast "southeast"
#define XtEStatic "static"
#define XtEUnmap "unmap"
void XmuCvtStringToGravity
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

Boolean XmuCvtGravityToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

typedef enum {
    XtJustifyLeft,       /* justify text to left side of button   */
    XtJustifyCenter,     /* justify text in center of button      */
    XtJustifyRight       /* justify text to right side of button  */
} XtJustify;
#ifndef XtRJustify
#define XtRJustify "Justify"
#endif
#define XtEleft "left"
#define XtEcenter "center"
#define XtEright "right"
#define XtEtop "top"
#define XtEbottom "bottom"
void XmuCvtStringToJustify
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

Boolean XmuCvtJustifyToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

#define XtRLong "Long"
void XmuCvtStringToLong
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );
Boolean XmuCvtLongToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

typedef enum {
    XtorientHorizontal,
    XtorientVertical
} XtOrientation;
void XmuCvtStringToOrientation
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

Boolean XmuCvtOrientationToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

void XmuCvtStringToBitmap
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

#define XtRShapeStyle "ShapeStyle"
#define XtERectangle "Rectangle"
#define XtEOval "Oval"
#define XtEEllipse "Ellipse"
#define XtERoundedRectangle "RoundedRectangle"

#define XmuShapeRectangle 1
#define XmuShapeOval 2
#define XmuShapeEllipse 3
#define XmuShapeRoundedRectangle 4

Boolean XmuCvtStringToShapeStyle
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

Boolean XmuCvtShapeStyleToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal,
                XtPointer		*converter_data
        );

Boolean XmuReshapeWidget
        (
                Widget			w,
                int			shape_style,
                int			corner_width,
                int			corner_height
        );

void XmuCvtStringToWidget
        (
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValuePtr		fromVal,
                XrmValuePtr		toVal
        );

Boolean XmuNewCvtStringToWidget
        (
                Display		*display,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValue		*fromVal,
                XrmValue		*toVal,
                XtPointer		*converter_data
        );

Boolean XmuCvtWidgetToString
        (
                Display		*dpy,
                XrmValue		*args,
                Cardinal		*num_args,
                XrmValue		*fromVal,
                XrmValue		*toVal,
                XtPointer		*converter_data
        );

_XFUNCPROTOEND

#endif /* _XMU_STRCONVERT_H_ */

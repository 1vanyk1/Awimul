#ifndef _XawPrivate_h
#define _XawPrivate_h

#define XawMax(a, b) ((a) > (b) ? (a) : (b))
#define XawMin(a, b) ((a) < (b) ? (a) : (b))
#define XawAbs(a)    ((a) < 0 ? -(a) : (a))

#define XawStackAlloc(size, stk_buffer)		\
((size) <= sizeof(stk_buffer)			\
 ? (XtPointer)(stk_buffer)			\
 : XtMalloc((Cardinal)(size)))

#define XawStackFree(pointer, stk_buffer)	\
do {						\
  if ((pointer) != (XtPointer)(stk_buffer))	\
    XtFree((char *)pointer);			\
} while (0)

#ifndef XtX
#define XtX(w)            (((RectObj)w)->rectangle.x)
#endif
#ifndef XtY
#define XtY(w)            (((RectObj)w)->rectangle.y)
#endif
#ifndef XtWidth
#define XtWidth(w)        (((RectObj)w)->rectangle.width)
#endif
#ifndef XtHeight
#define XtHeight(w)       (((RectObj)w)->rectangle.height)
#endif
#ifndef XtBorderWidth
#define XtBorderWidth(w)  (((RectObj)w)->rectangle.border_width)
#endif

#ifndef OLDXAW
#define XAW_PRIV_VAR_PREFIX '$'

typedef Bool (*XawParseBooleanProc)(Widget, String, XEvent*, Bool*);

typedef struct _XawActionVarList XawActionVarList;
typedef struct _XawActionResList XawActionResList;

/* Boolean expressions */
Bool XawParseBoolean(Widget, String, XEvent*, Bool*);
Bool XawBooleanExpression(Widget, String, XEvent*);

/* actions */
void XawPrintActionErrorMsg(String, Widget, String*, Cardinal*);
XawActionResList *XawGetActionResList(WidgetClass);
XawActionVarList *XawGetActionVarList(Widget);

void XawSetValuesAction(Widget, XEvent*, String*, Cardinal*);
void XawGetValuesAction(Widget, XEvent*, String*, Cardinal*);
void XawDeclareAction(Widget, XEvent*, String*, Cardinal*);
void XawCallProcAction(Widget, XEvent*, String*, Cardinal*);

/* display lists */
#define	XAWDL_CONVERT_ERROR	(XtPointer)-1
typedef struct _XawDL _XawDisplayList;
typedef struct _XawDLClass XawDLClass, XawDisplayListClass;

typedef void (*XawDisplayListProc)(Widget, XtPointer, XtPointer,
                                   XEvent*, Region);
typedef void *(*XawDLArgsInitProc)(String, String*, Cardinal*,
                                   Screen*, Colormap, int);
typedef void *(*XawDLDataInitProc)(String,
                                   Screen*, Colormap, int);
typedef void (*XawDLArgsDestructor)(Display*, String, XtPointer,
                                    String*, Cardinal*);
typedef void (*XawDLDataDestructor)(Display*, String, XtPointer);

void XawRunDisplayList(Widget, _XawDisplayList*, XEvent*, Region);
void XawDisplayListInitialize(void);

_XawDisplayList *XawCreateDisplayList(String, Screen*, Colormap, int);
void XawDestroyDisplayList(_XawDisplayList*);
String XawDisplayListString(_XawDisplayList*);
XawDLClass *XawGetDisplayListClass(String);
XawDLClass *XawCreateDisplayListClass(String,
                                      XawDLArgsInitProc, XawDLArgsDestructor,
                                      XawDLDataInitProc, XawDLDataDestructor);
Bool XawDeclareDisplayListProc(XawDLClass*, String, XawDisplayListProc);

/* pixmaps */
typedef struct _XawArgVal {
    String name;
    String value;
} XawArgVal;

typedef struct _XawParams {
    String name;
    String type;
    String ext;
    XawArgVal **args;
    Cardinal num_args;
} XawParams;

typedef struct _XawPixmap {
    String name;
    Pixmap pixmap;
    Pixmap mask;
    Dimension width;
    Dimension height;
} XawPixmap;

typedef Bool (*XawPixmapLoader)(XawParams*, Screen*, Colormap, int,
                                Pixmap*, Pixmap*,
                                Dimension*, Dimension*);
Bool XawPixmapsInitialize(void);
Bool XawAddPixmapLoader(String, String, XawPixmapLoader);
XawPixmap *XawLoadPixmap(String, Screen*, Colormap, int);
XawPixmap *XawPixmapFromXPixmap(Pixmap, Screen*, Colormap, int);
XawParams *XawParseParamsString(String name);
void XawFreeParamsStruct(XawParams *params);
XawArgVal *XawFindArgVal(XawParams *params, String name);
void XawReshapeWidget(Widget, XawPixmap*);
#endif /* OLDXAW */

/* misc */
void XawTypeToStringWarning(Display*, String);

/* OS.c */
int _XawGetPageSize(void);

#endif /* _XawPrivate_h */

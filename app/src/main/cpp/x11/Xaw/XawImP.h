#ifndef _XawImP_h
#define _XawImP_h

#define XtNinputMethod		"inputMethod"
#define XtCInputMethod		"InputMethod"
#define XtNpreeditType		"preeditType"
#define XtCPreeditType		"PreeditType"
#define XtNopenIm		"openIm"
#define XtCOpenIm		"OpenIm"
#define XtNsharedIc		"sharedIc"
#define XtCSharedIc		"SharedIc"

#include "Text.h"

#define	CIICFocus	(1 << 0)
#define	CIFontSet	(1 << 1)
#define	CIFg		(1 << 2)
#define	CIBg		(1 << 3)
#define	CIBgPixmap	(1 << 4)
#define	CICursorP	(1 << 5)
#define	CILineS		(1 << 6)

typedef	struct _XawImPart {
    XIM			xim;
    XrmResourceList	resources;
    Cardinal		num_resources;
    Boolean		open_im;
    Boolean		initialized;
    Dimension		area_height;
    String		input_method;
    String		preedit_type;
} XawImPart;

typedef struct _XawIcTablePart {
    Widget		widget;
    XIC			xic;
    XIMStyle		input_style;
    unsigned long	flg;
    unsigned long	prev_flg;
    Boolean		ic_focused;
    XFontSet		font_set;
    Pixel		foreground;
    Pixel		background;
    Pixmap		bg_pixmap;
    XawTextPosition	cursor_position;
    unsigned long	line_spacing;
    Boolean		openic_error;
    struct _XawIcTablePart *next;
} XawIcTablePart, *XawIcTableList;

typedef	struct _XawIcPart {
    XIMStyle		input_style;
    Boolean		shared_ic;
    XawIcTableList	shared_ic_table;
    XawIcTableList	current_ic_table;
    XawIcTableList	ic_table;
} XawIcPart;

typedef	struct _contextDataRec {
    Widget		parent;
    Widget		ve;
} contextDataRec;

typedef	struct _contextErrDataRec {
    Widget		widget;
    XIM			xim;
} contextErrDataRec;

void _XawImResizeVendorShell
        (
                Widget			w
        );

Dimension _XawImGetShellHeight
        (
                Widget			w
        );

void _XawImRealize
        (
                Widget			w
        );

void _XawImInitialize
        (
                Widget			w,
                Widget			ext
        );

void _XawImReconnect
        (
                Widget			w
        );

void _XawImRegister
        (
                Widget			w
        );

void _XawImUnregister
        (
                Widget			w
        );

void _XawImSetValues
        (
                Widget			w,
                ArgList		args,
                Cardinal		num_args
        );

void _XawImSetFocusValues
        (
                Widget			w,
                ArgList		args,
                Cardinal		num_args
        );

void _XawImUnsetFocus
        (
                Widget			w
        );

int _XawImWcLookupString
        (
                Widget			w,
                XKeyPressedEvent	*event,
                wchar_t		*buffer_return,
                int			bytes_buffer,
                KeySym			*keysym_return
        );

int _XawLookupString
        (
                Widget			w,
                XKeyEvent		*event,
                char			*buffer_return,
                int			buffer_size,
                KeySym			*keysym_return
        );

int _XawImGetImAreaHeight
        (
                Widget			w
        );

void _XawImCallVendorShellExtResize
        (
                Widget			w
        );

void _XawImDestroy
        (
                Widget			w,
                Widget			ext
        );

#endif	/* _XawImP_h */

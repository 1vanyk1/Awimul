#ifndef _XawTextSrcP_h
#define _XawTextSrcP_h

/*
 * TextSrc Object Private Data
 */
#include "../headers/xfuncproto.h"

#include "TextSrc.h"
#include "TextP.h"	/* This source works with the Text widget */

_XFUNCPROTOBEGIN

#ifndef OLDXAW
struct _XawTextAnchor {
    XawTextPosition position;
    XawTextEntity *entities, *cache;
};

#define	XAW_TENTF_HIDE		0x0001
#define XAW_TENTF_READ		0x0002
#define XAW_TENTF_REPLACE	0x0004
struct _XawTextEntity {
    short type;
    short flags;
    XawTextEntity *next;
    XtPointer data;
    XawTextPosition offset;	/* from the anchor */
    Cardinal length;
    XrmQuark property;
};
#endif

#if 0	/* no longer used */
/* New fields for the TextSrc object class */
typedef struct {
  XtPointer		next_extension;
  XrmQuark		record_type;
  long			version;
  Cardinal		record_size;
  int			(*Input)();
} TextSrcExtRec, *TextSrcExt;
#endif

typedef XawTextPosition (*_XawSrcReadProc)
        (Widget, XawTextPosition, XawTextBlock*, int);

typedef int (*_XawSrcReplaceProc)
        (Widget, XawTextPosition, XawTextPosition, XawTextBlock*);

typedef XawTextPosition (*_XawSrcScanProc)
        (Widget, XawTextPosition, XawTextScanType, XawTextScanDirection,
         int, Bool);

typedef XawTextPosition (*_XawSrcSearchProc)
        (Widget, XawTextPosition, XawTextScanDirection, XawTextBlock*);

typedef void (*_XawSrcSetSelectionProc)
        (Widget, XawTextPosition, XawTextPosition, Atom);

typedef Boolean (*_XawSrcConvertSelectionProc)
        (Widget, Atom*, Atom*, Atom*, XtPointer*, unsigned long*, int*);

typedef struct _TextSrcClassPart {
    _XawSrcReadProc Read;
    _XawSrcReplaceProc Replace;
    _XawSrcScanProc Scan;
    _XawSrcSearchProc Search;
    _XawSrcSetSelectionProc SetSelection;
    _XawSrcConvertSelectionProc ConvertSelection;
#ifndef OLDXAW
    XtPointer extension;
#endif
} TextSrcClassPart;

/* Full class record */
typedef struct _TextSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	textSrc_class;
} TextSrcClassRec;

extern TextSrcClassRec textSrcClassRec;

#ifndef OLDXAW
typedef struct _XawTextUndo XawTextUndo;
#endif

/* New fields for the TextSrc object */
typedef struct {
    /* resources */
    XawTextEditType	edit_mode;
    XrmQuark text_format;		/* 2 formats: FMT8BIT for Ascii
					   FMTWIDE for ISO 10646 */
#ifndef OLDXAW
    XtCallbackList callback;		/* A callback list to call when the
					   source is changed */
    Boolean changed;
    Boolean enable_undo;

    /* private state */
    Boolean undo_state;			/* to protect undo manipulation */
    XawTextUndo *undo;
    WidgetList text;			/* TextWidget's using this source */
    Cardinal num_text;
    XtCallbackList property_callback;
    XawTextAnchor **anchors;
    int num_anchors;
    XtPointer pad[1];	/* for future use and keep binary compatability */
#endif
} TextSrcPart;

/* Full instance record */
typedef struct _TextSrcRec {
    ObjectPart	object;
    TextSrcPart	textSrc;
} TextSrcRec;

/*
 * Semiprivate declarations of functions used in other modules
 */
char* _XawTextWCToMB
        (
                Display	*display,
                wchar_t	*wstr,
                int		*len_in_out
        );

wchar_t* _XawTextMBToWC
        (
                Display	*display,
                char		*str,
                int		*len_in_out
        );

#ifndef OLDXAW
XawTextAnchor *XawTextSourceAddAnchor
        (
                Widget			source,
                XawTextPosition	position
        );

XawTextAnchor *XawTextSourceFindAnchor
        (
                Widget			source,
                XawTextPosition	position
        );

XawTextAnchor *XawTextSourceNextAnchor
        (
                Widget			 source,
                XawTextAnchor		*anchor
        );

XawTextAnchor *XawTextSourcePrevAnchor
        (
                Widget			 source,
                XawTextAnchor		*anchor
        );

XawTextAnchor *XawTextSourceRemoveAnchor
        (
                Widget			 source,
                XawTextAnchor		*anchor
        );

Bool XawTextSourceAnchorAndEntity
        (
                Widget			  w,
                XawTextPosition	  position,
                XawTextAnchor		**anchor_return,
                XawTextEntity		**entity_return
        );

XawTextEntity *XawTextSourceAddEntity
        (
                Widget			source,
                int			type,
                int			flags,
                XtPointer		data,
                XawTextPosition	position,
                Cardinal		length,
                XrmQuark		property
        );

void XawTextSourceClearEntities
        (
                Widget			w,
                XawTextPosition	left,
                XawTextPosition	right
        );
#endif

#if 0	/* no longer used */
typedef XawTextPosition (*_XawTextPositionFunc)();
#endif

#define XtInheritInput			((_XawTextPositionFunc)_XtInherit)
#define XtInheritRead			((_XawSrcReadProc)_XtInherit)
#define XtInheritReplace		((_XawSrcReplaceProc)_XtInherit)
#define XtInheritScan			((_XawSrcScanProc)_XtInherit)
#define XtInheritSearch			((_XawSrcSearchProc)_XtInherit)
#define XtInheritSetSelection		((_XawSrcSetSelectionProc)_XtInherit)
#define XtInheritConvertSelection     ((_XawSrcConvertSelectionProc)_XtInherit)
#if 0
#define XtTextSrcExtVersion	      1
#define XtTextSrcExtTypeString        "XT_TEXTSRC_EXT"
#endif

_XFUNCPROTOEND

#endif /* _XawTextSrcP_h */

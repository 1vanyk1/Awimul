#ifndef _PDI_h_
#define _PDI_h_

#define KEYBOARD TRUE
#define POINTER  FALSE

_XFUNCPROTOBEGIN

typedef enum {
    XtNoServerGrab,
    XtPassiveServerGrab,
    XtActiveServerGrab,
    XtPseudoPassiveServerGrab,
    XtPseudoActiveServerGrab
}XtServerGrabType;

typedef struct _XtServerGrabRec {
    struct _XtServerGrabRec 	*next;
    Widget			widget;
    unsigned int		ownerEvents:1;
    unsigned int		pointerMode:1;
    unsigned int		keyboardMode:1;
    unsigned int		hasExt:1;
    unsigned int		confineToIsWidgetWin:1;
    KeyCode			keybut;
    unsigned short		modifiers;
    unsigned short		eventMask;
} XtServerGrabRec, *XtServerGrabPtr;

typedef struct _XtGrabExtRec {
    Mask			*pKeyButMask;
    Mask			*pModifiersMask;
    Window			confineTo;
    Cursor			cursor;
} XtServerGrabExtRec, *XtServerGrabExtPtr;

#define GRABEXT(p) ((XtServerGrabExtPtr)((p)+1))

typedef struct _XtDeviceRec{
    XtServerGrabRec	grab; 	/* need copy in order to protect
				   during grab */
    XtServerGrabType	grabType;
}XtDeviceRec, *XtDevice;

#define XtMyAncestor	0
#define XtMyDescendant	1
#define XtMyCousin	2
#define XtMySelf	3
#define XtUnrelated	4
typedef char XtGeneology; /* do not use an enum makes PerWidgetInput larger */

typedef struct {
    Widget		focusKid;
    XtServerGrabPtr	keyList, ptrList;
    Widget		queryEventDescendant;
    unsigned int	map_handler_added:1;
    unsigned int	realize_handler_added:1;
    unsigned int	active_handler_added:1;
    unsigned int	haveFocus:1;
    XtGeneology		focalPoint;
}XtPerWidgetInputRec, *XtPerWidgetInput;

typedef struct XtPerDisplayInputRec{
    XtGrabList 	grabList;
    XtDeviceRec keyboard, pointer;
    KeyCode	activatingKey;
    Widget 	*trace;
    int		traceDepth, traceMax;
    Widget 	focusWidget;
}XtPerDisplayInputRec, *XtPerDisplayInput;

#define IsServerGrab(g) ((g == XtPassiveServerGrab) ||\
			 (g == XtActiveServerGrab))

#define IsAnyGrab(g) ((g == XtPassiveServerGrab) ||\
		      (g == XtActiveServerGrab)  ||\
		      (g == XtPseudoPassiveServerGrab))

#define IsEitherPassiveGrab(g) ((g == XtPassiveServerGrab) ||\
				(g == XtPseudoPassiveServerGrab))

#define IsPseudoGrab(g) ((g == XtPseudoPassiveServerGrab))

extern void _XtDestroyServerGrabs(
        Widget		/* w */,
        XtPointer		/* pwi */, /*XtPerWidgetInput*/
        XtPointer		/* call_data */
);

extern XtPerWidgetInput _XtGetPerWidgetInput(
        Widget	/* widget */,
        _XtBoolean	/* create */
);

extern XtServerGrabPtr _XtCheckServerGrabsOnWidget(
        XEvent*		/* event */,
        Widget		/* widget */,
        _XtBoolean		/* isKeyboard */
);

/*
extern XtGrabList* _XtGetGrabList( XtPerDisplayInput );
*/

#define _XtGetGrabList(pdi) (&(pdi)->grabList)

extern void _XtFreePerWidgetInput(
        Widget		/* w */,
        XtPerWidgetInput	/* pwi */
);

extern Widget _XtProcessKeyboardEvent(
        XKeyEvent*		/* event */,
        Widget		/* widget */,
        XtPerDisplayInput	/* pdi */
);

extern Widget _XtProcessPointerEvent(
        XButtonEvent*	/* event */,
        Widget		/* widget */,
        XtPerDisplayInput	/* pdi */
);

extern void _XtRegisterPassiveGrabs(
        Widget		/* widget */
);

extern void _XtClearAncestorCache(
        Widget		/* widget */
);

_XFUNCPROTOEND

#endif /* _PDI_h_ */
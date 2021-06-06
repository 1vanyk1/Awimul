#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/x.h"

/*
 * This array can be used given an event type to determine the mask bits
 * that could have generated it.
 */
long const _Xevent_to_mask [LASTEvent] = {
        0,						/* no event 0 */
        0,						/* no event 1 */
        KeyPressMask,					/* KeyPress */
        KeyReleaseMask,					/* KeyRelease */
        ButtonPressMask,				/* ButtonPress */
        ButtonReleaseMask,				/* ButtonRelease */
        PointerMotionMask|PointerMotionHintMask|Button1MotionMask|
        Button2MotionMask|Button3MotionMask|Button4MotionMask|
        Button5MotionMask|ButtonMotionMask,	/* MotionNotify */
        EnterWindowMask,				/* EnterNotify */
        LeaveWindowMask,				/* LeaveNotify */
        FocusChangeMask,				/* FocusIn */
        FocusChangeMask,				/* FocusOut */
        KeymapStateMask,				/* KeymapNotify */
        ExposureMask,					/* Expose */
        ExposureMask,					/* GraphicsExpose */
        ExposureMask,					/* NoExpose */
        VisibilityChangeMask,				/* VisibilityNotify */
        SubstructureNotifyMask,				/* CreateNotify */
        StructureNotifyMask|SubstructureNotifyMask,	/* DestroyNotify */
        StructureNotifyMask|SubstructureNotifyMask,	/* UnmapNotify */
        StructureNotifyMask|SubstructureNotifyMask,	/* MapNotify */
        SubstructureRedirectMask,			/* MapRequest */
        SubstructureNotifyMask|StructureNotifyMask,	/* ReparentNotify */
        StructureNotifyMask|SubstructureNotifyMask,	/* ConfigureNotify */
        SubstructureRedirectMask,			/* ConfigureRequest */
        SubstructureNotifyMask|StructureNotifyMask,	/* GravityNotify */
        ResizeRedirectMask,				/* ResizeRequest */
        SubstructureNotifyMask|StructureNotifyMask,	/* CirculateNotify */
        SubstructureRedirectMask,			/* CirculateRequest */
        PropertyChangeMask,				/* PropertyNotify */
        0,						/* SelectionClear */
        0,						/* SelectionRequest */
        0,						/* SelectionNotify */
        ColormapChangeMask,				/* ColormapNotify */
        0,						/* ClientMessage */
        0,						/* MappingNotify */
};
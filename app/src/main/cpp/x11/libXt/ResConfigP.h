#ifndef _RESCONFIGP_H
#define _RESCONFIGP_H

#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

/*
 * Atom names for resource configuration management customization tool.
 */
#define RCM_DATA "Custom Data"
#define RCM_INIT "Custom Init"

extern void _XtResourceConfigurationEH(
        Widget 		/* w */,
        XtPointer 	/* client_data */,
        XEvent * 	/* event */
);

_XFUNCPROTOEND

#endif
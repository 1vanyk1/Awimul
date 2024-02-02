#ifndef _XMU_ERROR_H_
#define _XMU_ERROR_H_

#include <stdio.h>
#include "../headers/xlib.h"
#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

int XmuPrintDefaultErrorMessage
        (
                Display		*dpy,
                XErrorEvent		*event,
                FILE			*fp
        );

int XmuSimpleErrorHandler
        (
                Display		*dpy,
                XErrorEvent		*errorp
        );

_XFUNCPROTOEND

#endif /* _XMU_ERROR_H_ */


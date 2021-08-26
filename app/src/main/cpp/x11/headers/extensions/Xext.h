#ifndef _XEXT_H_
#define _XEXT_H_

#include "../xfuncproto.h"

_XFUNCPROTOBEGIN

typedef int (*XextErrorHandler) (
        Display *		/* dpy */,
        _Xconst char*	/* ext_name */,
        _Xconst char*	/* reason */
);

extern XextErrorHandler XSetExtensionErrorHandler(
        XextErrorHandler	/* handler */
);

extern int XMissingExtension(
        Display*		/* dpy */,
        _Xconst char*	/* ext_name */
);

_XFUNCPROTOEND

#define X_EXTENSION_UNKNOWN "unknown"
#define X_EXTENSION_MISSING "missing"

#endif /* _XEXT_H_ */

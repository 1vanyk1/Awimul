#ifndef _SYSUTIL_H_
#define _SYSUTIL_H_

#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

int XmuGetHostname
        (
                char		*buf_return,
                int			maxlen
        );

#ifndef _XMU_H_
int XmuSnprintf
        (
                char			*str,
                int			size,
                _Xconst char		*fmt,
                ...
        )
_X_ATTRIBUTE_PRINTF(3,4);
#endif

_XFUNCPROTOEND

#endif /* _SYSUTIL_H_ */

#ifndef _XIMTRINT_H
#define _XIMTRINT_H

#define UNIXCONN

#include "Ximint.h"

typedef struct {
    const char	*transportname;
    Bool	 (*config)(
            Xim,
            char *
    );
} TransportSW;

extern TransportSW _XimTransportRec[];

/*
 * Global symbols
 */

extern Bool	_XimXConf(
        Xim		 im,
        char	*address
);

#if defined(TCPCONN) || defined(UNIXCONN)

extern Bool	_XimTransConf(
    Xim		 im,
    char	*address
);

#endif

#endif /* _XIMTRINT_H */
#ifndef _XMU_CHARSET_H_
#define _XMU_CHARSET_H_

#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

void XmuCopyISOLatin1Lowered
        (
                char		*dst_return,
                _Xconst char	*src
        );

void XmuCopyISOLatin1Uppered
        (
                char		*dst_return,
                _Xconst char	*src
        );

int XmuCompareISOLatin1
        (
                _Xconst char	*first,
                _Xconst char	*second
        );

void XmuNCopyISOLatin1Lowered
        (
                char		*dst_return,
                _Xconst char	*src,
                int		 size
        );

void XmuNCopyISOLatin1Uppered
        (
                char		*dst_return,
                _Xconst char	*src,
                int		size
        );

_XFUNCPROTOEND

#endif /* _XMU_CHARSET_H_ */

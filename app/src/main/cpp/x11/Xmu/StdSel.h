#ifndef _XMU_SELECTION_H_
#define _XMU_SELECTION_H_

#include "../libXt/Intrinsic.h"
#include "../headers/xfuncproto.h"

_XFUNCPROTOBEGIN

        Boolean XmuConvertStandardSelection
(
        Widget			w,
Time			timev,
        Atom			*selection,
Atom			*target,
        Atom			*type_return,
XPointer		*value_return,
unsigned long		*length_return,
int			*format_return
);

_XFUNCPROTOEND

#endif /* _XMU_SELECTION_H_ */

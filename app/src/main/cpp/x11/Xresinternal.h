#ifndef _XRESINTERNAL_H_
#define _XRESINTERNAL_H_

#include "headers/xlib.h"
#include "headers/xresource.h"
#include <inttypes.h>
#include <stdint.h>
/* type defines */
typedef uint32_t Signature;

/* prototypes */
extern XrmQuark _XrmInternalStringToQuark(
        register _Xconst char *name, register int len, register Signature sig,
        Bool permstring);

#endif /* _XRESOURCEINTERNAL_H_ */
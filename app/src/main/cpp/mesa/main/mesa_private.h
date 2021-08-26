#ifndef MESA_PRIVATE_H
#define MESA_PRIVATE_H

#include "glheader.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Sometimes we treat floats as ints.  On x86 systems, moving a float
 * as an int (thereby using integer registers instead of FP registers) is
 * a performance win.  Typically, this can be done with ordinary casts.
 * But with gcc's -fstrict-aliasing flag (which defaults to on in gcc 3.0)
 * these casts generate warnings.
 * The following union typedef is used to solve that.
 */
typedef union { float f; int i; unsigned u; } fi_type;


#ifdef __cplusplus
}
#endif

#endif /* MESA_PRIVATE_H */

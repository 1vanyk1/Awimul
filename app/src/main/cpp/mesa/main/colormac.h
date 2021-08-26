#ifndef COLORMAC_H
#define COLORMAC_H


#include "config.h"
#include "macros.h"


/**
 * Convert four float values in [0,1] to ubytes in [0,255] with clamping.
 */
static inline void
_mesa_unclamped_float_rgba_to_ubyte(GLubyte dst[4], const GLfloat src[4])
{
    int i;
    for (i = 0; i < 4; i++)
        UNCLAMPED_FLOAT_TO_UBYTE(dst[i], src[i]);
}


/**
 * \name Generic color packing macros.  All inputs should be GLubytes.
 */
/*@{*/

#define PACK_COLOR_8888( X, Y, Z, W ) \
   (((X) << 24) | ((Y) << 16) | ((Z) << 8) | (W))

#define PACK_COLOR_565( X, Y, Z )                                  \
   ((((X) & 0xf8) << 8) | (((Y) & 0xfc) << 3) | (((Z) & 0xf8) >> 3))

#define PACK_COLOR_1555( A, B, G, R )					\
   ((((B) & 0xf8) << 7) | (((G) & 0xf8) << 2) | (((R) & 0xf8) >> 3) |	\
    (((A) & 0x80) << 8))

#define PACK_COLOR_4444( R, G, B, A )					\
   ((((R) & 0xf0) << 8) | (((G) & 0xf0) << 4) | ((B) & 0xf0) | ((A) >> 4))

#define PACK_COLOR_88( L, A )						\
   (((L) << 8) | (A))

/*@}*/


#endif /* COLORMAC_H */

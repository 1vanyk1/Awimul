#ifndef U_CHAN_H
#define U_CHAN_H


#include "../main/config.h"


/**
 * Default bits per color channel:  8, 16 or 32
 */
#ifndef CHAN_BITS
#define CHAN_BITS 8
#endif


/**
 * Color channel data type.
 */
#if CHAN_BITS == 8
typedef GLubyte GLchan;
#define CHAN_MAX 255
#define CHAN_MAXF 255.0F
#define CHAN_TYPE GL_UNSIGNED_BYTE
#elif CHAN_BITS == 16
typedef GLushort GLchan;
#define CHAN_MAX 65535
#define CHAN_MAXF 65535.0F
#define CHAN_TYPE GL_UNSIGNED_SHORT
#elif CHAN_BITS == 32
   typedef GLfloat GLchan;
#define CHAN_MAX 1.0
#define CHAN_MAXF 1.0F
#define CHAN_TYPE GL_FLOAT
#else
#error "illegal number of color channel bits"
#endif


#if CHAN_BITS == 8

#define CHAN_TO_UBYTE(c)  (c)
#define CHAN_TO_USHORT(c) (((c) << 8) | (c))
#define CHAN_TO_SHORT(c)  (((c) << 7) | ((c) >> 1))
#define CHAN_TO_FLOAT(c)  UBYTE_TO_FLOAT(c)

#define CLAMPED_FLOAT_TO_CHAN(c, f)    CLAMPED_FLOAT_TO_UBYTE(c, f)
#define UNCLAMPED_FLOAT_TO_CHAN(c, f)  UNCLAMPED_FLOAT_TO_UBYTE(c, f)

#define COPY_CHAN4(DST, SRC)  COPY_4UBV(DST, SRC)

#elif CHAN_BITS == 16

#define CHAN_TO_UBYTE(c)  ((c) >> 8)
#define CHAN_TO_USHORT(c) (c)
#define CHAN_TO_SHORT(c)  ((c) >> 1)
#define CHAN_TO_FLOAT(c)  ((GLfloat) ((c) * (1.0 / CHAN_MAXF)))

#define CLAMPED_FLOAT_TO_CHAN(c, f)    CLAMPED_FLOAT_TO_USHORT(c, f)
#define UNCLAMPED_FLOAT_TO_CHAN(c, f)  UNCLAMPED_FLOAT_TO_USHORT(c, f)

#define COPY_CHAN4(DST, SRC)  COPY_4V(DST, SRC)

#elif CHAN_BITS == 32

#define CHAN_TO_UBYTE(c)  FLOAT_TO_UBYTE(c)
#define CHAN_TO_USHORT(c) ((GLushort) (SATURATE((c)) * 65535.0))
#define CHAN_TO_SHORT(c)  ((GLshort) (SATURATE((c)) * 32767.0))
#define CHAN_TO_FLOAT(c)  (c)

#define CLAMPED_FLOAT_TO_CHAN(c, f)  c = (f)
#define UNCLAMPED_FLOAT_TO_CHAN(c, f)      c = (f)

#define COPY_CHAN4(DST, SRC)  COPY_4V(DST, SRC)

#else

#error unexpected CHAN_BITS size

#endif


/**
 * Convert 4 floats to GLchan values.
 * \param dst pointer to destination GLchan[4] array.
 * \param f pointer to source GLfloat[4] array.
 */
#define UNCLAMPED_FLOAT_TO_RGBA_CHAN(dst, f)	\
do {						\
   UNCLAMPED_FLOAT_TO_CHAN((dst)[0], (f)[0]);	\
   UNCLAMPED_FLOAT_TO_CHAN((dst)[1], (f)[1]);	\
   UNCLAMPED_FLOAT_TO_CHAN((dst)[2], (f)[2]);	\
   UNCLAMPED_FLOAT_TO_CHAN((dst)[3], (f)[3]);	\
} while (0)



#endif /* U_CHAN_H */

#ifndef UTIL_FORMAT_UTILS_H
#define UTIL_FORMAT_UTILS_H

#include "../half_float.h"
#include "../rounding.h"

/* Only guaranteed to work for BITS <= 32 */
#define MAX_UINT(BITS) ((BITS) == 32 ? UINT32_MAX : ((1u << (BITS)) - 1))
#define MAX_INT(BITS) ((int)MAX_UINT((BITS) - 1))
#define MIN_INT(BITS) ((BITS) == 32 ? INT32_MIN : (-(1 << (BITS - 1))))

/* Extends an integer of size SRC_BITS to one of size DST_BITS linearly */
#define EXTEND_NORMALIZED_INT(X, SRC_BITS, DST_BITS) \
      (((X) * (int)(MAX_UINT(DST_BITS) / MAX_UINT(SRC_BITS))) + \
       ((DST_BITS % SRC_BITS) ? ((X) >> (SRC_BITS - DST_BITS % SRC_BITS)) : 0))

static inline float
_mesa_unorm_to_float(unsigned x, unsigned src_bits)
{
    return x * (1.0f / (float)MAX_UINT(src_bits));
}

static inline float
_mesa_snorm_to_float(int x, unsigned src_bits)
{
    if (x <= -MAX_INT(src_bits))
        return -1.0f;
    else
        return x * (1.0f / (float)MAX_INT(src_bits));
}

static inline uint16_t
_mesa_unorm_to_half(unsigned x, unsigned src_bits)
{
    return _mesa_float_to_half(_mesa_unorm_to_float(x, src_bits));
}

static inline uint16_t
_mesa_snorm_to_half(int x, unsigned src_bits)
{
    return _mesa_float_to_half(_mesa_snorm_to_float(x, src_bits));
}

static inline unsigned
_mesa_float_to_unorm(float x, unsigned dst_bits)
{
    if (x < 0.0f)
        return 0;
    else if (x > 1.0f)
        return MAX_UINT(dst_bits);
    else
        return _mesa_i64roundevenf(x * MAX_UINT(dst_bits));
}

static inline unsigned
_mesa_half_to_unorm(uint16_t x, unsigned dst_bits)
{
    return _mesa_float_to_unorm(_mesa_half_to_float(x), dst_bits);
}

static inline unsigned
_mesa_unorm_to_unorm(unsigned x, unsigned src_bits, unsigned dst_bits)
{
    if (src_bits < dst_bits) {
        return EXTEND_NORMALIZED_INT(x, src_bits, dst_bits);
    } else if (src_bits > dst_bits) {
        unsigned src_half = (1u << (src_bits - 1)) - 1;

        if (src_bits + dst_bits > sizeof(x) * 8) {
            assert(src_bits + dst_bits <= sizeof(uint64_t) * 8);
            return (((uint64_t) x * MAX_UINT(dst_bits) + src_half) /
                    MAX_UINT(src_bits));
        } else {
            return (x * MAX_UINT(dst_bits) + src_half) / MAX_UINT(src_bits);
        }
    } else {
        return x;
    }
}

static inline unsigned
_mesa_snorm_to_unorm(int x, unsigned src_bits, unsigned dst_bits)
{
    if (x < 0)
        return 0;
    else
        return _mesa_unorm_to_unorm(x, src_bits - 1, dst_bits);
}

static inline int
_mesa_float_to_snorm(float x, unsigned dst_bits)
{
    if (x < -1.0f)
        return -MAX_INT(dst_bits);
    else if (x > 1.0f)
        return MAX_INT(dst_bits);
    else
        return _mesa_lroundevenf(x * MAX_INT(dst_bits));
}

static inline int
_mesa_half_to_snorm(uint16_t x, unsigned dst_bits)
{
    return _mesa_float_to_snorm(_mesa_half_to_float(x), dst_bits);
}

static inline int
_mesa_unorm_to_snorm(unsigned x, unsigned src_bits, unsigned dst_bits)
{
    return _mesa_unorm_to_unorm(x, src_bits, dst_bits - 1);
}

static inline int
_mesa_snorm_to_snorm(int x, unsigned src_bits, unsigned dst_bits)
{
    if (x < -MAX_INT(src_bits))
        return -MAX_INT(dst_bits);
    else if (src_bits < dst_bits)
        return EXTEND_NORMALIZED_INT(x, src_bits - 1, dst_bits - 1);
    else
        return x >> (src_bits - dst_bits);
}

static inline unsigned
_mesa_unsigned_to_unsigned(unsigned src, unsigned dst_size)
{
    return MIN2(src, MAX_UINT(dst_size));
}

static inline int
_mesa_unsigned_to_signed(unsigned src, unsigned dst_size)
{
    return MIN2(src, (unsigned)MAX_INT(dst_size));
}

static inline int
_mesa_signed_to_signed(int src, unsigned dst_size)
{
    return CLAMP(src, MIN_INT(dst_size), MAX_INT(dst_size));
}

static inline unsigned
_mesa_signed_to_unsigned(int src, unsigned dst_size)
{
    return CLAMP(src, 0, MAX_UINT(dst_size));
}

static inline unsigned
_mesa_float_to_unsigned(float src, unsigned dst_bits)
{
    if (src < 0.0f)
        return 0;
    if (src > (float)MAX_UINT(dst_bits))
        return MAX_UINT(dst_bits);
    return _mesa_signed_to_unsigned(src, dst_bits);
}

static inline unsigned
_mesa_float_to_signed(float src, unsigned dst_bits)
{
    if (src < (float)(-MAX_INT(dst_bits)))
        return -MAX_INT(dst_bits);
    if (src > (float)MAX_INT(dst_bits))
        return MAX_INT(dst_bits);
    return _mesa_signed_to_signed(src, dst_bits);
}

static inline unsigned
_mesa_half_to_unsigned(uint16_t src, unsigned dst_bits)
{
    if (_mesa_half_is_negative(src))
        return 0;
    return _mesa_unsigned_to_unsigned(_mesa_float_to_half(src), dst_bits);
}

static inline unsigned
_mesa_half_to_signed(uint16_t src, unsigned dst_bits)
{
    return _mesa_float_to_signed(_mesa_half_to_float(src), dst_bits);
}

#endif /* UTIL_FORMAT_UTILS_H */

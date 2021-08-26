#ifndef UTIL_BIGMATH_H
#define UTIL_BIGMATH_H

#include "macros.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

static inline bool
_ubm_add_u32arr(uint32_t *dst, unsigned dst_len,
                uint32_t *a, unsigned a_len,
                uint32_t *b, unsigned b_len)
{
    uint32_t carry = 0;
    for (unsigned i = 0; i < dst_len; i++) {
        uint64_t sum = carry;
        if (i < a_len)
            sum += a[i];
        if (i < b_len)
            sum += b[i];
        dst[i] = sum;
        carry = sum >> 32;
    }

    /* Now compute overflow */

    for (unsigned i = dst_len; i < a_len; i++) {
        if (a[i])
            return true;
    }

    for (unsigned i = dst_len; i < b_len; i++) {
        if (b[i])
            return true;
    }

    return carry;
}
#define ubm_add_u32arr(dst, a, b) \
   _ubm_add_u32arr(dst, ARRAY_SIZE(dst), a, ARRAY_SIZE(a), b, ARRAY_SIZE(b))

static inline bool
_ubm_mul_u32arr(uint32_t *dst, unsigned dst_len,
                uint32_t *a, unsigned a_len,
                uint32_t *b, unsigned b_len)
{
    memset(dst, 0, dst_len * sizeof(*dst));

    bool overflow = false;

    for (unsigned i = 0; i < a_len; i++) {
        uint32_t carry = 0;
        for (unsigned j = 0; j < b_len; j++) {
            /* The maximum values of a[i] and b[i] are UINT32_MAX so the maximum
             * value of tmp is UINT32_MAX * UINT32_MAX.  The maximum value that
             * will fit in tmp is
             *
             *    UINT64_MAX = UINT32_MAX << 32 + UINT32_MAX
             *               = UINT32_MAX * (UINT32_MAX + 1) + UINT32_MAX
             *               = UINT32_MAX * UINT32_MAX + 2 * UINT32_MAX
             *
             * so we're guaranteed that we can add in two more 32-bit values
             * without overflowing tmp.
             */
            uint64_t tmp = (uint64_t)a[i] * (uint64_t)b[j];
            tmp += carry;
            if (i + j < dst_len) {
                tmp += dst[i + j];
                dst[i + j] = tmp;
                carry = tmp >> 32;
            } else {
                /* We're trying to write a value that doesn't fit */
                overflow = overflow || tmp > 0;
                break;
            }
        }
        if (i + b_len < dst_len)
            dst[i + b_len] = carry;
        else
            overflow = overflow || carry > 0;
    }

    return overflow;
}
#define ubm_mul_u32arr(dst, a, b) \
   _ubm_mul_u32arr(dst, ARRAY_SIZE(dst), a, ARRAY_SIZE(a), b, ARRAY_SIZE(b))

#endif /* UTIL_BIGMATH_H */

#ifndef FORMAT_R11G11B10F_H
#define FORMAT_R11G11B10F_H

#include <stdint.h>

#define UF11(e, m)           ((e << 6) | (m))
#define UF11_EXPONENT_BIAS   15
#define UF11_EXPONENT_BITS   0x1F
#define UF11_EXPONENT_SHIFT  6
#define UF11_MANTISSA_BITS   0x3F
#define UF11_MANTISSA_SHIFT  (23 - UF11_EXPONENT_SHIFT)
#define UF11_MAX_EXPONENT    (UF11_EXPONENT_BITS << UF11_EXPONENT_SHIFT)

#define UF10(e, m)           ((e << 5) | (m))
#define UF10_EXPONENT_BIAS   15
#define UF10_EXPONENT_BITS   0x1F
#define UF10_EXPONENT_SHIFT  5
#define UF10_MANTISSA_BITS   0x1F
#define UF10_MANTISSA_SHIFT  (23 - UF10_EXPONENT_SHIFT)
#define UF10_MAX_EXPONENT    (UF10_EXPONENT_BITS << UF10_EXPONENT_SHIFT)

#define F32_INFINITY         0x7f800000

static inline uint32_t f32_to_uf11(float val)
{
    union {
        float f;
        uint32_t ui;
    } f32 = {val};

    uint16_t uf11 = 0;

    /* Decode little-endian 32-bit floating-point value */
    int sign = (f32.ui >> 16) & 0x8000;
    /* Map exponent to the range [-127,128] */
    int exponent = ((f32.ui >> 23) & 0xff) - 127;
    int mantissa = f32.ui & 0x007fffff;

    if (exponent == 128) { /* Infinity or NaN */
        /* From the GL_EXT_packed_float spec:
         *
         *     "Additionally: negative infinity is converted to zero; positive
         *      infinity is converted to positive infinity; and both positive and
         *      negative NaN are converted to positive NaN."
         */
        uf11 = UF11_MAX_EXPONENT;
        if (mantissa) {
            uf11 |= 1; /* NaN */
        } else {
            if (sign)
                uf11 = 0; /* 0.0 */
        }
    } else if (sign) {
        return 0;
    } else if (val > 65024.0f) {
        /* From the GL_EXT_packed_float spec:
         *
         *     "Likewise, finite positive values greater than 65024 (the maximum
         *      finite representable unsigned 11-bit floating-point value) are
         *      converted to 65024."
         */
        uf11 = UF11(30, 63);
    } else if (exponent > -15) { /* Representable value */
        exponent += UF11_EXPONENT_BIAS;
        mantissa >>= UF11_MANTISSA_SHIFT;
        uf11 = exponent << UF11_EXPONENT_SHIFT | mantissa;
    }

    return uf11;
}

static inline float uf11_to_f32(uint16_t val)
{
    union {
        float f;
        uint32_t ui;
    } f32;

    int exponent = (val & 0x07c0) >> UF11_EXPONENT_SHIFT;
    int mantissa = (val & 0x003f);

    f32.f = 0.0;

    if (exponent == 0) {
        if (mantissa != 0) {
            const float scale = 1.0 / (1 << 20);
            f32.f = scale * mantissa;
        }
    } else if (exponent == 31) {
        f32.ui = F32_INFINITY | mantissa;
    } else {
        float scale, decimal;
        exponent -= 15;
        if (exponent < 0) {
            scale = 1.0f / (1 << -exponent);
        } else {
            scale = (float) (1 << exponent);
        }
        decimal = 1.0f + (float) mantissa / 64;
        f32.f = scale * decimal;
    }

    return f32.f;
}

static inline uint32_t f32_to_uf10(float val)
{
    union {
        float f;
        uint32_t ui;
    } f32 = {val};

    uint16_t uf10 = 0;

    /* Decode little-endian 32-bit floating-point value */
    int sign = (f32.ui >> 16) & 0x8000;
    /* Map exponent to the range [-127,128] */
    int exponent = ((f32.ui >> 23) & 0xff) - 127;
    int mantissa = f32.ui & 0x007fffff;

    if (exponent == 128) {
        /* From the GL_EXT_packed_float spec:
         *
         *     "Additionally: negative infinity is converted to zero; positive
         *      infinity is converted to positive infinity; and both positive and
         *      negative NaN are converted to positive NaN."
         */
        uf10 = UF10_MAX_EXPONENT;
        if (mantissa) {
            uf10 |= 1; /* NaN */
        } else {
            if (sign)
                uf10 = 0; /* 0.0 */
        }
    } else if (sign) {
        return 0;
    } else if (val > 64512.0f) {
        /* From the GL_EXT_packed_float spec:
         *
         *     "Likewise, finite positive values greater than 64512 (the maximum
         *      finite representable unsigned 10-bit floating-point value) are
         *      converted to 64512."
         */
        uf10 = UF10(30, 31);
    } else if (exponent > -15) { /* Representable value */
        exponent += UF10_EXPONENT_BIAS;
        mantissa >>= UF10_MANTISSA_SHIFT;
        uf10 = exponent << UF10_EXPONENT_SHIFT | mantissa;
    }

    return uf10;
}

static inline float uf10_to_f32(uint16_t val)
{
    union {
        float f;
        uint32_t ui;
    } f32;

    int exponent = (val & 0x03e0) >> UF10_EXPONENT_SHIFT;
    int mantissa = (val & 0x001f);

    f32.f = 0.0;

    if (exponent == 0) {
        if (mantissa != 0) {
            const float scale = 1.0 / (1 << 19);
            f32.f = scale * mantissa;
        }
    } else if (exponent == 31) {
        f32.ui = F32_INFINITY | mantissa;
    } else {
        float scale, decimal;
        exponent -= 15;
        if (exponent < 0) {
            scale = 1.0f / (1 << -exponent);
        }
        else {
            scale = (float) (1 << exponent);
        }
        decimal = 1.0f + (float) mantissa / 32;
        f32.f = scale * decimal;
    }

    return f32.f;
}

static inline uint32_t float3_to_r11g11b10f(const float rgb[3])
{
    return ( f32_to_uf11(rgb[0]) & 0x7ff) |
           ((f32_to_uf11(rgb[1]) & 0x7ff) << 11) |
           ((f32_to_uf10(rgb[2]) & 0x3ff) << 22);
}

static inline void r11g11b10f_to_float3(uint32_t rgb, float retval[3])
{
    retval[0] = uf11_to_f32( rgb        & 0x7ff);
    retval[1] = uf11_to_f32((rgb >> 11) & 0x7ff);
    retval[2] = uf10_to_f32((rgb >> 22) & 0x3ff);
}

#endif /* FORMAT_R11G11B10F_H */

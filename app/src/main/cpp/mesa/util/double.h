#ifndef _DOUBLE_H_
#define _DOUBLE_H_


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This API is no more than a wrapper to the counterpart softfloat.h
 * calls. Still, softfloat.h conversion API is meant to be kept private. In
 * other words, only use the API published here, instead of calling directly
 * the softfloat.h one.
 */

float _mesa_double_to_float(double val);
float _mesa_double_to_float_rtz(double val);

static inline float
_mesa_double_to_float_rtne(double val)
{
    return _mesa_double_to_float(val);
}

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _DOUBLE_H_ */

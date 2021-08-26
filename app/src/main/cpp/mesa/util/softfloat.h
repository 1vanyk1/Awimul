#ifndef _SOFTFLOAT_H_
#define _SOFTFLOAT_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

double _mesa_double_add_rtz(double a, double b);
double _mesa_double_sub_rtz(double a, double b);
double _mesa_double_mul_rtz(double a, double b);
double _mesa_double_fma_rtz(double a, double b, double c);
float _mesa_float_fma_rtz(float a, float b, float c);
float _mesa_double_to_f32(double x, bool rtz);
uint16_t _mesa_float_to_half_rtz_slow(float x);

#ifdef __cplusplus
} /* extern C */
#endif

#endif  /* _SOFTFLOAT_H */

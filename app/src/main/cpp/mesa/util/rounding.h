#ifndef _ROUNDING_H
#define _ROUNDING_H

#include "../include/c99_math.h"

#include <limits.h>
#include <stdint.h>

#if defined(__SSE__) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || (defined(_M_X64) && !defined(_M_ARM64EC))
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

#ifdef __SSE4_1__
#include <smmintrin.h>
#endif

/* The C standard library has functions round()/rint()/nearbyint() that round
 * their arguments according to the rounding mode set in the floating-point
 * control register. While there are trunc()/ceil()/floor() functions that do
 * a specific operation without modifying the rounding mode, there is no
 * roundeven() in any version of C.
 *
 * Technical Specification 18661 (ISO/IEC TS 18661-1:2014) adds roundeven(),
 * but it's unfortunately not implemented by glibc.
 *
 * This implementation differs in that it does not raise the inexact exception.
 *
 * We use rint() to implement these functions, with the assumption that the
 * floating-point rounding mode has not been changed from the default Round
 * to Nearest.
 */

/**
 * \brief Rounds \c x to the nearest integer, with ties to the even integer.
 */
static inline float
_mesa_roundevenf(float x)
{
#ifdef __SSE4_1__
    float ret;
   __m128 m = _mm_load_ss(&x);
   m = _mm_round_ss(m, m, _MM_FROUND_CUR_DIRECTION | _MM_FROUND_NO_EXC);
   _mm_store_ss(&ret, m);
   return ret;
#else
    return rintf(x);
#endif
}

/**
 * \brief Rounds \c x to the nearest integer, with ties to the even integer.
 */
static inline double
_mesa_roundeven(double x)
{
#ifdef __SSE4_1__
    double ret;
   __m128d m = _mm_load_sd(&x);
   m = _mm_round_sd(m, m, _MM_FROUND_CUR_DIRECTION | _MM_FROUND_NO_EXC);
   _mm_store_sd(&ret, m);
   return ret;
#else
    return rint(x);
#endif
}

/**
 * \brief Rounds \c x to the nearest integer, with ties to the even integer,
 * and returns the value as a long int.
 */
static inline long
_mesa_lroundevenf(float x)
{
#if defined(__SSE__) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || (defined(_M_X64) && !defined(_M_ARM64EC))
    #if LONG_MAX == INT64_MAX
   return _mm_cvtss_si64(_mm_load_ss(&x));
#elif LONG_MAX == INT32_MAX
   return _mm_cvtss_si32(_mm_load_ss(&x));
#else
#error "Unsupported long size"
#endif
#else
    return lrintf(x);
#endif
}

/**
 * \brief Rounds \c x to the nearest integer, with ties to the even integer,
 * and returns the value as a long int.
 */
static inline long
_mesa_lroundeven(double x)
{
#if defined(__SSE2__) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 2)) || (defined(_M_X64) && !defined(_M_ARM64EC))
    #if LONG_MAX == INT64_MAX
   return _mm_cvtsd_si64(_mm_load_sd(&x));
#elif LONG_MAX == INT32_MAX
   return _mm_cvtsd_si32(_mm_load_sd(&x));
#else
#error "Unsupported long size"
#endif
#else
    return lrint(x);
#endif
}

/**
 * \brief Rounds \c x to the nearest integer, with ties to the even integer,
 * and returns the value as an int64_t.
 */
static inline int64_t
_mesa_i64roundevenf(float x)
{
#if LONG_MAX == INT64_MAX
    return _mesa_lroundevenf(x);
#elif LONG_MAX == INT32_MAX
    return llrintf(x);
#else
#error "Unsupported long size"
#endif
}

#endif

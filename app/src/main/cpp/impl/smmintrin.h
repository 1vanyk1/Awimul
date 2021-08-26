#ifndef __SMMINTRIN_H2
#define __SMMINTRIN_H2

typedef double __m128d __attribute__((__vector_size__(16), __aligned__(16)));
typedef long long __m128i __attribute__((__vector_size__(16), __aligned__(16)));

typedef double __m128d_u __attribute__((__vector_size__(16), __aligned__(1)));
typedef long long __m128i_u __attribute__((__vector_size__(16), __aligned__(1)));

/* Type defines.  */
typedef double __v2df __attribute__ ((__vector_size__ (16)));
typedef long long __v2di __attribute__ ((__vector_size__ (16)));
typedef short __v8hi __attribute__((__vector_size__(16)));
typedef char __v16qi __attribute__((__vector_size__(16)));

/* Unsigned types */
typedef unsigned long long __v2du __attribute__ ((__vector_size__ (16)));
typedef unsigned short __v8hu __attribute__((__vector_size__(16)));
typedef unsigned char __v16qu __attribute__((__vector_size__(16)));

/* We need an explicitly signed variant for char. Note that this shouldn't
 * appear in the interface though. */
typedef signed char __v16qs __attribute__((__vector_size__(16)));

/* Define the default attributes for the functions in this file. */


typedef int __v4si __attribute__((__vector_size__(16)));
typedef float __v4sf __attribute__((__vector_size__(16)));
typedef float __m128 __attribute__((__vector_size__(16), __aligned__(16)));

typedef float __m128_u __attribute__((__vector_size__(16), __aligned__(1)));

/* Unsigned types */
typedef unsigned int __v4su __attribute__((__vector_size__(16)));

/* This header should only be included in a hosted environment as it depends on
 * a standard library to provide allocation routines. */

/* Define the default attributes for the functions in this file. */
#define __DEFAULT_FN_ATTRS __attribute__((__always_inline__, __nodebug__, __min_vector_width__(128)))
#define __DEFAULT_FN_ATTRS_MMX __attribute__((__always_inline__, __nodebug__, __target__("mmx,sse"), __min_vector_width__(64)))

static __inline__  void __DEFAULT_FN_ATTRS
_mm_store_si128(__m128i *__p, __m128i __b)
{
    *__p = __b;
}

static __inline__  __m128i __DEFAULT_FN_ATTRS
_mm_set_epi32(int __i3, int __i2, int __i1, int __i0)
{
    return __extension__ (__m128i)(__v4si){ __i0, __i1, __i2, __i3};
}

static __inline__  __m128i __DEFAULT_FN_ATTRS
_mm_setzero_si128(void)
{
    return __extension__ (__m128i)(__v2di){ 0LL, 0LL };
}

static __inline__  __m128i __DEFAULT_FN_ATTRS
_mm_set1_epi32(int __i)
{
    return _mm_set_epi32(__i, __i, __i, __i);
}

static __inline__  __m128i __DEFAULT_FN_ATTRS
_mm_load_si128(__m128i const *__p)
{
    return *__p;
}


__m128i _mm_max_epu32(__m128i __V1, __m128i __V2) {
    unsigned __V1_arr[4] __attribute__ ((aligned (16)));
    unsigned __V2_arr[4] __attribute__ ((aligned (16)));
    _mm_store_si128((__m128i *)__V1_arr, __V1);
    _mm_store_si128((__m128i *)__V2_arr, __V2);
    for (int i = 0; i < 4; i++) {
        if (__V1_arr[i] > __V2_arr[i]) {
            return __V1;
        }
        if (__V1_arr[i] < __V2_arr[i]) {
            return __V2;
        }
    }
    return __V2;
}

__m128i _mm_min_epu32(__m128i __V1, __m128i __V2) {
    unsigned __V1_arr[4] __attribute__ ((aligned (16)));
    unsigned __V2_arr[4] __attribute__ ((aligned (16)));
    _mm_store_si128((__m128i *)__V1_arr, __V1);
    _mm_store_si128((__m128i *)__V2_arr, __V2);
    for (int i = 0; i < 4; i++) {
        if (__V1_arr[i] < __V2_arr[i]) {
            return __V1;
        }
        if (__V1_arr[i] > __V2_arr[i]) {
            return __V2;
        }
    }
    return __V2;
}

#endif //__SMMINTRIN_H2

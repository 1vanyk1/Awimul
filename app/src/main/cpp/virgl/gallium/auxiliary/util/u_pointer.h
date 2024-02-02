#ifndef U_POINTER_H
#define U_POINTER_H

#include "../../../mesa/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline intptr_t
pointer_to_intptr( const void *p )
{
    union {
        const void *p;
        intptr_t i;
    } pi;
    pi.p = p;
    return pi.i;
}

static inline void *
intptr_to_pointer( intptr_t i )
{
    union {
        void *p;
        intptr_t i;
    } pi;
    pi.i = i;
    return pi.p;
}

static inline uintptr_t
pointer_to_uintptr( const void *ptr )
{
    union {
        const void *p;
        uintptr_t u;
    } pu;
    pu.p = ptr;
    return pu.u;
}

static inline void *
uintptr_to_pointer( uintptr_t u )
{
    union {
        void *p;
        uintptr_t u;
    } pu;
    pu.u = u;
    return pu.p;
}

/**
 * Return a pointer aligned to next multiple of N bytes.
 */
static inline void *
align_pointer( const void *unaligned, uintptr_t alignment )
{
    uintptr_t aligned = (pointer_to_uintptr( unaligned ) + alignment - 1) & ~(alignment - 1);
    return uintptr_to_pointer( aligned );
}


/**
 * Return a pointer aligned to next multiple of 16 bytes.
 */
static inline void *
align16( void *unaligned )
{
    return align_pointer( unaligned, 16 );
}

typedef void (*func_pointer)(void);

static inline func_pointer
pointer_to_func( void *p )
{
    union {
        void *p;
        func_pointer f;
    } pf;
    pf.p = p;
    return pf.f;
}

static inline void *
func_to_pointer( func_pointer f )
{
    union {
        void *p;
        func_pointer f;
    } pf;
    pf.f = f;
    return pf.p;
}


#ifdef __cplusplus
}
#endif

#endif /* U_POINTER_H */

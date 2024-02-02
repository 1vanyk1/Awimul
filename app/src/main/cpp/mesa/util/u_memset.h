#include "../include/pipe/p_config.h"

static inline void *
util_memset32(void *s, uint32_t ui, size_t n)
{
#if defined(PIPE_CC_GCC) && defined(PIPE_ARCH_X86_64)
    long d0, d1;
   __asm__ volatile("rep\n\t"
                    "stosl"
                    : "=&c" (d0), "=&D" (d1)
                    : "a" (ui), "1" (s), "0" (n)
                    : "memory");
   return s;
#else
    uint32_t *xs = (uint32_t *)s;
    while (n--)
        *xs++ = ui;
    return s;
#endif
}

static inline void *
util_memset64(void *s, uint64_t ui, size_t n)
{
#if defined(PIPE_CC_GCC) && defined(PIPE_ARCH_X86_64)
    long d0, d1;
   __asm__ volatile("rep\n\t"
                    "stosq"
                    : "=&c" (d0), "=&D" (d1)
                    : "a" (ui), "1" (s), "0" (n)
                    : "memory");
   return s;
#else
    uint64_t *xs = (uint64_t *)s;
    while (n--)
        *xs++ = ui;
    return s;
#endif

}

#ifndef _INCLUDE_STACKINFO_H
#define _INCLUDE_STACKINFO_H        1
/* A sysdeps/.../stackinfo.h file defines details for the CPU.
   It is obliged to define either _STACK_GROWS_DOWN or _STACK_GROWS_UP.  */
#include "arm/stackinfo.h"
#if defined _STACK_GROWS_DOWN && _STACK_GROWS_DOWN
# ifdef _STACK_GROWS_UP
#  error "stackinfo.h should not define both!"
# else
#  define _STACK_GROWS_UP        0
# endif
#elif defined _STACK_GROWS_UP && _STACK_GROWS_UP
# ifdef _STACK_GROWS_DOWN
#  error "stackinfo.h should not define both!"
# else
#  define _STACK_GROWS_DOWN        0
# endif
#else
# error "stackinfo.h must define _STACK_GROWS_UP or _STACK_GROWS_DOWN!"
#endif
#endif  /* include/stackinfo.h */
#ifndef _Xtos_h
#define _Xtos_h

#define ALLOCATE_LOCAL_FALLBACK(_size) XtMalloc((unsigned long)(_size))
#define DEALLOCATE_LOCAL_FALLBACK(_ptr) XtFree((XtPointer)(_ptr))
#include "../headers/Xalloca.h"


#if defined(__SIZEOF_LONG__)
# if __SIZEOF_LONG__ == 8
#  define LONG64
# endif
# elif defined (_LP64) || \
    defined(__alpha) || defined(__alpha__) || \
    defined(__ia64__) || defined(ia64) || \
    defined(__sparc64__) || \
    defined(__s390x__) || \
    (defined(__hppa__) && defined(__LP64__)) || \
    defined(__amd64__) || defined(amd64) || \
    defined(__powerpc64__) || \
    (defined(sgi) && (_MIPS_SZLONG == 64))
#define LONG64
#endif

#endif /* _Xtos_h */
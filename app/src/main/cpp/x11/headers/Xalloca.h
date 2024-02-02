#ifndef XALLOCA_H
#define XALLOCA_H 1

#ifndef INCLUDE_ALLOCA_H
/* Need to add more here to match Imake *.cf's */
# if defined(HAVE_ALLOCA_H) || defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  define INCLUDE_ALLOCA_H
# endif
#endif

#ifdef INCLUDE_ALLOCA_H
#  include <alloca.h>
#endif

#ifndef NO_ALLOCA
/*
 * os-dependent definition of local allocation and deallocation
 * If you want something other than (DE)ALLOCATE_LOCAL_FALLBACK
 * for ALLOCATE/DEALLOCATE_LOCAL then you add that in here.
 */


#  ifdef __GNUC__
#    ifndef alloca
#      define alloca __builtin_alloca
#    endif /* !alloca */
#    define ALLOCATE_LOCAL(size) alloca((size_t)(size))
#  else /* ! __GNUC__ */

/*
 * warning: old mips alloca (pre 2.10) is unusable, new one is built in
 * Test is easy, the new one is named __builtin_alloca and comes
 * from alloca.h which #defines alloca.
 */
#      if defined(__sun) || defined(alloca)
/*
 * Some System V boxes extract alloca.o from /lib/libPW.a; if you
 * decide that you don't want to use alloca, you might want to fix it here.
 */
/* alloca might be a macro taking one arg (hi, Sun!), so give it one. */
#        if !defined(__cplusplus)
#          define __Xnullarg		/* as nothing */
           extern void *alloca(__Xnullarg);
#        endif
#        define ALLOCATE_LOCAL(size) alloca((size_t)(size))
#      endif /* who does alloca */
#  endif /* __GNUC__ */

#endif /* NO_ALLOCA */

#if !defined(ALLOCATE_LOCAL)
#  if defined(ALLOCATE_LOCAL_FALLBACK) && defined(DEALLOCATE_LOCAL_FALLBACK)
#    define ALLOCATE_LOCAL(_size)  ALLOCATE_LOCAL_FALLBACK(_size)
#    define DEALLOCATE_LOCAL(_ptr) DEALLOCATE_LOCAL_FALLBACK(_ptr)
#  else /* no fallbacks supplied; error */
#    define ALLOCATE_LOCAL(_size)  ALLOCATE_LOCAL_FALLBACK undefined!
#    define DEALLOCATE_LOCAL(_ptr) DEALLOCATE_LOCAL_FALLBACK undefined!
#  endif /* defined(ALLOCATE_LOCAL_FALLBACK && DEALLOCATE_LOCAL_FALLBACK) */
#else
#  if !defined(DEALLOCATE_LOCAL)
#    define DEALLOCATE_LOCAL(_ptr) do {} while(0)
#  endif
#endif /* defined(ALLOCATE_LOCAL) */

#endif /* XALLOCA_H */

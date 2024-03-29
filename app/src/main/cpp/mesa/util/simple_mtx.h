#ifndef _SIMPLE_MTX_H
#define _SIMPLE_MTX_H

#include "futex.h"
#include "macros.h"

#include "../include/c11/threads.h"

#if UTIL_FUTEX_SUPPORTED

#if defined(HAVE_VALGRIND) && !defined(NDEBUG)
#  include <valgrind.h>
#  include <helgrind.h>
#  define HG(x) x
#else
#  define HG(x)
#endif

/* mtx_t - Fast, simple mutex
 *
 * While modern pthread mutexes are very fast (implemented using futex), they
 * still incur a call to an external DSO and overhead of the generality and
 * features of pthread mutexes.  Most mutexes in mesa only needs lock/unlock,
 * and the idea here is that we can inline the atomic operation and make the
 * fast case just two intructions.  Mutexes are subtle and finicky to
 * implement, so we carefully copy the implementation from Ulrich Dreppers
 * well-written and well-reviewed paper:
 *
 *   "Futexes Are Tricky"
 *   http://www.akkadia.org/drepper/futex.pdf
 *
 * We implement "mutex3", which gives us a mutex that has no syscalls on
 * uncontended lock or unlock.  Further, the uncontended case boils down to a
 * locked cmpxchg and an untaken branch, the uncontended unlock is just a
 * locked decr and an untaken branch.  We use __builtin_expect() to indicate
 * that contention is unlikely so that gcc will put the contention code out of
 * the main code flow.
 *
 * A fast mutex only supports lock/unlock, can't be recursive or used with
 * condition variables.
 */

typedef struct {
   uint32_t val;
} simple_mtx_t;

#define _SIMPLE_MTX_INITIALIZER_NP { 0 }

#define _SIMPLE_MTX_INVALID_VALUE 0xd0d0d0d0

static inline void
simple_mtx_init(simple_mtx_t *mtx, ASSERTED int type)
{
   assert(type == mtx_plain);

   mtx->val = 0;

   HG(ANNOTATE_RWLOCK_CREATE(mtx));
}

static inline void
simple_mtx_destroy(ASSERTED simple_mtx_t *mtx)
{
   HG(ANNOTATE_RWLOCK_DESTROY(mtx));
#ifndef NDEBUG
   mtx->val = _SIMPLE_MTX_INVALID_VALUE;
#endif
}

static inline void
simple_mtx_lock(simple_mtx_t *mtx)
{
   uint32_t c;

   c = __sync_val_compare_and_swap(&mtx->val, 0, 1);

   assert(c != _SIMPLE_MTX_INVALID_VALUE);

   if (__builtin_expect(c != 0, 0)) {
      if (c != 2)
         c = __sync_lock_test_and_set(&mtx->val, 2);
      while (c != 0) {
         futex_wait(&mtx->val, 2, NULL);
         c = __sync_lock_test_and_set(&mtx->val, 2);
      }
   }

   HG(ANNOTATE_RWLOCK_ACQUIRED(mtx, 1));
}

static inline void
simple_mtx_unlock(simple_mtx_t *mtx)
{
   uint32_t c;

   HG(ANNOTATE_RWLOCK_RELEASED(mtx, 1));

   c = __sync_fetch_and_sub(&mtx->val, 1);

   assert(c != _SIMPLE_MTX_INVALID_VALUE);

   if (__builtin_expect(c != 1, 0)) {
      mtx->val = 0;
      futex_wake(&mtx->val, 1);
   }
}

static inline void
simple_mtx_assert_locked(simple_mtx_t *mtx)
{
   assert(mtx->val);
}

#else

typedef mtx_t simple_mtx_t;

#define _SIMPLE_MTX_INITIALIZER_NP _MTX_INITIALIZER_NP

static inline void
simple_mtx_init(simple_mtx_t *mtx, int type)
{
    mtx_init(mtx, type);
}

static inline void
simple_mtx_destroy(simple_mtx_t *mtx)
{
    mtx_destroy(mtx);
}

static inline void
simple_mtx_lock(simple_mtx_t *mtx)
{
    mtx_lock(mtx);
}

static inline void
simple_mtx_unlock(simple_mtx_t *mtx)
{
    mtx_unlock(mtx);
}

static inline void
simple_mtx_assert_locked(simple_mtx_t *mtx)
{
#ifdef DEBUG
    /* NOTE: this would not work for recursive mutexes, but
    * mtx_t doesn't support those
    */
   int ret = mtx_trylock(mtx);
   assert(ret == thrd_busy);
   if (ret == thrd_success)
      mtx_unlock(mtx);
#else
    (void)mtx;
#endif
}

#endif

#endif

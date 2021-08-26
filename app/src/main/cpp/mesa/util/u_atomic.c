#if defined(MISSING_64BIT_ATOMICS) && defined(HAVE_PTHREAD)

#include <stdint.h>
#include <pthread.h>

#if defined(HAVE_FUNC_ATTRIBUTE_WEAK) && !defined(__CYGWIN__)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif

static pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;

WEAK uint64_t
__sync_add_and_fetch_8(uint64_t *ptr, uint64_t val)
{
   uint64_t r;

   pthread_mutex_lock(&sync_mutex);
   *ptr += val;
   r = *ptr;
   pthread_mutex_unlock(&sync_mutex);

   return r;
}

WEAK uint64_t
__sync_sub_and_fetch_8(uint64_t *ptr, uint64_t val)
{
   uint64_t r;

   pthread_mutex_lock(&sync_mutex);
   *ptr -= val;
   r = *ptr;
   pthread_mutex_unlock(&sync_mutex);

   return r;
}

WEAK uint64_t
__sync_val_compare_and_swap_8(uint64_t *ptr, uint64_t oldval, uint64_t newval)
{
   uint64_t r;

   pthread_mutex_lock(&sync_mutex);
   r = *ptr;
   if (*ptr == oldval)
      *ptr = newval;
   pthread_mutex_unlock(&sync_mutex);

   return r;
}

#endif

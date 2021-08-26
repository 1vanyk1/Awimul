#ifndef _OS_MEMORY_H_
//#error "Must not be included directly. Include os_memory.h instead"
#endif


/**
 * Add two size_t values with integer overflow check.
 * TODO: leverage __builtin_add_overflow where available
 */
static inline bool
add_overflow_size_t(size_t a, size_t b, size_t *res)
{
    *res = a + b;
    return *res < a || *res < b;
}


#if defined(HAVE_POSIX_MEMALIGN)

static inline void *
os_malloc_aligned(size_t size, size_t alignment)
{
   void *ptr;
   alignment = (alignment + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
   if(posix_memalign(&ptr, alignment, size) != 0)
      return NULL;
   return ptr;
}

#define os_free_aligned(_ptr) free(_ptr)

#else

/**
 * Return memory on given byte alignment
 */
static inline void *
os_malloc_aligned(size_t size, size_t alignment)
{
    char *ptr, *buf;
    size_t alloc_size;

    /*
     * Calculate
     *
     *   alloc_size = size + alignment + sizeof(void *)
     *
     * while checking for overflow.
     */
    if (add_overflow_size_t(size, alignment, &alloc_size) ||
        add_overflow_size_t(alloc_size, sizeof(void *), &alloc_size)) {
        return NULL;
    }

    ptr = (char *) os_malloc(alloc_size);
    if (!ptr)
        return NULL;

    buf = (char *)(((uintptr_t)ptr + sizeof(void *) + alignment - 1) & ~((uintptr_t)(alignment - 1)));
    *(char **)(buf - sizeof(void *)) = ptr;

    return buf;
}


/**
 * Free memory returned by os_malloc_aligned().
 */
static inline void
os_free_aligned(void *ptr)
{
    if (ptr) {
        void **cubbyHole = (void **) ((char *) ptr - sizeof(void *));
        void *realAddr = *cubbyHole;
        os_free(realAddr);
    }
}

#endif

/**
 * Reallocate memeory, with alignment
 */
static inline void *
os_realloc_aligned(void *ptr, size_t oldsize, size_t newsize, size_t alignment)
{
    const size_t copySize = MIN2(oldsize, newsize);
    void *newBuf = os_malloc_aligned(newsize, alignment);
    if (newBuf && ptr && copySize > 0) {
        memcpy(newBuf, ptr, copySize);
    }

    os_free_aligned(ptr);
    return newBuf;
}

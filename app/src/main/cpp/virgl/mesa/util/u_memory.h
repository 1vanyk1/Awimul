#ifndef U_MEMORY_H
#define U_MEMORY_H

#include "u_debug.h"
#include "os_memory.h"


#ifdef __cplusplus
extern "C" {
#endif


#define MALLOC(_size)  os_malloc(_size)

#define CALLOC(_count, _size) os_calloc(_count, _size)

#define FREE(_ptr ) os_free(_ptr)

#define REALLOC(_ptr, _old_size, _size) os_realloc(_ptr, _old_size, _size)

#define MALLOC_STRUCT(T)   (struct T *) MALLOC(sizeof(struct T))

#define CALLOC_STRUCT(T)   (struct T *) CALLOC(1, sizeof(struct T))

#define CALLOC_VARIANT_LENGTH_STRUCT(T,more_size)   ((struct T *) CALLOC(1, sizeof(struct T) + more_size))


#define align_malloc(_size, _alignment) os_malloc_aligned(_size, _alignment)
#define align_free(_ptr) os_free_aligned(_ptr)
#define align_realloc(_ptr, _oldsize, _newsize, _alignment) os_realloc_aligned(_ptr, _oldsize, _newsize, _alignment)

static inline void *
align_calloc(size_t size, unsigned long alignment)
{
    void *ptr = align_malloc(size, alignment);
    if (ptr)
        memset(ptr, 0, size);
    return ptr;
}

/**
 * Duplicate a block of memory.
 */
static inline void *
mem_dup(const void *src, size_t size)
{
    void *dup = MALLOC(size);
    if (dup)
        memcpy(dup, src, size);
    return dup;
}


/**
 * Offset of a field in a struct, in bytes.
 */
#define Offset(TYPE, MEMBER) ((uintptr_t)&(((TYPE *)NULL)->MEMBER))



#ifdef __cplusplus
}
#endif


#endif /* U_MEMORY_H */

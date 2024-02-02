#ifndef _OS_MEMORY_H_
#define _OS_MEMORY_H_

#if defined(EMBEDDED_DEVICE)

#ifdef __cplusplus
extern "C" {
#endif

void *
os_malloc(size_t size);

void *
os_calloc(size_t count, size_t size);

void
os_free(void *ptr);

void *
os_realloc(void *ptr, size_t old_size, size_t new_size);

void *
os_malloc_aligned(size_t size, size_t alignment);

void
os_free_aligned(void *ptr);

void *
os_realloc_aligned(void *ptr, size_t oldsize, size_t newsize, size_t alignemnt);

#ifdef __cplusplus
}
#endif

#else

#  include "os_memory_stdc.h"

#endif

#endif /* _OS_MEMORY_H_ */

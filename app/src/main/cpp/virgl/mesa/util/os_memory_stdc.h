#ifndef _OS_MEMORY_H_
#error "Must not be included directly. Include os_memory.h instead"
#endif

#include <stdlib.h>


#define os_malloc(_size)  malloc(_size)
#define os_calloc(_count, _size )  calloc(_count, _size )
#define os_free(_ptr)  free(_ptr)

#define os_realloc( _old_ptr, _old_size, _new_size) \
   realloc(_old_ptr, _new_size + 0*(_old_size))

#if DETECT_OS_WINDOWS

#include <malloc.h>

#define os_malloc_aligned(_size, _align) _aligned_malloc(_size, _align)
#define os_free_aligned(_ptr) _aligned_free(_ptr)
#define os_realloc_aligned(_ptr, _oldsize, _newsize, _alignment) _aligned_realloc(_ptr, _newsize, _alignment)

#else

#include "os_memory_aligned.h"

#endif

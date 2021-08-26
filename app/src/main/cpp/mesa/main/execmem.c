#include <stdio.h>
#include "glheader.h"
#include "execmem.h"
#include "../include/c11/threads.h"


#if defined(__linux__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__sun) || defined(__HAIKU__)

/*
 * Allocate a large block of memory which can hold code then dole it out
 * in pieces by means of the generic memory manager code.
*/

#include <unistd.h>
#include <sys/mman.h>
#include "../util/u_mm.h"

#ifdef MESA_SELINUX
#include <selinux/selinux.h>
#endif


#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif


#define EXEC_HEAP_SIZE (10*1024*1024)

static mtx_t exec_mutex = _MTX_INITIALIZER_NP;

static struct mem_block *exec_heap = NULL;
static unsigned char *exec_mem = NULL;


static int
init_heap(void)
{
#ifdef MESA_SELINUX
   if (is_selinux_enabled()) {
      if (!security_get_boolean_active("allow_execmem") ||
	  !security_get_boolean_pending("allow_execmem"))
         return 0;
   }
#endif

   if (!exec_heap)
      exec_heap = u_mmInit( 0, EXEC_HEAP_SIZE );

   if (!exec_mem)
      exec_mem = mmap(NULL, EXEC_HEAP_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

   return (exec_mem != MAP_FAILED);
}


void *
_mesa_exec_malloc(unsigned size)
{
   struct mem_block *block = NULL;
   void *addr = NULL;

   mtx_lock(&exec_mutex);

   if (!init_heap())
      goto bail;

   if (exec_heap) {
      size = (size + 31) & ~31;
      block = u_mmAllocMem(exec_heap, size, 5, 0);
   }

   if (block)
      addr = exec_mem + block->ofs;
   else
      printf("_mesa_exec_malloc failed\n");

bail:
   mtx_unlock(&exec_mutex);

   return addr;
}


void
_mesa_exec_free(void *addr)
{
   mtx_lock(&exec_mutex);

   if (exec_heap) {
      struct mem_block *block = u_mmFindBlock(exec_heap, (unsigned char *)addr - exec_mem);

      if (block)
	 u_mmFreeMem(block);
   }

   mtx_unlock(&exec_mutex);
}


#else

/*
 * Just use regular memory.
 */

void *
_mesa_exec_malloc(unsigned size)
{
    return malloc( size );
}


void
_mesa_exec_free(void *addr)
{
    free(addr);
}


#endif

#ifndef EXECMEM_H
#define EXECMEM_H

#include "glheader.h"

extern void *
_mesa_exec_malloc(unsigned size);

extern void
_mesa_exec_free(void *addr);


#endif /* EXECMEM_H */

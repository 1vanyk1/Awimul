#ifndef _BITS_IPCTYPES_H
#define _BITS_IPCTYPES_H	1

#include "types.h"

/* Used in `struct shmid_ds'.  */
# if __WORDSIZE == 32
typedef unsigned short int __ipc_pid_t;
# else
typedef int __ipc_pid_t;
# endif


#endif /* bits/ipctypes.h */
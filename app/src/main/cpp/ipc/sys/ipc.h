#ifndef _SYS_IPC_H
#define _SYS_IPC_H	1

#include "../features.h"

#if !defined __USE_SVID && !defined __USE_XOPEN && __GNUC__ >= 2
# warning "Files using this header must be compiled with _SVID_SOURCE or _XOPEN_SOURCE"
#endif

/* Get system dependent definition of `struct ipc_perm' and more.  */
#include "../bits/ipctypes.h"
#include "../bits/ipc.h"

#ifndef __uid_t_defined
typedef __uid_t uid_t;
# define __uid_t_defined
#endif

#ifndef __gid_t_defined
typedef __gid_t gid_t;
# define __gid_t_defined
#endif

#ifndef __mode_t_defined
typedef __mode_t mode_t;
# define __mode_t_defined
#endif

#ifndef __key_t_defined
typedef __key_t key_t;
# define __key_t_defined
#endif

__BEGIN_DECLS

/* Generates key for System V style IPC.  */
extern key_t ftok (__const char *__pathname, int __proj_id);

__END_DECLS

#endif /* sys/ipc.h */
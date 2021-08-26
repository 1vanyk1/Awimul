#ifndef _SYS_IPC_H
//# error "Never use <bits/ipc.h> directly; include <sys/ipc.h> instead."
#endif

#include "types.h"

/* Mode bits for `msgget', `semget', and `shmget'.  */
#define IPC_CREAT	01000		/* Create key if key does not exist. */
#define IPC_EXCL	02000		/* Fail if key exists.  */
#define IPC_NOWAIT	04000		/* Return error on wait.  */

/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#define IPC_RMID	0		/* Remove identifier.  */
#define IPC_SET		1		/* Set `ipc_perm' options.  */
#define IPC_STAT	2		/* Get `ipc_perm' options.  */
#ifdef __USE_GNU
# define IPC_INFO	3		/* See ipcs.  */
#endif

/* Special key values.  */
#define IPC_PRIVATE	((__key_t) 0)	/* Private key.  */


/* Data structure used to pass permission information to IPC operations.  */
//struct ipc_perm
//{
//    __key_t __key;			/* Key.  */
//    __uid_t uid;			/* Owner's user ID.  */
//    __gid_t gid;			/* Owner's group ID.  */
//    __uid_t cuid;			/* Creator's user ID.  */
//    __gid_t cgid;			/* Creator's group ID.  */
//    unsigned short int mode;		/* Read/write permission.  */
//    unsigned short int __pad1;
//    unsigned short int __seq;		/* Sequence number.  */
//    unsigned short int __pad2;
//    unsigned long int __unused1;
//    unsigned long int __unused2;
//};
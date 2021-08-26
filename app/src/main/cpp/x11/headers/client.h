#ifndef CLIENT_H
#define CLIENT_H

#ifdef HAVE_DIX_CONFIG_H
#include "dix-config.h"
#endif                          /* HAVE_DIX_CONFIG_H */
#include "xfuncproto.h"
#include <sys/types.h>
#include "../../impl/sys/shm.h"

/* Client IDs. Use GetClientPid, GetClientCmdName and GetClientCmdArgs
 * instead of accessing the fields directly. */
typedef struct {
    pid_t pid;                  /* process ID, -1 if not available */
    const char *cmdname;        /* process name, NULL if not available */
    const char *cmdargs;        /* process arguments, NULL if not available */
} ClientIdRec, *ClientIdPtr;

struct _Client;

/* Initialize and clean up. */
void ReserveClientIds(struct _Client *client);
void ReleaseClientIds(struct _Client *client);

/* Determine client IDs for caching. Exported on purpose for
 * extensions such as SELinux. */
extern _X_EXPORT pid_t DetermineClientPid(struct _Client *client);
extern _X_EXPORT void DetermineClientCmd(pid_t, const char **cmdname,
const char **cmdargs);

/* Query cached client IDs. Exported on purpose for drivers. */
extern _X_EXPORT pid_t GetClientPid(struct _Client *client);
extern _X_EXPORT const char *GetClientCmdName(struct _Client *client);
extern _X_EXPORT const char *GetClientCmdArgs(struct _Client *client);

#endif                          /* CLIENT_H */
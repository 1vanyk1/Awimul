#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../x11/headers/x.h"
#include "../x11/headers/os.h"
#include "osdep.h"
#include "../x11/headers/dixstruct.h"

static struct auth {
    struct auth *next;
    unsigned short len;
    char *data;
    XID id;
} *mit_auth;

int
MitAddCookie(unsigned short data_length, const char *data, XID id)
{
    struct auth *new;

    new = malloc(sizeof(struct auth));
    if (!new)
        return 0;
    new->data = malloc((unsigned) data_length);
    if (!new->data) {
        free(new);
        return 0;
    }
    new->next = mit_auth;
    mit_auth = new;
    memmove(new->data, data, (int) data_length);
    new->len = data_length;
    new->id = id;
    return 1;
}

XID
MitCheckCookie(unsigned short data_length,
               const char *data, ClientPtr client, const char **reason)
{
    struct auth *auth;

    for (auth = mit_auth; auth; auth = auth->next) {
        if (data_length == auth->len &&
            timingsafe_memcmp(data, auth->data, (int) data_length) == 0)
            return auth->id;
    }
    *reason = "Invalid MIT-MAGIC-COOKIE-1 key";
    return (XID) -1;
}

int
MitResetCookie(void)
{
    struct auth *auth, *next;

    for (auth = mit_auth; auth; auth = next) {
        next = auth->next;
        free(auth->data);
        free(auth);
    }
    mit_auth = 0;
    return 0;
}

int
MitFromID(XID id, unsigned short *data_lenp, char **datap)
{
    struct auth *auth;

    for (auth = mit_auth; auth; auth = auth->next) {
        if (id == auth->id) {
            *data_lenp = auth->len;
            *datap = auth->data;
            return 1;
        }
    }
    return 0;
}

int
MitRemoveCookie(unsigned short data_length, const char *data)
{
    struct auth *auth, *prev;

    prev = 0;
    for (auth = mit_auth; auth; prev = auth, auth = auth->next) {
        if (data_length == auth->len &&
            memcmp(data, auth->data, data_length) == 0) {
            if (prev)
                prev->next = auth->next;
            else
                mit_auth = auth->next;
            free(auth->data);
            free(auth);
            return 1;
        }
    }
    return 0;
}

static char cookie[16];         /* 128 bits */

XID
MitGenerateCookie(unsigned data_length,
                  const char *data,
                  XID id, unsigned *data_length_return, char **data_return)
{
    int i = 0;
    int status;

    while (data_length--) {
        cookie[i++] += *data++;
        if (i >= sizeof(cookie))
            i = 0;
    }
    GenerateRandomData(sizeof(cookie), cookie);
    status = MitAddCookie(sizeof(cookie), cookie, id);
    if (!status) {
        id = -1;
    }
    else {
        *data_return = cookie;
        *data_length_return = sizeof(cookie);
    }
    return id;
}

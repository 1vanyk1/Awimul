#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xauth.h"
#include <stdlib.h>

void
XauDisposeAuth (Xauth *auth)
{
    if (auth) {
        free (auth->address);
        free (auth->number);
        free (auth->name);
        if (auth->data) {
            (void) bzero (auth->data, auth->data_length);
            (void) free (auth->data);
        }
        free ((char *) auth);
    }
    return;
}

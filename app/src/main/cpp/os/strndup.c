#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../x11/headers/os.h"

char *
strndup(const char *str, size_t n)
{
    size_t len;
    char *copy;

    for (len = 0; len < n && str[len]; len++)
        continue;

    if ((copy = malloc(len + 1)) == NULL)
        return (NULL);
    memcpy(copy, str, len);
    copy[len] = '\0';
    return (copy);
}

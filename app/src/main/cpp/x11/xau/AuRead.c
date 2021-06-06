#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xauth.h"
#include <stdlib.h>

static int
read_short (unsigned short *shortp, FILE *file)
{
    unsigned char   file_short[2];

    if (fread ((char *) file_short, sizeof (file_short), 1, file) != 1)
        return 0;
    *shortp = file_short[0] * 256 + file_short[1];
    return 1;
}

static int
read_counted_string (unsigned short *countp, char **stringp, FILE *file)
{
    unsigned short  len;
    char	    *data;

    if (read_short (&len, file) == 0)
        return 0;
    if (len == 0) {
        data = NULL;
    } else {
        data = malloc ((unsigned) len);
        if (!data)
            return 0;
        if (fread (data, sizeof (char), len, file) != len) {
            bzero (data, len);
            free (data);
            return 0;
        }
    }
    *stringp = data;
    *countp = len;
    return 1;
}

Xauth *
XauReadAuth (FILE *auth_file)
{
    Xauth   local;
    Xauth   *ret;

    if (read_short (&local.family, auth_file) == 0)
        return NULL;
    if (read_counted_string (&local.address_length, &local.address, auth_file) == 0)
        return NULL;
    if (read_counted_string (&local.number_length, &local.number, auth_file) == 0) {
        free (local.address);
        return NULL;
    }
    if (read_counted_string (&local.name_length, &local.name, auth_file) == 0) {
        free (local.address);
        free (local.number);
        return NULL;
    }
    if (read_counted_string (&local.data_length, &local.data, auth_file) == 0) {
        free (local.address);
        free (local.number);
        free (local.name);
        return NULL;
    }
    ret = (Xauth *) malloc (sizeof (Xauth));
    if (!ret) {
        free (local.address);
        free (local.number);
        free (local.name);
        if (local.data) {
            bzero (local.data, local.data_length);
            free (local.data);
        }
        return NULL;
    }
    *ret = local;
    return ret;
}

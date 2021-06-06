#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

int
_XFreeExtData (XExtData *extension)
{
    XExtData *temp;
    while (extension) {
        if (extension->free_private)
            (*extension->free_private)(extension);
        else Xfree (extension->private_data);
        temp = extension->next;
        Xfree (extension);
        extension = temp;
    }
    return 0;
}
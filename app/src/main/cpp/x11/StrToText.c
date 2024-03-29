#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"
#include "headers/xutil.h"

/*
 * XStringListToTextProperty - fill in TextProperty structure with
 * concatenated list of null-separated strings.  Return True if successful
 * else False.  Allocate room on end for trailing NULL, but don't include in
 * count.
 */

Status XStringListToTextProperty (
        char **argv,
        int argc,
        XTextProperty *textprop)
{
    register int i;
    register unsigned int nbytes;
    XTextProperty proto;

    /* figure out how much space we'll need for this list */
    for (i = 0, nbytes = 0; i < argc; i++) {
        nbytes += (unsigned) ((argv[i] ? strlen (argv[i]) : 0) + 1);
    }

    /* fill in a prototype containing results so far */
    proto.encoding = XA_STRING;
    proto.format = 8;
    if (nbytes)
        proto.nitems = nbytes - 1;	/* subtract one for trailing <NUL> */
    else
        proto.nitems = 0;
    proto.value = NULL;

    /* build concatenated list of strings */
    if (nbytes > 0) {
        register char *buf = Xmalloc (nbytes);
        if (!buf) return False;

        proto.value = (unsigned char *) buf;
        for (i = 0; i < argc; i++) {
            char *arg = argv[i];

            if (arg) {
                (void) strcpy (buf, arg);
                buf += (strlen (arg) + 1);
            } else {
                *buf++ = '\0';
            }
        }
    } else {
        proto.value = Xmalloc (1);		/* easier for client */
        if (!proto.value) return False;

        proto.value[0] = '\0';
    }

    /* we were successful, so set return value */
    *textprop = proto;
    return True;
}
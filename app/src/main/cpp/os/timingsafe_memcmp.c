//#include <dix-config.h>
#include <limits.h>
#include <string.h>
#include "../x11/headers/xfuncproto.h"
#include "../x11/headers/os.h"

int
timingsafe_memcmp(const void *b1, const void *b2, size_t len)
{
    const unsigned char *p1 = b1, *p2 = b2;
    size_t i;
    int res = 0, done = 0;

    for (i = 0; i < len; i++) {
        /* lt is -1 if p1[i] < p2[i]; else 0. */
        int lt = (p1[i] - p2[i]) >> CHAR_BIT;

        /* gt is -1 if p1[i] > p2[i]; else 0. */
        int gt = (p2[i] - p1[i]) >> CHAR_BIT;

        /* cmp is 1 if p1[i] > p2[i]; -1 if p1[i] < p2[i]; else 0. */
        int cmp = lt - gt;

        /* set res = cmp if !done. */
        res |= cmp & ~done;

        /* set done if p1[i] != p2[i]. */
        done |= lt | gt;
    }

    return (res);
}

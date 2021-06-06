#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xatom.h"
#include "headers/xutil.h"
#include "reallocarray.h"

/*
 * XTextPropertyToStringList - set list and count to contain data stored in
 * null-separated STRING property.
 */

Status XTextPropertyToStringList (
        XTextProperty *tp,
        char ***list_return,
        int *count_return)
{
    char **list;			/* return value */
    int nelements;			/* return value */
    register char *cp;			/* temp variable */
    char *start;			/* start of thing to copy */
    int i, j;				/* iterator variables */
    int datalen = (int) tp->nitems;	/* for convenience */

    /*
     * make sure we understand how to do it
     */
    if (tp->encoding != XA_STRING ||  tp->format != 8) return False;

    if (datalen == 0) {
        *list_return = NULL;
        *count_return = 0;
        return True;
    }

    /*
     * walk the list to figure out how many elements there are
     */
    nelements = 1;			/* since null-separated */
    for (cp = (char *) tp->value, i = datalen; i > 0; cp++, i--) {
        if (*cp == '\0') nelements++;
    }

    /*
     * allocate list and duplicate
     */
    list = Xmallocarray (nelements, sizeof (char *));
    if (!list) return False;

    start = Xmalloc ((datalen + 1) * sizeof (char));	/* for <NUL> */
    if (!start) {
        Xfree (list);
        return False;
    }

    /*
     * copy data
     */
    memcpy (start, (char *) tp->value, tp->nitems);
    start[datalen] = '\0';

    /*
     * walk down list setting value
     */
    for (cp = start, i = datalen + 1, j = 0; i > 0; cp++, i--) {
        if (*cp == '\0') {
            list[j] = start;
            start = (cp + 1);
            j++;
        }
    }

    /*
     * append final null pointer and then return data
     */
    *list_return = list;
    *count_return = nelements;
    return True;
}


void XFreeStringList (char **list)
{
    if (list) {
        if (list[0]) Xfree (list[0]);
        Xfree (list);
        list = NULL;
    }
}
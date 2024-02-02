#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include        "IntrinsicI.h"
#include        <stdio.h>

/*
 * This routine merges two arglists. It does NOT check for duplicate entries.
 */

ArgList
XtMergeArgLists(ArgList args1,
                Cardinal num_args1,
                ArgList args2,
                Cardinal num_args2)
{
    ArgList result, args;

    result = (ArgList) __XtCalloc((unsigned) num_args1 + num_args2,
                                  (unsigned) sizeof(Arg));

    for (args = result; num_args1 != 0; num_args1--)
        *args++ = *args1++;
    for (; num_args2 != 0; num_args2--)
        *args++ = *args2++;

    return result;
}

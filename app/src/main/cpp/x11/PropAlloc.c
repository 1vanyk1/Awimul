#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"
#include "headers/xutil.h"
#include <stdio.h>


/*
 * Routines for allocating space for structures that are expected to get
 * longer at some point.
 */

XSizeHints *XAllocSizeHints (void)
{
    return Xcalloc (1, sizeof (XSizeHints));
}


XStandardColormap *XAllocStandardColormap (void)
{
    return Xcalloc (1, sizeof (XStandardColormap));
}


XWMHints *XAllocWMHints (void)
{
    return Xcalloc (1, sizeof (XWMHints));
}


XClassHint *XAllocClassHint (void)
{
    return Xcalloc (1, sizeof (XClassHint));
}


XIconSize *XAllocIconSize (void)
{
    return Xcalloc (1, sizeof (XIconSize));
}


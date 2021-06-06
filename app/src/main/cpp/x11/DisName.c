#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "headers/xlib.h"

char *
XDisplayName(
        _Xconst char *display)
{
    char *d;
    if ( display != (char *)NULL && *display != '\0' )
        return( (char *)display );
    if ( (d = getenv( "DISPLAY" )) != (char *)NULL )
        return( d );
    return( (char *) "" );
}
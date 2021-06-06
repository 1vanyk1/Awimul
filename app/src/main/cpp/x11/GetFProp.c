#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "headers/xlibint.h"

Bool
XGetFontProperty(
        XFontStruct *fs,
        register Atom name,
        unsigned long *valuePtr)
{
    /* XXX this is a simple linear search for now.  If the
       protocol is changed to sort the property list, this should
       become a binary search. */
    register XFontProp *prop = fs->properties;
    register XFontProp *last = prop + fs->n_properties;

    while (prop != last) {
        if (prop->name == name) {
            *valuePtr = prop->card32;
            return (1);
        }
        prop++;
    }
    return (0);
}


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../headers/xlibint.h"
#include "xlcint.h"

/*
 * Compile the resource name. (resource_name ---> xrm_name)
 */
void
_XIMCompileResourceList(XIMResourceList res, unsigned int num_res)
{
    register unsigned int count;

    for (count = 0; count < num_res; res++, count++) {
        res->xrm_name = XrmStringToQuark(res->resource_name);
    }
}

void
_XCopyToArg(XPointer src, XPointer *dst, unsigned int size)
{
    if (!*dst) {
        union {
            long	longval;
#ifdef LONG64
            int		intval;
#endif
            short	shortval;
            char	charval;
            char*	charptr;
            XPointer	ptr;
        } u;
        if (size <= sizeof(XPointer)) {
            memcpy((char *)&u, (char *)src, (int)size);
            if (size == sizeof(long))	       *dst = (XPointer)u.longval;
#ifdef LONG64
                else if (size == sizeof(int))      *dst = (XPointer)(long)u.intval;
#endif
            else if (size == sizeof(short))    *dst = (XPointer)(long)u.shortval;
            else if (size == sizeof(char))     *dst = (XPointer)(long)u.charval;
            else if (size == sizeof(char*))    *dst = (XPointer)u.charptr;
            else if (size == sizeof(XPointer)) *dst = (XPointer)u.ptr;
            else memcpy( (char*)dst, (char*)src, (int)size );
        } else {
            memcpy( (char*)dst, (char*)src, (int)size );
        }
    } else {
        memcpy( (char*)*dst, (char*)src, (int)size );
    }
}

/*
 * Connects to an input method matching current locale specification, creates
 * a XIM object and return a pointer the newly created XIM back to the caller.
 */

XIM
XOpenIM(Display *display, XrmDatabase rdb, char *res_name, char *res_class)
{
    XLCd	lcd = _XOpenLC( (char *)NULL );

    if( !lcd )
        return( (XIM)NULL );
    return (*lcd->methods->open_im) (lcd, display, rdb, res_name, res_class);
}

/*
 * Close the connection to the input manager, and free the XIM structure
 */
Status
XCloseIM(XIM im)
{
    Status s;
    XIC ic;
    XLCd lcd = im->core.lcd;

    s = (im->methods->close) (im);
    for (ic = im->core.ic_chain; ic; ic = ic->core.next)
        ic->core.im = (XIM)NULL;
    Xfree (im);
    _XCloseLC (lcd);
    return (s);
}

/*
 * Return the Display associated with the input method.
 */
Display *
XDisplayOfIM(XIM im)
{
    return im->core.display;
}

/*
 * Return the Locale associated with the input method.
 */
char *
XLocaleOfIM(XIM im)
{
    return im->core.lcd->core->name;
}

/*
 * Register to a input method instantiation callback to prepare the
 * on-demand input method instantiation.
 */
Bool
XRegisterIMInstantiateCallback(
        Display	*display,
        XrmDatabase	 rdb,
        char	*res_name,
        char	*res_class,
        XIDProc	 callback,
        XPointer	 client_data)
{
    XLCd	lcd = _XOpenLC( (char *)NULL );

    if( !lcd )
        return( False );
    return( (*lcd->methods->register_callback)( lcd, display, rdb, res_name,
                                                res_class, callback,
                                                client_data ) );
}

/*
 * Unregister to a input method instantiation callback.
 */
Bool
XUnregisterIMInstantiateCallback(
        Display	*display,
        XrmDatabase	 rdb,
        char	*res_name,
        char	*res_class,
        XIDProc	 callback,
        XPointer	 client_data)
{
    XLCd	lcd = _XlcCurrentLC();

    if( !lcd )
        return( False );
    if( lcd->methods->unregister_callback == NULL )
        return( False );
    return( (*lcd->methods->unregister_callback)( lcd, display, rdb, res_name,
                                                  res_class, callback,
                                                  client_data ) );
}
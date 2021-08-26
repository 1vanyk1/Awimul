#ifndef XM_PUBLIC_H
#define XM_PUBLIC_H

#include "../../../x11/headers/xlib.h"

struct pipe_screen;
struct st_api;

/* This is the driver interface required by the glx/xlib frontends.
 */
struct xm_driver {
    struct pipe_screen *(*create_pipe_screen)( Display *display );
    struct st_api *(*create_st_api)( void );
};

extern void
xmesa_set_driver( const struct xm_driver *driver );


#endif /* XM_PUBLIC_H */

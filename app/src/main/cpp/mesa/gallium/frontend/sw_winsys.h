#ifndef SW_WINSYS_H
#define SW_WINSYS_H


#include "../../include/pipe/p_format.h"
#include "winsys_handle.h"

#ifdef __cplusplus
extern "C" {
#endif


struct pipe_screen;
struct pipe_context;
struct pipe_resource;
struct pipe_box;

/**
 * Opaque pointer.
 */
struct sw_displaytarget;


/**
 * This is the interface that sw expects any window system
 * hosting it to implement.
 *
 * sw is for the most part a self sufficient driver. The only thing it
 * does not know is how to display a surface.
 */
struct sw_winsys
{
    void
    (*destroy)( struct sw_winsys *ws );

    bool
    (*is_displaytarget_format_supported)( struct sw_winsys *ws,
                                          unsigned tex_usage,
                                          enum pipe_format format );

    /**
     * Allocate storage for a render target.
     *
     * Often surfaces which are meant to be blitted to the front screen (i.e.,
     * display targets) must be allocated with special characteristics, memory
     * pools, or obtained directly from the windowing system.
     *
     * This callback is invoked by the pipe_screen when creating a texture marked
     * with the PIPE_BIND_DISPLAY_TARGET flag to get the underlying
     * storage.
     */
    struct sw_displaytarget *
    (*displaytarget_create)( struct sw_winsys *ws,
                             unsigned tex_usage,
                             enum pipe_format format,
                             unsigned width, unsigned height,
                             unsigned alignment,
                             const void *front_private,
                             unsigned *stride );

    /**
     * Used to implement texture_from_handle.
     */
    struct sw_displaytarget *
    (*displaytarget_from_handle)( struct sw_winsys *ws,
                                  const struct pipe_resource *templat,
                                  struct winsys_handle *whandle,
                                  unsigned *stride );

    /**
     * Used to implement texture_get_handle.
     */
    bool
    (*displaytarget_get_handle)( struct sw_winsys *ws,
                                 struct sw_displaytarget *dt,
                                 struct winsys_handle *whandle );

    /**
     * \param flags  bitmask of PIPE_MAP_x flags
     */
    void *
    (*displaytarget_map)( struct sw_winsys *ws,
                          struct sw_displaytarget *dt,
                          unsigned flags );

    void
    (*displaytarget_unmap)( struct sw_winsys *ws,
                            struct sw_displaytarget *dt );

    /**
     * @sa pipe_screen:flush_frontbuffer.
     *
     * This call will likely become asynchronous eventually.
     */
    void
    (*displaytarget_display)( struct sw_winsys *ws,
                              struct sw_displaytarget *dt,
                              void *context_private,
                              struct pipe_box *box );

    void
    (*displaytarget_destroy)( struct sw_winsys *ws,
                              struct sw_displaytarget *dt );
};



#ifdef __cplusplus
}
#endif

#endif /* SW_WINSYS_H */

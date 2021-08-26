#ifndef XMESA_H
#define XMESA_H

#include "../../../x11/headers/xlib.h"
#include "../../../x11/headers/xutil.h"
#include "xmesa_x.h"
#include "../../../GL/gl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMESA_MAJOR_VERSION 6
#define XMESA_MINOR_VERSION 3



/*
 * Values passed to XMesaGetString:
 */
#define XMESA_VERSION 1
#define XMESA_EXTENSIONS 2


typedef struct xmesa_context *XMesaContext;

typedef struct xmesa_visual *XMesaVisual;

typedef struct xmesa_buffer *XMesaBuffer;



/*
 * Create a new X/Mesa visual.
 * Input:  display - X11 display
 *         visinfo - an XVisualInfo pointer
 *         rgb_flag - GL_TRUE = RGB mode,
 *                    GL_FALSE = color index mode
 *         alpha_flag - alpha buffer requested?
 *         db_flag - GL_TRUE = double-buffered,
 *                   GL_FALSE = single buffered
 *         stereo_flag - stereo visual?
 *         ximage_flag - GL_TRUE = use an XImage for back buffer,
 *                       GL_FALSE = use an off-screen pixmap for back buffer
 *         depth_size - requested bits/depth values, or zero
 *         stencil_size - requested bits/stencil values, or zero
 *         accum_red_size - requested bits/red accum values, or zero
 *         accum_green_size - requested bits/green accum values, or zero
 *         accum_blue_size - requested bits/blue accum values, or zero
 *         accum_alpha_size - requested bits/alpha accum values, or zero
 *         num_samples - number of samples/pixel if multisampling, or zero
 *         level - visual level, usually 0
 *         visualCaveat - ala the GLX extension, usually GLX_NONE_EXT
 * Return;  a new XMesaVisual or 0 if error.
 */
extern XMesaVisual XMesaCreateVisual( XMesaDisplay *display,
                                      XMesaVisualInfo visinfo,
                                      GLboolean rgb_flag,
                                      GLboolean alpha_flag,
                                      GLboolean db_flag,
                                      GLboolean stereo_flag,
                                      GLboolean ximage_flag,
                                      GLint depth_size,
                                      GLint stencil_size,
                                      GLint accum_red_size,
                                      GLint accum_green_size,
                                      GLint accum_blue_size,
                                      GLint accum_alpha_size,
                                      GLint num_samples,
                                      GLint level,
                                      GLint visualCaveat );

/*
 * Destroy an XMesaVisual, but not the associated XVisualInfo.
 */
extern void XMesaDestroyVisual( XMesaVisual v );



/*
 * Create a new XMesaContext for rendering into an X11 window.
 *
 * Input:  visual - an XMesaVisual
 *         share_list - another XMesaContext with which to share display
 *                      lists or NULL if no sharing is wanted.
 * Return:  an XMesaContext or NULL if error.
 */
extern XMesaContext XMesaCreateContext( XMesaVisual v,
                                        XMesaContext share_list );


/*
 * Destroy a rendering context as returned by XMesaCreateContext()
 */
extern void XMesaDestroyContext( XMesaContext c );




/*
 * Create an XMesaBuffer from an X window.
 */
extern XMesaBuffer XMesaCreateWindowBuffer( XMesaVisual v, XMesaWindow w );


/*
 * Create an XMesaBuffer from an X pixmap.
 */
extern XMesaBuffer XMesaCreatePixmapBuffer( XMesaVisual v,
                                            XMesaPixmap p,
                                            XMesaColormap cmap );


/*
 * Destroy an XMesaBuffer, but not the corresponding window or pixmap.
 */
extern void XMesaDestroyBuffer( XMesaBuffer b );


/*
 * Return the XMesaBuffer handle which corresponds to an X drawable, if any.
 *
 * New in Mesa 2.3.
 */
extern XMesaBuffer XMesaFindBuffer( XMesaDisplay *dpy,
                                    XMesaDrawable d );



/*
 * Bind a buffer to a context and make the context the current one.
 */
extern GLboolean XMesaMakeCurrent( XMesaContext c,
                                   XMesaBuffer b );


/*
 * Bind two buffers (read and draw) to a context and make the
 * context the current one.
 * New in Mesa 3.3
 */
extern GLboolean XMesaMakeCurrent2( XMesaContext c,
                                    XMesaBuffer drawBuffer,
                                    XMesaBuffer readBuffer );


/*
 * Unbind the current context from its buffer.
 */
extern GLboolean XMesaUnbindContext( XMesaContext c );


/*
 * Return a handle to the current context.
 */
extern XMesaContext XMesaGetCurrentContext( void );


/*
 * Return handle to the current (draw) buffer.
 */
extern XMesaBuffer XMesaGetCurrentBuffer( void );


/*
 * Return handle to the current read buffer.
 * New in Mesa 3.3
 */
extern XMesaBuffer XMesaGetCurrentReadBuffer( void );


/*
 * Return display of current context.
 */
extern Display *XMesaGetCurrentDisplay( void );


/*
 * Swap the front and back buffers for the given buffer.  No action is
 * taken if the buffer is not double buffered.
 */
extern void XMesaSwapBuffers( XMesaBuffer b );


/*
 * Copy a sub-region of the back buffer to the front buffer.
 *
 * New in Mesa 2.6
 */
extern void XMesaCopySubBuffer( XMesaBuffer b,
                                int x,
                                int y,
                                int width,
                                int height );


/*
 * Return a pointer to the Pixmap or XImage being used as the back
 * color buffer of an XMesaBuffer.  This function is a way to get "under
 * the hood" of X/Mesa so one can manipulate the back buffer directly.
 * Input:  b - the XMesaBuffer
 * Output:  pixmap - pointer to back buffer's Pixmap, or 0
 *          ximage - pointer to back buffer's XImage, or NULL
 * Return:  GL_TRUE = context is double buffered
 *          GL_FALSE = context is single buffered
 */
extern GLboolean XMesaGetBackBuffer( XMesaBuffer b,
                                     XMesaPixmap *pixmap,
                                     XMesaImage **ximage );



/*
 * Return the depth buffer associated with an XMesaBuffer.
 * Input:  b - the XMesa buffer handle
 * Output:  width, height - size of buffer in pixels
 *          bytesPerValue - bytes per depth value (2 or 4)
 *          buffer - pointer to depth buffer values
 * Return:  GL_TRUE or GL_FALSE to indicate success or failure.
 *
 * New in Mesa 2.4.
 */
extern GLboolean XMesaGetDepthBuffer( XMesaBuffer b,
                                      GLint *width,
                                      GLint *height,
                                      GLint *bytesPerValue,
                                      void **buffer );



/*
 * Flush/sync a context
 */
extern void XMesaFlush( XMesaContext c );



/*
 * Get an X/Mesa-specific string.
 * Input:  name - either XMESA_VERSION or XMESA_EXTENSIONS
 */
extern const char *XMesaGetString( XMesaContext c, int name );



/*
 * Scan for XMesaBuffers whose window/pixmap has been destroyed, then free
 * any memory used by that buffer.
 *
 * New in Mesa 2.3.
 */
extern void XMesaGarbageCollect( XMesaDisplay* dpy );



/*
 * Return a dithered pixel value.
 * Input:  c - XMesaContext
 *         x, y - window coordinate
 *         red, green, blue, alpha - color components in [0,1]
 * Return:  pixel value
 *
 * New in Mesa 2.3.
 */
extern unsigned long XMesaDitherColor( XMesaContext xmesa,
                                       GLint x,
                                       GLint y,
                                       GLfloat red,
                                       GLfloat green,
                                       GLfloat blue,
                                       GLfloat alpha );



/*
 * Reallocate the back/depth/stencil/accum/etc/ buffers associated with
 * buffer <b> if its size has changed.
 *
 * New in Mesa 4.0.2
 */
extern void XMesaResizeBuffers( XMesaBuffer b );



/*
 * Create a pbuffer.
 * New in Mesa 4.1
 */
extern XMesaBuffer XMesaCreatePBuffer(XMesaVisual v, XMesaColormap cmap,
                                      unsigned int width, unsigned int height);



/*
 * Texture from Pixmap
 * New in Mesa 7.1
 */
extern void
XMesaBindTexImage(XMesaDisplay *dpy, XMesaBuffer drawable, int buffer,
                  const int *attrib_list);

extern void
XMesaReleaseTexImage(XMesaDisplay *dpy, XMesaBuffer drawable, int buffer);


extern XMesaBuffer
XMesaCreatePixmapTextureBuffer(XMesaVisual v, XMesaPixmap p,
                               XMesaColormap cmap,
                               int format, int target, int mipmap);



#ifdef __cplusplus
}
#endif


#endif

#ifndef XM_API_H
#define XM_API_H


#include "../../main/mtypes.h" /* for gl_config */
#include "../frontend/api.h"
#include "../../os/os_thread.h"

#include "../frontend/xlibsw_api.h"

# include "../../../x11/headers/xlib.h"
# include "../../../x11/headers/xlibint.h"
# include "../../../x11/headers/xutil.h"

struct hud_context;

typedef struct xmesa_display *XMesaDisplay;
typedef struct xmesa_buffer *XMesaBuffer;
typedef struct xmesa_context *XMesaContext;
typedef struct xmesa_visual *XMesaVisual;


struct xmesa_display {
    mtx_t mutex;

    Display *display;
    struct pipe_screen *screen;
    struct st_manager *smapi;

    struct pipe_context *pipe;
};


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
extern XMesaVisual XMesaCreateVisual( Display *display,
                                      XVisualInfo * visinfo,
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
                                        XMesaContext share_list,
                                        GLuint major, GLuint minor,
                                        GLuint profileMask,
                                        GLuint contextFlags);


/*
 * Destroy a rendering context as returned by XMesaCreateContext()
 */
extern void XMesaDestroyContext( XMesaContext c );



/*
 * Create an XMesaBuffer from an X window.
 */
extern XMesaBuffer XMesaCreateWindowBuffer( XMesaVisual v, Window w );


/*
 * Create an XMesaBuffer from an X pixmap.
 */
extern XMesaBuffer XMesaCreatePixmapBuffer( XMesaVisual v,
                                            Pixmap p,
                                            Colormap cmap );


/*
 * Destroy an XMesaBuffer, but not the corresponding window or pixmap.
 */
extern void XMesaDestroyBuffer( XMesaBuffer b );


/*
 * Return the XMesaBuffer handle which corresponds to an X drawable, if any.
 *
 * New in Mesa 2.3.
 */
extern XMesaBuffer XMesaFindBuffer( Display *dpy,
                                    Drawable d );



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
 * Flush/sync a context
 */
extern void XMesaFlush( XMesaContext c );



/*
 * Scan for XMesaBuffers whose window/pixmap has been destroyed, then free
 * any memory used by that buffer.
 *
 * New in Mesa 2.3.
 */
extern void XMesaGarbageCollect( void );



/*
 * Create a pbuffer.
 * New in Mesa 4.1
 */
extern XMesaBuffer XMesaCreatePBuffer(XMesaVisual v, Colormap cmap,
                                      unsigned int width, unsigned int height);



/*
 * Texture from Pixmap
 * New in Mesa 7.1
 */
extern void
XMesaBindTexImage(Display *dpy, XMesaBuffer drawable, int buffer,
                  const int *attrib_list);

extern void
XMesaReleaseTexImage(Display *dpy, XMesaBuffer drawable, int buffer);


extern XMesaBuffer
XMesaCreatePixmapTextureBuffer(XMesaVisual v, Pixmap p,
                               Colormap cmap,
                               int format, int target, int mipmap);


extern void
XMesaCopyContext(XMesaContext src, XMesaContext dst, unsigned long mask);


/***********************************************************************
 */

/**
 * Visual inforation, derived from GLvisual.
 * Basically corresponds to an XVisualInfo.
 */
struct xmesa_visual {
    struct gl_config mesa_visual;/* Device independent visual parameters */
    int screen, visualID, visualType;
    Display *display;	/* The X11 display */
    XVisualInfo * visinfo;	/* X's visual info (pointer to private copy) */
    XVisualInfo *vishandle;	/* Only used in fakeglx.c */
    GLint BitsPerPixel;		/* True bits per pixel for XImages */

    GLboolean ximage_flag;	/* Use XImage for back buffer (not pixmap)? */

    struct st_visual stvis;
};


/**
 * Context info, derived from st_context.
 * Basically corresponds to a GLXContext.
 */
struct xmesa_context {
    struct st_context_iface *st;
    XMesaVisual xm_visual;	/** pixel format info */
    XMesaBuffer xm_buffer;	/** current drawbuffer */
    XMesaBuffer xm_read_buffer;  /** current readbuffer */
    struct hud_context *hud;
};


/**
 * Types of X/GLX drawables we might render into.
 */
typedef enum {
    WINDOW,          /* An X window */
    GLXWINDOW,       /* GLX window */
    PIXMAP,          /* GLX pixmap */
    PBUFFER          /* GLX Pbuffer */
} BufferType;


/**
 * Framebuffer information, derived from.
 * Basically corresponds to a GLXDrawable.
 */
struct xmesa_buffer {
    struct st_framebuffer_iface *stfb;
    struct xlib_drawable ws;

    GLboolean wasCurrent;	/* was ever the current buffer? */
    XMesaVisual xm_visual;	/* the X/Mesa visual */
    Colormap cmap;		/* the X colormap */
    BufferType type;             /* window, pixmap, pbuffer or glxwindow */

    GLboolean largestPbuffer;    /**< for pbuffers */
    GLboolean preservedContents; /**< for pbuffers */

    XImage *tempImage;
    unsigned long selectedEvents;/* for pbuffers only */


    GC gc;			/* scratch GC for span, line, tri drawing */

    /* GLX_EXT_texture_from_pixmap */
    GLint TextureTarget; /** GLX_TEXTURE_1D_EXT, for example */
    GLint TextureFormat; /** GLX_TEXTURE_FORMAT_RGB_EXT, for example */
    GLint TextureMipmap; /** 0 or 1 */

    struct xmesa_buffer *Next;	/* Linked list pointer: */

    unsigned width, height;
};



extern const char *
xmesa_get_name(void);

extern int
xmesa_init(Display *dpy);

extern XMesaBuffer
xmesa_find_buffer(Display *dpy, Colormap cmap, XMesaBuffer notThis);

extern void
xmesa_get_window_size1(Display *dpy, XMesaBuffer b,
                      GLuint *width, GLuint *height);

extern void
xmesa_notify_invalid_buffer(XMesaBuffer b);

extern void
xmesa_check_buffer_size(XMesaBuffer b);

extern void
xmesa_destroy_buffers_on_display(Display *dpy);

extern void
xmesa_close_display(Display *dpy);

static inline GLuint
xmesa_buffer_width(XMesaBuffer b)
{
    return b->width;
}

static inline GLuint
xmesa_buffer_height(XMesaBuffer b)
{
    return b->height;
}

extern boolean xmesa_strict_invalidate;

#endif

#ifndef _glxapi_h_
#define _glxapi_h_


#define GLX_GLXEXT_PROTOTYPES
#include "../../../GL/glx.h"


/* The GLX API dispatcher (i.e. this code) is being built into stand-alone
 * Mesa.  We don't know anything about XFree86 or real GLX so we define a
 * minimal __GLXContextRec here so some of the functions in this file can
 * work properly.
 */
typedef struct __GLXcontextRec {
    GLboolean isDirect;
    GLXDrawable currentDrawable;
    GLXDrawable currentReadable;
    XID xid;
} __GLXcontext;


/*
 * Almost all the GLX API functions get routed through this dispatch table.
 * The exceptions are the glXGetCurrentXXX() functions.
 *
 * This dispatch table allows multiple GLX client-side modules to coexist.
 * Specifically, a real GLX library (like SGI's or the Utah GLX) and Mesa's
 * pseudo-GLX can be present at the same time.  The former being used on
 * GLX-enabled X servers and the later on non-GLX X servers.
 *
 * Red Hat has been using this since Red Hat Linux 7.0 (I think).
 * This'll be a standard feature in XFree86 4.3.  It basically allows one
 * libGL to do both DRI-rendering and "fake GLX" rendering to X displays
 * that lack the GLX extension.
 */
struct _glxapi_table {
    /*** GLX_VERSION_1_0 ***/
    XVisualInfo *(*ChooseVisual)(Display *dpy, int screen, int *list);
    void (*CopyContext)(Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
    GLXContext (*CreateContext)(Display *dpy, XVisualInfo *visinfo, GLXContext shareList, Bool direct);
    GLXPixmap (*CreateGLXPixmap)(Display *dpy, XVisualInfo *visinfo, Pixmap pixmap);
    void (*DestroyContext)(Display *dpy, GLXContext ctx);
    void (*DestroyGLXPixmap)(Display *dpy, GLXPixmap pixmap);
    int (*GetConfig)(Display *dpy, XVisualInfo *visinfo, int attrib, int *value);
    GLXContext (*GetCurrentContext)(void);
    /*GLXDrawable (*GetCurrentDrawable)(void);*/
    Bool (*IsDirect)(Display *dpy, GLXContext ctx);
    Bool (*MakeCurrent)(Display *dpy, GLXDrawable drawable, GLXContext ctx);
    Bool (*QueryExtension)(Display *dpy, int *errorb, int *event);
    Bool (*QueryVersion)(Display *dpy, int *maj, int *min);
    void (*SwapBuffers)(Display *dpy, GLXDrawable drawable);
    void (*UseXFont)(Font font, int first, int count, int listBase);
    void (*WaitGL)(void);
    void (*WaitX)(void);

    /*** GLX_VERSION_1_1 ***/
    const char *(*GetClientString)(Display *dpy, int name);
    const char *(*QueryExtensionsString)(Display *dpy, int screen);
    const char *(*QueryServerString)(Display *dpy, int screen, int name);

    /*** GLX_VERSION_1_2 ***/
    /*Display *(*GetCurrentDisplay)(void);*/

    /*** GLX_VERSION_1_3 ***/
    GLXFBConfig *(*ChooseFBConfig)(Display *dpy, int screen, const int *attribList, int *nitems);
    GLXContext (*CreateNewContext)(Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
    GLXPbuffer (*CreatePbuffer)(Display *dpy, GLXFBConfig config, const int *attribList);
    GLXPixmap (*CreatePixmap)(Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
    GLXWindow (*CreateWindow)(Display *dpy, GLXFBConfig config, Window win, const int *attribList);
    void (*DestroyPbuffer)(Display *dpy, GLXPbuffer pbuf);
    void (*DestroyPixmap)(Display *dpy, GLXPixmap pixmap);
    void (*DestroyWindow)(Display *dpy, GLXWindow window);
    /*GLXDrawable (*GetCurrentReadDrawable)(void);*/
    int (*GetFBConfigAttrib)(Display *dpy, GLXFBConfig config, int attribute, int *value);
    GLXFBConfig *(*GetFBConfigs)(Display *dpy, int screen, int *nelements);
    void (*GetSelectedEvent)(Display *dpy, GLXDrawable drawable, unsigned long *mask);
    XVisualInfo *(*GetVisualFromFBConfig)(Display *dpy, GLXFBConfig config);
    Bool (*MakeContextCurrent)(Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
    int (*QueryContext)(Display *dpy, GLXContext ctx, int attribute, int *value);
    void (*QueryDrawable)(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
    void (*SelectEvent)(Display *dpy, GLXDrawable drawable, unsigned long mask);

    /*** GLX_SGI_swap_control ***/
    int (*SwapIntervalSGI)(int);

    /*** GLX_SGI_video_sync ***/
    int (*GetVideoSyncSGI)(unsigned int *count);
    int (*WaitVideoSyncSGI)(int divisor, int remainder, unsigned int *count);

    /*** GLX_SGI_make_current_read ***/
    Bool (*MakeCurrentReadSGI)(Display *, GLXDrawable, GLXDrawable, GLXContext);
    /*GLXDrawable (*GetCurrentReadDrawableSGI)(void);*/

    /*** GLX_SGIX_video_source (needs video library) ***/
#if defined(_VL_H_)
    GLXVideoSourceSGIX (*CreateGLXVideoSourceSGIX)(Display *, int, VLServer, VLPath, int, VLNode);
   void (*DestroyGLXVideoSourceSGIX)(Display *, GLXVideoSourceSGIX);
#else
    void *CreateGLXVideoSourceSGIX;
    void *DestroyGLXVideoSourceSGIX;
#endif

    /*** GLX_EXT_import_context ***/
    void (*FreeContextEXT)(Display *dpy, GLXContext context);
    GLXContextID (*GetContextIDEXT)(const GLXContext context);
    /*Display *(*GetCurrentDisplayEXT)(void);*/
    GLXContext (*ImportContextEXT)(Display *dpy, GLXContextID contextID);
    int (*QueryContextInfoEXT)(Display *dpy, GLXContext context, int attribute,int *value);

    /*** GLX_SGIX_fbconfig ***/
    int (*GetFBConfigAttribSGIX)(Display *, GLXFBConfigSGIX, int, int *);
    GLXFBConfigSGIX * (*ChooseFBConfigSGIX)(Display *, int, int *, int *);
    GLXPixmap (*CreateGLXPixmapWithConfigSGIX)(Display *, GLXFBConfigSGIX, Pixmap);
    GLXContext (*CreateContextWithConfigSGIX)(Display *, GLXFBConfigSGIX, int, GLXContext, Bool);
    XVisualInfo * (*GetVisualFromFBConfigSGIX)(Display *, GLXFBConfigSGIX);
    GLXFBConfigSGIX (*GetFBConfigFromVisualSGIX)(Display *, XVisualInfo *);

    /*** GLX_SGIX_pbuffer ***/
    GLXPbufferSGIX (*CreateGLXPbufferSGIX)(Display *, GLXFBConfigSGIX, unsigned int, unsigned int, int *);
    void (*DestroyGLXPbufferSGIX)(Display *, GLXPbufferSGIX);
    void (*QueryGLXPbufferSGIX)(Display *, GLXPbufferSGIX, int, unsigned int *);
    void (*SelectEventSGIX)(Display *, GLXDrawable, unsigned long);
    void (*GetSelectedEventSGIX)(Display *, GLXDrawable, unsigned long *);

    /*** GLX_SGI_cushion ***/
    void (*CushionSGI)(Display *, Window, float);

    /*** GLX_SGIX_video_resize ***/
    int (*BindChannelToWindowSGIX)(Display *, int, int, Window);
    int (*ChannelRectSGIX)(Display *, int, int, int, int, int, int);
    int (*QueryChannelRectSGIX)(Display *, int, int, int *, int *, int *, int *);
    int (*QueryChannelDeltasSGIX)(Display *, int, int, int *, int *, int *, int *);
    int (*ChannelRectSyncSGIX)(Display *, int, int, GLenum);

    /*** GLX_SGIX_dmbuffer (needs dmedia library) ***/
#if defined (_DM_BUFFER_H_)
    Bool (*AssociateDMPbufferSGIX)(Display *, GLXPbufferSGIX, DMparams *, DMbuffer);
#else
    void *AssociciateDMPbufferSGIX;
#endif

    /*** GLX_SUN_get_transparent_index ***/
    Status (*GetTransparentIndexSUN)(Display *, Window, Window, unsigned long *);

    /*** GLX_MESA_copy_sub_buffer ***/
    void (*CopySubBufferMESA)(Display *dpy, GLXDrawable drawable, int x, int y, int width, int height);

    /*** GLX_MESA_release_buffers ***/
    Bool (*ReleaseBuffersMESA)(Display *dpy, Window w);

    /*** GLX_MESA_pixmap_colormap ***/
    GLXPixmap (*CreateGLXPixmapMESA)(Display *dpy, XVisualInfo *visinfo, Pixmap pixmap, Colormap cmap);

    /*** GLX_EXT_texture_from_pixmap ***/
    void (*BindTexImageEXT)(Display *dpy, GLXDrawable drawable, int buffer,
                            const int *attrib_list);
    void (*ReleaseTexImageEXT)(Display *dpy, GLXDrawable drawable, int buffer);

    /*** GLX_ARB_create_context ***/
    GLXContext (*CreateContextAttribs)(Display *dpy, GLXFBConfig config,
                                       GLXContext share_context, Bool direct,
                                       const int *attrib_list);
};



extern const char *
_glxapi_get_version(void);


extern const char **
_glxapi_get_extensions(void);


extern GLuint
_glxapi_get_dispatch_table_size(void);


extern void
_glxapi_set_no_op_table(struct _glxapi_table *t);


extern __GLXextFuncPtr
_glxapi_get_proc_address(const char *funcName);


#endif

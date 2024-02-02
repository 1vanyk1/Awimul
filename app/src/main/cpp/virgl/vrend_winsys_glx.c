#define HAVE_CONFIG_H
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../libepoxy/gl.h"
#include "virglrenderer.h"
#include "vrend_winsys_glx.h"

struct virgl_glx {
    Display *display;
    GLXFBConfig* fbConfigs;
    GLXPbuffer pbuffer;
};

struct virgl_glx *virgl_glx_init(void)
{
    struct virgl_glx *d;

    d = malloc(sizeof(struct virgl_glx));
    if (!d)
        return NULL;

    d->display = XOpenDisplay(NULL);

    if (!d->display) {
        free(d);
        return NULL;
    }

    int visualAttribs[] = { None };
    int numberOfFramebufferConfigurations = 0;

    d->fbConfigs = glXChooseFBConfig(d->display, DefaultScreen(d->display),
                                     visualAttribs, &numberOfFramebufferConfigurations);

    int pbufferAttribs[] = {
            GLX_PBUFFER_WIDTH,  32,
            GLX_PBUFFER_HEIGHT, 32,
            None
    };
    d->pbuffer = glXCreatePbuffer(d->display, d->fbConfigs[0], pbufferAttribs);

    return d;
}

void virgl_glx_destroy(struct virgl_glx *d)
{
    XFree(d->fbConfigs);
    glXDestroyPbuffer(d->display,d->pbuffer);
    XCloseDisplay(d->display);
    free(d);
}

virgl_renderer_gl_context virgl_glx_create_context(struct virgl_glx *d, struct virgl_gl_ctx_param *vparams)
{
    int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, vparams->major_ver,
            GLX_CONTEXT_MINOR_VERSION_ARB, vparams->minor_ver,
            None
    };

    GLXContext ctx =
            glXCreateContextAttribsARB(d->display, d->fbConfigs[0],
                                       vparams->shared ? glXGetCurrentContext() : NULL,
                                       True, context_attribs);
    return (virgl_renderer_gl_context)ctx;
}

void virgl_glx_destroy_context(struct virgl_glx *d, virgl_renderer_gl_context virglctx)
{
    GLXContext ctx = (GLXContext)virglctx;

    glXDestroyContext(d->display,ctx);
}

int virgl_glx_make_context_current(struct virgl_glx *d, virgl_renderer_gl_context virglctx)
{
    return glXMakeContextCurrent(d->display, d->pbuffer, d->pbuffer, virglctx);
}

uint32_t virgl_glx_query_video_memory(struct virgl_glx *d)
{
    uint32_t video_memory = 0;
    if (d) {
        if (epoxy_has_glx_extension(d->display, DefaultScreen(d->display), "GLX_MESA_query_renderer")) {
            glXQueryCurrentRendererIntegerMESA(GLX_RENDERER_VIDEO_MEMORY_MESA, &video_memory);
        }
    }

    return video_memory;
}

#include "vrend_winsys.h"

#ifdef HAVE_EPOXY_GLX_H
#include "vrend_winsys_glx.h"
#endif

#include <stddef.h>

enum {
    CONTEXT_NONE,
    CONTEXT_EGL,
    CONTEXT_GLX
};

static int use_context = CONTEXT_NONE;

#ifdef HAVE_EPOXY_EGL_H
struct virgl_egl *egl = NULL;
struct virgl_gbm *gbm = NULL;
#endif

#ifdef HAVE_EPOXY_GLX_H
static struct virgl_glx *glx_info = NULL;
#endif

int vrend_winsys_init(uint32_t flags, int preferred_fd)
{
    if (flags & VIRGL_RENDERER_USE_EGL) {
#ifdef HAVE_EPOXY_EGL_H
        /*
       * If the user specifies a preferred DRM fd and we can't use it, fail. If the user doesn't
       * specify an fd, it's possible to initialize EGL without one.
       */
      gbm = virgl_gbm_init(preferred_fd);
      if (preferred_fd > 0 && !gbm)
         return -1;

      egl = virgl_egl_init(gbm, flags & VIRGL_RENDERER_USE_SURFACELESS,
                           flags & VIRGL_RENDERER_USE_GLES);
      if (!egl) {
         if (gbm) {
            virgl_gbm_fini(gbm);
            gbm = NULL;
         }

         return -1;
      }

      use_context = CONTEXT_EGL;
#else
        (void)preferred_fd;
        vrend_printf( "EGL is not supported on this platform\n");
        return -1;
#endif
    } else if (flags & VIRGL_RENDERER_USE_GLX) {
#ifdef HAVE_EPOXY_GLX_H
        glx_info = virgl_glx_init();
      if (!glx_info)
         return -1;
      use_context = CONTEXT_GLX;
#else
        vrend_printf( "GLX is not supported on this platform\n");
        return -1;
#endif
    }

    return 0;
}

void vrend_winsys_cleanup(void)
{
#ifdef HAVE_EPOXY_EGL_H
    if (use_context == CONTEXT_EGL) {
      virgl_egl_destroy(egl);
      egl = NULL;
      use_context = CONTEXT_NONE;
      if (gbm) {
         virgl_gbm_fini(gbm);
         gbm = NULL;
      }
   }
#endif
#ifdef HAVE_EPOXY_GLX_H
    if (use_context == CONTEXT_GLX) {
      virgl_glx_destroy(glx_info);
      glx_info = NULL;
      use_context = CONTEXT_NONE;
   }
#endif
}

virgl_renderer_gl_context vrend_winsys_create_context(struct virgl_gl_ctx_param *param)
{
#ifdef HAVE_EPOXY_EGL_H
    if (use_context == CONTEXT_EGL)
      return virgl_egl_create_context(egl, param);
#endif
#ifdef HAVE_EPOXY_GLX_H
    if (use_context == CONTEXT_GLX)
      return virgl_glx_create_context(glx_info, param);
#endif
    return NULL;
}

void vrend_winsys_destroy_context(virgl_renderer_gl_context ctx)
{
#ifdef HAVE_EPOXY_EGL_H
    if (use_context == CONTEXT_EGL) {
      virgl_egl_destroy_context(egl, ctx);
      return;
   }
#endif
#ifdef HAVE_EPOXY_GLX_H
    if (use_context == CONTEXT_GLX) {
      virgl_glx_destroy_context(glx_info, ctx);
      return;
   }
#endif
}

int vrend_winsys_make_context_current(virgl_renderer_gl_context ctx)
{
#ifdef HAVE_EPOXY_EGL_H
    if (use_context == CONTEXT_EGL)
      return virgl_egl_make_context_current(egl, ctx);
#endif
#ifdef HAVE_EPOXY_GLX_H
    if (use_context == CONTEXT_GLX)
      return virgl_glx_make_context_current(glx_info, ctx);
#endif
    return -1;
}

int vrend_winsys_has_gl_colorspace(void)
{
    bool egl_colorspace = false;
#ifdef HAVE_EPOXY_EGL_H
    if (egl)
      egl_colorspace = virgl_has_egl_khr_gl_colorspace(egl);
#endif
    return use_context == CONTEXT_NONE ||
           use_context == CONTEXT_GLX ||
           (use_context == CONTEXT_EGL && egl_colorspace);
}

int vrend_winsys_get_fourcc_for_texture(uint32_t tex_id, uint32_t format, int *fourcc)
{
#ifdef HAVE_EPOXY_EGL_H
    if (use_context == CONTEXT_EGL)
      return virgl_egl_get_fourcc_for_texture(egl, tex_id, format, fourcc);
#else
    (void)tex_id;
    (void)format;
    (void)fourcc;
#endif
    return 0;
}

int vrend_winsys_get_fd_for_texture(uint32_t tex_id, int *fd)
{
#ifdef HAVE_EPOXY_EGL_H
    if (!egl)
      return -1;

   return virgl_egl_get_fd_for_texture(egl, tex_id, fd);
#else
    (void)tex_id;
    (void)fd;
    return -1;
#endif
}

int vrend_winsys_get_fd_for_texture2(uint32_t tex_id, int *fd, int *stride, int *offset)
{
#ifdef HAVE_EPOXY_EGL_H
    if (!egl)
      return -1;

   return virgl_egl_get_fd_for_texture2(egl, tex_id, fd, stride, offset);
#else
    (void)tex_id;
    (void)fd;
    (void)stride;
    (void)offset;
    return -1;
#endif
}

uint32_t vrend_winsys_query_video_memory(void)
{
#ifdef HAVE_EPOXY_GLX_H
    return virgl_glx_query_video_memory(glx_info);
#else
    return 0;
#endif
}

/* different_gpu means that GBM and GL renderer are on two different DRM devices.
 * Linear buffers are used for scanouts to make them shareable.
 * Advise the client to use drawable shadowing for performance.
 */
bool vrend_winsys_different_gpu(void)
{
#ifdef HAVE_EPOXY_EGL_H
    if (egl)
      return virgl_egl_different_gpu(egl);
#endif
    return false;
}

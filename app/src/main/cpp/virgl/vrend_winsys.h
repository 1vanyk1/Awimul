#ifndef VREND_WINSYS_H
#define VREND_WINSYS_H

#include "config.h"

#ifdef HAVE_EPOXY_EGL_H
#include "vrend_winsys_gbm.h"
#include "vrend_winsys_egl.h"
#endif

#include "virglrenderer.h"

#ifndef DRM_FORMAT_MOD_INVALID
#define DRM_FORMAT_MOD_INVALID 0x00ffffffffffffffULL
#endif

struct virgl_gl_ctx_param;

#ifdef HAVE_EPOXY_EGL_H
extern struct virgl_egl *egl;
extern struct virgl_gbm *gbm;
#endif

int vrend_winsys_init(uint32_t flags, int preferred_fd);
void vrend_winsys_cleanup(void);

virgl_renderer_gl_context vrend_winsys_create_context(struct virgl_gl_ctx_param *param);
void vrend_winsys_destroy_context(virgl_renderer_gl_context ctx);
int vrend_winsys_make_context_current(virgl_renderer_gl_context ctx);

int vrend_winsys_has_gl_colorspace(void);

int vrend_winsys_get_fourcc_for_texture(uint32_t tex_id, uint32_t format, int *fourcc);
int vrend_winsys_get_fd_for_texture(uint32_t tex_id, int *fd);
int vrend_winsys_get_fd_for_texture2(uint32_t tex_id, int *fd, int *stride, int *offset);

uint32_t vrend_winsys_query_video_memory(void);
bool vrend_winsys_different_gpu(void);

#endif /* VREND_WINSYS_H */

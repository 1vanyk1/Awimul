#ifndef VIRGL_GLX_H
#define VIRGL_GLX_H

#include "vrend_renderer.h"
struct virgl_glx;

struct virgl_glx *virgl_glx_init(void);
void virgl_glx_destroy(struct virgl_glx *ve);

virgl_renderer_gl_context virgl_glx_create_context(struct virgl_glx *ve, struct virgl_gl_ctx_param *vparams);
void virgl_glx_destroy_context(struct virgl_glx *ve, virgl_renderer_gl_context virglctx);
int virgl_glx_make_context_current(struct virgl_glx *ve, virgl_renderer_gl_context virglctx);
uint32_t virgl_glx_query_video_memory(struct virgl_glx *ve);

#endif

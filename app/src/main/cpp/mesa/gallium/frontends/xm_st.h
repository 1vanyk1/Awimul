#ifndef _XM_ST_H_
#define _XM_ST_H_

#include "../../include/pipe/p_compiler.h"
#include "../frontend/api.h"

#include "xm_api.h"

struct st_framebuffer_iface *
xmesa_create_st_framebuffer(XMesaDisplay xmdpy, XMesaBuffer b);

void
xmesa_destroy_st_framebuffer(struct st_framebuffer_iface *stfbi);

struct pipe_resource *
xmesa_get_framebuffer_resource(struct st_framebuffer_iface *stfbi,
                               enum st_attachment_type att);

void
xmesa_swap_st_framebuffer(struct st_framebuffer_iface *stfbi);

void
xmesa_copy_st_framebuffer(struct st_framebuffer_iface *stfbi,
                          enum st_attachment_type src,
                          enum st_attachment_type dst,
                          int x, int y, int w, int h);

struct pipe_resource*
xmesa_get_attachment(struct st_framebuffer_iface *stfbi,
                     enum st_attachment_type st_attachment);

struct pipe_context*
xmesa_get_context(struct st_framebuffer_iface* stfbi);

bool
xmesa_st_framebuffer_validate_textures(struct st_framebuffer_iface *stfbi,
                                       unsigned width, unsigned height,
                                       unsigned mask);
#endif /* _XM_ST_H_ */

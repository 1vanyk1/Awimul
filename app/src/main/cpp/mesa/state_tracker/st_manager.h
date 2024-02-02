#ifndef ST_MANAGER_H
#define ST_MANAGER_H

#include "../main/menums.h"

#include "../include/pipe/p_compiler.h"

struct st_context;
struct st_framebuffer;
struct st_framebuffer_interface;
struct st_renderbuffer;
struct pipe_surface;

void
st_manager_flush_frontbuffer(struct st_context *st);

void
st_manager_validate_framebuffers(struct st_context *st);

bool
st_manager_add_color_renderbuffer(struct st_context *st, struct gl_framebuffer *fb,
                                  gl_buffer_index idx);

void
st_framebuffer_reference(struct st_framebuffer **ptr,
                         struct st_framebuffer *stfb);

void
st_framebuffer_interface_destroy(struct st_framebuffer_interface *stfbi);

void
st_manager_flush_swapbuffers(void);

void
st_set_ws_renderbuffer_surface(struct st_renderbuffer *strb,
                               struct pipe_surface *surf);

#endif /* ST_MANAGER_H */

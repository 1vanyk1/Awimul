#ifndef U_DRAWQUAD_H
#define U_DRAWQUAD_H


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_context.h"

#include "u_draw.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_resource;
struct cso_context;

extern void
util_draw_vertex_buffer(struct pipe_context *pipe, struct cso_context *cso,
                        struct pipe_resource *vbuf, uint vbuf_slot,
                        uint offset, uint prim_type, uint num_attribs,
                        uint num_verts);

void
util_draw_user_vertex_buffer(struct cso_context *cso, void *buffer,
                             uint prim_type, uint num_verts, uint num_attribs);

#ifdef __cplusplus
}
#endif


#endif

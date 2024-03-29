#ifndef ST_DRAW_H
#define ST_DRAW_H

#include "../main/glheader.h"

struct _mesa_index_buffer;
struct _mesa_prim;
struct gl_context;
struct st_context;

void st_init_draw_functions(struct pipe_screen *screen,
                            struct dd_function_table *functions);

void st_destroy_draw( struct st_context *st );

struct draw_context *st_get_draw_context(struct st_context *st);

extern void
st_feedback_draw_vbo(struct gl_context *ctx,
                     const struct _mesa_prim *prims,
                     unsigned nr_prims,
                     const struct _mesa_index_buffer *ib,
                     bool index_bounds_valid,
                     bool primitive_restart,
                     unsigned restart_index,
                     unsigned min_index,
                     unsigned max_index,
                     unsigned num_instances,
                     unsigned base_instance);

/**
 * When drawing with VBOs, the addresses specified with
 * glVertex/Color/TexCoordPointer() are really offsets into the VBO, not real
 * addresses.  At some point we need to convert those pointers to offsets.
 * This function is basically a cast wrapper to avoid warnings when building
 * in 64-bit mode.
 */
static inline unsigned
pointer_to_offset(const void *ptr)
{
    return (unsigned) (((GLsizeiptr) ptr) & 0xffffffffUL);
}


bool
st_draw_quad(struct st_context *st,
             float x0, float y0, float x1, float y1, float z,
             float s0, float t0, float s1, float t1,
             const float *color,
             unsigned num_instances);

void
st_draw_transform_feedback(struct gl_context *ctx, GLenum mode,
                           unsigned num_instances, unsigned stream,
                           struct gl_transform_feedback_object *tfb_vertcount);

void
st_indirect_draw_vbo(struct gl_context *ctx,
                     GLuint mode,
                     struct gl_buffer_object *indirect_data,
                     GLsizeiptr indirect_offset,
                     unsigned draw_count,
                     unsigned stride,
                     struct gl_buffer_object *indirect_draw_count,
                     GLsizeiptr indirect_draw_count_offset,
                     const struct _mesa_index_buffer *ib,
                     bool primitive_restart,
                     unsigned restart_index);
#endif

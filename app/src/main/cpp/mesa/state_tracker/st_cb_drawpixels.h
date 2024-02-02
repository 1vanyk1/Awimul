#ifndef ST_CB_DRAWPIXELS_H
#define ST_CB_DRAWPIXELS_H


#include <stdbool.h>

struct dd_function_table;
struct st_context;
struct gl_context;
struct gl_pixelstore_attrib;


extern void
st_destroy_drawpix(struct st_context *st);

extern const struct tgsi_token *
st_get_drawpix_shader(const struct tgsi_token *tokens, bool use_texcoord,
                      bool scale_and_bias, unsigned scale_const,
                      unsigned bias_const, bool pixel_maps,
                      unsigned drawpix_sampler, unsigned pixelmap_sampler,
                      unsigned texcoord_const, unsigned tex_target);

extern void
st_make_passthrough_vertex_shader(struct st_context *st);

void st_DrawPixels(struct gl_context *ctx, GLint x, GLint y,
                   GLsizei width, GLsizei height,
                   GLenum format, GLenum type,
                   const struct gl_pixelstore_attrib *unpack, const void *pixels);
void st_CopyPixels(struct gl_context *ctx, GLint srcx, GLint srcy,
                   GLsizei width, GLsizei height,
                   GLint dstx, GLint dsty, GLenum type);

#endif /* ST_CB_DRAWPIXELS_H */

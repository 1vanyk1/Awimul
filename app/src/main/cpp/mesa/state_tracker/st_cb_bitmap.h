#ifndef ST_CB_BITMAP_H
#define ST_CB_BITMAP_H


#include <stdbool.h>

struct dd_function_table;
struct st_context;
struct gl_program;
struct st_program;
struct gl_bitmap_atlas;
struct gl_context;
struct gl_pixelstore_attrib;

extern void
st_destroy_bitmap(struct st_context *st);

extern void
st_flush_bitmap_cache(struct st_context *st);

extern const struct tgsi_token *
st_get_bitmap_shader(const struct tgsi_token *tokens,
                     unsigned tex_target, unsigned sampler_index,
                     bool use_texcoord, bool swizzle_xxxx);

void st_Bitmap(struct gl_context *ctx, GLint x, GLint y,
               GLsizei width, GLsizei height,
               const struct gl_pixelstore_attrib *unpack, const GLubyte *bitmap);
void st_DrawAtlasBitmaps(struct gl_context *ctx,
                         const struct gl_bitmap_atlas *atlas,
                         GLuint count, const GLubyte *ids);
#endif /* ST_CB_BITMAP_H */

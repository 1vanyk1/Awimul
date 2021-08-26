#ifndef _DEBUG_H
#define _DEBUG_H

#include "glheader.h"

struct gl_context;
struct gl_texture_image;

extern void _mesa_print_enable_flags( const char *msg, GLuint flags );
extern void _mesa_print_state( const char *msg, GLuint state );
extern void _mesa_print_info( struct gl_context *ctx );
extern void _mesa_init_debug( struct gl_context *ctx );

extern void
_mesa_write_renderbuffer_image(const struct gl_renderbuffer *rb);

extern void
_mesa_dump_texture(GLuint texture, GLuint writeImages);

extern void
_mesa_dump_textures(GLuint writeImages);

extern void
_mesa_dump_renderbuffers(GLboolean writeImages);

extern void
_mesa_dump_color_buffer(const char *filename);

extern void
_mesa_dump_depth_buffer(const char *filename);

extern void
_mesa_dump_stencil_buffer(const char *filename);

extern void
_mesa_dump_image(const char *filename, const void *image, GLuint w, GLuint h,
                 GLenum format, GLenum type);

extern void
_mesa_print_texture(struct gl_context *ctx, struct gl_texture_image *img);

#endif

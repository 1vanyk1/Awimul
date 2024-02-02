#ifndef ST_FORMAT_H
#define ST_FORMAT_H

#include "../main/formats.h"
#include "../main/glheader.h"

#include "../include/pipe/p_format.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct pipe_screen;


extern enum pipe_format
st_mesa_format_to_pipe_format(const struct st_context *st, mesa_format mesaFormat);

extern mesa_format
st_pipe_format_to_mesa_format(enum pipe_format pipeFormat);


extern enum pipe_format
st_choose_format(struct st_context *st, GLenum internalFormat,
                 GLenum format, GLenum type,
                 enum pipe_texture_target target, unsigned sample_count,
                 unsigned storage_sample_count,
                 unsigned bindings, bool swap_bytes, bool allow_dxt);

extern enum pipe_format
st_choose_renderbuffer_format(struct st_context *st,
                              GLenum internalFormat, unsigned sample_count,
                              unsigned storage_sample_count);

extern enum pipe_format
st_choose_matching_format_noverify(struct st_context *st,
                                   GLenum format, GLenum type, GLboolean swapBytes);

extern enum pipe_format
st_choose_matching_format(struct st_context *st, unsigned bind,
                          GLenum format, GLenum type, GLboolean swapBytes);

extern mesa_format
st_ChooseTextureFormat(struct gl_context * ctx, GLenum target,
                       GLint internalFormat,
                       GLenum format, GLenum type);

void
st_QueryInternalFormat(struct gl_context *ctx, GLenum target,
                       GLenum internalFormat, GLenum pname, GLint *params);

extern void
st_translate_color(union pipe_color_union *color,
                   GLenum baseFormat, GLboolean is_integer);

#ifdef __cplusplus
}
#endif

#endif /* ST_FORMAT_H */

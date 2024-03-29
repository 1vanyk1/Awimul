#ifndef GLFORMATS_H
#define GLFORMATS_H


#include <stdbool.h>
#include <stdint.h>
#include "../../GL/gl.h"


#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

extern void
_mesa_compute_component_mapping(GLenum inFormat, GLenum outFormat, GLubyte *map);

extern GLboolean
_mesa_type_is_packed(GLenum type);

extern GLint
_mesa_sizeof_type( GLenum type );

extern GLint
_mesa_sizeof_packed_type( GLenum type );

extern GLint
_mesa_components_in_format( GLenum format );

extern GLint
_mesa_bytes_per_pixel( GLenum format, GLenum type );

extern GLint
_mesa_bytes_per_vertex_attrib(GLint comps, GLenum type);

extern GLboolean
_mesa_is_astc_format(GLenum internalFormat);

extern GLboolean
_mesa_is_etc2_format(GLenum internalFormat);

extern GLboolean
_mesa_is_type_unsigned(GLenum type);

extern GLboolean
_mesa_is_enum_format_unsized(GLenum format);

extern GLboolean
_mesa_is_enum_format_unorm(GLenum format);

extern GLboolean
_mesa_is_enum_format_snorm(GLenum format);

extern GLboolean
_mesa_is_enum_format_integer(GLenum format);

extern GLboolean
_mesa_is_enum_format_unsigned_int(GLenum format);

extern GLboolean
_mesa_is_enum_format_signed_int(GLenum format);

extern GLboolean
_mesa_is_color_format(GLenum format);

extern GLboolean
_mesa_is_depth_format(GLenum format);

extern GLboolean
_mesa_is_stencil_format(GLenum format);

extern GLboolean
_mesa_is_ycbcr_format(GLenum format);

extern GLboolean
_mesa_is_depthstencil_format(GLenum format);

extern GLboolean
_mesa_is_depth_or_stencil_format(GLenum format);

extern GLboolean
_mesa_has_depth_float_channel(GLenum internalFormat);

extern GLboolean
_mesa_is_compressed_format(const struct gl_context *ctx, GLenum format);

extern GLboolean
_mesa_is_srgb_format(GLenum format);

extern GLenum
_mesa_base_format_to_integer_format(GLenum format);

extern GLenum
_mesa_unpack_format_to_base_format(GLenum format);

extern GLboolean
_mesa_base_format_has_channel(GLenum base_format, GLenum pname);

extern GLenum
_mesa_generic_compressed_format_to_uncompressed_format(GLenum format);

extern GLenum
_mesa_get_nongeneric_internalformat(GLenum format);

extern GLenum
_mesa_get_linear_internalformat(GLenum format);

extern GLenum
_mesa_error_check_format_and_type(const struct gl_context *ctx,
                                  GLenum format, GLenum type);

extern GLenum
_mesa_es_error_check_format_and_type(const struct gl_context *ctx,
                                     GLenum format, GLenum type,
                                     unsigned dimensions);

extern GLenum
_mesa_gles_error_check_format_and_type(const struct gl_context *ctx,
                                       GLenum format, GLenum type,
                                       GLenum internalFormat);
extern GLint
_mesa_base_tex_format(const struct gl_context *ctx, GLint internalFormat );

extern uint32_t
_mesa_format_from_format_and_type(GLenum format, GLenum type);

bool
_mesa_swap_bytes_in_type_enum(GLenum *type);

extern uint32_t
_mesa_tex_format_from_format_and_type(const struct gl_context *ctx,
                                      GLenum gl_format, GLenum type);

extern bool
_mesa_is_es3_color_renderable(const struct gl_context *ctx,
                              GLenum internal_format);

extern bool
_mesa_is_es3_texture_filterable(const struct gl_context *ctx,
                                GLenum internal_format);

#ifdef __cplusplus
}
#endif

#endif /* GLFORMATS_H */

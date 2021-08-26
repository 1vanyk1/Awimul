#ifndef SHADERIMAGE_H
#define SHADERIMAGE_H

#include "glheader.h"
#include "formats.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

/**
 * Get the matching mesa_format for a shader image format GL enum.
 */
mesa_format
_mesa_get_shader_image_format(GLenum format);

/**
 * Get the GL image format class for a shader image format GL enum
 */
GLenum
_mesa_get_image_format_class(GLenum format);

/**
 * Return whether an image format should be supported based on the current API
 * version of the context.
 */
bool
_mesa_is_shader_image_format_supported(const struct gl_context *ctx,
                                       GLenum format);

/**
 * Get a single image unit struct with the default state.
 */
struct gl_image_unit
_mesa_default_image_unit(struct gl_context *ctx);

/**
 * Initialize a context's shader image units to the default state.
 */
void
_mesa_init_image_units(struct gl_context *ctx);

void
_mesa_free_image_textures(struct gl_context *ctx);

/**
 * Return GL_TRUE if the state of the image unit passed as argument is valid
 * and access from the shader is allowed.  Otherwise loads from this unit
 * should return zero and stores should have no effect.
 *
 * The result depends on context state other than the passed image unit, part
 * of the _NEW_TEXTURE_OBJECT set.
 */
GLboolean
_mesa_is_image_unit_valid(struct gl_context *ctx, struct gl_image_unit *u);

void GLAPIENTRY
_mesa_BindImageTexture_no_error(GLuint unit, GLuint texture, GLint level,
                                GLboolean layered, GLint layer, GLenum access,
                                GLenum format);

void GLAPIENTRY
_mesa_BindImageTexture(GLuint unit, GLuint texture, GLint level,
                       GLboolean layered, GLint layer, GLenum access,
                       GLenum format);

void GLAPIENTRY
_mesa_BindImageTextureEXT(GLuint unit, GLuint texture, GLint level,
                          GLboolean layered, GLint layer, GLenum access,
                          GLint format);

void GLAPIENTRY
_mesa_BindImageTextures_no_error(GLuint first, GLsizei count,
                                 const GLuint *textures);

void GLAPIENTRY
_mesa_BindImageTextures(GLuint first, GLsizei count, const GLuint *textures);

#ifdef __cplusplus
}
#endif

#endif

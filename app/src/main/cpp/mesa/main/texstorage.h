#ifndef TEXSTORAGE_H
#define TEXSTORAGE_H

/**
 * \name Internal functions
 */
/*@{*/

/**
 * Texture width, height and depth check shared with the
 * multisample variants of TexStorage functions.
 *
 * From OpenGL 4.5 Core spec, page 260 (section 8.19)
 *
 *     "An INVALID_VALUE error is generated if width, height, depth
 *     or levels are less than 1, for commands with the corresponding
 *     parameters."
 *
 * (referring to TextureStorage* commands, these also match values
 * specified for OpenGL ES 3.1.)
 */
static inline bool
_mesa_valid_tex_storage_dim(GLsizei width, GLsizei height, GLsizei depth)
{
    if (width < 1 || height < 1 || depth < 1)
        return false;
    return true;
}

/*@}*/

/**
 * \name API functions
 */
/*@{*/

void GLAPIENTRY
_mesa_TexStorage1D_no_error(GLenum target, GLsizei levels,
        GLenum internalformat, GLsizei width);

extern void GLAPIENTRY
_mesa_TexStorage1D(GLenum target, GLsizei levels, GLenum internalformat,
GLsizei width);

void GLAPIENTRY
_mesa_TexStorage2D_no_error(GLenum target, GLsizei levels,
        GLenum internalformat, GLsizei width,
        GLsizei height);

extern void GLAPIENTRY
_mesa_TexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
GLsizei width, GLsizei height);

void GLAPIENTRY
_mesa_TexStorage3D_no_error(GLenum target, GLsizei levels,
        GLenum internalformat, GLsizei width,
        GLsizei height, GLsizei depth);

extern void GLAPIENTRY
_mesa_TexStorage3D(GLenum target, GLsizei levels, GLenum internalformat,
GLsizei width, GLsizei height, GLsizei depth);

void GLAPIENTRY
_mesa_TextureStorage1D_no_error(GLuint texture, GLsizei levels,
        GLenum internalformat, GLsizei width);

extern void GLAPIENTRY
_mesa_TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat,
GLsizei width);

void GLAPIENTRY
_mesa_TextureStorage2D_no_error(GLuint texture, GLsizei levels,
        GLenum internalformat, GLsizei width,
        GLsizei height);

extern void GLAPIENTRY
_mesa_TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat,
GLsizei width, GLsizei height);

void GLAPIENTRY
_mesa_TextureStorage3D_no_error(GLuint texture, GLsizei levels,
        GLenum internalformat, GLsizei width,
        GLsizei height, GLsizei depth);

extern void GLAPIENTRY
_mesa_TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat,
GLsizei width, GLsizei height, GLsizei depth);


extern void GLAPIENTRY
_mesa_TextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels,
GLenum internalformat,
        GLsizei width);

extern void GLAPIENTRY
_mesa_TextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels,
GLenum internalformat,
        GLsizei width, GLsizei height);

extern void GLAPIENTRY
_mesa_TextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels,
GLenum internalformat,
        GLsizei width, GLsizei height, GLsizei depth);

extern GLboolean
_mesa_is_legal_tex_storage_format(const struct gl_context *ctx,
                                  GLenum internalformat);

extern GLboolean
_mesa_AllocTextureStorage_sw(struct gl_context *ctx,
                             struct gl_texture_object *texObj,
                             GLsizei levels, GLsizei width,
                             GLsizei height, GLsizei depth);

extern void
_mesa_texture_storage_memory(struct gl_context *ctx, GLuint dims,
                             struct gl_texture_object *texObj,
                             struct gl_memory_object *memObj,
                             GLenum target, GLsizei levels,
                             GLenum internalformat, GLsizei width,
                             GLsizei height, GLsizei depth,
                             GLuint64 offset, bool dsa);

#endif /* TEXSTORAGE_H */

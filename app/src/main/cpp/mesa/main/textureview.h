#ifndef TEXTUREVIEW_H
#define TEXTUREVIEW_H

bool
_mesa_texture_view_compatible_format(const struct gl_context *ctx,
                                     GLenum origInternalFormat,
                                     GLenum newInternalFormat);

/**
 * Lookup format view class based on internalformat
 * \return VIEW_CLASS if internalformat found in table, false otherwise.
 */
GLenum
_mesa_texture_view_lookup_view_class(const struct gl_context *ctx,
                                     GLenum internalformat);

void GLAPIENTRY
_mesa_TextureView_no_error(GLuint texture, GLenum target, GLuint origtexture,
GLenum internalformat,
        GLuint minlevel, GLuint numlevels,
        GLuint minlayer, GLuint numlayers);

extern void GLAPIENTRY
_mesa_TextureView(GLuint texture, GLenum target, GLuint origtexture,
GLenum internalformat,
        GLuint minlevel, GLuint numlevels,
        GLuint minlayer, GLuint numlayers);

extern void
_mesa_set_texture_view_state(struct gl_context *ctx,
                             struct gl_texture_object *texObj,
                             GLenum target, GLuint levels);

#endif /* TEXTUREVIEW_H */

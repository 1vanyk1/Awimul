#ifndef TEXTUREBINDLESS_H
#define TEXTUREBINDLESS_H

#include "glheader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

/**
 * \name Internal functions
 */
/*@{*/

void
_mesa_init_resident_handles(struct gl_context *ctx);
void
_mesa_free_resident_handles(struct gl_context *ctx);

void
_mesa_init_shared_handles(struct gl_shared_state *shared);
void
_mesa_free_shared_handles(struct gl_shared_state *shared);

void
_mesa_init_texture_handles(struct gl_texture_object *texObj);
void
_mesa_make_texture_handles_non_resident(struct gl_context *ctx,
                                        struct gl_texture_object *texObj);
void
_mesa_delete_texture_handles(struct gl_context *ctx,
                             struct gl_texture_object *texObj);

void
_mesa_init_sampler_handles(struct gl_sampler_object *sampObj);
void
_mesa_delete_sampler_handles(struct gl_context *ctx,
                             struct gl_sampler_object *sampObj);

/*@}*/

/**
 * \name API functions
 */
/*@{*/

GLuint64 GLAPIENTRY
_mesa_GetTextureHandleARB_no_error(GLuint texture);

GLuint64 GLAPIENTRY
_mesa_GetTextureHandleARB(GLuint texture);

GLuint64 GLAPIENTRY
_mesa_GetTextureSamplerHandleARB_no_error(GLuint texture, GLuint sampler);

GLuint64 GLAPIENTRY
_mesa_GetTextureSamplerHandleARB(GLuint texture, GLuint sampler);

void GLAPIENTRY
_mesa_MakeTextureHandleResidentARB_no_error(GLuint64 handle);

void GLAPIENTRY
_mesa_MakeTextureHandleResidentARB(GLuint64 handle);

void GLAPIENTRY
_mesa_MakeTextureHandleNonResidentARB_no_error(GLuint64 handle);

void GLAPIENTRY
_mesa_MakeTextureHandleNonResidentARB(GLuint64 handle);

GLuint64 GLAPIENTRY
_mesa_GetImageHandleARB_no_error(GLuint texture, GLint level, GLboolean layered,
                                 GLint layer, GLenum format);

GLuint64 GLAPIENTRY
_mesa_GetImageHandleARB(GLuint texture, GLint level, GLboolean layered,
                        GLint layer, GLenum format);

void GLAPIENTRY
_mesa_MakeImageHandleResidentARB_no_error(GLuint64 handle, GLenum access);

void GLAPIENTRY
_mesa_MakeImageHandleResidentARB(GLuint64 handle, GLenum access);

void GLAPIENTRY
_mesa_MakeImageHandleNonResidentARB_no_error(GLuint64 handle);

void GLAPIENTRY
_mesa_MakeImageHandleNonResidentARB(GLuint64 handle);

GLboolean GLAPIENTRY
_mesa_IsTextureHandleResidentARB_no_error(GLuint64 handle);

GLboolean GLAPIENTRY
_mesa_IsTextureHandleResidentARB(GLuint64 handle);

GLboolean GLAPIENTRY
_mesa_IsImageHandleResidentARB_no_error(GLuint64 handle);

GLboolean GLAPIENTRY
_mesa_IsImageHandleResidentARB(GLuint64 handle);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif

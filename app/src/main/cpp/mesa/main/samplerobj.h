#ifndef SAMPLEROBJ_H
#define SAMPLEROBJ_H

#include "mtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dd_function_table;

static inline struct gl_sampler_object *
_mesa_get_samplerobj(struct gl_context *ctx, GLuint unit)
{
    if (ctx->Texture.Unit[unit].Sampler)
        return ctx->Texture.Unit[unit].Sampler;
    else if (ctx->Texture.Unit[unit]._Current)
        return &ctx->Texture.Unit[unit]._Current->Sampler;
    else
        return NULL;
}


/** Does the given filter state do mipmap filtering? */
static inline GLboolean
_mesa_is_mipmap_filter(const struct gl_sampler_object *samp)
{
    return samp->Attrib.MinFilter != GL_NEAREST && samp->Attrib.MinFilter != GL_LINEAR;
}


extern void
_mesa_reference_sampler_object_(struct gl_context *ctx,
                                struct gl_sampler_object **ptr,
                                struct gl_sampler_object *samp);

static inline void
_mesa_reference_sampler_object(struct gl_context *ctx,
                               struct gl_sampler_object **ptr,
                               struct gl_sampler_object *samp)
{
    if (*ptr != samp)
        _mesa_reference_sampler_object_(ctx, ptr, samp);
}

extern struct gl_sampler_object *
_mesa_lookup_samplerobj(struct gl_context *ctx, GLuint name);

extern struct gl_sampler_object *
_mesa_new_sampler_object(struct gl_context *ctx, GLuint name);

extern void
_mesa_init_sampler_object_functions(struct dd_function_table *driver);

extern void
_mesa_set_sampler_wrap(struct gl_context *ctx, struct gl_sampler_object *samp,
                       GLenum s, GLenum t, GLenum r);

extern void
_mesa_set_sampler_filters(struct gl_context *ctx,
                          struct gl_sampler_object *samp,
                          GLenum min_filter, GLenum mag_filter);

extern void
_mesa_set_sampler_srgb_decode(struct gl_context *ctx,
                              struct gl_sampler_object *samp, GLenum param);

extern void
_mesa_bind_sampler(struct gl_context *ctx, GLuint unit,
                   struct gl_sampler_object *sampObj);

void GLAPIENTRY
_mesa_GenSamplers_no_error(GLsizei count, GLuint *samplers);

void GLAPIENTRY
_mesa_GenSamplers(GLsizei count, GLuint *samplers);

void GLAPIENTRY
_mesa_CreateSamplers_no_error(GLsizei count, GLuint *samplers);

void GLAPIENTRY
_mesa_CreateSamplers(GLsizei count, GLuint *samplers);

void GLAPIENTRY
_mesa_DeleteSamplers_no_error(GLsizei count, const GLuint *samplers);

void GLAPIENTRY
_mesa_DeleteSamplers(GLsizei count, const GLuint *samplers);

GLboolean GLAPIENTRY
_mesa_IsSampler(GLuint sampler);

void GLAPIENTRY
_mesa_BindSampler_no_error(GLuint unit, GLuint sampler);

void GLAPIENTRY
_mesa_BindSampler(GLuint unit, GLuint sampler);

void GLAPIENTRY
_mesa_BindSamplers_no_error(GLuint first, GLsizei count, const GLuint *samplers);

void GLAPIENTRY
_mesa_BindSamplers(GLuint first, GLsizei count, const GLuint *samplers);

void GLAPIENTRY
_mesa_SamplerParameteri(GLuint sampler, GLenum pname, GLint param);
void GLAPIENTRY
_mesa_SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
void GLAPIENTRY
_mesa_SamplerParameteriv(GLuint sampler, GLenum pname, const GLint *params);
void GLAPIENTRY
_mesa_SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *params);
void GLAPIENTRY
_mesa_SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *params);
void GLAPIENTRY
_mesa_SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *params);
void GLAPIENTRY
_mesa_GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params);
void GLAPIENTRY
_mesa_GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params);
void GLAPIENTRY
_mesa_GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params);

#ifdef __cplusplus
}
#endif

#endif /* SAMPLEROBJ_H */

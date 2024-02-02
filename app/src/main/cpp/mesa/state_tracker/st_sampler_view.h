#ifndef ST_SAMPLER_VIEW_H
#define ST_SAMPLER_VIEW_H

#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_context.h"
#include "../include/pipe/p_state.h"
#include "../util/u_sampler.h"

struct st_texture_object;


static inline struct pipe_sampler_view *
st_create_texture_sampler_view_format(struct pipe_context *pipe,
                                      struct pipe_resource *texture,
                                      enum pipe_format format)
{
    struct pipe_sampler_view templ;

    u_sampler_view_default_template(&templ, texture, format);

    return pipe->create_sampler_view(pipe, texture, &templ);
}


static inline struct pipe_sampler_view *
st_create_texture_sampler_view(struct pipe_context *pipe,
                               struct pipe_resource *texture)
{
    return st_create_texture_sampler_view_format(pipe, texture,
                                                 texture->format);
}


extern void
st_texture_release_context_sampler_view(struct st_context *st,
                                        struct st_texture_object *stObj);

extern void
st_texture_release_all_sampler_views(struct st_context *st,
                                     struct st_texture_object *stObj);

void
st_delete_texture_sampler_views(struct st_context *st,
                                struct st_texture_object *stObj);

struct st_sampler_view *
st_texture_get_current_sampler_view(const struct st_context *st,
                                    const struct st_texture_object *stObj);

struct pipe_sampler_view *
st_get_texture_sampler_view_from_stobj(struct st_context *st,
                                       struct st_texture_object *stObj,
                                       const struct gl_sampler_object *samp,
                                       bool glsl130_or_later,
                                       bool ignore_srgb_decode,
                                       bool get_reference);

struct pipe_sampler_view *
st_get_buffer_sampler_view_from_stobj(struct st_context *st,
                                      struct st_texture_object *stObj,
                                      bool get_reference);

#endif /* ST_SAMPLER_VIEW_H */

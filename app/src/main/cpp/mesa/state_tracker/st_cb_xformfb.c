#include "../main/bufferobj.h"
#include "../main/context.h"
#include "../main/transformfeedback.h"
#include "../util/u_memory.h"

#include "st_cb_xformfb.h"
#include "st_context.h"

#include "../include/pipe/p_context.h"
#include "../util/u_draw.h"
#include "../util/u_inlines.h"
#include "../gallium/cso_cache/cso_context.h"

struct st_transform_feedback_object {
    struct gl_transform_feedback_object base;

    unsigned num_targets;
    struct pipe_stream_output_target *targets[PIPE_MAX_SO_BUFFERS];

    /* This encapsulates the count that can be used as a source for draw_vbo.
     * It contains stream output targets from the last call of
     * EndTransformFeedback for each stream. */
    struct pipe_stream_output_target *draw_count[MAX_VERTEX_STREAMS];
};

static inline struct st_transform_feedback_object *
st_transform_feedback_object(struct gl_transform_feedback_object *obj)
{
    return (struct st_transform_feedback_object *) obj;
}

struct gl_transform_feedback_object *
st_new_transform_feedback(struct gl_context *ctx, GLuint name)
{
    struct st_transform_feedback_object *obj;

    obj = CALLOC_STRUCT(st_transform_feedback_object);
    if (!obj)
        return NULL;

    _mesa_init_transform_feedback_object(&obj->base, name);

    return &obj->base;
}


void
st_delete_transform_feedback(struct gl_context *ctx,
                             struct gl_transform_feedback_object *obj)
{
    struct st_transform_feedback_object *sobj =
            st_transform_feedback_object(obj);
    unsigned i;

    for (i = 0; i < ARRAY_SIZE(sobj->draw_count); i++)
        pipe_so_target_reference(&sobj->draw_count[i], NULL);

    /* Unreference targets. */
    for (i = 0; i < sobj->num_targets; i++) {
        pipe_so_target_reference(&sobj->targets[i], NULL);
    }

    _mesa_delete_transform_feedback_object(ctx, obj);
}


/* XXX Do we really need the mode? */
void
st_begin_transform_feedback(struct gl_context *ctx, GLenum mode,
                            struct gl_transform_feedback_object *obj)
{
    struct st_context *st = st_context(ctx);
    struct pipe_context *pipe = st->pipe;
    struct st_transform_feedback_object *sobj =
            st_transform_feedback_object(obj);
    unsigned i, max_num_targets;
    unsigned offsets[PIPE_MAX_SO_BUFFERS] = {0};

    max_num_targets = MIN2(ARRAY_SIZE(sobj->base.Buffers),
                           ARRAY_SIZE(sobj->targets));

    /* Convert the transform feedback state into the gallium representation. */
    for (i = 0; i < max_num_targets; i++) {
        struct gl_buffer_object *bo = sobj->base.Buffers[i];

        if (bo && bo->buffer) {
            unsigned stream = obj->program->sh.LinkedTransformFeedback->
                    Buffers[i].Stream;

            /* Check whether we need to recreate the target. */
            if (!sobj->targets[i] ||
                sobj->targets[i] == sobj->draw_count[stream] ||
                sobj->targets[i]->buffer != bo->buffer ||
                sobj->targets[i]->buffer_offset != sobj->base.Offset[i] ||
                sobj->targets[i]->buffer_size != sobj->base.Size[i]) {
                /* Create a new target. */
                struct pipe_stream_output_target *so_target =
                        pipe->create_stream_output_target(pipe, bo->buffer,
                                                          sobj->base.Offset[i],
                                                          sobj->base.Size[i]);

                pipe_so_target_reference(&sobj->targets[i], NULL);
                sobj->targets[i] = so_target;
            }

            sobj->num_targets = i+1;
        } else {
            pipe_so_target_reference(&sobj->targets[i], NULL);
        }
    }

    /* Start writing at the beginning of each target. */
    cso_set_stream_outputs(st->cso_context, sobj->num_targets,
                           sobj->targets, offsets);
}


void
st_pause_transform_feedback(struct gl_context *ctx,
                            struct gl_transform_feedback_object *obj)
{
    struct st_context *st = st_context(ctx);
    cso_set_stream_outputs(st->cso_context, 0, NULL, NULL);
}


void
st_resume_transform_feedback(struct gl_context *ctx,
                             struct gl_transform_feedback_object *obj)
{
    struct st_context *st = st_context(ctx);
    struct st_transform_feedback_object *sobj =
            st_transform_feedback_object(obj);
    unsigned offsets[PIPE_MAX_SO_BUFFERS];
    unsigned i;

    for (i = 0; i < PIPE_MAX_SO_BUFFERS; i++)
        offsets[i] = (unsigned)-1;

    cso_set_stream_outputs(st->cso_context, sobj->num_targets,
                           sobj->targets, offsets);
}

void
st_end_transform_feedback(struct gl_context *ctx,
                          struct gl_transform_feedback_object *obj)
{
    struct st_context *st = st_context(ctx);
    struct st_transform_feedback_object *sobj =
            st_transform_feedback_object(obj);
    unsigned i;

    cso_set_stream_outputs(st->cso_context, 0, NULL, NULL);

    /* The next call to glDrawTransformFeedbackStream should use the vertex
     * count from the last call to glEndTransformFeedback.
     * Therefore, save the targets for each stream.
     *
     * NULL means the vertex counter is 0 (initial state).
     */
    for (i = 0; i < ARRAY_SIZE(sobj->draw_count); i++)
        pipe_so_target_reference(&sobj->draw_count[i], NULL);

    for (i = 0; i < ARRAY_SIZE(sobj->targets); i++) {
        unsigned stream = obj->program->sh.LinkedTransformFeedback->
                Buffers[i].Stream;

        /* Is it not bound or already set for this stream? */
        if (!sobj->targets[i] || sobj->draw_count[stream])
            continue;

        pipe_so_target_reference(&sobj->draw_count[stream], sobj->targets[i]);
    }
}


bool
st_transform_feedback_draw_init(struct gl_transform_feedback_object *obj,
                                unsigned stream,
                                struct pipe_draw_indirect_info *out)
{
    struct st_transform_feedback_object *sobj =
            st_transform_feedback_object(obj);

    out->count_from_stream_output = sobj->draw_count[stream];
    return out->count_from_stream_output != NULL;
}

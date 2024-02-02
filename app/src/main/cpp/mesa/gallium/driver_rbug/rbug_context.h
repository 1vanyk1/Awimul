#ifndef RBUG_CONTEXT_H
#define RBUG_CONTEXT_H

#include "../../include/pipe/p_state.h"
#include "../../include/pipe/p_context.h"

#include "rbug_screen.h"


struct rbug_context {
    struct pipe_context base;  /**< base class */

    struct pipe_context *pipe;

    struct rbug_list list;

    /* call locking */
    mtx_t call_mutex;

    /* current state */
    struct {
        struct rbug_shader *shader[PIPE_SHADER_TYPES];

        struct rbug_sampler_view *views[PIPE_SHADER_TYPES][PIPE_MAX_SHADER_SAMPLER_VIEWS];
        struct rbug_resource *texs[PIPE_SHADER_TYPES][PIPE_MAX_SHADER_SAMPLER_VIEWS];
        unsigned num_views[PIPE_SHADER_TYPES];

        unsigned nr_cbufs;
        struct rbug_resource *cbufs[PIPE_MAX_COLOR_BUFS];
        struct rbug_resource *zsbuf;
    } curr;

    /* draw locking */
    mtx_t draw_mutex;
    cnd_t draw_cond;
    unsigned draw_num_rules;
    int draw_blocker;
    int draw_blocked;

    struct {
        struct rbug_shader *shader[PIPE_SHADER_TYPES];

        struct rbug_resource *texture;
        struct rbug_resource *surf;

        int blocker;
    } draw_rule;

    /* list of state objects */
    mtx_t list_mutex;
    unsigned num_shaders;
    struct rbug_list shaders;
};

static inline struct rbug_context *
rbug_context(struct pipe_context *pipe)
{
    return (struct rbug_context *)pipe;
}


/**********************************************************
 * rbug_context.c
 */

struct pipe_context *
rbug_context_create(struct pipe_screen *screen, struct pipe_context *pipe);


/**********************************************************
 * rbug_core.c
 */

void rbug_notify_draw_blocked(struct rbug_context *rb_context);


#endif /* RBUG_CONTEXT_H */

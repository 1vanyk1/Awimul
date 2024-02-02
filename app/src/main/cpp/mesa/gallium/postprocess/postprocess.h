#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "../../include/pipe/p_state.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cso_context;
struct st_context_iface;

struct pp_queue_t;              /* Forward definition */
struct pp_program;

/* Less typing later on */
typedef void (*pp_func) (struct pp_queue_t *, struct pipe_resource *,
                         struct pipe_resource *, unsigned int);

/* Main functions */

/**
 * Note enabled is an array of values, one per filter stage.
 * Zero indicates the stage is disabled.  Non-zero indicates the
 * stage is enabled.  For some stages, the value controls quality.
 */
struct pp_queue_t *pp_init(struct pipe_context *pipe,
                           const unsigned int *enabled,
                           struct cso_context *,
                           struct st_context_iface *st);

void pp_run(struct pp_queue_t *, struct pipe_resource *,
            struct pipe_resource *, struct pipe_resource *);
void pp_free(struct pp_queue_t *);

void pp_init_fbos(struct pp_queue_t *, unsigned int, unsigned int);


/* The filters */

void pp_nocolor(struct pp_queue_t *, struct pipe_resource *,
                struct pipe_resource *, unsigned int);

void pp_jimenezmlaa(struct pp_queue_t *, struct pipe_resource *,
                    struct pipe_resource *, unsigned int);
void pp_jimenezmlaa_color(struct pp_queue_t *, struct pipe_resource *,
                          struct pipe_resource *, unsigned int);

/* The filter init functions */

bool pp_celshade_init(struct pp_queue_t *, unsigned int, unsigned int);

bool pp_nored_init(struct pp_queue_t *, unsigned int, unsigned int);
bool pp_nogreen_init(struct pp_queue_t *, unsigned int, unsigned int);
bool pp_noblue_init(struct pp_queue_t *, unsigned int, unsigned int);

bool pp_jimenezmlaa_init(struct pp_queue_t *, unsigned int, unsigned int);
bool pp_jimenezmlaa_init_color(struct pp_queue_t *, unsigned int,
                               unsigned int);

/* The filter free functions */

void pp_celshade_free(struct pp_queue_t *, unsigned int);
void pp_nocolor_free(struct pp_queue_t *, unsigned int);
void pp_jimenezmlaa_free(struct pp_queue_t *, unsigned int);


#ifdef __cplusplus
}
#endif

#endif

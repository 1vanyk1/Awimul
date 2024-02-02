#ifndef TR_TEXTURE_H_
#define TR_TEXTURE_H_


#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_state.h"

#include "tr_screen.h"
#include "../../util/u_threaded_context.h"

struct trace_context;


struct tr_list
{
    struct tr_list *next;
    struct tr_list *prev;
};

struct trace_surface
{
    struct pipe_surface base;

    struct pipe_surface *surface;

    struct tr_list list;
};


struct trace_sampler_view
{
    struct pipe_sampler_view base;
    unsigned refcount;

    struct pipe_sampler_view *sampler_view;
};


struct trace_transfer
{
    struct threaded_transfer base;

    struct pipe_transfer *transfer;
    struct pipe_context *pipe;

    struct tr_list list;

    void *map;
};


static inline struct trace_surface *
trace_surface(struct pipe_surface *surface)
{
    if (!surface)
        return NULL;
    return (struct trace_surface *)surface;
}


static inline struct trace_sampler_view *
trace_sampler_view(struct pipe_sampler_view *sampler_view)
{
    if (!sampler_view)
        return NULL;
    return (struct trace_sampler_view *)sampler_view;
}


static inline struct trace_transfer *
trace_transfer(struct pipe_transfer *transfer)
{
    if (!transfer)
        return NULL;
    return (struct trace_transfer *)transfer;
}


struct pipe_surface *
trace_surf_create(struct trace_context *tr_ctx,
                  struct pipe_resource *tr_res,
                  struct pipe_surface *surface);

void
trace_surf_destroy(struct trace_surface *tr_surf);

struct pipe_transfer *
trace_transfer_create(struct trace_context *tr_ctx,
                      struct pipe_resource *tr_res,
                      struct pipe_transfer *transfer);

void
trace_transfer_destroy(struct trace_context *tr_ctx,
                       struct trace_transfer *tr_trans);


#endif /* TR_TEXTURE_H_ */

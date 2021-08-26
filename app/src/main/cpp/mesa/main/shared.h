#ifndef SHARED_H
#define SHARED_H

struct gl_context;

void
_mesa_reference_shared_state(struct gl_context *ctx,
                             struct gl_shared_state **ptr,
                             struct gl_shared_state *state);


struct gl_shared_state *
_mesa_alloc_shared_state(struct gl_context *ctx);


#endif

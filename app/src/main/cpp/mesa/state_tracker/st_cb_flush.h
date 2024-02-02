#ifndef ST_CB_FLUSH_H
#define ST_CB_FLUSH_H


#include "../include/pipe/p_compiler.h"

struct dd_function_table;
struct pipe_fence_handle;
struct st_context;

extern void
st_init_flush_functions(struct pipe_screen *screen,
                        struct dd_function_table *functions);

void st_glFlush(struct gl_context *ctx, unsigned gallium_flush_flags);
void st_glFinish(struct gl_context *ctx);

extern void
st_flush(struct st_context *st,
         struct pipe_fence_handle **fence,
         unsigned flags);

extern void
st_finish(struct st_context *st);

extern void
st_install_device_reset_callback(struct st_context *st);


#endif /* ST_CB_FLUSH_H */

#ifndef ST_CB_CLEAR_H
#define ST_CB_CLEAR_H


struct dd_function_table;
struct st_context;

extern void
st_init_clear(struct st_context *st);


extern void
st_destroy_clear(struct st_context *st);


void st_Clear(struct gl_context *ctx, GLbitfield mask);

#endif /* ST_CB_CLEAR_H */


#ifndef ST_CB_XFORMFB_H
#define ST_CB_XFORMFB_H


struct gl_transform_feedback_object;
struct pipe_draw_indirect_info;

struct gl_transform_feedback_object *
st_new_transform_feedback(struct gl_context *ctx, GLuint name);

void
st_delete_transform_feedback(struct gl_context *ctx,
                             struct gl_transform_feedback_object *obj);

void
st_begin_transform_feedback(struct gl_context *ctx, GLenum mode,
                            struct gl_transform_feedback_object *obj);

void
st_pause_transform_feedback(struct gl_context *ctx,
                            struct gl_transform_feedback_object *obj);

void
st_resume_transform_feedback(struct gl_context *ctx,
                             struct gl_transform_feedback_object *obj);

void
st_end_transform_feedback(struct gl_context *ctx,
                          struct gl_transform_feedback_object *obj);

extern bool
st_transform_feedback_draw_init(struct gl_transform_feedback_object *obj,
                                unsigned stream,
                                struct pipe_draw_indirect_info *out);


#endif /* ST_CB_XFORMFB_H */

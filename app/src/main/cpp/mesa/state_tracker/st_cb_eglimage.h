#ifndef ST_CB_EGLIMAGE_H
#define ST_CB_EGLIMAGE_H


struct dd_function_table;

extern void
st_init_eglimage_functions(struct dd_function_table *functions,
                           bool has_egl_image_validate);

void st_egl_image_target_texture_2d(struct gl_context *ctx, GLenum target,
                                    struct gl_texture_object *texObj,
                                    struct gl_texture_image *texImage,
                                    GLeglImageOES image_handle);
void st_egl_image_target_tex_storage(struct gl_context *ctx, GLenum target,
                                     struct gl_texture_object *texObj,
                                     struct gl_texture_image *texImage,
                                     GLeglImageOES image_handle);
void st_egl_image_target_renderbuffer_storage(struct gl_context *ctx,
                                              struct gl_renderbuffer *rb,
                                              GLeglImageOES image_handle);
#endif /* ST_CB_EGLIMAGE_H */

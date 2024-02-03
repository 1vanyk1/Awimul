#ifndef ST_CB_TEXTURE_H
#define ST_CB_TEXTURE_H


#include "../main/glheader.h"

struct dd_function_table;
struct gl_context;
struct gl_texture_object;
struct pipe_context;
struct st_context;
struct st_texture_object;
struct gl_renderbuffer;
struct gl_pixelstore_attrib;
struct gl_memory_object;
struct gl_sampler_object;

extern enum pipe_texture_target
gl_target_to_pipe(GLenum target);

unsigned
st_get_blit_mask(GLenum srcFormat, GLenum dstFormat);

extern GLboolean
st_finalize_texture(struct gl_context *ctx,
                    struct pipe_context *pipe,
                    struct gl_texture_object *tObj,
                    GLuint cubeMapFace);

struct gl_texture_image *st_NewTextureImage(struct gl_context *ctx);
void st_DeleteTextureImage(struct gl_context *ctx, struct gl_texture_image *img);
struct gl_texture_object *st_NewTextureObject(struct gl_context *ctx,
                                              GLuint name, GLenum target);
void st_DeleteTextureObject(struct gl_context *ctx,
                            struct gl_texture_object *texObj);
void st_TextureReleaseAllSamplerViews(struct gl_context *ctx,
                                      struct gl_texture_object *texObj);
void st_FreeTextureImageBuffer(struct gl_context *ctx,
                               struct gl_texture_image *texImage);
void st_MapTextureImage(struct gl_context *ctx,
                        struct gl_texture_image *texImage,
                        GLuint slice, GLuint x, GLuint y, GLuint w, GLuint h,
                        GLbitfield mode,
                        GLubyte **mapOut, GLint *rowStrideOut);
void st_UnmapTextureImage(struct gl_context *ctx,
                          struct gl_texture_image *texImage,
                          GLuint slice);
GLboolean st_AllocTextureImageBuffer(struct gl_context *ctx,
                                     struct gl_texture_image *texImage);
void st_TexSubImage(struct gl_context *ctx, GLuint dims,
                    struct gl_texture_image *texImage,
                    GLint xoffset, GLint yoffset, GLint zoffset,
                    GLint width, GLint height, GLint depth,
                    GLenum format, GLenum type, const void *pixels,
                    const struct gl_pixelstore_attrib *unpack);
void st_TexImage(struct gl_context * ctx, GLuint dims,
                 struct gl_texture_image *texImage,
                 GLenum format, GLenum type, const void *pixels,
                 const struct gl_pixelstore_attrib *unpack);
void st_CompressedTexSubImage(struct gl_context *ctx, GLuint dims,
                              struct gl_texture_image *texImage,
                              GLint x, GLint y, GLint z,
                              GLsizei w, GLsizei h, GLsizei d,
                              GLenum format, GLsizei imageSize, const void *data);
void st_CompressedTexImage(struct gl_context *ctx, GLuint dims,
                           struct gl_texture_image *texImage,
                           GLsizei imageSize, const void *data);
void st_GetTexSubImage(struct gl_context * ctx,
                       GLint xoffset, GLint yoffset, GLint zoffset,
                       GLsizei width, GLsizei height, GLint depth,
                       GLenum format, GLenum type, void * pixels,
                       struct gl_texture_image *texImage);
void st_CopyTexSubImage(struct gl_context *ctx, GLuint dims,
                        struct gl_texture_image *texImage,
                        GLint destX, GLint destY, GLint slice,
                        struct gl_renderbuffer *rb,
                        GLint srcX, GLint srcY, GLsizei width, GLsizei height);
GLboolean st_AllocTextureStorage(struct gl_context *ctx,
                                 struct gl_texture_object *texObj,
                                 GLsizei levels, GLsizei width,
                                 GLsizei height, GLsizei depth);
GLboolean st_TestProxyTexImage(struct gl_context *ctx, GLenum target,
                               GLuint numLevels, GLint level,
                               mesa_format format, GLuint numSamples,
                               GLint width, GLint height, GLint depth);
GLboolean st_TextureView(struct gl_context *ctx,
                         struct gl_texture_object *texObj,
                         struct gl_texture_object *origTexObj);
void st_ClearTexSubImage(struct gl_context *ctx,
                         struct gl_texture_image *texImage,
                         GLint xoffset, GLint yoffset, GLint zoffset,
                         GLsizei width, GLsizei height, GLsizei depth,
                         const void *clearValue);
void st_TexParameter(struct gl_context *ctx,
                     struct gl_texture_object *texObj, GLenum pname);
GLboolean st_SetTextureStorageForMemoryObject(struct gl_context *ctx,
                                              struct gl_texture_object *texObj,
                                              struct gl_memory_object *memObj,
                                              GLsizei levels, GLsizei width,
                                              GLsizei height, GLsizei depth,
                                              GLuint64 offset);
GLuint64 st_NewTextureHandle(struct gl_context *ctx, struct gl_texture_object *texObj,
                             struct gl_sampler_object *sampObj);
void st_DeleteTextureHandle(struct gl_context *ctx, GLuint64 handle);
void st_MakeTextureHandleResident(struct gl_context *ctx, GLuint64 handle,
                                  bool resident);
GLuint64 st_NewImageHandle(struct gl_context *ctx, struct gl_image_unit *imgObj);
void st_DeleteImageHandle(struct gl_context *ctx, GLuint64 handle);
void st_MakeImageHandleResident(struct gl_context *ctx, GLuint64 handle,
                                GLenum access, bool resident);
GLboolean st_GetSparseTextureVirtualPageSize(struct gl_context *ctx,
                                             GLenum target, mesa_format format,
                                             unsigned index, int *x, int *y, int *z);
void st_TexturePageCommitment(struct gl_context *ctx,
                              struct gl_texture_object *tex_obj,
                              int level, int xoffset, int yoffset, int zoffset,
                              int width, int height, int depth, bool commit);
#endif /* ST_CB_TEXTURE_H */

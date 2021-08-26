#ifndef PBO_H
#define PBO_H


#include "glheader.h"

struct gl_context;
struct gl_pixelstore_attrib;

extern GLboolean
_mesa_validate_pbo_access(GLuint dimensions,
                          const struct gl_pixelstore_attrib *pack,
                          GLsizei width, GLsizei height, GLsizei depth,
                          GLenum format, GLenum type, GLsizei clientMemSize,
                          const GLvoid *ptr);

extern const GLvoid *
_mesa_map_pbo_source(struct gl_context *ctx,
                     const struct gl_pixelstore_attrib *unpack,
                     const GLvoid *src);

extern const GLvoid *
_mesa_map_validate_pbo_source(struct gl_context *ctx,
                              GLuint dimensions,
                              const struct gl_pixelstore_attrib *unpack,
                              GLsizei width, GLsizei height, GLsizei depth,
                              GLenum format, GLenum type, GLsizei clientMemSize,
                              const GLvoid *ptr, const char *where);

extern void
_mesa_unmap_pbo_source(struct gl_context *ctx,
                       const struct gl_pixelstore_attrib *unpack);

extern void *
_mesa_map_pbo_dest(struct gl_context *ctx,
                   const struct gl_pixelstore_attrib *pack,
                   GLvoid *dest);

extern GLvoid *
_mesa_map_validate_pbo_dest(struct gl_context *ctx,
                            GLuint dimensions,
                            const struct gl_pixelstore_attrib *unpack,
                            GLsizei width, GLsizei height, GLsizei depth,
                            GLenum format, GLenum type, GLsizei clientMemSize,
                            GLvoid *ptr, const char *where);

extern void
_mesa_unmap_pbo_dest(struct gl_context *ctx,
                     const struct gl_pixelstore_attrib *pack);


extern const GLvoid *
_mesa_validate_pbo_teximage(struct gl_context *ctx, GLuint dimensions,
                            GLsizei width, GLsizei height, GLsizei depth,
                            GLenum format, GLenum type, const GLvoid *pixels,
                            const struct gl_pixelstore_attrib *unpack,
                            const char *funcName);

extern const GLvoid *
_mesa_validate_pbo_compressed_teximage(struct gl_context *ctx,
                                       GLuint dimensions, GLsizei imageSize,
                                       const GLvoid *pixels,
                                       const struct gl_pixelstore_attrib *packing,
                                       const char *funcName);

extern void
_mesa_unmap_teximage_pbo(struct gl_context *ctx,
                         const struct gl_pixelstore_attrib *unpack);


extern bool
_mesa_validate_pbo_source(struct gl_context *ctx, GLuint dimensions,
                          const struct gl_pixelstore_attrib *unpack,
                          GLsizei width, GLsizei height, GLsizei depth,
                          GLenum format, GLenum type,
                          GLsizei clientMemSize,
                          const GLvoid *ptr, const char *where);

extern bool
_mesa_validate_pbo_source_compressed(struct gl_context *ctx, GLuint dimensions,
                                     const struct gl_pixelstore_attrib *unpack,
                                     GLsizei imageSize, const GLvoid *ptr,
                                     const char *where);

#endif

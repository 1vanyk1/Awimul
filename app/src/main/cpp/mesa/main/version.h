#ifndef VERSION_H
#define VERSION_H

#include <stdbool.h>
#include "glheader.h"
#include "menums.h"

struct gl_context;
struct gl_constants;
struct gl_extensions;

extern GLuint
_mesa_get_version(const struct gl_extensions *extensions,
                  struct gl_constants *consts, gl_api api);

extern void
_mesa_compute_version(struct gl_context *ctx);

extern bool
_mesa_override_gl_version_contextless(struct gl_constants *consts,
                                      gl_api *apiOut, GLuint *versionOut);

extern void
_mesa_override_gl_version(struct gl_context *ctx);

extern void
_mesa_override_glsl_version(struct gl_constants *consts);

extern void
_mesa_get_driver_uuid(struct gl_context *ctx, GLint *uuid);

extern void
_mesa_get_device_uuid(struct gl_context *ctx, GLint *uuid);

extern int
_mesa_get_shading_language_version(const struct gl_context *ctx,
                                   int index,
                                   char **versionOut);

#endif /* VERSION_H */

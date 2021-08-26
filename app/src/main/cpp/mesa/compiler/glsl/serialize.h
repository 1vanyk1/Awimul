#ifndef GLSL_SERIALIZE
#define GLSL_SERIALIZE

#include <stdbool.h>

struct blob;
struct blob_reader;
struct gl_context;
struct gl_shader_program;

#ifdef __cplusplus
extern "C" {
#endif

void
serialize_glsl_program(struct blob *blob, struct gl_context *ctx,
                       struct gl_shader_program *prog);

bool
deserialize_glsl_program(struct blob_reader *blob, struct gl_context *ctx,
                         struct gl_shader_program *prog);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GLSL_SERIALIZE */

#ifndef SHADER_CACHE_H
#define SHADER_CACHE_H

#include "../../util/disk_cache.h"

struct gl_context;
struct gl_shader_program;

void
shader_cache_write_program_metadata(struct gl_context *ctx,
                                    struct gl_shader_program *prog);

bool
shader_cache_read_program_metadata(struct gl_context *ctx,
                                   struct gl_shader_program *prog);

#endif /* SHADER_CACHE_H */

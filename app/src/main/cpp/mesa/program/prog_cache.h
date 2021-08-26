#ifndef PROG_CACHE_H
#define PROG_CACHE_H


#include "../main/glheader.h"


#ifdef __cplusplus
extern "C" {
#endif


struct gl_context;

/** Opaque type */
struct gl_program_cache;


extern struct gl_program_cache *
_mesa_new_program_cache(void);

extern void
_mesa_delete_program_cache(struct gl_context *ctx, struct gl_program_cache *pc);

extern void
_mesa_delete_shader_cache(struct gl_context *ctx,
                          struct gl_program_cache *cache);

extern struct gl_program *
_mesa_search_program_cache(struct gl_program_cache *cache,
                           const void *key, GLuint keysize);

extern void
_mesa_program_cache_insert(struct gl_context *ctx,
                           struct gl_program_cache *cache,
                           const void *key, GLuint keysize,
                           struct gl_program *program);

void
_mesa_shader_cache_insert(struct gl_context *ctx,
                          struct gl_program_cache *cache,
                          const void *key, GLuint keysize,
                          struct gl_shader_program *program);


#ifdef __cplusplus
}
#endif


#endif /* PROG_CACHE_H */

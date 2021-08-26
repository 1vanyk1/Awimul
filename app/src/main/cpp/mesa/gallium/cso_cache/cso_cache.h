#ifndef CSO_CACHE_H
#define CSO_CACHE_H

#include "../../include/pipe/p_context.h"
#include "../../include/pipe/p_state.h"

/* cso_hash.h is necessary for cso_hash_iter, as MSVC requires structures
 * returned by value to be fully defined */
#include "cso_hash.h"


#ifdef	__cplusplus
extern "C" {
#endif

enum cso_cache_type {
    CSO_RASTERIZER,
    CSO_BLEND,
    CSO_DEPTH_STENCIL_ALPHA,
    CSO_SAMPLER,
    CSO_VELEMENTS,
    CSO_CACHE_MAX,
};

typedef void (*cso_delete_cso_callback)(void *ctx, void *state,
                                        enum cso_cache_type type);

typedef void (*cso_state_callback)(void *ctx, void *obj);

typedef void (*cso_sanitize_callback)(struct cso_hash *hash,
                                      enum cso_cache_type type,
                                      int max_size,
                                      void *user_data);

struct cso_cache {
    struct cso_hash hashes[CSO_CACHE_MAX];
    int    max_size;

    cso_sanitize_callback sanitize_cb;
    void                 *sanitize_data;

    cso_delete_cso_callback delete_cso;
    void                    *delete_cso_ctx;
};

struct cso_blend {
    struct pipe_blend_state state;
    void *data;
};

struct cso_depth_stencil_alpha {
    struct pipe_depth_stencil_alpha_state state;
    void *data;
};

struct cso_rasterizer {
    struct pipe_rasterizer_state state;
    void *data;
};

struct cso_sampler {
    struct pipe_sampler_state state;
    void *data;
    unsigned hash_key;
};

struct cso_velems_state {
    unsigned count;
    struct pipe_vertex_element velems[PIPE_MAX_ATTRIBS];
};

struct cso_velements {
    struct cso_velems_state state;
    void *data;
};

void cso_cache_init(struct cso_cache *sc, struct pipe_context *pipe);
void cso_cache_delete(struct cso_cache *sc);

void cso_cache_set_sanitize_callback(struct cso_cache *sc,
                                     cso_sanitize_callback cb,
                                     void *user_data);
void cso_cache_set_delete_cso_callback(struct cso_cache *sc,
                                       cso_delete_cso_callback delete_cso,
                                       void *ctx);

struct cso_hash_iter cso_insert_state(struct cso_cache *sc,
                                      unsigned hash_key, enum cso_cache_type type,
                                      void *state);
struct cso_hash_iter cso_find_state(struct cso_cache *sc,
                                    unsigned hash_key, enum cso_cache_type type);
struct cso_hash_iter cso_find_state_template(struct cso_cache *sc,
                                             unsigned hash_key, enum cso_cache_type type,
                                             void *templ, unsigned size);
void cso_set_maximum_cache_size(struct cso_cache *sc, int number);
void cso_delete_state(struct pipe_context *pipe, void *state,
                      enum cso_cache_type type);

static inline unsigned
cso_construct_key(void *key, int key_size)
{
    unsigned hash = 0, *ikey = (unsigned *)key;
    unsigned num_elements = key_size / 4;

    assert(key_size % 4 == 0);

    for (unsigned i = 0; i < num_elements; i++)
        hash ^= ikey[i];

    return hash;
}

#ifdef	__cplusplus
}
#endif

#endif

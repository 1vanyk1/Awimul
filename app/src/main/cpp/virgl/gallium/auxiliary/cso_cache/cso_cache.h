#ifndef CSO_CACHE_H
#define CSO_CACHE_H

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

typedef void (*cso_state_callback)(void *ctx, void *obj);

typedef void (*cso_sanitize_callback)(struct cso_hash *hash,
                                      enum cso_cache_type type,
                                      int max_size,
                                      void *user_data);

struct cso_cache;

struct cso_blend {
    struct pipe_blend_state state;
    void *data;
    cso_state_callback delete_state;
    struct pipe_context *context;
};

struct cso_depth_stencil_alpha {
    struct pipe_depth_stencil_alpha_state state;
    void *data;
    cso_state_callback delete_state;
    struct pipe_context *context;
};

struct cso_rasterizer {
    struct pipe_rasterizer_state state;
    void *data;
    cso_state_callback delete_state;
    struct pipe_context *context;
};

struct cso_sampler {
    struct pipe_sampler_state state;
    void *data;
    cso_state_callback delete_state;
    struct pipe_context *context;
};

struct cso_velems_state {
    unsigned count;
    struct pipe_vertex_element velems[PIPE_MAX_ATTRIBS];
};

struct cso_velements {
    struct cso_velems_state state;
    void *data;
    cso_state_callback delete_state;
    struct pipe_context *context;
};

unsigned cso_construct_key(void *item, int item_size);

struct cso_cache *cso_cache_create(void);
void cso_cache_delete(struct cso_cache *sc);

void cso_cache_set_sanitize_callback(struct cso_cache *sc,
                                     cso_sanitize_callback cb,
                                     void *user_data);

struct cso_hash_iter cso_insert_state(struct cso_cache *sc,
                                      unsigned hash_key, enum cso_cache_type type,
                                      void *state);
struct cso_hash_iter cso_find_state(struct cso_cache *sc,
                                    unsigned hash_key, enum cso_cache_type type);
struct cso_hash_iter cso_find_state_template(struct cso_cache *sc,
                                             unsigned hash_key, enum cso_cache_type type,
                                             void *templ, unsigned size);
void cso_for_each_state(struct cso_cache *sc, enum cso_cache_type type,
                        cso_state_callback func, void *user_data);
void * cso_take_state(struct cso_cache *sc, unsigned hash_key,
                      enum cso_cache_type type);

void cso_set_maximum_cache_size(struct cso_cache *sc, int number);
int cso_maximum_cache_size(const struct cso_cache *sc);

#ifdef	__cplusplus
}
#endif

#endif

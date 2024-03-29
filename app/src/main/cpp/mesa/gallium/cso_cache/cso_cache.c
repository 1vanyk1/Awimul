#include "../../util/u_debug.h"

#include "../../util/u_memory.h"

#include "cso_cache.h"
#include "cso_hash.h"


static inline struct cso_hash *_cso_hash_for_type(struct cso_cache *sc, enum cso_cache_type type)
{
    return &sc->hashes[type];
}

/* Default delete callback. It can also be used by custom callbacks. */
void cso_delete_state(struct pipe_context *pipe, void *state,
                      enum cso_cache_type type)
{
    switch (type) {
        case CSO_BLEND:
            pipe->delete_blend_state(pipe, ((struct cso_blend*)state)->data);
            break;
        case CSO_SAMPLER:
            pipe->delete_sampler_state(pipe, ((struct cso_sampler*)state)->data);
            break;
        case CSO_DEPTH_STENCIL_ALPHA:
            pipe->delete_depth_stencil_alpha_state(pipe,
                                                   ((struct cso_depth_stencil_alpha*)state)->data);
            break;
        case CSO_RASTERIZER:
            pipe->delete_rasterizer_state(pipe, ((struct cso_rasterizer*)state)->data);
            break;
        case CSO_VELEMENTS:
            pipe->delete_vertex_elements_state(pipe, ((struct cso_velements*)state)->data);
            break;
        default:
            assert(0);
    }

    FREE(state);
}


static inline void sanitize_hash(struct cso_cache *sc,
                                 struct cso_hash *hash,
                                 enum cso_cache_type type,
                                 int max_size)
{
    if (sc->sanitize_cb)
        sc->sanitize_cb(hash, type, max_size, sc->sanitize_data);
}


static inline void sanitize_cb(struct cso_hash *hash, enum cso_cache_type type,
                               int max_size, void *user_data)
{
    struct cso_cache *cache = (struct cso_cache *)user_data;

    /* if we're approach the maximum size, remove fourth of the entries
     * otherwise every subsequent call will go through the same */
    int hash_size = cso_hash_size(hash);
    int max_entries = (max_size > hash_size) ? max_size : hash_size;
    int to_remove =  (max_size < max_entries) * max_entries/4;
    if (hash_size > max_size)
        to_remove += hash_size - max_size;
    while (to_remove) {
        /*remove elements until we're good */
        /*fixme: currently we pick the nodes to remove at random*/
        struct cso_hash_iter iter = cso_hash_first_node(hash);
        void  *cso = cso_hash_take(hash, cso_hash_iter_key(iter));
        cache->delete_cso(cache->delete_cso_ctx, cso, type);
        --to_remove;
    }
}

struct cso_hash_iter
cso_insert_state(struct cso_cache *sc,
                 unsigned hash_key, enum cso_cache_type type,
                 void *state)
{
    struct cso_hash *hash = _cso_hash_for_type(sc, type);
    sanitize_hash(sc, hash, type, sc->max_size);

    return cso_hash_insert(hash, hash_key, state);
}

struct cso_hash_iter
cso_find_state(struct cso_cache *sc,
               unsigned hash_key, enum cso_cache_type type)
{
    struct cso_hash *hash = _cso_hash_for_type(sc, type);

    return cso_hash_find(hash, hash_key);
}


void *cso_hash_find_data_from_template( struct cso_hash *hash,
                                        unsigned hash_key,
                                        void *templ,
                                        int size )
{
    struct cso_hash_iter iter = cso_hash_find(hash, hash_key);
    while (!cso_hash_iter_is_null(iter)) {
        void *iter_data = cso_hash_iter_data(iter);
        if (!memcmp(iter_data, templ, size)) {
            /* We found a match
             */
            return iter_data;
        }
        iter = cso_hash_iter_next(iter);
    }
    return NULL;
}


struct cso_hash_iter cso_find_state_template(struct cso_cache *sc,
                                             unsigned hash_key, enum cso_cache_type type,
                                             void *templ, unsigned size)
{
    struct cso_hash_iter iter = cso_find_state(sc, hash_key, type);
    while (!cso_hash_iter_is_null(iter)) {
        void *iter_data = cso_hash_iter_data(iter);
        if (!memcmp(iter_data, templ, size))
            return iter;
        iter = cso_hash_iter_next(iter);
    }
    return iter;
}

void cso_cache_init(struct cso_cache *sc, struct pipe_context *pipe)
{
    memset(sc, 0, sizeof(*sc));

    sc->max_size           = 4096;
    for (int i = 0; i < CSO_CACHE_MAX; i++)
        cso_hash_init(&sc->hashes[i]);

    sc->sanitize_cb        = sanitize_cb;
    sc->sanitize_data      = sc;
    sc->delete_cso = (cso_delete_cso_callback)cso_delete_state;
    sc->delete_cso_ctx = pipe;
}

static void cso_delete_all(struct cso_cache *sc, enum cso_cache_type type)
{
    struct cso_hash *hash = _cso_hash_for_type(sc, type);
    struct cso_hash_iter iter;

    iter = cso_hash_first_node(hash);
    while (!cso_hash_iter_is_null(iter)) {
        void *state = cso_hash_iter_data(iter);
        iter = cso_hash_iter_next(iter);
        if (state) {
            sc->delete_cso(sc->delete_cso_ctx, state, type);
        }
    }
}

void cso_cache_delete(struct cso_cache *sc)
{
    int i;

    /* delete driver data */
    cso_delete_all(sc, CSO_BLEND);
    cso_delete_all(sc, CSO_DEPTH_STENCIL_ALPHA);
    cso_delete_all(sc, CSO_RASTERIZER);
    cso_delete_all(sc, CSO_SAMPLER);
    cso_delete_all(sc, CSO_VELEMENTS);

    for (i = 0; i < CSO_CACHE_MAX; i++)
        cso_hash_deinit(&sc->hashes[i]);
}

void cso_set_maximum_cache_size(struct cso_cache *sc, int number)
{
    int i;

    sc->max_size = number;

    for (i = 0; i < CSO_CACHE_MAX; i++)
        sanitize_hash(sc, &sc->hashes[i], i, sc->max_size);
}

void cso_cache_set_sanitize_callback(struct cso_cache *sc,
                                     cso_sanitize_callback cb,
                                     void *user_data)
{
    sc->sanitize_cb   = cb;
    sc->sanitize_data = user_data;
}

void cso_cache_set_delete_cso_callback(struct cso_cache *sc,
                                       cso_delete_cso_callback delete_cso,
                                       void *ctx)
{
    sc->delete_cso = delete_cso;
    sc->delete_cso_ctx = ctx;
}

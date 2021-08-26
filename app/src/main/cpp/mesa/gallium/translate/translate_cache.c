#include "../../util/u_memory.h"
#include "../../include/pipe/p_state.h"
#include "translate.h"
#include "translate_cache.h"

#include "../cso_cache/cso_cache.h"
#include "../cso_cache/cso_hash.h"

struct translate_cache {
    struct cso_hash hash;
};

struct translate_cache * translate_cache_create( void )
{
    struct translate_cache *cache = MALLOC_STRUCT(translate_cache);
    if (!cache) {
        return NULL;
    }

    cso_hash_init(&cache->hash);
    return cache;
}


static inline void delete_translates(struct translate_cache *cache)
{
    struct cso_hash *hash = &cache->hash;
    struct cso_hash_iter iter = cso_hash_first_node(hash);
    while (!cso_hash_iter_is_null(iter)) {
        struct translate *state = (struct translate*)cso_hash_iter_data(iter);
        iter = cso_hash_iter_next(iter);
        if (state) {
            state->release(state);
        }
    }
}

void translate_cache_destroy(struct translate_cache *cache)
{
    delete_translates(cache);
    cso_hash_deinit(&cache->hash);
    FREE(cache);
}


static inline unsigned translate_hash_key_size(struct translate_key *key)
{
    unsigned size = sizeof(struct translate_key) -
                    sizeof(struct translate_element) * (TRANSLATE_MAX_ATTRIBS - key->nr_elements);
    return size;
}

static inline unsigned create_key(struct translate_key *key)
{
    unsigned hash_key;
    unsigned size = translate_hash_key_size(key);
    /*debug_printf("key size = %d, (els = %d)\n",
      size, key->nr_elements);*/
    hash_key = cso_construct_key(key, size);
    return hash_key;
}

struct translate * translate_cache_find(struct translate_cache *cache,
                                        struct translate_key *key)
{
    unsigned hash_key = create_key(key);
    struct translate *translate = (struct translate*)
            cso_hash_find_data_from_template(&cache->hash,
                                             hash_key,
                                             key, sizeof(*key));

    if (!translate) {
        /* create/insert */
        translate = translate_create(key);
        cso_hash_insert(&cache->hash, hash_key, translate);
    }

    return translate;
}

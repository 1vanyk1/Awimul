#include "../../../mesa/pipe/p_compiler.h"
#include "../../../mesa/util/u_debug.h"

#include "../cso_cache/cso_hash.h"

#include "../../../mesa/util/u_memory.h"
#include "u_pointer.h"
#include "u_hash_table.h"


struct util_hash_table
{
    struct cso_hash *cso;

    /** Hash function */
    unsigned (*hash)(void *key);

    /** Compare two keys */
    int (*compare)(void *key1, void *key2);

    /** free value */
    void (*destroy)(void *value);
};


struct util_hash_table_item
{
    void *key;
    void *value;
};


static inline struct util_hash_table_item *
util_hash_table_item(struct cso_hash_iter iter)
{
    return (struct util_hash_table_item *)cso_hash_iter_data(iter);
}


struct util_hash_table *
util_hash_table_create(unsigned (*hash)(void *key),
                       int (*compare)(void *key1, void *key2),
                       void (*destroy)(void *value))
{
    struct util_hash_table *ht;

    ht = MALLOC_STRUCT(util_hash_table);
    if(!ht)
        return NULL;

    ht->cso = cso_hash_create();
    if(!ht->cso) {
        FREE(ht);
        return NULL;
    }

    ht->hash = hash;
    ht->compare = compare;
    ht->destroy = destroy;

    return ht;
}


static inline struct cso_hash_iter
util_hash_table_find_iter(struct util_hash_table *ht,
                          void *key,
                          unsigned key_hash)
{
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;

    iter = cso_hash_find(ht->cso, key_hash);
    while (!cso_hash_iter_is_null(iter)) {
        item = (struct util_hash_table_item *)cso_hash_iter_data(iter);
        if (!ht->compare(item->key, key))
            break;
        iter = cso_hash_iter_next(iter);
    }

    return iter;
}


static inline struct util_hash_table_item *
util_hash_table_find_item(struct util_hash_table *ht,
                          void *key,
                          unsigned key_hash)
{
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;

    iter = cso_hash_find(ht->cso, key_hash);
    while (!cso_hash_iter_is_null(iter)) {
        item = (struct util_hash_table_item *)cso_hash_iter_data(iter);
        if (!ht->compare(item->key, key))
            return item;
        iter = cso_hash_iter_next(iter);
    }

    return NULL;
}


enum pipe_error
util_hash_table_set(struct util_hash_table *ht,
                    void *key,
                    void *value)
{
    unsigned key_hash;
    struct util_hash_table_item *item;
    struct cso_hash_iter iter;

    assert(ht);
    if (!ht)
        return PIPE_ERROR_BAD_INPUT;

    key_hash = ht->hash(key);

    item = util_hash_table_find_item(ht, key, key_hash);
    if(item) {
        ht->destroy(item->value);
        item->value = value;
        return PIPE_OK;
    }

    item = MALLOC_STRUCT(util_hash_table_item);
    if(!item)
        return PIPE_ERROR_OUT_OF_MEMORY;

    item->key = key;
    item->value = value;

    iter = cso_hash_insert(ht->cso, key_hash, item);
    if(cso_hash_iter_is_null(iter)) {
        FREE(item);
        return PIPE_ERROR_OUT_OF_MEMORY;
    }

    return PIPE_OK;
}


void *
util_hash_table_get(struct util_hash_table *ht,
                    void *key)
{
    unsigned key_hash;
    struct util_hash_table_item *item;

    assert(ht);
    if (!ht)
        return NULL;

    key_hash = ht->hash(key);

    item = util_hash_table_find_item(ht, key, key_hash);
    if(!item)
        return NULL;

    return item->value;
}


void
util_hash_table_remove(struct util_hash_table *ht,
                       void *key)
{
    unsigned key_hash;
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;

    assert(ht);
    if (!ht)
        return;

    key_hash = ht->hash(key);

    iter = util_hash_table_find_iter(ht, key, key_hash);
    if(cso_hash_iter_is_null(iter))
        return;

    item = util_hash_table_item(iter);
    assert(item);
    ht->destroy(item->value);
    FREE(item);

    cso_hash_erase(ht->cso, iter);
}


void
util_hash_table_clear(struct util_hash_table *ht)
{
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;

    assert(ht);
    if (!ht)
        return;

    iter = cso_hash_first_node(ht->cso);
    while (!cso_hash_iter_is_null(iter)) {
        item = (struct util_hash_table_item *)cso_hash_take(ht->cso, cso_hash_iter_key(iter));
        ht->destroy(item->value);
        FREE(item);
        iter = cso_hash_first_node(ht->cso);
    }
}


enum pipe_error
util_hash_table_foreach(struct util_hash_table *ht,
                        enum pipe_error (*callback)
                                (void *key, void *value, void *data),
                        void *data)
{
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;
    enum pipe_error result;

    assert(ht);
    if (!ht)
        return PIPE_ERROR_BAD_INPUT;

    iter = cso_hash_first_node(ht->cso);
    while (!cso_hash_iter_is_null(iter)) {
        item = (struct util_hash_table_item *)cso_hash_iter_data(iter);
        result = callback(item->key, item->value, data);
        if(result != PIPE_OK)
            return result;
        iter = cso_hash_iter_next(iter);
    }

    return PIPE_OK;
}


void
util_hash_table_destroy(struct util_hash_table *ht)
{
    struct cso_hash_iter iter;
    struct util_hash_table_item *item;

    assert(ht);
    if (!ht)
        return;

    iter = cso_hash_first_node(ht->cso);
    while (!cso_hash_iter_is_null(iter)) {
        item = (struct util_hash_table_item *)cso_hash_iter_data(iter);
        ht->destroy(item->value);
        FREE(item);
        iter = cso_hash_iter_next(iter);
    }

    cso_hash_delete(ht->cso);

    FREE(ht);
}

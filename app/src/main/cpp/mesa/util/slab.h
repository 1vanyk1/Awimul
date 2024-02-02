#ifndef SLAB_H
#define SLAB_H

#include "simple_mtx.h"

#ifdef __cplusplus
extern "C" {
#endif

struct slab_element_header;
struct slab_page_header;

struct slab_parent_pool {
    simple_mtx_t mutex;
    unsigned element_size;
    unsigned num_elements;
};

struct slab_child_pool {
    struct slab_parent_pool *parent;

    struct slab_page_header *pages;

    /* Free elements. */
    struct slab_element_header *free;

    /* Elements that are owned by this pool but were freed with a different
     * pool as the argument to slab_free.
     *
     * This list is protected by the parent mutex.
     */
    struct slab_element_header *migrated;
};

void slab_create_parent(struct slab_parent_pool *parent,
                        unsigned item_size,
                        unsigned num_items);
void slab_destroy_parent(struct slab_parent_pool *parent);
void slab_create_child(struct slab_child_pool *pool,
                       struct slab_parent_pool *parent);
void slab_destroy_child(struct slab_child_pool *pool);
void *slab_alloc(struct slab_child_pool *pool);
void slab_free(struct slab_child_pool *pool, void *ptr);

struct slab_mempool {
    struct slab_parent_pool parent;
    struct slab_child_pool child;
};

void slab_create(struct slab_mempool *mempool,
                 unsigned item_size,
                 unsigned num_items);
void slab_destroy(struct slab_mempool *mempool);
void *slab_alloc_st(struct slab_mempool *mempool);
void slab_free_st(struct slab_mempool *mempool, void *ptr);

#ifdef __cplusplus
}
#endif

#endif

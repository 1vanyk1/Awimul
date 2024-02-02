#ifndef VREND_OBJECT_H
#define VREND_OBJECT_H

#include "virgl_protocol.h"

struct vrend_resource;

struct util_hash_table *vrend_object_init_ctx_table(void);
void vrend_object_fini_ctx_table(struct util_hash_table *ctx_hash);

void vrend_object_remove(struct util_hash_table *handle_hash, uint32_t handle, enum virgl_object_type obj);
void *vrend_object_lookup(struct util_hash_table *handle_hash, uint32_t handle, enum virgl_object_type obj);
uint32_t vrend_object_insert(struct util_hash_table *handle_hash,
                             void *data,
                             uint32_t handle,
                             enum virgl_object_type type);

void vrend_object_set_destroy_callback(int type, void (*cb)(void *));

struct util_hash_table *vrend_ctx_resource_init_table(void);
void vrend_ctx_resource_fini_table(struct util_hash_table *res_hash);

void vrend_ctx_resource_insert(struct util_hash_table *res_hash,
                               uint32_t res_id,
                               struct vrend_resource *res);
void vrend_ctx_resource_remove(struct util_hash_table *res_hash,
                               uint32_t res_id);
struct vrend_resource *vrend_ctx_resource_lookup(struct util_hash_table *res_hash,
                                                 uint32_t res_id);

#endif

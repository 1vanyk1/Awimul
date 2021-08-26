#ifndef U_HASH_TABLE_H_
#define U_HASH_TABLE_H_


#include "../include/pipe/p_defines.h"
#include "hash_table.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a hash table where the keys are generic pointers.
 */
struct hash_table *
util_hash_table_create_ptr_keys(void);


/**
 * Create a hash table where the keys are device FDs.
 */
struct hash_table *
util_hash_table_create_fd_keys(void);


void *
util_hash_table_get(struct hash_table *ht,
                    void *key);


enum pipe_error
util_hash_table_foreach(struct hash_table *ht,
                        enum pipe_error (*callback)
                                (void *key, void *value, void *data),
                        void *data);

#ifdef __cplusplus
}
#endif

#endif /* U_HASH_TABLE_H_ */

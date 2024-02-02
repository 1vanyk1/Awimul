#ifndef U_HASH_TABLE_H_
#define U_HASH_TABLE_H_


#include "../../include/pipe/p_defines.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Generic purpose hash table.
 */
struct util_hash_table;


/**
 * Create an hash table.
 *
 * @param hash hash function
 * @param compare should return 0 for two equal keys.
 */
struct util_hash_table *
util_hash_table_create(unsigned (*hash)(void *key),
                       int (*compare)(void *key1, void *key2),
                       void (*destroy)(void *value));


enum pipe_error
util_hash_table_set(struct util_hash_table *ht,
                    void *key,
                    void *value);

void *
util_hash_table_get(struct util_hash_table *ht,
                    void *key);


void
util_hash_table_remove(struct util_hash_table *ht,
                       void *key);


void
util_hash_table_clear(struct util_hash_table *ht);


enum pipe_error
util_hash_table_foreach(struct util_hash_table *ht,
                        enum pipe_error (*callback)
                                (void *key, void *value, void *data),
                        void *data);

void
util_hash_table_destroy(struct util_hash_table *ht);


#ifdef __cplusplus
}
#endif

#endif /* U_HASH_TABLE_H_ */

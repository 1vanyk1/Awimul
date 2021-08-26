#ifndef U_HANDLE_BITMASK_H_
#define U_HANDLE_BITMASK_H_


#include "../include/pipe/p_compiler.h"


#ifdef __cplusplus
extern "C" {
#endif


#define UTIL_BITMASK_INVALID_INDEX (~0U)


/**
 * Abstract data type to represent arbitrary set of bits.
 */
struct util_bitmask;


struct util_bitmask *
util_bitmask_create(void);


/**
 * Search a cleared bit and set it.
 *
 * It searches for the first cleared bit.
 *
 * Returns the bit index on success, or UTIL_BITMASK_INVALID_INDEX on out of
 * memory growing the bitmask.
 */
unsigned
util_bitmask_add(struct util_bitmask *bm);

/**
 * Set a bit.
 *
 * Returns the input index on success, or UTIL_BITMASK_INVALID_INDEX on out of
 * memory growing the bitmask.
 */
unsigned
util_bitmask_set(struct util_bitmask *bm,
                 unsigned index);

void
util_bitmask_clear(struct util_bitmask *bm,
                   unsigned index);

boolean
util_bitmask_get(struct util_bitmask *bm,
                 unsigned index);


void
util_bitmask_destroy(struct util_bitmask *bm);


/**
 * Search for the first set bit.
 *
 * Returns UTIL_BITMASK_INVALID_INDEX if a set bit cannot be found.
 */
unsigned
util_bitmask_get_first_index(struct util_bitmask *bm);


/**
 * Search for the first set bit, starting from the giving index.
 *
 * Returns UTIL_BITMASK_INVALID_INDEX if a set bit cannot be found.
 */
unsigned
util_bitmask_get_next_index(struct util_bitmask *bm,
                            unsigned index);


#ifdef __cplusplus
}
#endif

#endif /* U_HANDLE_BITMASK_H_ */

#ifndef U_IDALLOC_H
#define U_IDALLOC_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct util_idalloc
{
    uint32_t *data;
    unsigned num_elements;
    unsigned lowest_free_idx;
};

void
util_idalloc_init(struct util_idalloc *buf);

void
util_idalloc_fini(struct util_idalloc *buf);

void
util_idalloc_resize(struct util_idalloc *buf, unsigned new_num_elements);

unsigned
util_idalloc_alloc(struct util_idalloc *buf);

void
util_idalloc_free(struct util_idalloc *buf, unsigned id);

void
util_idalloc_reserve(struct util_idalloc *buf, unsigned id);

#ifdef __cplusplus
}
#endif

#endif /* U_IDALLOC_H */

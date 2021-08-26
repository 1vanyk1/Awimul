#include "u_idalloc.h"
#include "u_math.h"
#include <stdlib.h>

void
util_idalloc_init(struct util_idalloc *buf)
{
    memset(buf, 0, sizeof(*buf));
}

void
util_idalloc_fini(struct util_idalloc *buf)
{
    if (buf->data)
        free(buf->data);
}

void
util_idalloc_resize(struct util_idalloc *buf, unsigned new_num_elements)
{
    new_num_elements = align(new_num_elements, 32);

    if (new_num_elements > buf->num_elements) {
        unsigned i;

        buf->data = realloc(buf->data,
                            (new_num_elements / 32) * sizeof(*buf->data));

        for (i = buf->num_elements / 32; i < new_num_elements / 32; i++)
            buf->data[i] = 0;
        buf->num_elements = new_num_elements;
    }
}

unsigned
util_idalloc_alloc(struct util_idalloc *buf)
{
    unsigned num_elements = buf->num_elements;

    for (unsigned i = buf->lowest_free_idx; i < num_elements / 32; i++) {
        if (buf->data[i] == 0xffffffff)
            continue;

        unsigned bit = ffs(~buf->data[i]) - 1;
        buf->data[i] |= 1u << bit;
        buf->lowest_free_idx = i;
        return i * 32 + bit;
    }

    /* No slots available, resize and return the first free. */
    util_idalloc_resize(buf, num_elements * 2);

    buf->lowest_free_idx = num_elements / 32;

    buf->data[num_elements / 32] |= 1 << (num_elements % 32);

    return num_elements;
}

void
util_idalloc_free(struct util_idalloc *buf, unsigned id)
{
    assert(id < buf->num_elements);
    unsigned idx = id / 32;
    buf->lowest_free_idx = MIN2(idx, buf->lowest_free_idx);
    buf->data[idx] &= ~(1 << (id % 32));
}

void
util_idalloc_reserve(struct util_idalloc *buf, unsigned id)
{
    if (id >= buf->num_elements)
        util_idalloc_resize(buf, id * 2);
    assert((buf->data[id / 32] & (1u << (id % 32))) == 0);
    buf->data[id / 32] |= 1u << (id % 32);
}

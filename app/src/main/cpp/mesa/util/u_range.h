#ifndef U_RANGE_H
#define U_RANGE_H

#include "../os/os_thread.h"
#include "../include/pipe/p_state.h"
#include "../include/pipe/p_screen.h"
#include "u_atomic.h"
#include "u_math.h"
#include "simple_mtx.h"

struct util_range {
    unsigned start; /* inclusive */
    unsigned end; /* exclusive */

    /* for the range to be consistent with multiple contexts: */
    simple_mtx_t write_mutex;
};


static inline void
util_range_set_empty(struct util_range *range)
{
    range->start = ~0;
    range->end = 0;
}

/* This is like a union of two sets. */
static inline void
util_range_add(struct pipe_resource *resource, struct util_range *range,
               unsigned start, unsigned end)
{
    if (start < range->start || end > range->end) {
        if (resource->flags & PIPE_RESOURCE_FLAG_SINGLE_THREAD_USE ||
            p_atomic_read(&resource->screen->num_contexts) == 1) {
            range->start = MIN2(start, range->start);
            range->end = MAX2(end, range->end);
        } else {
            simple_mtx_lock(&range->write_mutex);
            range->start = MIN2(start, range->start);
            range->end = MAX2(end, range->end);
            simple_mtx_unlock(&range->write_mutex);
        }
    }
}

static inline boolean
util_ranges_intersect(const struct util_range *range,
                      unsigned start, unsigned end)
{
    return MAX2(start, range->start) < MIN2(end, range->end);
}


/* Init/deinit */

static inline void
util_range_init(struct util_range *range)
{
    (void) simple_mtx_init(&range->write_mutex, mtx_plain);
    util_range_set_empty(range);
}

static inline void
util_range_destroy(struct util_range *range)
{
    simple_mtx_destroy(&range->write_mutex);
}

#endif

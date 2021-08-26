#ifndef U_VECTOR_H
#define U_VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO - move to u_math.h - name it better etc */
static inline uint32_t
u_align_u32(uint32_t v, uint32_t a)
{
    assert(a != 0 && a == (a & -((int32_t) a)));
    return (v + a - 1) & ~(a - 1);
}

struct u_vector {
    uint32_t head;
    uint32_t tail;
    uint32_t element_size;
    uint32_t size;
    void *data;
};

int u_vector_init(struct u_vector *queue, uint32_t element_size, uint32_t size);
void *u_vector_add(struct u_vector *queue);
void *u_vector_remove(struct u_vector *queue);

static inline int
u_vector_length(struct u_vector *queue)
{
    return (queue->head - queue->tail) / queue->element_size;
}

static inline void *
u_vector_head(struct u_vector *vector)
{
    assert(vector->tail < vector->head);
    return (void *)((char *)vector->data +
                    ((vector->head - vector->element_size) &
                     (vector->size - 1)));
}

static inline void *
u_vector_tail(struct u_vector *vector)
{
    return (void *)((char *)vector->data + (vector->tail & (vector->size - 1)));
}

static inline void
u_vector_finish(struct u_vector *queue)
{
    free(queue->data);
}

#define u_vector_foreach(elem, queue)                                  \
   STATIC_ASSERT(__builtin_types_compatible_p(__typeof__(queue), struct u_vector *)); \
   for (uint32_t __u_vector_offset = (queue)->tail;                                \
        elem = (void *)((char *)(queue)->data + (__u_vector_offset & ((queue)->size - 1))), __u_vector_offset != (queue)->head; \
        __u_vector_offset += (queue)->element_size)

#ifdef __cplusplus
}
#endif

#endif


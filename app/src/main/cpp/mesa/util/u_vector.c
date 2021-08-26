#include <string.h>
#include "u_math.h"
#include "u_vector.h"

/** @file u_vector.c
 *
 * A dynamically growable, circular buffer.  Elements are added at head and
 * removed from tail. head and tail are free-running uint32_t indices and we
 * only compute the modulo with size when accessing the array.  This way,
 * number of bytes in the queue is always head - tail, even in case of
 * wraparound.
 */

int
u_vector_init(struct u_vector *vector, uint32_t element_size, uint32_t size)
{
    assert(util_is_power_of_two_nonzero(size));
    assert(element_size < size && util_is_power_of_two_nonzero(element_size));

    vector->head = 0;
    vector->tail = 0;
    vector->element_size = element_size;
    vector->size = size;
    vector->data = malloc(size);

    return vector->data != NULL;
}

void *
u_vector_add(struct u_vector *vector)
{
    uint32_t offset, size, split, src_tail, dst_tail;
    void *data;

    if (vector->head - vector->tail == vector->size) {
        size = vector->size * 2;
        data = malloc(size);
        if (data == NULL)
            return NULL;
        src_tail = vector->tail & (vector->size - 1);
        dst_tail = vector->tail & (size - 1);
        if (src_tail == 0) {
            /* Since we know that the vector is full, this means that it's
             * linear from start to end so we can do one copy.
             */
            memcpy((char *)data + dst_tail, vector->data, vector->size);
        } else {
            /* In this case, the vector is split into two pieces and we have
             * to do two copies.  We have to be careful to make sure each
             * piece goes to the right locations.  Thanks to the change in
             * size, it may or may not still wrap around.
             */
            split = u_align_u32(vector->tail, vector->size);
            assert(vector->tail <= split && split < vector->head);
            memcpy((char *)data + dst_tail, (char *)vector->data + src_tail,
                   split - vector->tail);
            memcpy((char *)data + (split & (size - 1)), vector->data,
                   vector->head - split);
        }
        free(vector->data);
        vector->data = data;
        vector->size = size;
    }

    assert(vector->head - vector->tail < vector->size);

    offset = vector->head & (vector->size - 1);
    vector->head += vector->element_size;

    return (char *)vector->data + offset;
}

void *
u_vector_remove(struct u_vector *vector)
{
    uint32_t offset;

    if (vector->head == vector->tail)
        return NULL;

    assert(vector->head - vector->tail <= vector->size);

    offset = vector->tail & (vector->size - 1);
    vector->tail += vector->element_size;

    return (char *)vector->data + offset;
}

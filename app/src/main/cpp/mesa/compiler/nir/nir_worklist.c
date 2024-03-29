#include "nir_worklist.h"

void
nir_block_worklist_init(nir_block_worklist *w, unsigned num_blocks,
                        void *mem_ctx)
{
    w->size = num_blocks;
    w->count = 0;
    w->start = 0;

    w->blocks_present = rzalloc_array(mem_ctx, BITSET_WORD,
                                      BITSET_WORDS(num_blocks));
    w->blocks = rzalloc_array(mem_ctx, nir_block *, num_blocks);
}

void
nir_block_worklist_fini(nir_block_worklist *w)
{
    ralloc_free(w->blocks_present);
    ralloc_free(w->blocks);
}

void
nir_block_worklist_add_all(nir_block_worklist *w, nir_function_impl *impl)
{
    nir_foreach_block(block, impl) {
        nir_block_worklist_push_tail(w, block);
    }
}

void
nir_block_worklist_push_head(nir_block_worklist *w, nir_block *block)
{
    /* Pushing a block we already have is a no-op */
    if (BITSET_TEST(w->blocks_present, block->index))
        return;

    assert(w->count < w->size);

    if (w->start == 0)
        w->start = w->size - 1;
    else
        w->start--;

    w->count++;

    w->blocks[w->start] = block;
    BITSET_SET(w->blocks_present, block->index);
}

nir_block *
nir_block_worklist_peek_head(const nir_block_worklist *w)
{
    assert(w->count > 0);

    return w->blocks[w->start];
}

nir_block *
nir_block_worklist_pop_head(nir_block_worklist *w)
{
    assert(w->count > 0);

    unsigned head = w->start;

    w->start = (w->start + 1) % w->size;
    w->count--;

    BITSET_CLEAR(w->blocks_present, w->blocks[head]->index);
    return w->blocks[head];
}

void
nir_block_worklist_push_tail(nir_block_worklist *w, nir_block *block)
{
    /* Pushing a block we already have is a no-op */
    if (BITSET_TEST(w->blocks_present, block->index))
        return;

    assert(w->count < w->size);

    w->count++;

    unsigned tail = (w->start + w->count - 1) % w->size;

    w->blocks[tail] = block;
    BITSET_SET(w->blocks_present, block->index);
}

nir_block *
nir_block_worklist_peek_tail(const nir_block_worklist *w)
{
    assert(w->count > 0);

    unsigned tail = (w->start + w->count - 1) % w->size;

    return w->blocks[tail];
}

nir_block *
nir_block_worklist_pop_tail(nir_block_worklist *w)
{
    assert(w->count > 0);

    unsigned tail = (w->start + w->count - 1) % w->size;

    w->count--;

    BITSET_CLEAR(w->blocks_present, w->blocks[tail]->index);
    return w->blocks[tail];
}

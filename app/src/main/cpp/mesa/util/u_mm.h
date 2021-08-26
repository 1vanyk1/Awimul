#ifndef _U_MM_H_
#define _U_MM_H_


struct mem_block {
    struct mem_block *next, *prev;
    struct mem_block *next_free, *prev_free;
    struct mem_block *heap;
    int ofs,size;
    unsigned int free:1;
    unsigned int reserved:1;
};



/**
 * input: total size in bytes
 * return: a heap pointer if OK, NULL if error
 */
extern struct mem_block *u_mmInit(int ofs, int size);

/**
 * Allocate 'size' bytes with 2^align2 bytes alignment,
 * restrict the search to free memory after 'startSearch'
 * depth and back buffers should be in different 4mb banks
 * to get better page hits if possible
 * input:	size = size of block
 *       	align2 = 2^align2 bytes alignment
 *		startSearch = linear offset from start of heap to begin search
 * return: pointer to the allocated block, 0 if error
 */
extern struct mem_block *u_mmAllocMem(struct mem_block *heap, int size, int align2,
                                      int startSearch);

/**
 * Free block starts at offset
 * input: pointer to a block
 * return: 0 if OK, -1 if error
 */
extern int u_mmFreeMem(struct mem_block *b);

/**
 * Free block starts at offset
 * input: pointer to a heap, start offset
 * return: pointer to a block
 */
extern struct mem_block *u_mmFindBlock(struct mem_block *heap, int start);

/**
 * destroy MM
 */
extern void u_mmDestroy(struct mem_block *mmInit);

/**
 * For debugging purposes.
 */
extern void u_mmDumpMemInfo(const struct mem_block *mmInit);

#endif

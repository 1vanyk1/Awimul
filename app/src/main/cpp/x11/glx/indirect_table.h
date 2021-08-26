#ifndef INDIRECT_TABLE_H
#define INDIRECT_TABLE_H

#include <inttypes.h>

/**
 */
struct __glXDispatchInfo {
    /**
     * Number of significant bits in the protocol opcode.  Opcodes with values
     * larger than ((1 << bits) - 1) are invalid.
     */
    unsigned bits;

    /**
     */
    const int_fast16_t *dispatch_tree;

    /**
     * Array of protocol decode and dispatch functions index by the opcode
     * search tree (i.e., \c dispatch_tree).  The first element in each pair
     * is the non-byte-swapped version, and the second element is the
     * byte-swapped version.
     */
    const void *(*dispatch_functions)[2];

    /**
     * Pointer to size validation data.  This table is indexed with the same
     * value as ::dispatch_functions.
     *
     * The first element in the pair is the size, in bytes, of the fixed-size
     * portion of the protocol.
     *
     * For opcodes that have a variable-size portion, the second value is an
     * index in \c size_func_table to calculate that size.  If there is no
     * variable-size portion, this index will be ~0.
     *
     * \note
     * If size checking is not to be performed on this type of protocol
     * data, this pointer will be \c NULL.
     */
    const int_fast16_t(*size_table)[2];

    /**
     * Array of functions used to calculate the variable-size portion of
     * protocol messages.  Indexed by the second element of the entries
     * in \c ::size_table.
     *
     * \note
     * If size checking is not to be performed on this type of protocol
     * data, this pointer will be \c NULL.
     */
    const gl_proto_size_func *size_func_table;
};

/**
 * Sentinel value for an empty leaf in the \c dispatch_tree.
 */
#define EMPTY_LEAF         INT_FAST16_MIN

/**
 * Declare the index \c x as a leaf index.
 */
#define LEAF(x)            -x

/**
 * Determine if an index is a leaf index.
 */
#define IS_LEAF_INDEX(x)   ((x) <= 0)

extern const struct __glXDispatchInfo Single_dispatch_info;
extern const struct __glXDispatchInfo Render_dispatch_info;
extern const struct __glXDispatchInfo VendorPriv_dispatch_info;

#endif                          /* INDIRECT_TABLE_H */

#ifndef GLSL_IR_ARRAY_REFCOUNT_H
#define GLSL_IR_ARRAY_REFCOUNT_H

#include "ir.h"
#include "ir_visitor.h"
#include "linker_util.h"
#include "../glsl_types.h"
#include "../../util/bitset.h"

class ir_array_refcount_entry
{
public:
    ir_array_refcount_entry(ir_variable *var);
    ~ir_array_refcount_entry();

    ir_variable *var; /* The key: the variable's pointer. */

    /** Has the variable been referenced? */
    bool is_referenced;

    /** Count of nested arrays in the type. */
    unsigned array_depth;

    /** Set of bit-flags to note which array elements have been accessed. */
    BITSET_WORD *bits;

    /** Has a linearized array index been referenced? */
    bool is_linearized_index_referenced(unsigned linearized_index) const
    {
        assert(bits != 0);
        assert(linearized_index <= num_bits);

        return BITSET_TEST(bits, linearized_index);
    }

private:

    /**
     * Total number of bits referenced by \c bits.
     *
     * Also the total number of array(s-of-arrays) elements of \c var.
     */
    unsigned num_bits;

    friend class array_refcount_test;
};

class ir_array_refcount_visitor : public ir_hierarchical_visitor {
public:
    ir_array_refcount_visitor(void);
    ~ir_array_refcount_visitor(void);

    virtual ir_visitor_status visit(ir_dereference_variable *);

    virtual ir_visitor_status visit_enter(ir_function_signature *);
    virtual ir_visitor_status visit_enter(ir_dereference_array *);

    /**
     * Find variable in the hash table, and insert it if not present
     */
    ir_array_refcount_entry *get_variable_entry(ir_variable *var);

    /**
     * Hash table mapping ir_variable to ir_array_refcount_entry.
     */
    struct hash_table *ht;

    void *mem_ctx;

private:
    /** Get an array_deref_range element from private tracking. */
    array_deref_range *get_array_deref();

    /**
     * Last ir_dereference_array that was visited
     *
     * Used to prevent some redundant calculations.
     *
     * \sa ::visit_enter(ir_dereference_array *)
     */
    ir_dereference_array *last_array_deref;

    /**
     * \name array_deref_range tracking
     */
    /*@{*/
    /** Currently allocated block of derefs. */
    array_deref_range *derefs;

    /** Number of derefs used in current processing. */
    unsigned num_derefs;

    /** Size of the derefs buffer in bytes. */
    unsigned derefs_size;
    /*@}*/
};

#endif /* GLSL_IR_ARRAY_REFCOUNT_H */

#ifndef LOWER_BUFFER_ACCESS_H
#define LOWER_BUFFER_ACCESS_H

#include "ir.h"
#include "ir_rvalue_visitor.h"

namespace lower_buffer_access {

    class lower_buffer_access : public ir_rvalue_enter_visitor {
    public:
        virtual void
        insert_buffer_access(void *mem_ctx, ir_dereference *deref,
                             const glsl_type *type, ir_rvalue *offset,
                             unsigned mask, int channel) = 0;

        void emit_access(void *mem_ctx, bool is_write, ir_dereference *deref,
                         ir_variable *base_offset, unsigned int deref_offset,
                         bool row_major, const glsl_type *matrix_type,
                         enum glsl_interface_packing packing,
                         unsigned int write_mask);

        bool is_dereferenced_thing_row_major(const ir_rvalue *deref);

        void setup_buffer_access(void *mem_ctx, ir_rvalue *deref,
                                 ir_rvalue **offset, unsigned *const_offset,
                                 bool *row_major,
                                 const glsl_type **matrix_type,
                                 const glsl_struct_field **struct_field,
                                 enum glsl_interface_packing packing);

    protected:
        bool use_std430_as_default;
    };

} /* namespace lower_buffer_access */

#endif /* LOWER_BUFFER_ACCESS_H */

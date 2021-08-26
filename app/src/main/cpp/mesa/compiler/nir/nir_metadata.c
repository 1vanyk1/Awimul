#include "nir.h"

/*
 * Handles management of the metadata.
 */

void
nir_metadata_require(nir_function_impl *impl, nir_metadata required, ...)
{
#define NEEDS_UPDATE(X) ((required & ~impl->valid_metadata) & (X))

    if (NEEDS_UPDATE(nir_metadata_block_index))
        nir_index_blocks(impl);
    if (NEEDS_UPDATE(nir_metadata_instr_index))
        nir_index_instrs(impl);
    if (NEEDS_UPDATE(nir_metadata_dominance))
        nir_calc_dominance_impl(impl);
    if (NEEDS_UPDATE(nir_metadata_live_ssa_defs))
        nir_live_ssa_defs_impl(impl);
    if (NEEDS_UPDATE(nir_metadata_loop_analysis)) {
        va_list ap;
        va_start(ap, required);
        nir_loop_analyze_impl(impl, va_arg(ap, nir_variable_mode));
        va_end(ap);
    }

#undef NEEDS_UPDATE

    impl->valid_metadata |= required;
}

void
nir_metadata_preserve(nir_function_impl *impl, nir_metadata preserved)
{
    impl->valid_metadata &= preserved;
}

void
nir_shader_preserve_all_metadata(nir_shader *shader)
{
    nir_foreach_function(function, shader) {
        if (function->impl)
            nir_metadata_preserve(function->impl, nir_metadata_all);
    }
}

#ifndef NDEBUG
/**
 * Make sure passes properly invalidate metadata (part 1).
 *
 * Call this before running a pass to set a bogus metadata flag, which will
 * only be preserved if the pass forgets to call nir_metadata_preserve().
 */
void
nir_metadata_set_validation_flag(nir_shader *shader)
{
    nir_foreach_function(function, shader) {
        if (function->impl) {
            function->impl->valid_metadata |= nir_metadata_not_properly_reset;
        }
    }
}

/**
 * Make sure passes properly invalidate metadata (part 2).
 *
 * Call this after a pass makes progress to verify that the bogus metadata set by
 * the earlier function was properly thrown away.  Note that passes may not call
 * nir_metadata_preserve() if they don't actually make any changes at all.
 */
void
nir_metadata_check_validation_flag(nir_shader *shader)
{
    nir_foreach_function(function, shader) {
        if (function->impl) {
            assert(!(function->impl->valid_metadata &
                     nir_metadata_not_properly_reset));
        }
    }
}
#endif

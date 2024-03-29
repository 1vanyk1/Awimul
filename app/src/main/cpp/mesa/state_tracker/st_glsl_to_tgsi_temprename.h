#ifndef MESA_GLSL_TO_TGSI_TEMPRENAME_H
#define MESA_GLSL_TO_TGSI_TEMPRENAME_H

#include "st_glsl_to_tgsi_array_merge.h"

/** Storage to record the required live range of a temporary register
 * begin == end == -1 indicates that the register can be reused without
 * limitations. Otherwise, "begin" indicates the first instruction in which
 * a write operation may target this temporary, and end indicates the
 * last instruction in which a value can be read from this temporary.
 * Hence, a register R2 can be merged with a register R1 if R1.end <= R2.begin.
 */
struct register_live_range {
    int begin;
    int end;
};

/** Evaluates the required live ranges of temporary registers in a shader.
 * The live range estimation can only be run sucessfully if the shader doesn't
 * call a subroutine.
 * @param[in] mem_ctx a memory context that can be used with the ralloc_*
 *            functions
 * @param[in] instructions the shader to be anlzyed
 * @param[in] ntemps number of temporaries reserved for this shader
 * @param[in,out] reg_live_ranges memory location to store the estimated
 *   required live ranges for each temporary register. The parameter must
 *   point to allocated memory that can hold ntemps register_live_range
 *   structures. On output the live ranges contains the live ranges for
 *   the registers with the exception of TEMP[0]
 * @param[in] narrays number of array sreserved for this shader
 * @param[in,out] arr_live_ranges memory location to store the estimated required
 *   live ranges for each array. The parameter must point to allocated memory
 *   that can hold narrays array_live_range structures. On output the live
 *   ranges contains the live ranges for the registers with the exception of
 *   ARRAY[0].
 * @returns: true if the lifetimes were estimated, false if not (i.e. if a
 * subroutine was called).
 */
bool
get_temp_registers_required_live_ranges(void *mem_ctx, exec_list *instructions,
                                        int ntemps, struct register_live_range *register_live_ranges,
                                        int narrays, array_live_range *array_live_ranges);

/** Estimate the merge remapping of the registers.
 * @param[in] mem_ctx a memory context that can be used with the ralloc_*
 *            functions
 * @param[in] ntemps number of temporaries reserved for this shader
 * @param[in] reg_live_ranges required live range for each temporary register.
 * @param[in,out] result memory location to store the register remapping table.
 *  On input the parameter must point to allocated memory that can hold the
 *  renaming information for ntemps registers, on output the mapping is stored.
 *  Note that TEMP[0] is not considered for register renaming.
 */
void get_temp_registers_remapping(void *mem_ctx, int ntemps,
                                  const struct register_live_range* reg_live_ranges,
                                  struct rename_reg_pair *result);
#endif

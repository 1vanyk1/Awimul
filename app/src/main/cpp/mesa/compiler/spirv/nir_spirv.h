#ifndef _NIR_SPIRV_H_
#define _NIR_SPIRV_H_

#include "../../util/disk_cache.h"
#include "../nir/nir.h"
#include "../shader_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nir_spirv_specialization {
    uint32_t id;
    nir_const_value value;
    bool defined_on_module;
};

enum nir_spirv_debug_level {
    NIR_SPIRV_DEBUG_LEVEL_INFO,
    NIR_SPIRV_DEBUG_LEVEL_WARNING,
    NIR_SPIRV_DEBUG_LEVEL_ERROR,
};

enum nir_spirv_execution_environment {
    NIR_SPIRV_VULKAN = 0,
    NIR_SPIRV_OPENCL,
    NIR_SPIRV_OPENGL,
};

struct spirv_to_nir_options {
    enum nir_spirv_execution_environment environment;

    /* Whether to make FragCoord to a system value, the same as
     * GLSLFragCoordIsSysVal in GLSL.
     */
    bool frag_coord_is_sysval;

    /* Whether to keep ViewIndex as an input instead of rewriting to a sysval.
     */
    bool view_index_is_input;

    /* Create a nir library. */
    bool create_library;

    /* Whether to use nir_intrinsic_deref_buffer_array_length intrinsic instead
     * of nir_intrinsic_get_ssbo_size to lower OpArrayLength.
     */
    bool use_deref_buffer_array_length;

    /* Initial value for shader_info::float_controls_execution_mode,
     * indicates hardware requirements rather than shader author intent
     */
    uint16_t float_controls_execution_mode;

    struct spirv_supported_capabilities caps;

    /* Address format for various kinds of pointers. */
    nir_address_format ubo_addr_format;
    nir_address_format ssbo_addr_format;
    nir_address_format phys_ssbo_addr_format;
    nir_address_format push_const_addr_format;
    nir_address_format shared_addr_format;
    nir_address_format global_addr_format;
    nir_address_format temp_addr_format;
    nir_address_format constant_addr_format;

    const nir_shader *clc_shader;

    struct {
        void (*func)(void *private_data,
                     enum nir_spirv_debug_level level,
                     size_t spirv_offset,
                     const char *message);
        void *private_data;
    } debug;
};

bool gl_spirv_validation(const uint32_t *words, size_t word_count,
                         struct nir_spirv_specialization *spec, unsigned num_spec,
                         gl_shader_stage stage, const char *entry_point_name);

nir_shader *spirv_to_nir(const uint32_t *words, size_t word_count,
                         struct nir_spirv_specialization *specializations,
                         unsigned num_specializations,
                         gl_shader_stage stage, const char *entry_point_name,
                         const struct spirv_to_nir_options *options,
                         const nir_shader_compiler_options *nir_options);

bool nir_can_find_libclc(unsigned ptr_bit_size);

nir_shader *
nir_load_libclc_shader(unsigned ptr_bit_size,
                       struct disk_cache *disk_cache,
                       const struct spirv_to_nir_options *spirv_options,
                       const nir_shader_compiler_options *nir_options);

bool nir_lower_libclc(nir_shader *shader, const nir_shader *clc_shader);

#ifdef __cplusplus
}
#endif

#endif /* _NIR_SPIRV_H_ */

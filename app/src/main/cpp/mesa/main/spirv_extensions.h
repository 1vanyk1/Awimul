#ifndef _SPIRVEXTENSIONS_H_
#define _SPIRVEXTENSIONS_H_

#include "mtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

enum SpvExtension {
    SPV_KHR_16bit_storage = 0,
    SPV_KHR_device_group,
    SPV_KHR_multiview,
    SPV_KHR_shader_ballot,
    SPV_KHR_shader_draw_parameters,
    SPV_KHR_storage_buffer_storage_class,
    SPV_KHR_subgroup_vote,
    SPV_KHR_variable_pointers,
    SPV_AMD_gcn_shader,
    SPV_EXTENSIONS_COUNT
};

struct spirv_supported_extensions {
    /** Flags the supported extensions. Array to make it easier to iterate. */
    bool supported[SPV_EXTENSIONS_COUNT];

    /** Number of supported extensions */
    unsigned int count;
};

extern GLuint
_mesa_get_spirv_extension_count(struct gl_context *ctx);

extern const GLubyte *
_mesa_get_enabled_spirv_extension(struct gl_context *ctx,
                                  GLuint index);

const char *_mesa_spirv_extensions_to_string(enum SpvExtension ext);

void _mesa_fill_supported_spirv_extensions(struct spirv_supported_extensions *ext,
                                           const struct spirv_supported_capabilities *cap);

#ifdef __cplusplus
}
#endif

#endif /* SPIRVEXTENSIONS_H */

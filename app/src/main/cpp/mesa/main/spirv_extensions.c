#include <stdbool.h>
#include "spirv_extensions.h"

GLuint
_mesa_get_spirv_extension_count(struct gl_context *ctx)
{
    if (ctx->Const.SpirVExtensions == NULL)
        return 0;

    return ctx->Const.SpirVExtensions->count;
}

const GLubyte *
_mesa_get_enabled_spirv_extension(struct gl_context *ctx,
                                  GLuint index)
{
    unsigned int n = 0;

    if (ctx->Const.SpirVExtensions == NULL)
        return (const GLubyte *) 0;

    for (unsigned int i = 0; i < SPV_EXTENSIONS_COUNT; i++) {
        if (ctx->Const.SpirVExtensions->supported[i]) {
            if (n == index)
                return (const GLubyte *) _mesa_spirv_extensions_to_string(i);
            else
                n++;
        }
    }

    return (const GLubyte *) 0;
}

const char *
_mesa_spirv_extensions_to_string(enum SpvExtension ext)
{
#define STR(x) case x: return #x;
    switch (ext) {
        STR(SPV_KHR_16bit_storage);
        STR(SPV_KHR_device_group);
        STR(SPV_KHR_multiview);
        STR(SPV_KHR_shader_ballot);
        STR(SPV_KHR_shader_draw_parameters);
        STR(SPV_KHR_storage_buffer_storage_class);
        STR(SPV_KHR_subgroup_vote);
        STR(SPV_KHR_variable_pointers);
        STR(SPV_AMD_gcn_shader);
        case SPV_EXTENSIONS_COUNT:
            unreachable("Unknown SPIR-V extension");
    }
#undef STR

    return "unknown";
}

/**
 * Sets the supported flags for known SPIR-V extensions based on the
 * capabilites supported (spirv capabilities based on the spirv to nir
 * support).
 *
 * One could argue that makes more sense in the other way around, as from the
 * spec pov capabilities are enable for a given extension. But from our pov,
 * we support or not (depending on the driver) some given capability, and
 * spirv_to_nir check for capabilities not extensions. Also we usually fill
 * first the supported capabilities, that are not always related to an
 * extension.
 */
void
_mesa_fill_supported_spirv_extensions(struct spirv_supported_extensions *ext,
                                      const struct spirv_supported_capabilities *cap)
{
    memset(ext->supported, 0, sizeof(ext->supported));

    ext->count = 0;

    ext->supported[SPV_KHR_shader_draw_parameters] = cap->draw_parameters;
    ext->supported[SPV_KHR_multiview] = cap->multiview;
    ext->supported[SPV_KHR_storage_buffer_storage_class] = true;
    ext->supported[SPV_KHR_variable_pointers] = cap->variable_pointers;
    ext->supported[SPV_AMD_gcn_shader] = cap->amd_gcn_shader;
    ext->supported[SPV_KHR_shader_ballot] = cap->subgroup_ballot;
    ext->supported[SPV_KHR_subgroup_vote] = cap->subgroup_vote;

    for (unsigned i = 0; i < SPV_EXTENSIONS_COUNT; i++) {
        if (ext->supported[i])
            ext->count++;
    }
}

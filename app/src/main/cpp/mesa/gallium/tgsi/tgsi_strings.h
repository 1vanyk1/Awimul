#ifndef TGSI_STRINGS_H
#define TGSI_STRINGS_H

#include "../../include/pipe/p_shader_tokens.h"
#include "../../include/pipe/p_state.h"


#if defined __cplusplus
extern "C" {
#endif


extern const char *tgsi_processor_type_names[PIPE_SHADER_TYPES];

extern const char *tgsi_semantic_names[TGSI_SEMANTIC_COUNT];

extern const char *tgsi_texture_names[TGSI_TEXTURE_COUNT];

extern const char *tgsi_property_names[TGSI_PROPERTY_COUNT];

extern const char *tgsi_return_type_names[TGSI_RETURN_TYPE_COUNT];

extern const char *tgsi_interpolate_names[TGSI_INTERPOLATE_COUNT];

extern const char *tgsi_interpolate_locations[TGSI_INTERPOLATE_LOC_COUNT];

extern const char *tgsi_invariant_name;

extern const char *tgsi_primitive_names[PIPE_PRIM_MAX];

extern const char *tgsi_fs_coord_origin_names[2];

extern const char *tgsi_fs_coord_pixel_center_names[2];

extern const char *tgsi_immediate_type_names[6];

extern const char *tgsi_memory_names[4];


const char *
tgsi_file_name(unsigned file);


#if defined __cplusplus
}
#endif


#endif /* TGSI_STRINGS_H */

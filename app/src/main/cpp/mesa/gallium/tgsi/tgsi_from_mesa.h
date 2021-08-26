#ifndef TGSI_FROM_MESA_H
#define TGSI_FROM_MESA_H

#include <stdbool.h>

#include "../../include/pipe/p_compiler.h"
#include "../../include/pipe/p_defines.h"
#include "../../include/pipe/p_shader_tokens.h"

#include "../../compiler/shader_enums.h"

#ifdef __cplusplus
extern "C" {
#endif


void
tgsi_get_gl_varying_semantic(gl_varying_slot attr,
                             bool needs_texcoord_semantic,
                             unsigned *semantic_name,
                             unsigned *semantic_index);

unsigned
tgsi_get_generic_gl_varying_index(gl_varying_slot attr,
                                  bool needs_texcoord_semantic);

void
tgsi_get_gl_frag_result_semantic(gl_frag_result frag_result,
                                 unsigned *semantic_name,
                                 unsigned *semantic_index);

enum tgsi_semantic
tgsi_get_sysval_semantic(unsigned sysval);

enum tgsi_interpolate_mode
tgsi_get_interp_mode(enum glsl_interp_mode mode, bool color);

static inline enum pipe_shader_type
pipe_shader_type_from_mesa(gl_shader_stage stage)
{
    switch (stage) {
        case MESA_SHADER_VERTEX:
            return PIPE_SHADER_VERTEX;
        case MESA_SHADER_TESS_CTRL:
            return PIPE_SHADER_TESS_CTRL;
        case MESA_SHADER_TESS_EVAL:
            return PIPE_SHADER_TESS_EVAL;
        case MESA_SHADER_GEOMETRY:
            return PIPE_SHADER_GEOMETRY;
        case MESA_SHADER_FRAGMENT:
            return PIPE_SHADER_FRAGMENT;
        case MESA_SHADER_COMPUTE:
        case MESA_SHADER_KERNEL:
            return PIPE_SHADER_COMPUTE;
        default:
            unreachable("bad shader stage");
    }
}

static inline gl_shader_stage
tgsi_processor_to_shader_stage(unsigned processor)
{
    switch (processor) {
        case PIPE_SHADER_FRAGMENT:  return MESA_SHADER_FRAGMENT;
        case PIPE_SHADER_VERTEX:    return MESA_SHADER_VERTEX;
        case PIPE_SHADER_GEOMETRY:  return MESA_SHADER_GEOMETRY;
        case PIPE_SHADER_TESS_CTRL: return MESA_SHADER_TESS_CTRL;
        case PIPE_SHADER_TESS_EVAL: return MESA_SHADER_TESS_EVAL;
        case PIPE_SHADER_COMPUTE:   return MESA_SHADER_COMPUTE;
        default:
            unreachable("invalid TGSI processor");
    }
}

#ifdef __cplusplus
}
#endif

#endif /* TGSI_FROM_MESA_H */

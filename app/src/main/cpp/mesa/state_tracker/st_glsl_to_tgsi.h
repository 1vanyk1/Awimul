#ifndef ST_GLSL_TO_TGSI_H
#define ST_GLSL_TO_TGSI_H

#include "../include/pipe/p_defines.h"
#include "../include/pipe/p_shader_tokens.h"
#include "../main/mtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_shader;
struct gl_shader_program;
struct glsl_to_tgsi_visitor;
struct ureg_program;

enum pipe_error st_translate_program(
        struct gl_context *ctx,
        enum pipe_shader_type procType,
        struct ureg_program *ureg,
        struct glsl_to_tgsi_visitor *program,
        const struct gl_program *proginfo,
        GLuint numInputs,
        const ubyte inputMapping[],
        const ubyte inputSlotToAttr[],
        const ubyte inputSemanticName[],
        const ubyte inputSemanticIndex[],
        const ubyte interpMode[],
        GLuint numOutputs,
        const ubyte outputMapping[],
        const ubyte outputSemanticName[],
        const ubyte outputSemanticIndex[]);

void free_glsl_to_tgsi_visitor(struct glsl_to_tgsi_visitor *v);

GLboolean
st_link_tgsi(struct gl_context *ctx, struct gl_shader_program *prog);

enum tgsi_semantic
_mesa_sysval_to_semantic(unsigned sysval);

#ifdef __cplusplus
}
#endif

#endif

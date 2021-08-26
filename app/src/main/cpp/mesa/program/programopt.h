#ifndef PROGRAMOPT_H
#define PROGRAMOPT_H 1

#include "../main/glheader.h"
#include "prog_parameter.h"


#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_program;

extern void
_mesa_insert_mvp_code(struct gl_context *ctx, struct gl_program *vprog);

extern void
_mesa_append_fog_code(struct gl_context *ctx, struct gl_program *fprog,
                      GLenum fog_mode, GLboolean saturate);

extern void
_mesa_count_texture_indirections(struct gl_program *prog);

extern void
_mesa_count_texture_instructions(struct gl_program *prog);

extern void
_mesa_remove_output_reads(struct gl_program *prog, gl_register_file type);

extern void
_mesa_program_fragment_position_to_sysval(struct gl_program *prog);

#ifdef __cplusplus
}
#endif

#endif /* PROGRAMOPT_H */

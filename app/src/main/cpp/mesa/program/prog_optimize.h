#ifndef PROG_OPT_H
#define PROG_OPT_H


#include "../main/config.h"
#include "../main/glheader.h"


#ifdef __cplusplus
extern "C" {
#endif


struct gl_context;
struct gl_program;
struct prog_instruction;


extern GLboolean
_mesa_find_temp_intervals(const struct prog_instruction *instructions,
                          GLuint numInstructions,
                          GLint intBegin[MAX_PROGRAM_TEMPS],
                          GLint intEnd[MAX_PROGRAM_TEMPS]);

extern void
_mesa_optimize_program(struct gl_program *program, void *mem_ctx);

extern GLboolean
_mesa_constant_fold(struct gl_program *prog);


#ifdef __cplusplus
}
#endif


#endif

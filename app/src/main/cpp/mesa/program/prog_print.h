#ifndef PROG_PRINT_H
#define PROG_PRINT_H

#include <stdio.h>

#include "../main/glheader.h"
#include "prog_parameter.h"


#ifdef __cplusplus
extern "C" {
#endif


struct gl_program;
struct gl_program_parameter_list;
struct gl_shader;
struct prog_instruction;


/**
 * The output style to use when printing programs.
 */
typedef enum {
    PROG_PRINT_ARB,
    PROG_PRINT_DEBUG
} gl_prog_print_mode;


extern const char *
_mesa_register_file_name(gl_register_file f);

extern void
_mesa_print_vp_inputs(GLbitfield inputs);

extern void
_mesa_print_fp_inputs(GLbitfield inputs);

extern const char *
_mesa_condcode_string(GLuint condcode);

extern const char *
_mesa_swizzle_string(GLuint swizzle, GLuint negateBase, GLboolean extended);

const char *
_mesa_writemask_string(GLuint writeMask);

extern void
_mesa_print_swizzle(GLuint swizzle);

extern void
_mesa_fprint_alu_instruction(FILE *f,
                             const struct prog_instruction *inst,
                             const char *opcode_string, GLuint numRegs,
                             gl_prog_print_mode mode,
                             const struct gl_program *prog);

extern void
_mesa_print_alu_instruction(const struct prog_instruction *inst,
                            const char *opcode_string, GLuint numRegs);

extern void
_mesa_print_instruction(const struct prog_instruction *inst);

extern GLint
_mesa_fprint_instruction_opt(FILE *f,
                             const struct prog_instruction *inst,
                             GLint indent,
                             gl_prog_print_mode mode,
                             const struct gl_program *prog);

extern GLint
_mesa_print_instruction_opt(const struct prog_instruction *inst, GLint indent,
                            gl_prog_print_mode mode,
                            const struct gl_program *prog);

extern void
_mesa_print_program(const struct gl_program *prog);

extern void
_mesa_fprint_program_opt(FILE *f,
                         const struct gl_program *prog, gl_prog_print_mode mode,
                         GLboolean lineNumbers);

extern void
_mesa_print_program_parameters(struct gl_context *ctx, const struct gl_program *prog);

extern void
_mesa_print_parameter_list(const struct gl_program_parameter_list *list);


extern void
_mesa_write_shader_to_file(const struct gl_shader *shader);

extern void
_mesa_append_uniforms_to_file(const struct gl_program *prog);


#ifdef __cplusplus
}
#endif


#endif /* PROG_PRINT_H */

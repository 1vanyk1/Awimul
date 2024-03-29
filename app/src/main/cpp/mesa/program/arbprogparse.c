#include "../main/glheader.h"

#include "../main/context.h"
#include "../main/mtypes.h"
#include "arbprogparse.h"
#include "programopt.h"
#include "prog_parameter.h"
#include "prog_statevars.h"
#include "prog_instruction.h"
#include "prog_optimize.h"
#include "program_parser.h"


void
_mesa_parse_arb_fragment_program(struct gl_context* ctx, GLenum target,
                                 const GLvoid *str, GLsizei len,
                                 struct gl_program *program)
{
    struct gl_program prog;
    struct asm_parser_state state;
    GLuint i;

    assert(target == GL_FRAGMENT_PROGRAM_ARB);

    memset(&prog, 0, sizeof(prog));
    memset(&state, 0, sizeof(state));
    state.prog = &prog;
    state.mem_ctx = program;

    if (!_mesa_parse_arb_program(ctx, target, (const GLubyte*) str, len,
                                 &state)) {
        /* Error in the program. Just return. */
        return;
    }

    ralloc_free(program->String);

    /* Copy the relevant contents of the arb_program struct into the
     * fragment_program struct.
     */
    program->String          = prog.String;
    program->arb.NumInstructions = prog.arb.NumInstructions;
    program->arb.NumTemporaries  = prog.arb.NumTemporaries;
    program->arb.NumParameters   = prog.arb.NumParameters;
    program->arb.NumAttributes   = prog.arb.NumAttributes;
    program->arb.NumAddressRegs  = prog.arb.NumAddressRegs;
    program->arb.NumNativeInstructions = prog.arb.NumNativeInstructions;
    program->arb.NumNativeTemporaries = prog.arb.NumNativeTemporaries;
    program->arb.NumNativeParameters = prog.arb.NumNativeParameters;
    program->arb.NumNativeAttributes = prog.arb.NumNativeAttributes;
    program->arb.NumNativeAddressRegs = prog.arb.NumNativeAddressRegs;
    program->arb.NumAluInstructions   = prog.arb.NumAluInstructions;
    program->arb.NumTexInstructions   = prog.arb.NumTexInstructions;
    program->arb.NumTexIndirections   = prog.arb.NumTexIndirections;
    program->arb.NumNativeAluInstructions = prog.arb.NumAluInstructions;
    program->arb.NumNativeTexInstructions = prog.arb.NumTexInstructions;
    program->arb.NumNativeTexIndirections = prog.arb.NumTexIndirections;
    program->info.inputs_read      = prog.info.inputs_read;
    program->info.outputs_written = prog.info.outputs_written;
    program->arb.IndirectRegisterFiles = prog.arb.IndirectRegisterFiles;
    for (i = 0; i < MAX_TEXTURE_IMAGE_UNITS; i++) {
        program->TexturesUsed[i] = prog.TexturesUsed[i];
        if (prog.TexturesUsed[i])
            program->SamplersUsed |= (1 << i);
    }
    program->ShadowSamplers = prog.ShadowSamplers;
    program->info.fs.origin_upper_left = state.option.OriginUpperLeft;
    program->info.fs.pixel_center_integer = state.option.PixelCenterInteger;

    program->info.fs.uses_discard = state.fragment.UsesKill;

    ralloc_free(program->arb.Instructions);
    program->arb.Instructions = prog.arb.Instructions;

    if (program->Parameters)
        _mesa_free_parameter_list(program->Parameters);
    program->Parameters    = prog.Parameters;

    /* Append fog instructions now if the program has "OPTION ARB_fog_exp"
     * or similar.  We used to leave this up to drivers, but it appears
     * there's no hardware that wants to do fog in a discrete stage separate
     * from the fragment shader.
     */
    if (state.option.Fog != OPTION_NONE) {
        static const GLenum fog_modes[4] = {
                GL_NONE, GL_EXP, GL_EXP2, GL_LINEAR
        };

        /* XXX: we should somehow recompile this to remove clamping if disabled
         * On the ATI driver, this is unclampled if fragment clamping is disabled
         */
        _mesa_append_fog_code(ctx, program, fog_modes[state.option.Fog], GL_TRUE);
    }

#if DEBUG_FP
    printf("____________Fragment program %u ________\n", program->Id);
   _mesa_print_program(&program->Base);
#endif
}



/**
 * Parse the vertex program string.  If success, update the given
 * vertex_program object with the new program.  Else, leave the vertex_program
 * object unchanged.
 */
void
_mesa_parse_arb_vertex_program(struct gl_context *ctx, GLenum target,
                               const GLvoid *str, GLsizei len,
                               struct gl_program *program)
{
    struct gl_program prog;
    struct asm_parser_state state;

    assert(target == GL_VERTEX_PROGRAM_ARB);

    memset(&prog, 0, sizeof(prog));
    memset(&state, 0, sizeof(state));
    state.prog = &prog;
    state.mem_ctx = program;

    if (!_mesa_parse_arb_program(ctx, target, (const GLubyte*) str, len,
                                 &state)) {
        _mesa_error(ctx, GL_INVALID_OPERATION, "glProgramString(bad program)");
        return;
    }

    _mesa_optimize_program(&prog, program);

    ralloc_free(program->String);

    /* Copy the relevant contents of the arb_program struct into the
     * vertex_program struct.
     */
    program->String          = prog.String;
    program->arb.NumInstructions = prog.arb.NumInstructions;
    program->arb.NumTemporaries  = prog.arb.NumTemporaries;
    program->arb.NumParameters   = prog.arb.NumParameters;
    program->arb.NumAttributes   = prog.arb.NumAttributes;
    program->arb.NumAddressRegs  = prog.arb.NumAddressRegs;
    program->arb.NumNativeInstructions = prog.arb.NumNativeInstructions;
    program->arb.NumNativeTemporaries = prog.arb.NumNativeTemporaries;
    program->arb.NumNativeParameters = prog.arb.NumNativeParameters;
    program->arb.NumNativeAttributes = prog.arb.NumNativeAttributes;
    program->arb.NumNativeAddressRegs = prog.arb.NumNativeAddressRegs;
    program->info.inputs_read     = prog.info.inputs_read;
    program->info.outputs_written = prog.info.outputs_written;
    program->arb.IndirectRegisterFiles = prog.arb.IndirectRegisterFiles;
    program->arb.IsPositionInvariant = (state.option.PositionInvariant)
                                       ? GL_TRUE : GL_FALSE;

    ralloc_free(program->arb.Instructions);
    program->arb.Instructions = prog.arb.Instructions;

    if (program->Parameters)
        _mesa_free_parameter_list(program->Parameters);
    program->Parameters = prog.Parameters;

#if DEBUG_VP
    printf("____________Vertex program %u __________\n", program->Id);
   _mesa_print_program(program);
#endif
}

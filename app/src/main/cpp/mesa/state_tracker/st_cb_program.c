#include "../main/glheader.h"
#include "../main/macros.h"
#include "../main/enums.h"
#include "../main/shaderapi.h"
#include "../program/prog_instruction.h"
#include "../program/program.h"

#include "../gallium/cso_cache/cso_context.h"
#include "../gallium/draw/draw_context.h"

#include "st_context.h"
#include "st_debug.h"
#include "st_program.h"
#include "st_cb_program.h"
#include "st_glsl_to_ir.h"
#include "st_atifs_to_nir.h"
#include "st_util.h"


/**
 * Called via ctx->Driver.NewProgram() to allocate a new vertex or
 * fragment program.
 */
static struct gl_program *
st_new_program(struct gl_context *ctx, gl_shader_stage stage, GLuint id,
               bool is_arb_asm)
{
    struct st_program *prog;

    switch (stage) {
        case MESA_SHADER_VERTEX:
            prog = (struct st_program*)rzalloc(NULL, struct st_vertex_program);
            break;
        default:
            prog = rzalloc(NULL, struct st_program);
            break;
    }

    return _mesa_init_gl_program(&prog->Base, stage, id, is_arb_asm);
}


void
st_delete_program(struct gl_context *ctx, struct gl_program *prog)
{
    struct st_context *st = st_context(ctx);
    struct st_program *stp = st_program(prog);

    st_release_variants(st, stp);

    if (stp->glsl_to_tgsi)
        free_glsl_to_tgsi_visitor(stp->glsl_to_tgsi);

    free(stp->serialized_nir);

    /* delete base class */
    _mesa_delete_program( ctx, prog );
}

/**
 * Called when the program's text/code is changed.  We have to free
 * all shader variants and corresponding gallium shaders when this happens.
 */
GLboolean
st_program_string_notify( struct gl_context *ctx,
                          GLenum target,
                          struct gl_program *prog )
{
    struct st_context *st = st_context(ctx);
    struct st_program *stp = (struct st_program *) prog;

    /* GLSL-to-NIR should not end up here. */
    assert(!stp->shader_program);

    st_release_variants(st, stp);

    if (target == GL_FRAGMENT_PROGRAM_ARB ||
        target == GL_FRAGMENT_SHADER_ATI) {
        if (target == GL_FRAGMENT_SHADER_ATI) {
            assert(stp->ati_fs);
            assert(stp->ati_fs->Program == prog);

            st_init_atifs_prog(ctx, prog);
        }

        if (!st_translate_fragment_program(st, stp))
            return false;
    } else if (target == GL_VERTEX_PROGRAM_ARB) {
        if (!st_translate_vertex_program(st, stp))
            return false;
    } else {
        if (!st_translate_common_program(st, stp))
            return false;
    }

    st_finalize_program(st, prog);
    return GL_TRUE;
}

/**
 * Called in glEndFragmentShaderATI()
 */
struct gl_program *
st_new_ati_fs(struct gl_context *ctx, struct ati_fragment_shader *curProg)
{
    struct gl_program *prog = st_new_program(ctx, MESA_SHADER_FRAGMENT,
                                             curProg->Id, true);
    struct st_program *stfp = (struct st_program *)prog;
    stfp->ati_fs = curProg;
    return prog;
}

void
st_max_shader_compiler_threads(struct gl_context *ctx, unsigned count)
{
    struct pipe_screen *screen = st_context(ctx)->screen;

    if (screen->set_max_shader_compiler_threads)
        screen->set_max_shader_compiler_threads(screen, count);
}

bool
st_get_shader_program_completion_status(struct gl_context *ctx,
                                        struct gl_shader_program *shprog)
{
    struct pipe_screen *screen = st_context(ctx)->screen;

    if (!screen->is_parallel_shader_compilation_finished)
        return true;

    for (unsigned i = 0; i < MESA_SHADER_STAGES; i++) {
        struct gl_linked_shader *linked = shprog->_LinkedShaders[i];
        void *sh = NULL;

        if (!linked || !linked->Program)
            continue;

        if (st_program(linked->Program)->variants)
            sh = st_program(linked->Program)->variants->driver_shader;

        unsigned type = pipe_shader_type_from_mesa(i);

        if (sh &&
            !screen->is_parallel_shader_compilation_finished(screen, sh, type))
            return false;
    }
    return true;
}

/**
 * Plug in the program and shader-related device driver functions.
 */
void
st_init_program_functions(struct dd_function_table *functions)
{
    functions->NewProgram = st_new_program;
}

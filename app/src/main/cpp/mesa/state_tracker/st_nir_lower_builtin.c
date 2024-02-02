#include "../compiler/nir/nir.h"
#include "../compiler/nir/nir_builder.h"
#include "../compiler/nir/nir_deref.h"
#include "st_nir.h"
#include "../compiler/glsl/ir.h"
#include "../main/uniforms.h"
#include "../program/prog_instruction.h"

typedef struct {
    nir_shader *shader;
    nir_builder builder;
    void *mem_ctx;
} lower_builtin_state;

static const struct gl_builtin_uniform_element *
get_element(const struct gl_builtin_uniform_desc *desc, nir_deref_path *path)
{
    int idx = 1;

    assert(path->path[0]->deref_type == nir_deref_type_var);

    if ((desc->num_elements == 1) && (desc->elements[0].field == NULL))
        return NULL;

    /* we handle arrays in get_variable(): */
    if (path->path[idx]->deref_type == nir_deref_type_array)
        idx++;

    /* don't need to deal w/ non-struct or array of non-struct: */
    if (!path->path[idx])
        return NULL;

    if (path->path[idx]->deref_type != nir_deref_type_struct)
        return NULL;

    assert(path->path[idx]->strct.index < desc->num_elements);

    return &desc->elements[path->path[idx]->strct.index ];
}

static nir_variable *
get_variable(lower_builtin_state *state, nir_deref_path *path,
             const struct gl_builtin_uniform_element *element)
{
    nir_shader *shader = state->shader;
    gl_state_index16 tokens[STATE_LENGTH];
    int idx = 1;

    memcpy(tokens, element->tokens, sizeof(tokens));

    if (path->path[idx]->deref_type == nir_deref_type_array) {
        /* we need to fixup the array index slot: */
        switch (tokens[0]) {
            case STATE_MODELVIEW_MATRIX:
            case STATE_MODELVIEW_MATRIX_INVERSE:
            case STATE_MODELVIEW_MATRIX_TRANSPOSE:
            case STATE_MODELVIEW_MATRIX_INVTRANS:
            case STATE_PROJECTION_MATRIX:
            case STATE_PROJECTION_MATRIX_INVERSE:
            case STATE_PROJECTION_MATRIX_TRANSPOSE:
            case STATE_PROJECTION_MATRIX_INVTRANS:
            case STATE_MVP_MATRIX:
            case STATE_MVP_MATRIX_INVERSE:
            case STATE_MVP_MATRIX_TRANSPOSE:
            case STATE_MVP_MATRIX_INVTRANS:
            case STATE_TEXTURE_MATRIX:
            case STATE_TEXTURE_MATRIX_INVERSE:
            case STATE_TEXTURE_MATRIX_TRANSPOSE:
            case STATE_TEXTURE_MATRIX_INVTRANS:
            case STATE_PROGRAM_MATRIX:
            case STATE_PROGRAM_MATRIX_INVERSE:
            case STATE_PROGRAM_MATRIX_TRANSPOSE:
            case STATE_PROGRAM_MATRIX_INVTRANS:
            case STATE_LIGHT:
            case STATE_LIGHTPROD:
            case STATE_TEXGEN:
            case STATE_TEXENV_COLOR:
            case STATE_CLIPPLANE:
                tokens[1] = nir_src_as_uint(path->path[idx]->arr.index);
                break;
        }
    }

    char *name = _mesa_program_state_string(tokens);

    nir_foreach_uniform_variable(var, shader) {
        if (strcmp(var->name, name) == 0) {
            free(name);
            return var;
        }
    }

    /* variable doesn't exist yet, so create it: */
    nir_variable *var =
            nir_variable_create(shader, nir_var_uniform, glsl_vec4_type(), name);

    var->num_state_slots = 1;
    var->state_slots = rzalloc_array(var, nir_state_slot, 1);
    memcpy(var->state_slots[0].tokens, tokens,
           sizeof(var->state_slots[0].tokens));

    free(name);

    return var;
}

static bool
lower_builtin_block(lower_builtin_state *state, nir_block *block)
{
    nir_builder *b = &state->builder;
    bool progress = false;

    nir_foreach_instr_safe(instr, block) {
        if (instr->type != nir_instr_type_intrinsic)
            continue;

        nir_intrinsic_instr *intrin = nir_instr_as_intrinsic(instr);

        if (intrin->intrinsic != nir_intrinsic_load_deref)
            continue;

        nir_variable *var =
                nir_deref_instr_get_variable(nir_src_as_deref(intrin->src[0]));
        if (var->data.mode != nir_var_uniform)
            continue;

        /* built-in's will always start with "gl_" */
        if (strncmp(var->name, "gl_", 3) != 0)
            continue;

        const struct gl_builtin_uniform_desc *desc =
                _mesa_glsl_get_builtin_uniform_desc(var->name);

        /* if no descriptor, it isn't something we need to handle specially: */
        if (!desc)
            continue;

        nir_deref_path path;
        nir_deref_path_init(&path, nir_src_as_deref(intrin->src[0]), NULL);

        const struct gl_builtin_uniform_element *element = get_element(desc, &path);

        /* matrix elements (array_deref) do not need special handling: */
        if (!element) {
            nir_deref_path_finish(&path);
            continue;
        }

        /* remove existing var from uniform list: */
        exec_node_remove(&var->node);
        /* the _self_link() ensures we can remove multiple times, rather than
         * trying to keep track of what we have already removed:
         */
        exec_node_self_link(&var->node);

        nir_variable *new_var = get_variable(state, &path, element);
        nir_deref_path_finish(&path);

        b->cursor = nir_before_instr(instr);

        nir_ssa_def *def = nir_load_var(b, new_var);

        /* swizzle the result: */
        unsigned swiz[NIR_MAX_VEC_COMPONENTS] = {0};
        for (unsigned i = 0; i < 4; i++) {
            swiz[i] = GET_SWZ(element->swizzle, i);
            assert(swiz[i] <= SWIZZLE_W);
        }
        def = nir_swizzle(b, def, swiz, intrin->num_components);

        /* and rewrite uses of original instruction: */
        assert(intrin->dest.is_ssa);
        nir_ssa_def_rewrite_uses(&intrin->dest.ssa, def);

        /* at this point intrin should be unused.  We need to remove it
         * (rather than waiting for DCE pass) to avoid dangling reference
         * to remove'd var.  And we have to remove the original uniform
         * var since we don't want it to get uniform space allocated.
         */
        nir_instr_remove(&intrin->instr);

        progress = true;
    }

    return progress;
}

static void
lower_builtin_impl(lower_builtin_state *state, nir_function_impl *impl)
{
    nir_builder_init(&state->builder, impl);
    state->mem_ctx = ralloc_parent(impl);

    bool progress = false;
    nir_foreach_block(block, impl) {
        progress |= lower_builtin_block(state, block);
    }

    if (progress)
        nir_remove_dead_derefs_impl(impl);

    nir_metadata_preserve(impl, nir_metadata_block_index |
                                nir_metadata_dominance);
}

void
st_nir_lower_builtin(nir_shader *shader)
{
    lower_builtin_state state;
    state.shader = shader;
    nir_foreach_function(function, shader) {
        if (function->impl)
            lower_builtin_impl(&state, function->impl);
    }
}

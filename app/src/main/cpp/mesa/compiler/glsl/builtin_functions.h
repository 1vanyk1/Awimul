#ifndef BULITIN_FUNCTIONS_H
#define BULITIN_FUNCTIONS_H

struct gl_shader;

#ifdef __cplusplus
extern "C" {
#endif

void
_mesa_glsl_builtin_functions_init_or_ref();

void
_mesa_glsl_builtin_functions_decref(void);

#ifdef __cplusplus

} /* extern "C" */

extern ir_function_signature *
_mesa_glsl_find_builtin_function(_mesa_glsl_parse_state *state,
                                 const char *name, exec_list *actual_parameters);

extern bool
_mesa_glsl_has_builtin_function(_mesa_glsl_parse_state *state,
                                const char *name);

extern gl_shader *
_mesa_glsl_get_builtin_function_shader(void);

extern ir_function_signature *
_mesa_get_main_function_signature(glsl_symbol_table *symbols);

namespace generate_ir {

ir_function_signature *
udiv64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
idiv64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
umod64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
imod64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
umul64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
sign64(void *mem_ctx, builtin_available_predicate avail);

ir_function_signature *
udivmod64(void *mem_ctx, builtin_available_predicate avail);

}

#endif /* __cplusplus */

#endif /* BULITIN_FUNCTIONS_H */

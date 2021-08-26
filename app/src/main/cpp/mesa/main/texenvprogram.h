#ifndef TEXENVPROGRAM_H
#define TEXENVPROGRAM_H


#ifdef __cplusplus
extern "C" {
#endif


struct gl_context;

extern struct gl_shader_program *
_mesa_get_fixed_func_fragment_program(struct gl_context *ctx);


#ifdef __cplusplus
}
#endif


#endif

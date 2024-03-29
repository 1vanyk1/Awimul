#ifndef ST_NIR_H
#define ST_NIR_H

#include "st_context.h"
#include "../compiler/shader_enums.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nir_shader;

void st_nir_lower_builtin(struct nir_shader *shader);
void st_nir_lower_tex_src_plane(struct nir_shader *shader, unsigned free_slots,
                                unsigned lower_2plane, unsigned lower_3plane);

void st_nir_lower_wpos_ytransform(struct nir_shader *nir,
                                  struct gl_program *prog,
                                  struct pipe_screen *pscreen);

char *st_finalize_nir(struct st_context *st, struct gl_program *prog,
                      struct gl_shader_program *shader_program,
                      struct nir_shader *nir, bool finalize_by_driver,
                      bool is_before_variants);

void st_nir_opts(struct nir_shader *nir);

bool
st_link_nir(struct gl_context *ctx,
            struct gl_shader_program *shader_program);

void st_nir_assign_vs_in_locations(struct nir_shader *nir);
void st_nir_assign_varying_locations(struct st_context *st,
                                     struct nir_shader *nir);

void st_nir_lower_samplers(struct pipe_screen *screen, struct nir_shader *nir,
                           struct gl_shader_program *shader_program,
                           struct gl_program *prog);
void st_nir_lower_uniforms(struct st_context *st, struct nir_shader *nir);

struct pipe_shader_state *
st_nir_finish_builtin_shader(struct st_context *st,
                             struct nir_shader *nir);

struct pipe_shader_state *
st_nir_make_passthrough_shader(struct st_context *st,
                               const char *shader_name,
                               gl_shader_stage stage,
                               unsigned num_vars,
                               unsigned *input_locations,
                               unsigned *output_locations,
                               unsigned *interpolation_modes,
                               unsigned sysval_mask);

#ifdef __cplusplus
}
#endif

#endif /* ST_NIR_H */

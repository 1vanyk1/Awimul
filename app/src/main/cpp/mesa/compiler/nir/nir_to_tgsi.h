#ifndef NIR_TO_TGSI_H
#define NIR_TO_TGSI_H

struct nir_shader;
struct pipe_screen;

const void *nir_to_tgsi(struct nir_shader *s,
                        struct pipe_screen *screen);

const void *
nir_to_tgsi_get_compiler_options(struct pipe_screen *pscreen,
                                 enum pipe_shader_ir ir,
                                 unsigned shader);

#endif /* NIR_TO_TGSI_H */

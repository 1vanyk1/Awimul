#ifndef U_SIMPLE_SHADERS_H
#define U_SIMPLE_SHADERS_H


#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_shader_tokens.h"


struct pipe_context;
struct pipe_shader_state;
struct pipe_stream_output_info;


#ifdef __cplusplus
extern "C" {
#endif


extern void *
util_make_vertex_passthrough_shader(struct pipe_context *pipe,
                                    uint num_attribs,
                                    const enum tgsi_semantic *semantic_names,
                                    const uint *semantic_indexes,
                                    bool window_space);

extern void *
util_make_vertex_passthrough_shader_with_so(struct pipe_context *pipe,
                                            uint num_attribs,
                                            const enum tgsi_semantic *semantic_names,
                                            const uint *semantic_indexes,
                                            bool window_space, bool layered,
                                            const struct pipe_stream_output_info *so);

extern void *
util_make_layered_clear_vertex_shader(struct pipe_context *pipe);

extern void *
util_make_layered_clear_helper_vertex_shader(struct pipe_context *pipe);

extern void *
util_make_layered_clear_geometry_shader(struct pipe_context *pipe);

void *
util_make_fragment_tex_shader_xrbias(struct pipe_context *pipe,
                                     enum tgsi_texture_type tex_target);

extern void *
util_make_fragment_tex_shader_writemask(struct pipe_context *pipe,
                                        enum tgsi_texture_type tex_target,
                                        enum tgsi_interpolate_mode interp_mode,
                                        unsigned writemask,
                                        enum tgsi_return_type stype,
                                        enum tgsi_return_type dtype,
                                        bool load_level_zero,
                                        bool use_txf);

extern void *
util_make_fragment_tex_shader(struct pipe_context *pipe,
                              enum tgsi_texture_type tex_target,
                              enum tgsi_interpolate_mode interp_mode,
                              enum tgsi_return_type stype,
                              enum tgsi_return_type dtype,
                              bool load_level_zero,
                              bool use_txf);

extern void *
util_make_fs_blit_zs(struct pipe_context *pipe, unsigned zs_mask,
                     enum tgsi_texture_type tex_target,
                     bool load_level_zero, bool use_txf);

extern void *
util_make_fragment_passthrough_shader(struct pipe_context *pipe,
                                      int input_semantic,
                                      int input_interpolate,
                                      boolean write_all_cbufs);


extern void *
util_make_empty_fragment_shader(struct pipe_context *pipe);


extern void *
util_make_fragment_cloneinput_shader(struct pipe_context *pipe, int num_cbufs,
                                     int input_semantic,
                                     int input_interpolate);


extern void *
util_make_fs_blit_msaa_color(struct pipe_context *pipe,
                             enum tgsi_texture_type tgsi_tex,
                             enum tgsi_return_type stype,
                             enum tgsi_return_type dtype);


extern void *
util_make_fs_blit_msaa_depth(struct pipe_context *pipe,
                             enum tgsi_texture_type tgsi_tex);


extern void *
util_make_fs_blit_msaa_depthstencil(struct pipe_context *pipe,
                                    enum tgsi_texture_type tgsi_tex);


void *
util_make_fs_blit_msaa_stencil(struct pipe_context *pipe,
                               enum tgsi_texture_type tgsi_tex);


void *
util_make_fs_msaa_resolve(struct pipe_context *pipe,
                          enum tgsi_texture_type tgsi_tex, unsigned nr_samples,
                          enum tgsi_return_type stype);


void *
util_make_fs_msaa_resolve_bilinear(struct pipe_context *pipe,
                                   enum tgsi_texture_type tgsi_tex,
                                   unsigned nr_samples,
                                   enum tgsi_return_type stype);

extern void *
util_make_geometry_passthrough_shader(struct pipe_context *pipe,
                                      uint num_attribs,
                                      const ubyte *semantic_names,
                                      const ubyte *semantic_indexes);

void *
util_make_fs_pack_color_zs(struct pipe_context *pipe,
                           enum tgsi_texture_type tex_target,
                           enum pipe_format zs_format,
                           bool dst_is_color);

extern void *
util_make_tess_ctrl_passthrough_shader(struct pipe_context *pipe,
                                       uint num_vs_outputs,
                                       uint num_tes_inputs,
                                       const ubyte *vs_semantic_names,
                                       const ubyte *vs_semantic_indexes,
                                       const ubyte *tes_semantic_names,
                                       const ubyte *tes_semantic_indexes,
                                       const unsigned vertices_per_patch);

void *
util_make_fs_stencil_blit(struct pipe_context *pipe, bool msaa_src);

#ifdef __cplusplus
}
#endif


#endif

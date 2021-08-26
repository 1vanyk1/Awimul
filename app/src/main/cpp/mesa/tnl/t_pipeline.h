#ifndef _T_PIPELINE_H_
#define _T_PIPELINE_H_

#include "../main/mtypes.h"
#include "t_context.h"

extern void _tnl_run_pipeline( struct gl_context *ctx );

extern void _tnl_destroy_pipeline( struct gl_context *ctx );

extern void _tnl_install_pipeline( struct gl_context *ctx,
                                   const struct tnl_pipeline_stage **stages );


/* These are implemented in the t_vb_*.c files:
 */
extern const struct tnl_pipeline_stage _tnl_vertex_transform_stage;
extern const struct tnl_pipeline_stage _tnl_normal_transform_stage;
extern const struct tnl_pipeline_stage _tnl_lighting_stage;
extern const struct tnl_pipeline_stage _tnl_fog_coordinate_stage;
extern const struct tnl_pipeline_stage _tnl_texgen_stage;
extern const struct tnl_pipeline_stage _tnl_texture_transform_stage;
extern const struct tnl_pipeline_stage _tnl_point_attenuation_stage;
extern const struct tnl_pipeline_stage _tnl_vertex_program_stage;
extern const struct tnl_pipeline_stage _tnl_render_stage;

/* Shorthand to plug in the default pipeline:
 */
extern const struct tnl_pipeline_stage *_tnl_default_pipeline[];
extern const struct tnl_pipeline_stage *_tnl_vp_pipeline[];


/* Convenience routines provided by t_vb_render.c:
 */
extern tnl_render_func _tnl_render_tab_elts[];
extern tnl_render_func _tnl_render_tab_verts[];

extern void _tnl_RenderClippedPolygon( struct gl_context *ctx,
                                       const GLuint *elts, GLuint n );

extern void _tnl_RenderClippedLine( struct gl_context *ctx, GLuint ii, GLuint jj );


#endif

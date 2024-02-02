#ifndef AWIMUL_DRI_PRIVATE_H
#define AWIMUL_DRI_PRIVATE_H

#include <stdlib.h>
#include "dd.h"

#define PP_FILTERS 6            /* Increment this if you add filters */
#define PP_MAX_PASSES 6

enum pipe_texture_target
{
    PIPE_BUFFER,
    PIPE_TEXTURE_1D,
    PIPE_TEXTURE_2D,
    PIPE_TEXTURE_3D,
    PIPE_TEXTURE_CUBE,
    PIPE_TEXTURE_RECT,
    PIPE_TEXTURE_1D_ARRAY,
    PIPE_TEXTURE_2D_ARRAY,
    PIPE_TEXTURE_CUBE_ARRAY,
    PIPE_MAX_TEXTURE_TYPES,
};

struct st_config_options
{
    bool disable_blend_func_extended;
    bool disable_glsl_line_continuations;
    bool disable_arb_gpu_shader5;
    bool force_compat_shaders;
    bool force_glsl_extensions_warn;
    unsigned force_glsl_version;
    bool allow_extra_pp_tokens;
    bool allow_glsl_extension_directive_midshader;
    bool allow_glsl_120_subset_in_110;
    bool allow_glsl_builtin_const_expression;
    bool allow_glsl_relaxed_es;
    bool allow_glsl_builtin_variable_redeclaration;
    bool allow_higher_compat_version;
    bool allow_glsl_compat_shaders;
    bool glsl_ignore_write_to_readonly_var;
    bool glsl_zero_init;
    bool vs_position_always_invariant;
    bool vs_position_always_precise;
    bool force_glsl_abs_sqrt;
    bool allow_glsl_cross_stage_interpolation_mismatch;
    bool do_dce_before_clip_cull_analysis;
    bool allow_draw_out_of_order;
    bool glthread_nop_check_framebuffer_status;
    bool ignore_map_unsynchronized;
    bool force_integer_tex_nearest;
    bool force_gl_names_reuse;
    bool transcode_etc;
    bool transcode_astc;
    char *force_gl_vendor;
    char *force_gl_renderer;
    char *mesa_extension_override;
    unsigned char config_options_sha1[20];
};

enum pipe_fd_type
{
    PIPE_FD_TYPE_NATIVE_SYNC,
    PIPE_FD_TYPE_SYNCOBJ,
};

static inline struct dri_screen *
dri_screen(__DRIscreen * sPriv)
{
    return (struct dri_screen *)sPriv->driverPrivate;
}

enum pipe_loader_device_type {
    PIPE_LOADER_DEVICE_SOFTWARE,
    PIPE_LOADER_DEVICE_PCI,
    PIPE_LOADER_DEVICE_PLATFORM,
    NUM_PIPE_LOADER_DEVICE_TYPES
};

struct pipe_loader_ops {
    struct pipe_screen *(*create_screen)(struct pipe_loader_device *dev,
                                         const struct pipe_screen_config *config, bool sw_vk);

    const struct driOptionDescription *(*get_driconf)(struct pipe_loader_device *dev,
                                                      unsigned *count);

    void (*release)(struct pipe_loader_device **dev);
};

enum st_api_type {
    ST_API_OPENGL,
    ST_API_OPENVG,
    ST_API_COUNT
};

enum st_context_error {
    ST_CONTEXT_SUCCESS = 0,
    ST_CONTEXT_ERROR_NO_MEMORY,
    ST_CONTEXT_ERROR_BAD_API,
    ST_CONTEXT_ERROR_BAD_VERSION,
    ST_CONTEXT_ERROR_BAD_FLAG,
    ST_CONTEXT_ERROR_UNKNOWN_ATTRIBUTE,
    ST_CONTEXT_ERROR_UNKNOWN_FLAG
};

#define	SHA1_BLOCK_LENGTH		64
#define	SHA1_DIGEST_LENGTH		20
#define	SHA1_DIGEST_STRING_LENGTH	(SHA1_DIGEST_LENGTH * 2 + 1)

typedef struct _SHA1_CTX {
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[SHA1_BLOCK_LENGTH];
} SHA1_CTX;

#define mesa_sha1 _SHA1_CTX

struct st_api
{
    const char *name;
    enum st_api_type api;
    unsigned profile_mask;
    unsigned feature_mask;
    void (*destroy)(struct st_api *stapi);
    void (*query_versions)(struct st_api *stapi, struct st_manager *sm,
                           struct st_config_options *options,
                           int *gl_core_version,
                           int *gl_compat_version,
                           int *gl_es1_version,
                           int *gl_es2_version);
    struct st_context_iface *(*create_context)(struct st_api *stapi,
                                               struct st_manager *smapi,
                                               const struct st_context_attribs *attribs,
                                               enum st_context_error *error,
                                               struct st_context_iface *stsharei);
    bool (*make_current)(struct st_api *stapi,
                         struct st_context_iface *stctxi,
                         struct st_framebuffer_iface *stdrawi,
                         struct st_framebuffer_iface *streadi);
    struct st_context_iface *(*get_current)(struct st_api *stapi);
    void (*destroy_drawable)(struct st_api *stapi,
                             struct st_framebuffer_iface *stfbi);
};

enum pipe_cap
{
    PIPE_CAP_GRAPHICS,
    PIPE_CAP_NPOT_TEXTURES,
    PIPE_CAP_MAX_DUAL_SOURCE_RENDER_TARGETS,
    PIPE_CAP_ANISOTROPIC_FILTER,
    PIPE_CAP_POINT_SPRITE,
    PIPE_CAP_MAX_RENDER_TARGETS,
    PIPE_CAP_OCCLUSION_QUERY,
    PIPE_CAP_QUERY_TIME_ELAPSED,
    PIPE_CAP_TEXTURE_SHADOW_MAP,
    PIPE_CAP_TEXTURE_SWIZZLE,
    PIPE_CAP_MAX_TEXTURE_2D_SIZE,
    PIPE_CAP_MAX_TEXTURE_3D_LEVELS,
    PIPE_CAP_MAX_TEXTURE_CUBE_LEVELS,
    PIPE_CAP_TEXTURE_MIRROR_CLAMP,
    PIPE_CAP_BLEND_EQUATION_SEPARATE,
    PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS,
    PIPE_CAP_PRIMITIVE_RESTART,
    /** subset of PRIMITIVE_RESTART where the restart index is always the fixed
     * maximum value for the index type
     */
    PIPE_CAP_PRIMITIVE_RESTART_FIXED_INDEX,
    /** blend enables and write masks per rendertarget */
    PIPE_CAP_INDEP_BLEND_ENABLE,
    /** different blend funcs per rendertarget */
    PIPE_CAP_INDEP_BLEND_FUNC,
    PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS,
    PIPE_CAP_TGSI_FS_COORD_ORIGIN_UPPER_LEFT,
    PIPE_CAP_TGSI_FS_COORD_ORIGIN_LOWER_LEFT,
    PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_HALF_INTEGER,
    PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_INTEGER,
    PIPE_CAP_DEPTH_CLIP_DISABLE,
    PIPE_CAP_DEPTH_CLIP_DISABLE_SEPARATE,
    PIPE_CAP_DEPTH_CLAMP_ENABLE,
    PIPE_CAP_SHADER_STENCIL_EXPORT,
    PIPE_CAP_TGSI_INSTANCEID,
    PIPE_CAP_VERTEX_ELEMENT_INSTANCE_DIVISOR,
    PIPE_CAP_FRAGMENT_COLOR_CLAMPED,
    PIPE_CAP_MIXED_COLORBUFFER_FORMATS,
    PIPE_CAP_SEAMLESS_CUBE_MAP,
    PIPE_CAP_SEAMLESS_CUBE_MAP_PER_TEXTURE,
    PIPE_CAP_MIN_TEXEL_OFFSET,
    PIPE_CAP_MAX_TEXEL_OFFSET,
    PIPE_CAP_CONDITIONAL_RENDER,
    PIPE_CAP_TEXTURE_BARRIER,
    PIPE_CAP_MAX_STREAM_OUTPUT_SEPARATE_COMPONENTS,
    PIPE_CAP_MAX_STREAM_OUTPUT_INTERLEAVED_COMPONENTS,
    PIPE_CAP_STREAM_OUTPUT_PAUSE_RESUME,
    PIPE_CAP_TGSI_CAN_COMPACT_CONSTANTS,
    PIPE_CAP_VERTEX_COLOR_UNCLAMPED,
    PIPE_CAP_VERTEX_COLOR_CLAMPED,
    PIPE_CAP_GLSL_FEATURE_LEVEL,
    PIPE_CAP_GLSL_FEATURE_LEVEL_COMPATIBILITY,
    PIPE_CAP_ESSL_FEATURE_LEVEL,
    PIPE_CAP_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION,
    PIPE_CAP_USER_VERTEX_BUFFERS,
    PIPE_CAP_VERTEX_BUFFER_OFFSET_4BYTE_ALIGNED_ONLY,
    PIPE_CAP_VERTEX_BUFFER_STRIDE_4BYTE_ALIGNED_ONLY,
    PIPE_CAP_VERTEX_ELEMENT_SRC_OFFSET_4BYTE_ALIGNED_ONLY,
    PIPE_CAP_VERTEX_ATTRIB_ELEMENT_ALIGNED_ONLY,
    PIPE_CAP_COMPUTE,
    PIPE_CAP_CONSTANT_BUFFER_OFFSET_ALIGNMENT,
    PIPE_CAP_START_INSTANCE,
    PIPE_CAP_QUERY_TIMESTAMP,
    PIPE_CAP_TEXTURE_MULTISAMPLE,
    PIPE_CAP_MIN_MAP_BUFFER_ALIGNMENT,
    PIPE_CAP_CUBE_MAP_ARRAY,
    PIPE_CAP_TEXTURE_BUFFER_OBJECTS,
    PIPE_CAP_TEXTURE_BUFFER_OFFSET_ALIGNMENT,
    PIPE_CAP_BUFFER_SAMPLER_VIEW_RGBA_ONLY,
    PIPE_CAP_TGSI_TEXCOORD,
    PIPE_CAP_TEXTURE_BUFFER_SAMPLER,
    PIPE_CAP_TEXTURE_TRANSFER_MODES,
    PIPE_CAP_QUERY_PIPELINE_STATISTICS,
    PIPE_CAP_TEXTURE_BORDER_COLOR_QUIRK,
    PIPE_CAP_MAX_TEXTURE_BUFFER_SIZE,
    PIPE_CAP_MAX_VIEWPORTS,
    PIPE_CAP_ENDIANNESS,
    PIPE_CAP_MIXED_FRAMEBUFFER_SIZES,
    PIPE_CAP_TGSI_VS_LAYER_VIEWPORT,
    PIPE_CAP_MAX_GEOMETRY_OUTPUT_VERTICES,
    PIPE_CAP_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS,
    PIPE_CAP_MAX_TEXTURE_GATHER_COMPONENTS,
    PIPE_CAP_TEXTURE_GATHER_SM5,
    PIPE_CAP_BUFFER_MAP_PERSISTENT_COHERENT,
    PIPE_CAP_FAKE_SW_MSAA,
    PIPE_CAP_TEXTURE_QUERY_LOD,
    PIPE_CAP_MIN_TEXTURE_GATHER_OFFSET,
    PIPE_CAP_MAX_TEXTURE_GATHER_OFFSET,
    PIPE_CAP_SAMPLE_SHADING,
    PIPE_CAP_TEXTURE_GATHER_OFFSETS,
    PIPE_CAP_TGSI_VS_WINDOW_SPACE_POSITION,
    PIPE_CAP_MAX_VERTEX_STREAMS,
    PIPE_CAP_DRAW_INDIRECT,
    PIPE_CAP_TGSI_FS_FINE_DERIVATIVE,
    PIPE_CAP_VENDOR_ID,
    PIPE_CAP_DEVICE_ID,
    PIPE_CAP_ACCELERATED,
    PIPE_CAP_VIDEO_MEMORY,
    PIPE_CAP_UMA,
    PIPE_CAP_CONDITIONAL_RENDER_INVERTED,
    PIPE_CAP_MAX_VERTEX_ATTRIB_STRIDE,
    PIPE_CAP_SAMPLER_VIEW_TARGET,
    PIPE_CAP_CLIP_HALFZ,
    PIPE_CAP_VERTEXID_NOBASE,
    PIPE_CAP_POLYGON_OFFSET_CLAMP,
    PIPE_CAP_MULTISAMPLE_Z_RESOLVE,
    PIPE_CAP_RESOURCE_FROM_USER_MEMORY,
    PIPE_CAP_RESOURCE_FROM_USER_MEMORY_COMPUTE_ONLY,
    PIPE_CAP_DEVICE_RESET_STATUS_QUERY,
    PIPE_CAP_MAX_SHADER_PATCH_VARYINGS,
    PIPE_CAP_TEXTURE_FLOAT_LINEAR,
    PIPE_CAP_TEXTURE_HALF_FLOAT_LINEAR,
    PIPE_CAP_DEPTH_BOUNDS_TEST,
    PIPE_CAP_TGSI_TXQS,
    PIPE_CAP_FORCE_PERSAMPLE_INTERP,
    PIPE_CAP_SHAREABLE_SHADERS,
    PIPE_CAP_COPY_BETWEEN_COMPRESSED_AND_PLAIN_FORMATS,
    PIPE_CAP_CLEAR_TEXTURE,
    PIPE_CAP_CLEAR_SCISSORED,
    PIPE_CAP_DRAW_PARAMETERS,
    PIPE_CAP_TGSI_PACK_HALF_FLOAT,
    PIPE_CAP_MULTI_DRAW_INDIRECT,
    PIPE_CAP_MULTI_DRAW_INDIRECT_PARAMS,
    PIPE_CAP_TGSI_FS_POSITION_IS_SYSVAL,
    PIPE_CAP_TGSI_FS_POINT_IS_SYSVAL,
    PIPE_CAP_TGSI_FS_FACE_IS_INTEGER_SYSVAL,
    PIPE_CAP_SHADER_BUFFER_OFFSET_ALIGNMENT,
    PIPE_CAP_INVALIDATE_BUFFER,
    PIPE_CAP_GENERATE_MIPMAP,
    PIPE_CAP_STRING_MARKER,
    PIPE_CAP_SURFACE_REINTERPRET_BLOCKS,
    PIPE_CAP_QUERY_BUFFER_OBJECT,
    PIPE_CAP_QUERY_MEMORY_INFO,
    PIPE_CAP_PCI_GROUP,
    PIPE_CAP_PCI_BUS,
    PIPE_CAP_PCI_DEVICE,
    PIPE_CAP_PCI_FUNCTION,
    PIPE_CAP_FRAMEBUFFER_NO_ATTACHMENT,
    PIPE_CAP_ROBUST_BUFFER_ACCESS_BEHAVIOR,
    PIPE_CAP_CULL_DISTANCE,
    PIPE_CAP_TGSI_VOTE,
    PIPE_CAP_MAX_WINDOW_RECTANGLES,
    PIPE_CAP_POLYGON_OFFSET_UNITS_UNSCALED,
    PIPE_CAP_VIEWPORT_SUBPIXEL_BITS,
    PIPE_CAP_RASTERIZER_SUBPIXEL_BITS,
    PIPE_CAP_MIXED_COLOR_DEPTH_BITS,
    PIPE_CAP_TGSI_ARRAY_COMPONENTS,
    PIPE_CAP_STREAM_OUTPUT_INTERLEAVE_BUFFERS,
    PIPE_CAP_TGSI_CAN_READ_OUTPUTS,
    PIPE_CAP_NATIVE_FENCE_FD,
    PIPE_CAP_GLSL_OPTIMIZE_CONSERVATIVELY,
    PIPE_CAP_GLSL_TESS_LEVELS_AS_INPUTS,
    PIPE_CAP_FBFETCH,
    PIPE_CAP_TGSI_MUL_ZERO_WINS,
    PIPE_CAP_DOUBLES,
    PIPE_CAP_INT64,
    PIPE_CAP_INT64_DIVMOD,
    PIPE_CAP_TGSI_TEX_TXF_LZ,
    PIPE_CAP_TGSI_CLOCK,
    PIPE_CAP_POLYGON_MODE_FILL_RECTANGLE,
    PIPE_CAP_SPARSE_BUFFER_PAGE_SIZE,
    PIPE_CAP_TGSI_BALLOT,
    PIPE_CAP_TGSI_TES_LAYER_VIEWPORT,
    PIPE_CAP_CAN_BIND_CONST_BUFFER_AS_VERTEX,
    PIPE_CAP_ALLOW_MAPPED_BUFFERS_DURING_EXECUTION,
    PIPE_CAP_POST_DEPTH_COVERAGE,
    PIPE_CAP_BINDLESS_TEXTURE,
    PIPE_CAP_NIR_SAMPLERS_AS_DEREF,
    PIPE_CAP_QUERY_SO_OVERFLOW,
    PIPE_CAP_MEMOBJ,
    PIPE_CAP_LOAD_CONSTBUF,
    PIPE_CAP_TILE_RASTER_ORDER,
    PIPE_CAP_MAX_COMBINED_SHADER_OUTPUT_RESOURCES,
    PIPE_CAP_FRAMEBUFFER_MSAA_CONSTRAINTS,
    PIPE_CAP_SIGNED_VERTEX_BUFFER_OFFSET,
    PIPE_CAP_CONTEXT_PRIORITY_MASK,
    PIPE_CAP_FENCE_SIGNAL,
    PIPE_CAP_CONSTBUF0_FLAGS,
    PIPE_CAP_PACKED_UNIFORMS,
    PIPE_CAP_CONSERVATIVE_RASTER_POST_SNAP_TRIANGLES,
    PIPE_CAP_CONSERVATIVE_RASTER_POST_SNAP_POINTS_LINES,
    PIPE_CAP_CONSERVATIVE_RASTER_PRE_SNAP_TRIANGLES,
    PIPE_CAP_CONSERVATIVE_RASTER_PRE_SNAP_POINTS_LINES,
    PIPE_CAP_MAX_CONSERVATIVE_RASTER_SUBPIXEL_PRECISION_BIAS,
    PIPE_CAP_CONSERVATIVE_RASTER_POST_DEPTH_COVERAGE,
    PIPE_CAP_CONSERVATIVE_RASTER_INNER_COVERAGE,
    PIPE_CAP_PROGRAMMABLE_SAMPLE_LOCATIONS,
    PIPE_CAP_MAX_GS_INVOCATIONS,
    PIPE_CAP_MAX_SHADER_BUFFER_SIZE,
    PIPE_CAP_TEXTURE_MIRROR_CLAMP_TO_EDGE,
    PIPE_CAP_MAX_COMBINED_SHADER_BUFFERS,
    PIPE_CAP_MAX_COMBINED_HW_ATOMIC_COUNTERS,
    PIPE_CAP_MAX_COMBINED_HW_ATOMIC_COUNTER_BUFFERS,
    PIPE_CAP_MAX_TEXTURE_UPLOAD_MEMORY_BUDGET,
    PIPE_CAP_MAX_VERTEX_ELEMENT_SRC_OFFSET,
    PIPE_CAP_SURFACE_SAMPLE_COUNT,
    PIPE_CAP_TGSI_ATOMFADD,
    PIPE_CAP_QUERY_PIPELINE_STATISTICS_SINGLE,
    PIPE_CAP_RGB_OVERRIDE_DST_ALPHA_BLEND,
    PIPE_CAP_DEST_SURFACE_SRGB_CONTROL,
    PIPE_CAP_NIR_COMPACT_ARRAYS,
    PIPE_CAP_MAX_VARYINGS,
    PIPE_CAP_COMPUTE_GRID_INFO_LAST_BLOCK,
    PIPE_CAP_COMPUTE_SHADER_DERIVATIVES,
    PIPE_CAP_TGSI_SKIP_SHRINK_IO_ARRAYS,
    PIPE_CAP_IMAGE_LOAD_FORMATTED,
    PIPE_CAP_IMAGE_STORE_FORMATTED,
    PIPE_CAP_THROTTLE,
    PIPE_CAP_DMABUF,
    PIPE_CAP_PREFER_COMPUTE_FOR_MULTIMEDIA,
    PIPE_CAP_FRAGMENT_SHADER_INTERLOCK,
    PIPE_CAP_FBFETCH_COHERENT,
    PIPE_CAP_CS_DERIVED_SYSTEM_VALUES_SUPPORTED,
    PIPE_CAP_ATOMIC_FLOAT_MINMAX,
    PIPE_CAP_TGSI_DIV,
    PIPE_CAP_FRAGMENT_SHADER_TEXTURE_LOD,
    PIPE_CAP_FRAGMENT_SHADER_DERIVATIVES,
    PIPE_CAP_VERTEX_SHADER_SATURATE,
    PIPE_CAP_TEXTURE_SHADOW_LOD,
    PIPE_CAP_SHADER_SAMPLES_IDENTICAL,
    PIPE_CAP_TGSI_ATOMINC_WRAP,
    PIPE_CAP_PREFER_IMM_ARRAYS_AS_CONSTBUF,
    PIPE_CAP_GL_SPIRV,
    PIPE_CAP_GL_SPIRV_VARIABLE_POINTERS,
    PIPE_CAP_DEMOTE_TO_HELPER_INVOCATION,
    PIPE_CAP_TGSI_TG4_COMPONENT_IN_SWIZZLE,
    PIPE_CAP_FLATSHADE,
    PIPE_CAP_ALPHA_TEST,
    PIPE_CAP_POINT_SIZE_FIXED,
    PIPE_CAP_TWO_SIDED_COLOR,
    PIPE_CAP_CLIP_PLANES,
    PIPE_CAP_MAX_VERTEX_BUFFERS,
    PIPE_CAP_OPENCL_INTEGER_FUNCTIONS,
    PIPE_CAP_INTEGER_MULTIPLY_32X16,
    /* Turn draw, dispatch, blit into NOOP */
    PIPE_CAP_FRONTEND_NOOP,
    PIPE_CAP_NIR_IMAGES_AS_DEREF,
    PIPE_CAP_PACKED_STREAM_OUTPUT,
    PIPE_CAP_VIEWPORT_TRANSFORM_LOWERED,
    PIPE_CAP_PSIZ_CLAMPED,
    PIPE_CAP_GL_BEGIN_END_BUFFER_SIZE,
    PIPE_CAP_VIEWPORT_SWIZZLE,
    PIPE_CAP_SYSTEM_SVM,
    PIPE_CAP_VIEWPORT_MASK,
    PIPE_CAP_ALPHA_TO_COVERAGE_DITHER_CONTROL,
    PIPE_CAP_MAP_UNSYNCHRONIZED_THREAD_SAFE,
    PIPE_CAP_GLSL_ZERO_INIT,
    PIPE_CAP_BLEND_EQUATION_ADVANCED,
    PIPE_CAP_NIR_ATOMICS_AS_DEREF,
    PIPE_CAP_NO_CLIP_ON_COPY_TEX,
    PIPE_CAP_MAX_TEXTURE_MB,
    PIPE_CAP_SHADER_ATOMIC_INT64,
    PIPE_CAP_DEVICE_PROTECTED_CONTENT,
    PIPE_CAP_PREFER_REAL_BUFFER_IN_CONSTBUF0,
    PIPE_CAP_GL_CLAMP,
    PIPE_CAP_TEXRECT,
    PIPE_CAP_SAMPLER_REDUCTION_MINMAX,
    PIPE_CAP_SAMPLER_REDUCTION_MINMAX_ARB,
    PIPE_CAP_ALLOW_DYNAMIC_VAO_FASTPATH,
    PIPE_CAP_EMULATE_NONFIXED_PRIMITIVE_RESTART,
    PIPE_CAP_SUPPORTED_PRIM_MODES,
    PIPE_CAP_SUPPORTED_PRIM_MODES_WITH_RESTART,
    PIPE_CAP_PREFER_BACK_BUFFER_REUSE,
    PIPE_CAP_DRAW_VERTEX_STATE,
    PIPE_CAP_PREFER_POT_ALIGNED_VARYINGS,
    PIPE_CAP_MAX_SPARSE_TEXTURE_SIZE,
    PIPE_CAP_MAX_SPARSE_3D_TEXTURE_SIZE,
    PIPE_CAP_MAX_SPARSE_ARRAY_TEXTURE_LAYERS,
    PIPE_CAP_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS,
    PIPE_CAP_QUERY_SPARSE_TEXTURE_RESIDENCY,
    PIPE_CAP_CLAMP_SPARSE_TEXTURE_LOD,

    PIPE_CAP_LAST,
    /* XXX do not add caps after PIPE_CAP_LAST! */
};


enum pipe_capf
{
    PIPE_CAPF_MAX_LINE_WIDTH,
    PIPE_CAPF_MAX_LINE_WIDTH_AA,
    PIPE_CAPF_MAX_POINT_WIDTH,
    PIPE_CAPF_MAX_POINT_WIDTH_AA,
    PIPE_CAPF_MAX_TEXTURE_ANISOTROPY,
    PIPE_CAPF_MAX_TEXTURE_LOD_BIAS,
    PIPE_CAPF_MIN_CONSERVATIVE_RASTER_DILATE,
    PIPE_CAPF_MAX_CONSERVATIVE_RASTER_DILATE,
    PIPE_CAPF_CONSERVATIVE_RASTER_DILATE_GRANULARITY,
};

enum pipe_shader_type {
    PIPE_SHADER_VERTEX,
    PIPE_SHADER_FRAGMENT,
    PIPE_SHADER_GEOMETRY,
    PIPE_SHADER_TESS_CTRL,
    PIPE_SHADER_TESS_EVAL,
    PIPE_SHADER_COMPUTE,
    PIPE_SHADER_TYPES,
};

enum pipe_shader_cap
{
    PIPE_SHADER_CAP_MAX_INSTRUCTIONS, /* if 0, it means the stage is unsupported */
    PIPE_SHADER_CAP_MAX_ALU_INSTRUCTIONS,
    PIPE_SHADER_CAP_MAX_TEX_INSTRUCTIONS,
    PIPE_SHADER_CAP_MAX_TEX_INDIRECTIONS,
    PIPE_SHADER_CAP_MAX_CONTROL_FLOW_DEPTH,
    PIPE_SHADER_CAP_MAX_INPUTS,
    PIPE_SHADER_CAP_MAX_OUTPUTS,
    PIPE_SHADER_CAP_MAX_CONST_BUFFER_SIZE,
    PIPE_SHADER_CAP_MAX_CONST_BUFFERS,
    PIPE_SHADER_CAP_MAX_TEMPS,
    /* boolean caps */
    PIPE_SHADER_CAP_TGSI_CONT_SUPPORTED,
    PIPE_SHADER_CAP_INDIRECT_INPUT_ADDR,
    PIPE_SHADER_CAP_INDIRECT_OUTPUT_ADDR,
    PIPE_SHADER_CAP_INDIRECT_TEMP_ADDR,
    PIPE_SHADER_CAP_INDIRECT_CONST_ADDR,
    PIPE_SHADER_CAP_SUBROUTINES, /* BGNSUB, ENDSUB, CAL, RET */
    PIPE_SHADER_CAP_INTEGERS,
    PIPE_SHADER_CAP_INT64_ATOMICS,
    PIPE_SHADER_CAP_FP16,
    PIPE_SHADER_CAP_FP16_DERIVATIVES,
    PIPE_SHADER_CAP_FP16_CONST_BUFFERS,
    PIPE_SHADER_CAP_INT16,
    PIPE_SHADER_CAP_GLSL_16BIT_CONSTS,
    PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS,
    PIPE_SHADER_CAP_PREFERRED_IR,
    PIPE_SHADER_CAP_TGSI_SQRT_SUPPORTED,
    PIPE_SHADER_CAP_MAX_SAMPLER_VIEWS,
    PIPE_SHADER_CAP_TGSI_DROUND_SUPPORTED, /* all rounding modes */
    PIPE_SHADER_CAP_TGSI_DFRACEXP_DLDEXP_SUPPORTED,
    PIPE_SHADER_CAP_TGSI_FMA_SUPPORTED,
    PIPE_SHADER_CAP_TGSI_ANY_INOUT_DECL_RANGE,
    PIPE_SHADER_CAP_MAX_UNROLL_ITERATIONS_HINT,
    PIPE_SHADER_CAP_MAX_SHADER_BUFFERS,
    PIPE_SHADER_CAP_SUPPORTED_IRS,
    PIPE_SHADER_CAP_MAX_SHADER_IMAGES,
    PIPE_SHADER_CAP_LOWER_IF_THRESHOLD,
    PIPE_SHADER_CAP_TGSI_SKIP_MERGE_REGISTERS,
    PIPE_SHADER_CAP_TGSI_LDEXP_SUPPORTED,
    PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTERS,
    PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTER_BUFFERS,
};

enum pipe_video_profile
{
    PIPE_VIDEO_PROFILE_UNKNOWN,
    PIPE_VIDEO_PROFILE_MPEG1,
    PIPE_VIDEO_PROFILE_MPEG2_SIMPLE,
    PIPE_VIDEO_PROFILE_MPEG2_MAIN,
    PIPE_VIDEO_PROFILE_MPEG4_SIMPLE,
    PIPE_VIDEO_PROFILE_MPEG4_ADVANCED_SIMPLE,
    PIPE_VIDEO_PROFILE_VC1_SIMPLE,
    PIPE_VIDEO_PROFILE_VC1_MAIN,
    PIPE_VIDEO_PROFILE_VC1_ADVANCED,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_BASELINE,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_CONSTRAINED_BASELINE,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_MAIN,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_EXTENDED,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH10,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH422,
    PIPE_VIDEO_PROFILE_MPEG4_AVC_HIGH444,
    PIPE_VIDEO_PROFILE_HEVC_MAIN,
    PIPE_VIDEO_PROFILE_HEVC_MAIN_10,
    PIPE_VIDEO_PROFILE_HEVC_MAIN_STILL,
    PIPE_VIDEO_PROFILE_HEVC_MAIN_12,
    PIPE_VIDEO_PROFILE_HEVC_MAIN_444,
    PIPE_VIDEO_PROFILE_JPEG_BASELINE,
    PIPE_VIDEO_PROFILE_VP9_PROFILE0,
    PIPE_VIDEO_PROFILE_VP9_PROFILE2,
    PIPE_VIDEO_PROFILE_AV1_MAIN,
    PIPE_VIDEO_PROFILE_MAX
};

enum pipe_video_entrypoint
{
    PIPE_VIDEO_ENTRYPOINT_UNKNOWN,
    PIPE_VIDEO_ENTRYPOINT_BITSTREAM,
    PIPE_VIDEO_ENTRYPOINT_IDCT,
    PIPE_VIDEO_ENTRYPOINT_MC,
    PIPE_VIDEO_ENTRYPOINT_ENCODE
};

enum pipe_video_cap
{
    PIPE_VIDEO_CAP_SUPPORTED = 0,
    PIPE_VIDEO_CAP_NPOT_TEXTURES = 1,
    PIPE_VIDEO_CAP_MAX_WIDTH = 2,
    PIPE_VIDEO_CAP_MAX_HEIGHT = 3,
    PIPE_VIDEO_CAP_PREFERED_FORMAT = 4,
    PIPE_VIDEO_CAP_PREFERS_INTERLACED = 5,
    PIPE_VIDEO_CAP_SUPPORTS_PROGRESSIVE = 6,
    PIPE_VIDEO_CAP_SUPPORTS_INTERLACED = 7,
    PIPE_VIDEO_CAP_MAX_LEVEL = 8,
    PIPE_VIDEO_CAP_STACKED_FRAMES = 9
};

enum pipe_shader_ir
{
    PIPE_SHADER_IR_TGSI = 0,
    PIPE_SHADER_IR_NATIVE,
    PIPE_SHADER_IR_NIR,
    PIPE_SHADER_IR_NIR_SERIALIZED,
};

enum pipe_compute_cap
{
    PIPE_COMPUTE_CAP_ADDRESS_BITS,
    PIPE_COMPUTE_CAP_IR_TARGET,
    PIPE_COMPUTE_CAP_GRID_DIMENSION,
    PIPE_COMPUTE_CAP_MAX_GRID_SIZE,
    PIPE_COMPUTE_CAP_MAX_BLOCK_SIZE,
    PIPE_COMPUTE_CAP_MAX_THREADS_PER_BLOCK,
    PIPE_COMPUTE_CAP_MAX_GLOBAL_SIZE,
    PIPE_COMPUTE_CAP_MAX_LOCAL_SIZE,
    PIPE_COMPUTE_CAP_MAX_PRIVATE_SIZE,
    PIPE_COMPUTE_CAP_MAX_INPUT_SIZE,
    PIPE_COMPUTE_CAP_MAX_MEM_ALLOC_SIZE,
    PIPE_COMPUTE_CAP_MAX_CLOCK_FREQUENCY,
    PIPE_COMPUTE_CAP_MAX_COMPUTE_UNITS,
    PIPE_COMPUTE_CAP_IMAGES_SUPPORTED,
    PIPE_COMPUTE_CAP_SUBGROUP_SIZE,
    PIPE_COMPUTE_CAP_MAX_VARIABLE_THREADS_PER_BLOCK,
};

enum pipe_format {
    PIPE_FORMAT_NONE,
    /* Vertex formats must be first and must be <= 255. */
    PIPE_FORMAT_R64_UINT,    /**< raw doubles (ARB_vertex_attrib_64bit) */
    PIPE_FORMAT_R64G64_UINT,
    PIPE_FORMAT_R64G64B64_UINT,
    PIPE_FORMAT_R64G64B64A64_UINT,
    PIPE_FORMAT_R64_FLOAT,   /**< doubles converted to float */
    PIPE_FORMAT_R64G64_FLOAT,
    PIPE_FORMAT_R64G64B64_FLOAT,
    PIPE_FORMAT_R64G64B64A64_FLOAT,
    PIPE_FORMAT_R32_FLOAT,
    PIPE_FORMAT_R32G32_FLOAT,
    PIPE_FORMAT_R32G32B32_FLOAT,
    PIPE_FORMAT_R32G32B32A32_FLOAT,
    PIPE_FORMAT_R32_UNORM,
    PIPE_FORMAT_R32G32_UNORM,
    PIPE_FORMAT_R32G32B32_UNORM,
    PIPE_FORMAT_R32G32B32A32_UNORM,
    PIPE_FORMAT_R32_USCALED,
    PIPE_FORMAT_R32G32_USCALED,
    PIPE_FORMAT_R32G32B32_USCALED,
    PIPE_FORMAT_R32G32B32A32_USCALED,
    PIPE_FORMAT_R32_SNORM,
    PIPE_FORMAT_R32G32_SNORM,
    PIPE_FORMAT_R32G32B32_SNORM,
    PIPE_FORMAT_R32G32B32A32_SNORM,
    PIPE_FORMAT_R32_SSCALED,
    PIPE_FORMAT_R32G32_SSCALED,
    PIPE_FORMAT_R32G32B32_SSCALED,
    PIPE_FORMAT_R32G32B32A32_SSCALED,
    PIPE_FORMAT_R16_UNORM,
    PIPE_FORMAT_R16G16_UNORM,
    PIPE_FORMAT_R16G16B16_UNORM,
    PIPE_FORMAT_R16G16B16A16_UNORM,
    PIPE_FORMAT_R16_USCALED,
    PIPE_FORMAT_R16G16_USCALED,
    PIPE_FORMAT_R16G16B16_USCALED,
    PIPE_FORMAT_R16G16B16A16_USCALED,
    PIPE_FORMAT_R16_SNORM,
    PIPE_FORMAT_R16G16_SNORM,
    PIPE_FORMAT_R16G16B16_SNORM,
    PIPE_FORMAT_R16G16B16A16_SNORM,
    PIPE_FORMAT_R16_SSCALED,
    PIPE_FORMAT_R16G16_SSCALED,
    PIPE_FORMAT_R16G16B16_SSCALED,
    PIPE_FORMAT_R16G16B16A16_SSCALED,
    PIPE_FORMAT_R8_UNORM,
    PIPE_FORMAT_R8G8_UNORM,
    PIPE_FORMAT_R8G8B8_UNORM,
    PIPE_FORMAT_R8G8B8A8_UNORM,
    PIPE_FORMAT_B8G8R8A8_UNORM,
    PIPE_FORMAT_R8_USCALED,
    PIPE_FORMAT_R8G8_USCALED,
    PIPE_FORMAT_R8G8B8_USCALED,
    PIPE_FORMAT_B8G8R8_USCALED,
    PIPE_FORMAT_R8G8B8A8_USCALED,
    PIPE_FORMAT_B8G8R8A8_USCALED,
    PIPE_FORMAT_A8B8G8R8_USCALED,
    PIPE_FORMAT_R8_SNORM,
    PIPE_FORMAT_R8G8_SNORM,
    PIPE_FORMAT_R8G8B8_SNORM,
    PIPE_FORMAT_B8G8R8_SNORM,
    PIPE_FORMAT_R8G8B8A8_SNORM,
    PIPE_FORMAT_B8G8R8A8_SNORM,
    PIPE_FORMAT_R8_SSCALED,
    PIPE_FORMAT_R8G8_SSCALED,
    PIPE_FORMAT_R8G8B8_SSCALED,
    PIPE_FORMAT_B8G8R8_SSCALED,
    PIPE_FORMAT_R8G8B8A8_SSCALED,
    PIPE_FORMAT_B8G8R8A8_SSCALED,
    PIPE_FORMAT_A8B8G8R8_SSCALED,
    PIPE_FORMAT_A8R8G8B8_UNORM,
    PIPE_FORMAT_R32_FIXED,
    PIPE_FORMAT_R32G32_FIXED,
    PIPE_FORMAT_R32G32B32_FIXED,
    PIPE_FORMAT_R32G32B32A32_FIXED,
    PIPE_FORMAT_R16_FLOAT,
    PIPE_FORMAT_R16G16_FLOAT,
    PIPE_FORMAT_R16G16B16_FLOAT,
    PIPE_FORMAT_R16G16B16A16_FLOAT,
    PIPE_FORMAT_R8_UINT,
    PIPE_FORMAT_R8G8_UINT,
    PIPE_FORMAT_R8G8B8_UINT,
    PIPE_FORMAT_R8G8B8A8_UINT,
    PIPE_FORMAT_R8_SINT,
    PIPE_FORMAT_R8G8_SINT,
    PIPE_FORMAT_R8G8B8_SINT,
    PIPE_FORMAT_R8G8B8A8_SINT,
    PIPE_FORMAT_R16_UINT,
    PIPE_FORMAT_R16G16_UINT,
    PIPE_FORMAT_R16G16B16_UINT,
    PIPE_FORMAT_R16G16B16A16_UINT,
    PIPE_FORMAT_R16_SINT,
    PIPE_FORMAT_R16G16_SINT,
    PIPE_FORMAT_R16G16B16_SINT,
    PIPE_FORMAT_R16G16B16A16_SINT,
    PIPE_FORMAT_R32_UINT,
    PIPE_FORMAT_R32G32_UINT,
    PIPE_FORMAT_R32G32B32_UINT,
    PIPE_FORMAT_R32G32B32A32_UINT,
    PIPE_FORMAT_R32_SINT,
    PIPE_FORMAT_R32G32_SINT,
    PIPE_FORMAT_R32G32B32_SINT,
    PIPE_FORMAT_R32G32B32A32_SINT,

    PIPE_FORMAT_R10G10B10A2_UNORM,
    PIPE_FORMAT_R10G10B10A2_SNORM,
    PIPE_FORMAT_R10G10B10A2_USCALED,
    PIPE_FORMAT_R10G10B10A2_SSCALED,

    PIPE_FORMAT_B10G10R10A2_UNORM,
    PIPE_FORMAT_B10G10R10A2_SNORM,
    PIPE_FORMAT_B10G10R10A2_USCALED,
    PIPE_FORMAT_B10G10R10A2_SSCALED,

    PIPE_FORMAT_R11G11B10_FLOAT,

    PIPE_FORMAT_B10G10R10A2_UINT,
    PIPE_FORMAT_B10G10R10A2_SINT,

    /* End of vertex formats. */

    /* Texture-only formats are below. */
    PIPE_FORMAT_B8G8R8_UNORM,
    PIPE_FORMAT_B8G8R8X8_UNORM,
    PIPE_FORMAT_X8B8G8R8_UNORM,
    PIPE_FORMAT_X8R8G8B8_UNORM,
    PIPE_FORMAT_B5G5R5A1_UNORM,
    PIPE_FORMAT_R4G4B4A4_UNORM,
    PIPE_FORMAT_B4G4R4A4_UNORM,
    PIPE_FORMAT_R5G6B5_UNORM,
    PIPE_FORMAT_B5G6R5_UNORM,
    PIPE_FORMAT_L8_UNORM,    /**< ubyte luminance */
    PIPE_FORMAT_A8_UNORM,    /**< ubyte alpha */
    PIPE_FORMAT_I8_UNORM,    /**< ubyte intensity */
    PIPE_FORMAT_L8A8_UNORM,  /**< ubyte alpha, luminance */
    PIPE_FORMAT_L16_UNORM,   /**< ushort luminance */
    PIPE_FORMAT_UYVY,
    PIPE_FORMAT_YUYV,
    PIPE_FORMAT_Z16_UNORM,
    PIPE_FORMAT_Z16_UNORM_S8_UINT,
    PIPE_FORMAT_Z32_UNORM,
    PIPE_FORMAT_Z32_FLOAT,
    PIPE_FORMAT_Z24_UNORM_S8_UINT,
    PIPE_FORMAT_S8_UINT_Z24_UNORM,
    PIPE_FORMAT_Z24X8_UNORM,
    PIPE_FORMAT_X8Z24_UNORM,
    PIPE_FORMAT_S8_UINT,     /**< ubyte stencil */

    /* sRGB formats */
    PIPE_FORMAT_L8_SRGB,
    PIPE_FORMAT_R8_SRGB,
    PIPE_FORMAT_L8A8_SRGB,
    PIPE_FORMAT_R8G8_SRGB,
    PIPE_FORMAT_R8G8B8_SRGB,
    PIPE_FORMAT_B8G8R8_SRGB,
    PIPE_FORMAT_A8B8G8R8_SRGB,
    PIPE_FORMAT_X8B8G8R8_SRGB,
    PIPE_FORMAT_B8G8R8A8_SRGB,
    PIPE_FORMAT_B8G8R8X8_SRGB,
    PIPE_FORMAT_A8R8G8B8_SRGB,
    PIPE_FORMAT_X8R8G8B8_SRGB,
    PIPE_FORMAT_R8G8B8A8_SRGB,

    /* compressed formats */
    PIPE_FORMAT_DXT1_RGB,
    PIPE_FORMAT_DXT1_RGBA,
    PIPE_FORMAT_DXT3_RGBA,
    PIPE_FORMAT_DXT5_RGBA,

    /* sRGB, compressed */
    PIPE_FORMAT_DXT1_SRGB,
    PIPE_FORMAT_DXT1_SRGBA,
    PIPE_FORMAT_DXT3_SRGBA,
    PIPE_FORMAT_DXT5_SRGBA,

    /* rgtc compressed */
    PIPE_FORMAT_RGTC1_UNORM,
    PIPE_FORMAT_RGTC1_SNORM,
    PIPE_FORMAT_RGTC2_UNORM,
    PIPE_FORMAT_RGTC2_SNORM,

    PIPE_FORMAT_R8G8_B8G8_UNORM,
    PIPE_FORMAT_G8R8_G8B8_UNORM,

    /* mixed formats */
    PIPE_FORMAT_R8SG8SB8UX8U_NORM,
    PIPE_FORMAT_R5SG5SB6U_NORM,

    /* TODO: re-order these */
    PIPE_FORMAT_A8B8G8R8_UNORM,
    PIPE_FORMAT_B5G5R5X1_UNORM,
    PIPE_FORMAT_R9G9B9E5_FLOAT,
    PIPE_FORMAT_Z32_FLOAT_S8X24_UINT,
    PIPE_FORMAT_R1_UNORM,
    PIPE_FORMAT_R10G10B10X2_USCALED,
    PIPE_FORMAT_R10G10B10X2_SNORM,
    PIPE_FORMAT_L4A4_UNORM,
    PIPE_FORMAT_A2R10G10B10_UNORM,
    PIPE_FORMAT_A2B10G10R10_UNORM,
    PIPE_FORMAT_R10SG10SB10SA2U_NORM,
    PIPE_FORMAT_R8G8Bx_SNORM,
    PIPE_FORMAT_R8G8B8X8_UNORM,
    PIPE_FORMAT_B4G4R4X4_UNORM,

    /* some stencil samplers formats */
    PIPE_FORMAT_X24S8_UINT,
    PIPE_FORMAT_S8X24_UINT,
    PIPE_FORMAT_X32_S8X24_UINT,

    PIPE_FORMAT_R3G3B2_UNORM,
    PIPE_FORMAT_B2G3R3_UNORM,
    PIPE_FORMAT_L16A16_UNORM,
    PIPE_FORMAT_A16_UNORM,
    PIPE_FORMAT_I16_UNORM,

    PIPE_FORMAT_LATC1_UNORM,
    PIPE_FORMAT_LATC1_SNORM,
    PIPE_FORMAT_LATC2_UNORM,
    PIPE_FORMAT_LATC2_SNORM,

    PIPE_FORMAT_A8_SNORM,
    PIPE_FORMAT_L8_SNORM,
    PIPE_FORMAT_L8A8_SNORM,
    PIPE_FORMAT_I8_SNORM,
    PIPE_FORMAT_A16_SNORM,
    PIPE_FORMAT_L16_SNORM,
    PIPE_FORMAT_L16A16_SNORM,
    PIPE_FORMAT_I16_SNORM,

    PIPE_FORMAT_A16_FLOAT,
    PIPE_FORMAT_L16_FLOAT,
    PIPE_FORMAT_L16A16_FLOAT,
    PIPE_FORMAT_I16_FLOAT,
    PIPE_FORMAT_A32_FLOAT,
    PIPE_FORMAT_L32_FLOAT,
    PIPE_FORMAT_L32A32_FLOAT,
    PIPE_FORMAT_I32_FLOAT,

    PIPE_FORMAT_YV12,
    PIPE_FORMAT_YV16,
    PIPE_FORMAT_IYUV,  /**< aka I420 */
    PIPE_FORMAT_NV12,
    PIPE_FORMAT_NV21,

    /* PIPE_FORMAT_Y8_U8_V8_420_UNORM = IYUV */
    /* PIPE_FORMAT_Y8_U8V8_420_UNORM = NV12 */
    PIPE_FORMAT_Y8_U8_V8_422_UNORM,
    PIPE_FORMAT_Y8_U8V8_422_UNORM,
    PIPE_FORMAT_Y8_U8_V8_444_UNORM,

    PIPE_FORMAT_Y16_U16_V16_420_UNORM,
    /* PIPE_FORMAT_Y16_U16V16_420_UNORM */
    PIPE_FORMAT_Y16_U16_V16_422_UNORM,
    PIPE_FORMAT_Y16_U16V16_422_UNORM,
    PIPE_FORMAT_Y16_U16_V16_444_UNORM,

    PIPE_FORMAT_A4R4_UNORM,
    PIPE_FORMAT_R4A4_UNORM,
    PIPE_FORMAT_R8A8_UNORM,
    PIPE_FORMAT_A8R8_UNORM,

    PIPE_FORMAT_R64_SINT,

    PIPE_FORMAT_A8_UINT,
    PIPE_FORMAT_I8_UINT,
    PIPE_FORMAT_L8_UINT,
    PIPE_FORMAT_L8A8_UINT,

    PIPE_FORMAT_A8_SINT,
    PIPE_FORMAT_I8_SINT,
    PIPE_FORMAT_L8_SINT,
    PIPE_FORMAT_L8A8_SINT,

    PIPE_FORMAT_A16_UINT,
    PIPE_FORMAT_I16_UINT,
    PIPE_FORMAT_L16_UINT,
    PIPE_FORMAT_L16A16_UINT,

    PIPE_FORMAT_A16_SINT,
    PIPE_FORMAT_I16_SINT,
    PIPE_FORMAT_L16_SINT,
    PIPE_FORMAT_L16A16_SINT,

    PIPE_FORMAT_A32_UINT,
    PIPE_FORMAT_I32_UINT,
    PIPE_FORMAT_L32_UINT,
    PIPE_FORMAT_L32A32_UINT,

    PIPE_FORMAT_A32_SINT,
    PIPE_FORMAT_I32_SINT,
    PIPE_FORMAT_L32_SINT,
    PIPE_FORMAT_L32A32_SINT,

    PIPE_FORMAT_B8G8R8_UINT,
    PIPE_FORMAT_B8G8R8A8_UINT,

    PIPE_FORMAT_B8G8R8_SINT,
    PIPE_FORMAT_B8G8R8A8_SINT,

    PIPE_FORMAT_A8R8G8B8_UINT,
    PIPE_FORMAT_A8B8G8R8_UINT,
    PIPE_FORMAT_A2R10G10B10_UINT,
    PIPE_FORMAT_A2B10G10R10_UINT,
    PIPE_FORMAT_R5G6B5_UINT,
    PIPE_FORMAT_B5G6R5_UINT,
    PIPE_FORMAT_R5G5B5A1_UINT,
    PIPE_FORMAT_B5G5R5A1_UINT,
    PIPE_FORMAT_A1R5G5B5_UINT,
    PIPE_FORMAT_A1B5G5R5_UINT,
    PIPE_FORMAT_R4G4B4A4_UINT,
    PIPE_FORMAT_B4G4R4A4_UINT,
    PIPE_FORMAT_A4R4G4B4_UINT,
    PIPE_FORMAT_A4B4G4R4_UINT,
    PIPE_FORMAT_R3G3B2_UINT,
    PIPE_FORMAT_B2G3R3_UINT,

    PIPE_FORMAT_ETC1_RGB8,

    PIPE_FORMAT_R8G8_R8B8_UNORM,
    PIPE_FORMAT_G8R8_B8R8_UNORM,

    PIPE_FORMAT_R8G8B8X8_SNORM,
    PIPE_FORMAT_R8G8B8X8_SRGB,
    PIPE_FORMAT_R8G8B8X8_UINT,
    PIPE_FORMAT_R8G8B8X8_SINT,
    PIPE_FORMAT_B10G10R10X2_UNORM,
    PIPE_FORMAT_R16G16B16X16_UNORM,
    PIPE_FORMAT_R16G16B16X16_SNORM,
    PIPE_FORMAT_R16G16B16X16_FLOAT,
    PIPE_FORMAT_R16G16B16X16_UINT,
    PIPE_FORMAT_R16G16B16X16_SINT,
    PIPE_FORMAT_R32G32B32X32_FLOAT,
    PIPE_FORMAT_R32G32B32X32_UINT,
    PIPE_FORMAT_R32G32B32X32_SINT,

    PIPE_FORMAT_R8A8_SNORM,
    PIPE_FORMAT_R16A16_UNORM,
    PIPE_FORMAT_R16A16_SNORM,
    PIPE_FORMAT_R16A16_FLOAT,
    PIPE_FORMAT_R32A32_FLOAT,
    PIPE_FORMAT_R8A8_UINT,
    PIPE_FORMAT_R8A8_SINT,
    PIPE_FORMAT_R16A16_UINT,
    PIPE_FORMAT_R16A16_SINT,
    PIPE_FORMAT_R32A32_UINT,
    PIPE_FORMAT_R32A32_SINT,
    PIPE_FORMAT_R10G10B10A2_UINT,
    PIPE_FORMAT_R10G10B10A2_SINT,

    PIPE_FORMAT_B5G6R5_SRGB,

    PIPE_FORMAT_BPTC_RGBA_UNORM,
    PIPE_FORMAT_BPTC_SRGBA,
    PIPE_FORMAT_BPTC_RGB_FLOAT,
    PIPE_FORMAT_BPTC_RGB_UFLOAT,

    PIPE_FORMAT_G8R8_UNORM,
    PIPE_FORMAT_G8R8_SNORM,
    PIPE_FORMAT_G16R16_UNORM,
    PIPE_FORMAT_G16R16_SNORM,

    PIPE_FORMAT_A8B8G8R8_SNORM,
    PIPE_FORMAT_X8B8G8R8_SNORM,

    PIPE_FORMAT_ETC2_RGB8,
    PIPE_FORMAT_ETC2_SRGB8,
    PIPE_FORMAT_ETC2_RGB8A1,
    PIPE_FORMAT_ETC2_SRGB8A1,
    PIPE_FORMAT_ETC2_RGBA8,
    PIPE_FORMAT_ETC2_SRGBA8,
    PIPE_FORMAT_ETC2_R11_UNORM,
    PIPE_FORMAT_ETC2_R11_SNORM,
    PIPE_FORMAT_ETC2_RG11_UNORM,
    PIPE_FORMAT_ETC2_RG11_SNORM,

    PIPE_FORMAT_ASTC_4x4,
    PIPE_FORMAT_ASTC_5x4,
    PIPE_FORMAT_ASTC_5x5,
    PIPE_FORMAT_ASTC_6x5,
    PIPE_FORMAT_ASTC_6x6,
    PIPE_FORMAT_ASTC_8x5,
    PIPE_FORMAT_ASTC_8x6,
    PIPE_FORMAT_ASTC_8x8,
    PIPE_FORMAT_ASTC_10x5,
    PIPE_FORMAT_ASTC_10x6,
    PIPE_FORMAT_ASTC_10x8,
    PIPE_FORMAT_ASTC_10x10,
    PIPE_FORMAT_ASTC_12x10,
    PIPE_FORMAT_ASTC_12x12,

    PIPE_FORMAT_ASTC_4x4_SRGB,
    PIPE_FORMAT_ASTC_5x4_SRGB,
    PIPE_FORMAT_ASTC_5x5_SRGB,
    PIPE_FORMAT_ASTC_6x5_SRGB,
    PIPE_FORMAT_ASTC_6x6_SRGB,
    PIPE_FORMAT_ASTC_8x5_SRGB,
    PIPE_FORMAT_ASTC_8x6_SRGB,
    PIPE_FORMAT_ASTC_8x8_SRGB,
    PIPE_FORMAT_ASTC_10x5_SRGB,
    PIPE_FORMAT_ASTC_10x6_SRGB,
    PIPE_FORMAT_ASTC_10x8_SRGB,
    PIPE_FORMAT_ASTC_10x10_SRGB,
    PIPE_FORMAT_ASTC_12x10_SRGB,
    PIPE_FORMAT_ASTC_12x12_SRGB,

    PIPE_FORMAT_ASTC_3x3x3,
    PIPE_FORMAT_ASTC_4x3x3,
    PIPE_FORMAT_ASTC_4x4x3,
    PIPE_FORMAT_ASTC_4x4x4,
    PIPE_FORMAT_ASTC_5x4x4,
    PIPE_FORMAT_ASTC_5x5x4,
    PIPE_FORMAT_ASTC_5x5x5,
    PIPE_FORMAT_ASTC_6x5x5,
    PIPE_FORMAT_ASTC_6x6x5,
    PIPE_FORMAT_ASTC_6x6x6,

    PIPE_FORMAT_ASTC_3x3x3_SRGB,
    PIPE_FORMAT_ASTC_4x3x3_SRGB,
    PIPE_FORMAT_ASTC_4x4x3_SRGB,
    PIPE_FORMAT_ASTC_4x4x4_SRGB,
    PIPE_FORMAT_ASTC_5x4x4_SRGB,
    PIPE_FORMAT_ASTC_5x5x4_SRGB,
    PIPE_FORMAT_ASTC_5x5x5_SRGB,
    PIPE_FORMAT_ASTC_6x5x5_SRGB,
    PIPE_FORMAT_ASTC_6x6x5_SRGB,
    PIPE_FORMAT_ASTC_6x6x6_SRGB,

    PIPE_FORMAT_FXT1_RGB,
    PIPE_FORMAT_FXT1_RGBA,

    PIPE_FORMAT_P010,
    PIPE_FORMAT_P012,
    PIPE_FORMAT_P016,

    PIPE_FORMAT_Y210,
    PIPE_FORMAT_Y212,
    PIPE_FORMAT_Y216,

    PIPE_FORMAT_Y410,
    PIPE_FORMAT_Y412,
    PIPE_FORMAT_Y416,

    PIPE_FORMAT_R10G10B10X2_UNORM,
    PIPE_FORMAT_A1R5G5B5_UNORM,
    PIPE_FORMAT_A1B5G5R5_UNORM,
    PIPE_FORMAT_X1B5G5R5_UNORM,
    PIPE_FORMAT_R5G5B5A1_UNORM,
    PIPE_FORMAT_A4R4G4B4_UNORM,
    PIPE_FORMAT_A4B4G4R4_UNORM,

    PIPE_FORMAT_G8R8_SINT,
    PIPE_FORMAT_A8B8G8R8_SINT,
    PIPE_FORMAT_X8B8G8R8_SINT,

    PIPE_FORMAT_ATC_RGB,
    PIPE_FORMAT_ATC_RGBA_EXPLICIT,
    PIPE_FORMAT_ATC_RGBA_INTERPOLATED,

    PIPE_FORMAT_Z24_UNORM_S8_UINT_AS_R8G8B8A8,

    PIPE_FORMAT_AYUV,
    PIPE_FORMAT_XYUV,

    PIPE_FORMAT_R8_G8B8_420_UNORM,
    PIPE_FORMAT_G8_B8R8_420_UNORM,
    PIPE_FORMAT_G8_B8_R8_420_UNORM,
    PIPE_FORMAT_Y8_UNORM,

    PIPE_FORMAT_B8G8R8X8_SNORM,
    PIPE_FORMAT_B8G8R8X8_UINT,
    PIPE_FORMAT_B8G8R8X8_SINT,
    PIPE_FORMAT_A8R8G8B8_SNORM,
    PIPE_FORMAT_A8R8G8B8_SINT,
    PIPE_FORMAT_X8R8G8B8_SNORM,
    PIPE_FORMAT_X8R8G8B8_SINT,
    PIPE_FORMAT_R5G5B5X1_UNORM,
    PIPE_FORMAT_X1R5G5B5_UNORM,
    PIPE_FORMAT_R4G4B4X4_UNORM,
    PIPE_FORMAT_B10G10R10X2_SNORM,
    PIPE_FORMAT_R5G6B5_SRGB,

    PIPE_FORMAT_COUNT
};

struct format_mapping
{
    GLenum glFormats[18];       /**< list of GLenum formats, 0-terminated */
    enum pipe_format pipeFormats[14]; /**< list of pipe formats, 0-terminated */
};

enum pipe_resource_param
{
    PIPE_RESOURCE_PARAM_NPLANES,
    PIPE_RESOURCE_PARAM_STRIDE,
    PIPE_RESOURCE_PARAM_OFFSET,
    PIPE_RESOURCE_PARAM_MODIFIER,
    PIPE_RESOURCE_PARAM_HANDLE_TYPE_SHARED,
    PIPE_RESOURCE_PARAM_HANDLE_TYPE_KMS,
    PIPE_RESOURCE_PARAM_HANDLE_TYPE_FD,
    PIPE_RESOURCE_PARAM_LAYER_STRIDE,
};

struct pipe_screen {
    unsigned num_contexts;
    struct u_transfer_helper *transfer_helper;
    void (*destroy)( struct pipe_screen * );
    const char *(*get_name)( struct pipe_screen * );
    const char *(*get_vendor)( struct pipe_screen * );
    const char *(*get_device_vendor)( struct pipe_screen * );
    int (*get_param)( struct pipe_screen *, enum pipe_cap param );
    float (*get_paramf)( struct pipe_screen *, enum pipe_capf param );
    int (*get_shader_param)( struct pipe_screen *, enum pipe_shader_type shader,
                             enum pipe_shader_cap param );
    int (*get_video_param)( struct pipe_screen *,
                            enum pipe_video_profile profile,
                            enum pipe_video_entrypoint entrypoint,
                            enum pipe_video_cap param );
    int (*get_compute_param)(struct pipe_screen *,
                             enum pipe_shader_ir ir_type,
                             enum pipe_compute_cap param,
                             void *ret);
    void (*get_sample_pixel_grid)(struct pipe_screen *, unsigned sample_count,
                                  unsigned *out_width, unsigned *out_height);
    uint64_t (*get_timestamp)(struct pipe_screen *);
    struct pipe_context * (*context_create)(struct pipe_screen *screen,
                                            void *priv, unsigned flags);
    bool (*is_format_supported)( struct pipe_screen *,
                                 enum pipe_format format,
                                 enum pipe_texture_target target,
                                 unsigned sample_count,
                                 unsigned storage_sample_count,
                                 unsigned bindings );
    bool (*is_video_format_supported)( struct pipe_screen *,
                                       enum pipe_format format,
                                       enum pipe_video_profile profile,
                                       enum pipe_video_entrypoint entrypoint );
    bool (*can_create_resource)(struct pipe_screen *screen,
                                const struct pipe_resource *templat);
    struct pipe_resource * (*resource_create)(struct pipe_screen *,
                                              const struct pipe_resource *templat);

    struct pipe_resource * (*resource_create_front)(struct pipe_screen *,
                                                    const struct pipe_resource *templat,
                                                    const void *map_front_private);
    struct pipe_resource * (*resource_from_handle)(struct pipe_screen *,
                                                   const struct pipe_resource *templat,
                                                   struct winsys_handle *handle,
                                                   unsigned usage);
    struct pipe_resource * (*resource_from_user_memory)(struct pipe_screen *,
                                                        const struct pipe_resource *t,
                                                        void *user_memory);
    bool (*check_resource_capability)(struct pipe_screen *screen,
                                      struct pipe_resource *resource,
                                      unsigned bind);
    bool (*resource_get_handle)(struct pipe_screen *,
                                struct pipe_context *context,
                                struct pipe_resource *tex,
                                struct winsys_handle *handle,
                                unsigned usage);
    bool (*resource_get_param)(struct pipe_screen *screen,
                               struct pipe_context *context,
                               struct pipe_resource *resource,
                               unsigned plane,
                               unsigned layer,
                               unsigned level,
                               enum pipe_resource_param param,
                               unsigned handle_usage,
                               uint64_t *value);
    void (*resource_get_info)(struct pipe_screen *screen,
                              struct pipe_resource *resource,
                              unsigned *stride,
                              unsigned *offset);
    void (*resource_changed)(struct pipe_screen *, struct pipe_resource *pt);

    void (*resource_destroy)(struct pipe_screen *,
                             struct pipe_resource *pt);
    void (*flush_frontbuffer)( struct pipe_screen *screen,
                               struct pipe_context *ctx,
                               struct pipe_resource *resource,
                               unsigned level, unsigned layer,
                               void *winsys_drawable_handle,
                               struct pipe_box *subbox );
    void (*fence_reference)( struct pipe_screen *screen,
                             struct pipe_fence_handle **ptr,
                             struct pipe_fence_handle *fence );
    bool (*fence_finish)(struct pipe_screen *screen,
                         struct pipe_context *ctx,
                         struct pipe_fence_handle *fence,
                         uint64_t timeout);
    int (*fence_get_fd)(struct pipe_screen *screen,
                        struct pipe_fence_handle *fence);
    int (*get_driver_query_info)(struct pipe_screen *screen,
                                 unsigned index,
                                 struct pipe_driver_query_info *info);
    int (*get_driver_query_group_info)(struct pipe_screen *screen,
                                       unsigned index,
                                       struct pipe_driver_query_group_info *info);
    void (*query_memory_info)(struct pipe_screen *screen,
                              struct pipe_memory_info *info);
    const void *(*get_compiler_options)(struct pipe_screen *screen,
                                        enum pipe_shader_ir ir,
                                        enum pipe_shader_type shader);
    struct disk_cache *(*get_disk_shader_cache)(struct pipe_screen *screen);
    struct pipe_resource * (*resource_create_with_modifiers)(
            struct pipe_screen *,
            const struct pipe_resource *templat,
            const uint64_t *modifiers, int count);
    void (*query_dmabuf_modifiers)(struct pipe_screen *screen,
                                   enum pipe_format format, int max,
                                   uint64_t *modifiers,
                                   unsigned int *external_only, int *count);
    struct pipe_memory_object *(*memobj_create_from_handle)(struct pipe_screen *screen,
                                                            struct winsys_handle *handle,
                                                            bool dedicated);
    void (*memobj_destroy)(struct pipe_screen *screen,
                           struct pipe_memory_object *memobj);
    struct pipe_resource * (*resource_from_memobj)(struct pipe_screen *screen,
                                                   const struct pipe_resource *t,
                                                   struct pipe_memory_object *memobj,
                                                   uint64_t offset);
    void (*get_driver_uuid)(struct pipe_screen *screen, char *uuid);
    void (*get_device_uuid)(struct pipe_screen *screen, char *uuid);
    void (*set_max_shader_compiler_threads)(struct pipe_screen *screen,
                                            unsigned max_threads);
    bool (*is_parallel_shader_compilation_finished)(struct pipe_screen *screen,
                                                    void *shader,
                                                    unsigned shader_type);
    void (*set_damage_region)(struct pipe_screen *screen,
                              struct pipe_resource *resource,
                              unsigned int nrects,
                              const struct pipe_box *rects);
    void (*finalize_nir)(struct pipe_screen *screen, void *nir, bool optimize);
    struct pipe_resource * (*resource_create_unbacked)(struct pipe_screen *,
                                                       const struct pipe_resource *templat,
                                                       uint64_t *size_required);
    struct pipe_memory_allocation *(*allocate_memory)(struct pipe_screen *screen,
                                                      uint64_t size);
    void (*free_memory)(struct pipe_screen *screen,
                        struct pipe_memory_allocation *);
    void (*resource_bind_backing)(struct pipe_screen *screen,
                                  struct pipe_resource *pt,
                                  struct pipe_memory_allocation *pmem,
                                  uint64_t offset);
    void *(*map_memory)(struct pipe_screen *screen,
                        struct pipe_memory_allocation *pmem);
    void (*unmap_memory)(struct pipe_screen *screen,
                         struct pipe_memory_allocation *pmem);
    bool (*is_dmabuf_modifier_supported)(struct pipe_screen *screen,
                                         uint64_t modifier, enum pipe_format,
                                         bool *external_only);
    unsigned int (*get_dmabuf_modifier_planes)(struct pipe_screen *screen,
                                               uint64_t modifier,
                                               enum pipe_format format);
};

typedef void (*debug_reference_descriptor)(char*, const struct pipe_reference*);

static inline void
debug_reference(const struct pipe_reference* p,
                debug_reference_descriptor get_desc, int change)
{
}

struct util_format_block
{
    /** Block width in pixels */
    unsigned width;

    /** Block height in pixels */
    unsigned height;

    /** Block depth in pixels */
    unsigned depth;

    /** Block size in bits */
    unsigned bits;
};

enum util_format_layout {
    /**
     * Formats with util_format_block::width == util_format_block::height == 1
     * that can be described as an ordinary data structure.
     */
    UTIL_FORMAT_LAYOUT_PLAIN,

    /**
     * Formats with sub-sampled channels.
     *
     * This is for formats like YVYU where there is less than one sample per
     * pixel.
     */
    UTIL_FORMAT_LAYOUT_SUBSAMPLED,

    /**
     * S3 Texture Compression formats.
     */
    UTIL_FORMAT_LAYOUT_S3TC,

    /**
     * Red-Green Texture Compression formats.
     */
    UTIL_FORMAT_LAYOUT_RGTC,

    /**
     * Ericsson Texture Compression
     */
    UTIL_FORMAT_LAYOUT_ETC,

    /**
     * BC6/7 Texture Compression
     */
    UTIL_FORMAT_LAYOUT_BPTC,

    UTIL_FORMAT_LAYOUT_ASTC,

    UTIL_FORMAT_LAYOUT_ATC,

    /** Formats with 2 or more planes. */
    UTIL_FORMAT_LAYOUT_PLANAR2,
    UTIL_FORMAT_LAYOUT_PLANAR3,

    UTIL_FORMAT_LAYOUT_FXT1 = 10,

    /**
     * Everything else that doesn't fit in any of the above layouts.
     */
    UTIL_FORMAT_LAYOUT_OTHER,
};

enum util_format_colorspace {
    UTIL_FORMAT_COLORSPACE_RGB = 0,
    UTIL_FORMAT_COLORSPACE_SRGB = 1,
    UTIL_FORMAT_COLORSPACE_YUV = 2,
    UTIL_FORMAT_COLORSPACE_ZS = 3
};

struct util_format_channel_description
{
    unsigned type:5;        /**< UTIL_FORMAT_TYPE_x */
    unsigned normalized:1;
    unsigned pure_integer:1;
    unsigned size:9;        /**< bits per channel */
    unsigned shift:16;      /** number of bits from lsb */
};

struct util_format_description
{
    enum pipe_format format;

    const char *name;

    const char *short_name;
    struct util_format_block block;
    enum util_format_layout layout;
    unsigned nr_channels:3;
    unsigned is_array:1;
    unsigned is_bitmask:1;
    unsigned is_mixed:1;
    unsigned is_unorm:1;
    unsigned is_snorm:1;
    struct util_format_channel_description channel[4];

    unsigned char swizzle[4];

    enum util_format_colorspace colorspace;
};

const struct util_format_description *
util_format_description(enum pipe_format format);

#define PIPE_ATOMIC "Unlocked"

#define p_atomic_set(_v, _i) (*(_v) = (_i))
#define p_atomic_read(_v) (*(_v))
#define p_atomic_read_relaxed(_v) (*(_v))
#define p_atomic_dec_zero(_v) (p_atomic_dec_return(_v) == 0)
#define p_atomic_inc(_v) ((void) p_atomic_inc_return(_v))
#define p_atomic_dec(_v) ((void) p_atomic_dec_return(_v))
#define p_atomic_add(_v, _i) ((void) p_atomic_add_return((_v), (_i)))
#define p_atomic_inc_return(_v) (++(*(_v)))
#define p_atomic_dec_return(_v) (--(*(_v)))
#define p_atomic_add_return(_v, _i) (*(_v) = *(_v) + (_i))
#define p_atomic_cmpxchg(_v, _old, _new) (*(_v) == (_old) ? (*(_v) = (_new), (_old)) : *(_v))

struct dri2_format_mapping {
    int dri_fourcc;
    int dri_format; /* image format */
    int dri_components;
    enum pipe_format pipe_format;
    int nplanes;
    struct {
        int buffer_index;
        int width_shift;
        int height_shift;
        uint32_t dri_format; /* plane format */
    } planes[3];
};

struct pipe_reference
{
    int32_t count; /* atomic */
};

typedef enum pipe_format mesa_format;

struct pipe_resource
{
    struct pipe_reference reference;

    unsigned width0; /**< Used by both buffers and textures. */
    uint16_t height0; /* Textures: The maximum height/depth/array_size is 16k. */
    uint16_t depth0;
    uint16_t array_size;

    enum pipe_format format:16;         /**< PIPE_FORMAT_x */
    enum pipe_texture_target target:8; /**< PIPE_TEXTURE_x */
    unsigned last_level:8;    /**< Index of last mipmap level present/defined */

    /** Number of samples determining quality, driving rasterizer, shading,
     *  and framebuffer.
     */
    unsigned nr_samples:8;

    /** Multiple samples within a pixel can have the same value.
     *  nr_storage_samples determines how many slots for different values
     *  there are per pixel. Only color buffers can set this lower than
     *  nr_samples.
     */
    unsigned nr_storage_samples:8;

    unsigned usage:8;         /**< PIPE_USAGE_x (not a bitmask) */
    unsigned bind;            /**< bitmask of PIPE_BIND_x */
    unsigned flags;           /**< bitmask of PIPE_RESOURCE_FLAG_x */

    /**
     * For planar images, ie. YUV EGLImage external, etc, pointer to the
     * next plane.
     */
    struct pipe_resource *next;
    /* The screen pointer should be last for optimal structure packing. */
    struct pipe_screen *screen; /**< screen that this texture belongs to */
};

struct st_egl_image
{
    /* this is owned by the caller */
    struct pipe_resource *texture;

    /* format only differs from texture->format for multi-planar (YUV): */
    enum pipe_format format;

    unsigned level;
    unsigned layer;
    /* GL internal format. */
    unsigned internalformat;
};

static inline void
mtx_destroy(mtx_t *mtx)
{
    assert(mtx != NULL);
    pthread_mutex_destroy(mtx);
}

struct pipe_loader_device {
    enum pipe_loader_device_type type;

    union {
        struct {
            int vendor_id;
            int chip_id;
        } pci;
    } u; /**< Discriminated by \a type */

    char *driver_name;
    const struct pipe_loader_ops *ops;

    driOptionCache option_cache;
    driOptionCache option_info;
};

#define FREE(_ptr ) os_free(_ptr)
#define os_free(_ptr)  free(_ptr)

static inline fi_type FLOAT_AS_UNION(GLfloat f)
{
    fi_type tmp;
    tmp.f = f;
    return tmp;
}

typedef bool (*opencl_dri_event_add_ref_t)(void *cl_event);
typedef bool (*opencl_dri_event_release_t)(void *cl_event);
typedef bool (*opencl_dri_event_wait_t)(void *cl_event, uint64_t timeout);
typedef struct pipe_fence_handle *(*opencl_dri_event_get_fence_t)(void *cl_event);

#define os_calloc(_count, _size )  calloc(_count, _size )

#define CALLOC(_count, _size) os_calloc(_count, _size)

#define CALLOC_STRUCT(T)   (struct T *) CALLOC(1, sizeof(struct T))

#define fourcc_code(a, b, c, d) ((__u32)(a) | ((__u32)(b) << 8) | \
				 ((__u32)(c) << 16) | ((__u32)(d) << 24))

#define DRM_FORMAT_BIG_ENDIAN (1U<<31) /* format is big endian instead of little endian */

/* Reserve 0 for the invalid format specifier */
#define DRM_FORMAT_INVALID	0

/* color index */
#define DRM_FORMAT_C8		fourcc_code('C', '8', ' ', ' ') /* [7:0] C */

/* 8 bpp Red */
#define DRM_FORMAT_R8		fourcc_code('R', '8', ' ', ' ') /* [7:0] R */

/* 16 bpp Red */
#define DRM_FORMAT_R16		fourcc_code('R', '1', '6', ' ') /* [15:0] R little endian */

/* 16 bpp RG */
#define DRM_FORMAT_RG88		fourcc_code('R', 'G', '8', '8') /* [15:0] R:G 8:8 little endian */
#define DRM_FORMAT_GR88		fourcc_code('G', 'R', '8', '8') /* [15:0] G:R 8:8 little endian */

/* 32 bpp RG */
#define DRM_FORMAT_RG1616	fourcc_code('R', 'G', '3', '2') /* [31:0] R:G 16:16 little endian */
#define DRM_FORMAT_GR1616	fourcc_code('G', 'R', '3', '2') /* [31:0] G:R 16:16 little endian */

/* 8 bpp RGB */
#define DRM_FORMAT_RGB332	fourcc_code('R', 'G', 'B', '8') /* [7:0] R:G:B 3:3:2 */
#define DRM_FORMAT_BGR233	fourcc_code('B', 'G', 'R', '8') /* [7:0] B:G:R 2:3:3 */

/* 16 bpp RGB */
#define DRM_FORMAT_XRGB4444	fourcc_code('X', 'R', '1', '2') /* [15:0] x:R:G:B 4:4:4:4 little endian */
#define DRM_FORMAT_XBGR4444	fourcc_code('X', 'B', '1', '2') /* [15:0] x:B:G:R 4:4:4:4 little endian */
#define DRM_FORMAT_RGBX4444	fourcc_code('R', 'X', '1', '2') /* [15:0] R:G:B:x 4:4:4:4 little endian */
#define DRM_FORMAT_BGRX4444	fourcc_code('B', 'X', '1', '2') /* [15:0] B:G:R:x 4:4:4:4 little endian */

#define DRM_FORMAT_ARGB4444	fourcc_code('A', 'R', '1', '2') /* [15:0] A:R:G:B 4:4:4:4 little endian */
#define DRM_FORMAT_ABGR4444	fourcc_code('A', 'B', '1', '2') /* [15:0] A:B:G:R 4:4:4:4 little endian */
#define DRM_FORMAT_RGBA4444	fourcc_code('R', 'A', '1', '2') /* [15:0] R:G:B:A 4:4:4:4 little endian */
#define DRM_FORMAT_BGRA4444	fourcc_code('B', 'A', '1', '2') /* [15:0] B:G:R:A 4:4:4:4 little endian */

#define DRM_FORMAT_XRGB1555	fourcc_code('X', 'R', '1', '5') /* [15:0] x:R:G:B 1:5:5:5 little endian */
#define DRM_FORMAT_XBGR1555	fourcc_code('X', 'B', '1', '5') /* [15:0] x:B:G:R 1:5:5:5 little endian */
#define DRM_FORMAT_RGBX5551	fourcc_code('R', 'X', '1', '5') /* [15:0] R:G:B:x 5:5:5:1 little endian */
#define DRM_FORMAT_BGRX5551	fourcc_code('B', 'X', '1', '5') /* [15:0] B:G:R:x 5:5:5:1 little endian */

#define DRM_FORMAT_ARGB1555	fourcc_code('A', 'R', '1', '5') /* [15:0] A:R:G:B 1:5:5:5 little endian */
#define DRM_FORMAT_ABGR1555	fourcc_code('A', 'B', '1', '5') /* [15:0] A:B:G:R 1:5:5:5 little endian */
#define DRM_FORMAT_RGBA5551	fourcc_code('R', 'A', '1', '5') /* [15:0] R:G:B:A 5:5:5:1 little endian */
#define DRM_FORMAT_BGRA5551	fourcc_code('B', 'A', '1', '5') /* [15:0] B:G:R:A 5:5:5:1 little endian */

#define DRM_FORMAT_RGB565	fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_BGR565	fourcc_code('B', 'G', '1', '6') /* [15:0] B:G:R 5:6:5 little endian */

/* 24 bpp RGB */
#define DRM_FORMAT_RGB888	fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_BGR888	fourcc_code('B', 'G', '2', '4') /* [23:0] B:G:R little endian */

/* 32 bpp RGB */
#define DRM_FORMAT_XRGB8888	fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */
#define DRM_FORMAT_XBGR8888	fourcc_code('X', 'B', '2', '4') /* [31:0] x:B:G:R 8:8:8:8 little endian */
#define DRM_FORMAT_RGBX8888	fourcc_code('R', 'X', '2', '4') /* [31:0] R:G:B:x 8:8:8:8 little endian */
#define DRM_FORMAT_BGRX8888	fourcc_code('B', 'X', '2', '4') /* [31:0] B:G:R:x 8:8:8:8 little endian */

#define DRM_FORMAT_ARGB8888	fourcc_code('A', 'R', '2', '4') /* [31:0] A:R:G:B 8:8:8:8 little endian */
#define DRM_FORMAT_ABGR8888	fourcc_code('A', 'B', '2', '4') /* [31:0] A:B:G:R 8:8:8:8 little endian */
#define DRM_FORMAT_RGBA8888	fourcc_code('R', 'A', '2', '4') /* [31:0] R:G:B:A 8:8:8:8 little endian */
#define DRM_FORMAT_BGRA8888	fourcc_code('B', 'A', '2', '4') /* [31:0] B:G:R:A 8:8:8:8 little endian */

#define DRM_FORMAT_XRGB2101010	fourcc_code('X', 'R', '3', '0') /* [31:0] x:R:G:B 2:10:10:10 little endian */
#define DRM_FORMAT_XBGR2101010	fourcc_code('X', 'B', '3', '0') /* [31:0] x:B:G:R 2:10:10:10 little endian */
#define DRM_FORMAT_RGBX1010102	fourcc_code('R', 'X', '3', '0') /* [31:0] R:G:B:x 10:10:10:2 little endian */
#define DRM_FORMAT_BGRX1010102	fourcc_code('B', 'X', '3', '0') /* [31:0] B:G:R:x 10:10:10:2 little endian */

#define DRM_FORMAT_ARGB2101010	fourcc_code('A', 'R', '3', '0') /* [31:0] A:R:G:B 2:10:10:10 little endian */
#define DRM_FORMAT_ABGR2101010	fourcc_code('A', 'B', '3', '0') /* [31:0] A:B:G:R 2:10:10:10 little endian */
#define DRM_FORMAT_RGBA1010102	fourcc_code('R', 'A', '3', '0') /* [31:0] R:G:B:A 10:10:10:2 little endian */
#define DRM_FORMAT_BGRA1010102	fourcc_code('B', 'A', '3', '0') /* [31:0] B:G:R:A 10:10:10:2 little endian */

/* 64 bpp RGB */
#define DRM_FORMAT_XRGB16161616	fourcc_code('X', 'R', '4', '8') /* [63:0] x:R:G:B 16:16:16:16 little endian */
#define DRM_FORMAT_XBGR16161616	fourcc_code('X', 'B', '4', '8') /* [63:0] x:B:G:R 16:16:16:16 little endian */

#define DRM_FORMAT_ARGB16161616	fourcc_code('A', 'R', '4', '8') /* [63:0] A:R:G:B 16:16:16:16 little endian */
#define DRM_FORMAT_ABGR16161616	fourcc_code('A', 'B', '4', '8') /* [63:0] A:B:G:R 16:16:16:16 little endian */

/*
 * Floating point 64bpp RGB
 * IEEE 754-2008 binary16 half-precision float
 * [15:0] sign:exponent:mantissa 1:5:10
 */
#define DRM_FORMAT_XRGB16161616F fourcc_code('X', 'R', '4', 'H') /* [63:0] x:R:G:B 16:16:16:16 little endian */
#define DRM_FORMAT_XBGR16161616F fourcc_code('X', 'B', '4', 'H') /* [63:0] x:B:G:R 16:16:16:16 little endian */

#define DRM_FORMAT_ARGB16161616F fourcc_code('A', 'R', '4', 'H') /* [63:0] A:R:G:B 16:16:16:16 little endian */
#define DRM_FORMAT_ABGR16161616F fourcc_code('A', 'B', '4', 'H') /* [63:0] A:B:G:R 16:16:16:16 little endian */

/*
 * RGBA format with 10-bit components packed in 64-bit per pixel, with 6 bits
 * of unused padding per component:
 */
#define DRM_FORMAT_AXBXGXRX106106106106 fourcc_code('A', 'B', '1', '0') /* [63:0] A:x:B:x:G:x:R:x 10:6:10:6:10:6:10:6 little endian */

/* packed YCbCr */
#define DRM_FORMAT_YUYV		fourcc_code('Y', 'U', 'Y', 'V') /* [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian */
#define DRM_FORMAT_YVYU		fourcc_code('Y', 'V', 'Y', 'U') /* [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian */
#define DRM_FORMAT_UYVY		fourcc_code('U', 'Y', 'V', 'Y') /* [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian */
#define DRM_FORMAT_VYUY		fourcc_code('V', 'Y', 'U', 'Y') /* [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian */

#define DRM_FORMAT_AYUV		fourcc_code('A', 'Y', 'U', 'V') /* [31:0] A:Y:Cb:Cr 8:8:8:8 little endian */
#define DRM_FORMAT_XYUV8888	fourcc_code('X', 'Y', 'U', 'V') /* [31:0] X:Y:Cb:Cr 8:8:8:8 little endian */
#define DRM_FORMAT_VUY888	fourcc_code('V', 'U', '2', '4') /* [23:0] Cr:Cb:Y 8:8:8 little endian */
#define DRM_FORMAT_VUY101010	fourcc_code('V', 'U', '3', '0') /* Y followed by U then V, 10:10:10. Non-linear modifier only */

/*
 * packed Y2xx indicate for each component, xx valid data occupy msb
 * 16-xx padding occupy lsb
 */
#define DRM_FORMAT_Y210         fourcc_code('Y', '2', '1', '0') /* [63:0] Cr0:0:Y1:0:Cb0:0:Y0:0 10:6:10:6:10:6:10:6 little endian per 2 Y pixels */
#define DRM_FORMAT_Y212         fourcc_code('Y', '2', '1', '2') /* [63:0] Cr0:0:Y1:0:Cb0:0:Y0:0 12:4:12:4:12:4:12:4 little endian per 2 Y pixels */
#define DRM_FORMAT_Y216         fourcc_code('Y', '2', '1', '6') /* [63:0] Cr0:Y1:Cb0:Y0 16:16:16:16 little endian per 2 Y pixels */

/*
 * packed Y4xx indicate for each component, xx valid data occupy msb
 * 16-xx padding occupy lsb except Y410
 */
#define DRM_FORMAT_Y410         fourcc_code('Y', '4', '1', '0') /* [31:0] A:Cr:Y:Cb 2:10:10:10 little endian */
#define DRM_FORMAT_Y412         fourcc_code('Y', '4', '1', '2') /* [63:0] A:0:Cr:0:Y:0:Cb:0 12:4:12:4:12:4:12:4 little endian */
#define DRM_FORMAT_Y416         fourcc_code('Y', '4', '1', '6') /* [63:0] A:Cr:Y:Cb 16:16:16:16 little endian */

#define DRM_FORMAT_YUV410	fourcc_code('Y', 'U', 'V', '9') /* 4x4 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU410	fourcc_code('Y', 'V', 'U', '9') /* 4x4 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV411	fourcc_code('Y', 'U', '1', '1') /* 4x1 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU411	fourcc_code('Y', 'V', '1', '1') /* 4x1 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV420	fourcc_code('Y', 'U', '1', '2') /* 2x2 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU420	fourcc_code('Y', 'V', '1', '2') /* 2x2 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV422	fourcc_code('Y', 'U', '1', '6') /* 2x1 subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU422	fourcc_code('Y', 'V', '1', '6') /* 2x1 subsampled Cr (1) and Cb (2) planes */
#define DRM_FORMAT_YUV444	fourcc_code('Y', 'U', '2', '4') /* non-subsampled Cb (1) and Cr (2) planes */
#define DRM_FORMAT_YVU444	fourcc_code('Y', 'V', '2', '4') /* non-subsampled Cr (1) and Cb (2) planes */

#define DRM_FORMAT_NV12		fourcc_code('N', 'V', '1', '2') /* 2x2 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV21		fourcc_code('N', 'V', '2', '1') /* 2x2 subsampled Cb:Cr plane */
#define DRM_FORMAT_NV16		fourcc_code('N', 'V', '1', '6') /* 2x1 subsampled Cr:Cb plane */
#define DRM_FORMAT_NV61		fourcc_code('N', 'V', '6', '1') /* 2x1 subsampled Cb:Cr plane */
#define DRM_FORMAT_NV24		fourcc_code('N', 'V', '2', '4') /* non-subsampled Cr:Cb plane */
#define DRM_FORMAT_NV42		fourcc_code('N', 'V', '4', '2') /* non-subsampled Cb:Cr plane */

/*
 * 2 plane YCbCr MSB aligned
 * index 0 = Y plane, [15:0] Y:x [10:6] little endian
 * index 1 = Cr:Cb plane, [31:0] Cr:x:Cb:x [10:6:10:6] little endian
 */
#define DRM_FORMAT_P010		fourcc_code('P', '0', '1', '0') /* 2x2 subsampled Cr:Cb plane 10 bits per channel */

/*
 * 2 plane YCbCr MSB aligned
 * index 0 = Y plane, [15:0] Y:x [12:4] little endian
 * index 1 = Cr:Cb plane, [31:0] Cr:x:Cb:x [12:4:12:4] little endian
 */
#define DRM_FORMAT_P012		fourcc_code('P', '0', '1', '2') /* 2x2 subsampled Cr:Cb plane 12 bits per channel */

/*
 * 2 plane YCbCr MSB aligned
 * index 0 = Y plane, [15:0] Y little endian
 * index 1 = Cr:Cb plane, [31:0] Cr:Cb [16:16] little endian
 */
#define DRM_FORMAT_P016		fourcc_code('P', '0', '1', '6') /* 2x2 subsampled Cr:Cb plane 16 bits per channel */

#define PIPE_FORMAT_RGBA8888_UNORM PIPE_FORMAT_R8G8B8A8_UNORM
#define PIPE_FORMAT_RGBX8888_UNORM PIPE_FORMAT_R8G8B8X8_UNORM
#define PIPE_FORMAT_BGRA8888_UNORM PIPE_FORMAT_B8G8R8A8_UNORM
#define PIPE_FORMAT_BGRX8888_UNORM PIPE_FORMAT_B8G8R8X8_UNORM
#define PIPE_FORMAT_ARGB8888_UNORM PIPE_FORMAT_A8R8G8B8_UNORM
#define PIPE_FORMAT_XRGB8888_UNORM PIPE_FORMAT_X8R8G8B8_UNORM
#define PIPE_FORMAT_ABGR8888_UNORM PIPE_FORMAT_A8B8G8R8_UNORM
#define PIPE_FORMAT_XBGR8888_UNORM PIPE_FORMAT_X8B8G8R8_UNORM
#define PIPE_FORMAT_RGBA8888_SNORM PIPE_FORMAT_R8G8B8A8_SNORM
#define PIPE_FORMAT_RGBX8888_SNORM PIPE_FORMAT_R8G8B8X8_SNORM
#define PIPE_FORMAT_ABGR8888_SNORM PIPE_FORMAT_A8B8G8R8_SNORM
#define PIPE_FORMAT_XBGR8888_SNORM PIPE_FORMAT_X8B8G8R8_SNORM
#define PIPE_FORMAT_RGBA8888_SRGB PIPE_FORMAT_R8G8B8A8_SRGB
#define PIPE_FORMAT_RGBX8888_SRGB PIPE_FORMAT_R8G8B8X8_SRGB
#define PIPE_FORMAT_BGRA8888_SRGB PIPE_FORMAT_B8G8R8A8_SRGB
#define PIPE_FORMAT_BGRX8888_SRGB PIPE_FORMAT_B8G8R8X8_SRGB
#define PIPE_FORMAT_ARGB8888_SRGB PIPE_FORMAT_A8R8G8B8_SRGB
#define PIPE_FORMAT_XRGB8888_SRGB PIPE_FORMAT_X8R8G8B8_SRGB
#define PIPE_FORMAT_ABGR8888_SRGB PIPE_FORMAT_A8B8G8R8_SRGB
#define PIPE_FORMAT_XBGR8888_SRGB PIPE_FORMAT_X8B8G8R8_SRGB
#define PIPE_FORMAT_RGBA8888_USCALED PIPE_FORMAT_R8G8B8A8_USCALED
#define PIPE_FORMAT_RGBA8888_SSCALED PIPE_FORMAT_R8G8B8A8_SSCALED
#define PIPE_FORMAT_RGBA8888_UINT PIPE_FORMAT_R8G8B8A8_UINT
#define PIPE_FORMAT_BGRA8888_UINT PIPE_FORMAT_B8G8R8A8_UINT
#define PIPE_FORMAT_ARGB8888_UINT PIPE_FORMAT_A8R8G8B8_UINT
#define PIPE_FORMAT_ABGR8888_UINT PIPE_FORMAT_A8B8G8R8_UINT
#define PIPE_FORMAT_RGBA8888_SINT PIPE_FORMAT_R8G8B8A8_SINT
#define PIPE_FORMAT_RG88_UNORM PIPE_FORMAT_R8G8_UNORM
#define PIPE_FORMAT_GR88_UNORM PIPE_FORMAT_G8R8_UNORM
#define PIPE_FORMAT_RG88_SNORM PIPE_FORMAT_R8G8_SNORM
#define PIPE_FORMAT_GR88_SNORM PIPE_FORMAT_G8R8_SNORM
#define PIPE_FORMAT_RG1616_UNORM PIPE_FORMAT_R16G16_UNORM
#define PIPE_FORMAT_GR1616_UNORM PIPE_FORMAT_G16R16_UNORM
#define PIPE_FORMAT_RG1616_SNORM PIPE_FORMAT_R16G16_SNORM
#define PIPE_FORMAT_GR1616_SNORM PIPE_FORMAT_G16R16_SNORM

#define MESA_FORMAT_NONE                         PIPE_FORMAT_NONE
#define MESA_FORMAT_A8B8G8R8_UNORM               PIPE_FORMAT_ABGR8888_UNORM
#define MESA_FORMAT_X8B8G8R8_UNORM               PIPE_FORMAT_XBGR8888_UNORM
#define MESA_FORMAT_R8G8B8A8_UNORM               PIPE_FORMAT_RGBA8888_UNORM
#define MESA_FORMAT_R8G8B8X8_UNORM               PIPE_FORMAT_RGBX8888_UNORM
#define MESA_FORMAT_B8G8R8A8_UNORM               PIPE_FORMAT_BGRA8888_UNORM
#define MESA_FORMAT_B8G8R8X8_UNORM               PIPE_FORMAT_BGRX8888_UNORM
#define MESA_FORMAT_A8R8G8B8_UNORM               PIPE_FORMAT_ARGB8888_UNORM
#define MESA_FORMAT_X8R8G8B8_UNORM               PIPE_FORMAT_XRGB8888_UNORM
#define MESA_FORMAT_B5G6R5_UNORM                 PIPE_FORMAT_B5G6R5_UNORM
#define MESA_FORMAT_R5G6B5_UNORM                 PIPE_FORMAT_R5G6B5_UNORM
#define MESA_FORMAT_B4G4R4A4_UNORM               PIPE_FORMAT_B4G4R4A4_UNORM
#define MESA_FORMAT_B4G4R4X4_UNORM               PIPE_FORMAT_B4G4R4X4_UNORM
#define MESA_FORMAT_A4R4G4B4_UNORM               PIPE_FORMAT_A4R4G4B4_UNORM
#define MESA_FORMAT_A1B5G5R5_UNORM               PIPE_FORMAT_A1B5G5R5_UNORM
#define MESA_FORMAT_X1B5G5R5_UNORM               PIPE_FORMAT_X1B5G5R5_UNORM
#define MESA_FORMAT_B5G5R5A1_UNORM               PIPE_FORMAT_B5G5R5A1_UNORM
#define MESA_FORMAT_B5G5R5X1_UNORM               PIPE_FORMAT_B5G5R5X1_UNORM
#define MESA_FORMAT_A1R5G5B5_UNORM               PIPE_FORMAT_A1R5G5B5_UNORM
#define MESA_FORMAT_L4A4_UNORM                   PIPE_FORMAT_L4A4_UNORM
#define MESA_FORMAT_B2G3R3_UNORM                 PIPE_FORMAT_B2G3R3_UNORM
#define MESA_FORMAT_B10G10R10A2_UNORM            PIPE_FORMAT_B10G10R10A2_UNORM
#define MESA_FORMAT_B10G10R10X2_UNORM            PIPE_FORMAT_B10G10R10X2_UNORM
#define MESA_FORMAT_R10G10B10A2_UNORM            PIPE_FORMAT_R10G10B10A2_UNORM
#define MESA_FORMAT_R10G10B10X2_UNORM            PIPE_FORMAT_R10G10B10X2_UNORM
#define MESA_FORMAT_S8_UINT_Z24_UNORM            PIPE_FORMAT_S8_UINT_Z24_UNORM
#define MESA_FORMAT_X8_UINT_Z24_UNORM            PIPE_FORMAT_X8Z24_UNORM
#define MESA_FORMAT_Z24_UNORM_S8_UINT            PIPE_FORMAT_Z24_UNORM_S8_UINT
#define MESA_FORMAT_Z24_UNORM_X8_UINT            PIPE_FORMAT_Z24X8_UNORM
#define MESA_FORMAT_R3G3B2_UNORM                 PIPE_FORMAT_R3G3B2_UNORM
#define MESA_FORMAT_A4B4G4R4_UNORM               PIPE_FORMAT_A4B4G4R4_UNORM
#define MESA_FORMAT_R4G4B4A4_UNORM               PIPE_FORMAT_R4G4B4A4_UNORM
#define MESA_FORMAT_R5G5B5A1_UNORM               PIPE_FORMAT_R5G5B5A1_UNORM
#define MESA_FORMAT_A2B10G10R10_UNORM            PIPE_FORMAT_A2B10G10R10_UNORM
#define MESA_FORMAT_A2R10G10B10_UNORM            PIPE_FORMAT_A2R10G10B10_UNORM
#define MESA_FORMAT_YCBCR                        PIPE_FORMAT_UYVY
#define MESA_FORMAT_YCBCR_REV                    PIPE_FORMAT_YUYV
#define MESA_FORMAT_A_UNORM8                     PIPE_FORMAT_A8_UNORM
#define MESA_FORMAT_A_UNORM16                    PIPE_FORMAT_A16_UNORM
#define MESA_FORMAT_L_UNORM8                     PIPE_FORMAT_L8_UNORM
#define MESA_FORMAT_L_UNORM16                    PIPE_FORMAT_L16_UNORM
#define MESA_FORMAT_LA_UNORM8                    PIPE_FORMAT_L8A8_UNORM
#define MESA_FORMAT_LA_UNORM16                   PIPE_FORMAT_L16A16_UNORM
#define MESA_FORMAT_I_UNORM8                     PIPE_FORMAT_I8_UNORM
#define MESA_FORMAT_I_UNORM16                    PIPE_FORMAT_I16_UNORM
#define MESA_FORMAT_R_UNORM8                     PIPE_FORMAT_R8_UNORM
#define MESA_FORMAT_R_UNORM16                    PIPE_FORMAT_R16_UNORM
#define MESA_FORMAT_RG_UNORM8                    PIPE_FORMAT_R8G8_UNORM
#define MESA_FORMAT_RG_UNORM16                   PIPE_FORMAT_R16G16_UNORM
#define MESA_FORMAT_BGR_UNORM8                   PIPE_FORMAT_B8G8R8_UNORM
#define MESA_FORMAT_RGB_UNORM8                   PIPE_FORMAT_R8G8B8_UNORM
#define MESA_FORMAT_RGBA_UNORM16                 PIPE_FORMAT_R16G16B16A16_UNORM
#define MESA_FORMAT_RGBX_UNORM16                 PIPE_FORMAT_R16G16B16X16_UNORM
#define MESA_FORMAT_Z_UNORM16                    PIPE_FORMAT_Z16_UNORM
#define MESA_FORMAT_Z_UNORM32                    PIPE_FORMAT_Z32_UNORM
#define MESA_FORMAT_S_UINT8                      PIPE_FORMAT_S8_UINT
#define MESA_FORMAT_A8B8G8R8_SNORM               PIPE_FORMAT_ABGR8888_SNORM
#define MESA_FORMAT_X8B8G8R8_SNORM               PIPE_FORMAT_XBGR8888_SNORM
#define MESA_FORMAT_R8G8B8A8_SNORM               PIPE_FORMAT_RGBA8888_SNORM
#define MESA_FORMAT_R8G8B8X8_SNORM               PIPE_FORMAT_RGBX8888_SNORM
#define MESA_FORMAT_A_SNORM8                     PIPE_FORMAT_A8_SNORM
#define MESA_FORMAT_A_SNORM16                    PIPE_FORMAT_A16_SNORM
#define MESA_FORMAT_L_SNORM8                     PIPE_FORMAT_L8_SNORM
#define MESA_FORMAT_L_SNORM16                    PIPE_FORMAT_L16_SNORM
#define MESA_FORMAT_I_SNORM8                     PIPE_FORMAT_I8_SNORM
#define MESA_FORMAT_I_SNORM16                    PIPE_FORMAT_I16_SNORM
#define MESA_FORMAT_R_SNORM8                     PIPE_FORMAT_R8_SNORM
#define MESA_FORMAT_R_SNORM16                    PIPE_FORMAT_R16_SNORM
#define MESA_FORMAT_LA_SNORM8                    PIPE_FORMAT_L8A8_SNORM
#define MESA_FORMAT_LA_SNORM16                   PIPE_FORMAT_L16A16_SNORM
#define MESA_FORMAT_RG_SNORM8                    PIPE_FORMAT_R8G8_SNORM
#define MESA_FORMAT_RG_SNORM16                   PIPE_FORMAT_R16G16_SNORM
#define MESA_FORMAT_RGB_SNORM16                  PIPE_FORMAT_R16G16B16_SNORM
#define MESA_FORMAT_RGBA_SNORM16                 PIPE_FORMAT_R16G16B16A16_SNORM
#define MESA_FORMAT_RGBX_SNORM16                 PIPE_FORMAT_R16G16B16X16_SNORM
#define MESA_FORMAT_A8B8G8R8_SRGB                PIPE_FORMAT_ABGR8888_SRGB
#define MESA_FORMAT_B8G8R8A8_SRGB                PIPE_FORMAT_BGRA8888_SRGB
#define MESA_FORMAT_A8R8G8B8_SRGB                PIPE_FORMAT_ARGB8888_SRGB
#define MESA_FORMAT_B8G8R8X8_SRGB                PIPE_FORMAT_BGRX8888_SRGB
#define MESA_FORMAT_X8R8G8B8_SRGB                PIPE_FORMAT_XRGB8888_SRGB
#define MESA_FORMAT_R8G8B8A8_SRGB                PIPE_FORMAT_RGBA8888_SRGB
#define MESA_FORMAT_R8G8B8X8_SRGB                PIPE_FORMAT_RGBX8888_SRGB
#define MESA_FORMAT_X8B8G8R8_SRGB                PIPE_FORMAT_XBGR8888_SRGB
#define MESA_FORMAT_R_SRGB8                      PIPE_FORMAT_R8_SRGB
#define MESA_FORMAT_RG_SRGB8                     PIPE_FORMAT_R8G8_SRGB
#define MESA_FORMAT_L_SRGB8                      PIPE_FORMAT_L8_SRGB
#define MESA_FORMAT_LA_SRGB8                     PIPE_FORMAT_L8A8_SRGB
#define MESA_FORMAT_BGR_SRGB8                    PIPE_FORMAT_R8G8B8_SRGB
#define MESA_FORMAT_R9G9B9E5_FLOAT               PIPE_FORMAT_R9G9B9E5_FLOAT
#define MESA_FORMAT_R11G11B10_FLOAT              PIPE_FORMAT_R11G11B10_FLOAT
#define MESA_FORMAT_Z32_FLOAT_S8X24_UINT         PIPE_FORMAT_Z32_FLOAT_S8X24_UINT
#define MESA_FORMAT_A_FLOAT16                    PIPE_FORMAT_A16_FLOAT
#define MESA_FORMAT_A_FLOAT32                    PIPE_FORMAT_A32_FLOAT
#define MESA_FORMAT_L_FLOAT16                    PIPE_FORMAT_L16_FLOAT
#define MESA_FORMAT_L_FLOAT32                    PIPE_FORMAT_L32_FLOAT
#define MESA_FORMAT_LA_FLOAT16                   PIPE_FORMAT_L16A16_FLOAT
#define MESA_FORMAT_LA_FLOAT32                   PIPE_FORMAT_L32A32_FLOAT
#define MESA_FORMAT_I_FLOAT16                    PIPE_FORMAT_I16_FLOAT
#define MESA_FORMAT_I_FLOAT32                    PIPE_FORMAT_I32_FLOAT
#define MESA_FORMAT_R_FLOAT16                    PIPE_FORMAT_R16_FLOAT
#define MESA_FORMAT_R_FLOAT32                    PIPE_FORMAT_R32_FLOAT
#define MESA_FORMAT_RG_FLOAT16                   PIPE_FORMAT_R16G16_FLOAT
#define MESA_FORMAT_RG_FLOAT32                   PIPE_FORMAT_R32G32_FLOAT
#define MESA_FORMAT_RGB_FLOAT16                  PIPE_FORMAT_R16G16B16_FLOAT
#define MESA_FORMAT_RGB_FLOAT32                  PIPE_FORMAT_R32G32B32_FLOAT
#define MESA_FORMAT_RGBA_FLOAT16                 PIPE_FORMAT_R16G16B16A16_FLOAT
#define MESA_FORMAT_RGBA_FLOAT32                 PIPE_FORMAT_R32G32B32A32_FLOAT
#define MESA_FORMAT_RGBX_FLOAT16                 PIPE_FORMAT_R16G16B16X16_FLOAT
#define MESA_FORMAT_RGBX_FLOAT32                 PIPE_FORMAT_R32G32B32X32_FLOAT
#define MESA_FORMAT_Z_FLOAT32                    PIPE_FORMAT_Z32_FLOAT
#define MESA_FORMAT_A8B8G8R8_UINT                PIPE_FORMAT_ABGR8888_UINT
#define MESA_FORMAT_A8R8G8B8_UINT                PIPE_FORMAT_ARGB8888_UINT
#define MESA_FORMAT_R8G8B8A8_UINT                PIPE_FORMAT_RGBA8888_UINT
#define MESA_FORMAT_B8G8R8A8_UINT                PIPE_FORMAT_BGRA8888_UINT
#define MESA_FORMAT_B10G10R10A2_UINT             PIPE_FORMAT_B10G10R10A2_UINT
#define MESA_FORMAT_R10G10B10A2_UINT             PIPE_FORMAT_R10G10B10A2_UINT
#define MESA_FORMAT_A2B10G10R10_UINT             PIPE_FORMAT_A2B10G10R10_UINT
#define MESA_FORMAT_A2R10G10B10_UINT             PIPE_FORMAT_A2R10G10B10_UINT
#define MESA_FORMAT_B5G6R5_UINT                  PIPE_FORMAT_B5G6R5_UINT
#define MESA_FORMAT_R5G6B5_UINT                  PIPE_FORMAT_R5G6B5_UINT
#define MESA_FORMAT_B2G3R3_UINT                  PIPE_FORMAT_B2G3R3_UINT
#define MESA_FORMAT_R3G3B2_UINT                  PIPE_FORMAT_R3G3B2_UINT
#define MESA_FORMAT_A4B4G4R4_UINT                PIPE_FORMAT_A4B4G4R4_UINT
#define MESA_FORMAT_R4G4B4A4_UINT                PIPE_FORMAT_R4G4B4A4_UINT
#define MESA_FORMAT_B4G4R4A4_UINT                PIPE_FORMAT_B4G4R4A4_UINT
#define MESA_FORMAT_A4R4G4B4_UINT                PIPE_FORMAT_A4R4G4B4_UINT
#define MESA_FORMAT_A1B5G5R5_UINT                PIPE_FORMAT_A1B5G5R5_UINT
#define MESA_FORMAT_B5G5R5A1_UINT                PIPE_FORMAT_B5G5R5A1_UINT
#define MESA_FORMAT_A1R5G5B5_UINT                PIPE_FORMAT_A1R5G5B5_UINT
#define MESA_FORMAT_R5G5B5A1_UINT                PIPE_FORMAT_R5G5B5A1_UINT
#define MESA_FORMAT_A_UINT8                      PIPE_FORMAT_A8_UINT
#define MESA_FORMAT_A_UINT16                     PIPE_FORMAT_A16_UINT
#define MESA_FORMAT_A_UINT32                     PIPE_FORMAT_A32_UINT
#define MESA_FORMAT_A_SINT8                      PIPE_FORMAT_A8_SINT
#define MESA_FORMAT_A_SINT16                     PIPE_FORMAT_A16_SINT
#define MESA_FORMAT_A_SINT32                     PIPE_FORMAT_A32_SINT
#define MESA_FORMAT_I_UINT8                      PIPE_FORMAT_I8_UINT
#define MESA_FORMAT_I_UINT16                     PIPE_FORMAT_I16_UINT
#define MESA_FORMAT_I_UINT32                     PIPE_FORMAT_I32_UINT
#define MESA_FORMAT_I_SINT8                      PIPE_FORMAT_I8_SINT
#define MESA_FORMAT_I_SINT16                     PIPE_FORMAT_I16_SINT
#define MESA_FORMAT_I_SINT32                     PIPE_FORMAT_I32_SINT
#define MESA_FORMAT_L_UINT8                      PIPE_FORMAT_L8_UINT
#define MESA_FORMAT_L_UINT16                     PIPE_FORMAT_L16_UINT
#define MESA_FORMAT_L_UINT32                     PIPE_FORMAT_L32_UINT
#define MESA_FORMAT_L_SINT8                      PIPE_FORMAT_L8_SINT
#define MESA_FORMAT_L_SINT16                     PIPE_FORMAT_L16_SINT
#define MESA_FORMAT_L_SINT32                     PIPE_FORMAT_L32_SINT
#define MESA_FORMAT_LA_UINT8                     PIPE_FORMAT_L8A8_UINT
#define MESA_FORMAT_LA_UINT16                    PIPE_FORMAT_L16A16_UINT
#define MESA_FORMAT_LA_UINT32                    PIPE_FORMAT_L32A32_UINT
#define MESA_FORMAT_LA_SINT8                     PIPE_FORMAT_L8A8_SINT
#define MESA_FORMAT_LA_SINT16                    PIPE_FORMAT_L16A16_SINT
#define MESA_FORMAT_LA_SINT32                    PIPE_FORMAT_L32A32_SINT
#define MESA_FORMAT_R_UINT8                      PIPE_FORMAT_R8_UINT
#define MESA_FORMAT_R_UINT16                     PIPE_FORMAT_R16_UINT
#define MESA_FORMAT_R_UINT32                     PIPE_FORMAT_R32_UINT
#define MESA_FORMAT_R_SINT8                      PIPE_FORMAT_R8_SINT
#define MESA_FORMAT_R_SINT16                     PIPE_FORMAT_R16_SINT
#define MESA_FORMAT_R_SINT32                     PIPE_FORMAT_R32_SINT
#define MESA_FORMAT_RG_UINT8                     PIPE_FORMAT_R8G8_UINT
#define MESA_FORMAT_RG_UINT16                    PIPE_FORMAT_R16G16_UINT
#define MESA_FORMAT_RG_UINT32                    PIPE_FORMAT_R32G32_UINT
#define MESA_FORMAT_RG_SINT8                     PIPE_FORMAT_R8G8_SINT
#define MESA_FORMAT_RG_SINT16                    PIPE_FORMAT_R16G16_SINT
#define MESA_FORMAT_RG_SINT32                    PIPE_FORMAT_R32G32_SINT
#define MESA_FORMAT_RGB_UINT8                    PIPE_FORMAT_R8G8B8_UINT
#define MESA_FORMAT_RGB_UINT16                   PIPE_FORMAT_R16G16B16_UINT
#define MESA_FORMAT_RGB_UINT32                   PIPE_FORMAT_R32G32B32_UINT
#define MESA_FORMAT_RGB_SINT8                    PIPE_FORMAT_R8G8B8_SINT
#define MESA_FORMAT_RGB_SINT16                   PIPE_FORMAT_R16G16B16_SINT
#define MESA_FORMAT_RGB_SINT32                   PIPE_FORMAT_R32G32B32_SINT
#define MESA_FORMAT_RGBA_UINT16                  PIPE_FORMAT_R16G16B16A16_UINT
#define MESA_FORMAT_RGBA_UINT32                  PIPE_FORMAT_R32G32B32A32_UINT
#define MESA_FORMAT_RGBA_SINT8                   PIPE_FORMAT_R8G8B8A8_SINT
#define MESA_FORMAT_RGBA_SINT16                  PIPE_FORMAT_R16G16B16A16_SINT
#define MESA_FORMAT_RGBA_SINT32                  PIPE_FORMAT_R32G32B32A32_SINT
#define MESA_FORMAT_RGBX_UINT8                   PIPE_FORMAT_R8G8B8X8_UINT
#define MESA_FORMAT_RGBX_UINT16                  PIPE_FORMAT_R16G16B16X16_UINT
#define MESA_FORMAT_RGBX_UINT32                  PIPE_FORMAT_R32G32B32X32_UINT
#define MESA_FORMAT_RGBX_SINT8                   PIPE_FORMAT_R8G8B8X8_SINT
#define MESA_FORMAT_RGBX_SINT16                  PIPE_FORMAT_R16G16B16X16_SINT
#define MESA_FORMAT_RGBX_SINT32                  PIPE_FORMAT_R32G32B32X32_SINT
#define MESA_FORMAT_RGB_DXT1                     PIPE_FORMAT_DXT1_RGB
#define MESA_FORMAT_RGBA_DXT1                    PIPE_FORMAT_DXT1_RGBA
#define MESA_FORMAT_RGBA_DXT3                    PIPE_FORMAT_DXT3_RGBA
#define MESA_FORMAT_RGBA_DXT5                    PIPE_FORMAT_DXT5_RGBA
#define MESA_FORMAT_SRGB_DXT1                    PIPE_FORMAT_DXT1_SRGB
#define MESA_FORMAT_SRGBA_DXT1                   PIPE_FORMAT_DXT1_SRGBA
#define MESA_FORMAT_SRGBA_DXT3                   PIPE_FORMAT_DXT3_SRGBA
#define MESA_FORMAT_SRGBA_DXT5                   PIPE_FORMAT_DXT5_SRGBA
#define MESA_FORMAT_RGB_FXT1                     PIPE_FORMAT_FXT1_RGB
#define MESA_FORMAT_RGBA_FXT1                    PIPE_FORMAT_FXT1_RGBA
#define MESA_FORMAT_R_RGTC1_UNORM                PIPE_FORMAT_RGTC1_UNORM
#define MESA_FORMAT_R_RGTC1_SNORM                PIPE_FORMAT_RGTC1_SNORM
#define MESA_FORMAT_RG_RGTC2_UNORM               PIPE_FORMAT_RGTC2_UNORM
#define MESA_FORMAT_RG_RGTC2_SNORM               PIPE_FORMAT_RGTC2_SNORM
#define MESA_FORMAT_L_LATC1_UNORM                PIPE_FORMAT_LATC1_UNORM
#define MESA_FORMAT_L_LATC1_SNORM                PIPE_FORMAT_LATC1_SNORM
#define MESA_FORMAT_LA_LATC2_UNORM               PIPE_FORMAT_LATC2_UNORM
#define MESA_FORMAT_LA_LATC2_SNORM               PIPE_FORMAT_LATC2_SNORM
#define MESA_FORMAT_ETC1_RGB8                    PIPE_FORMAT_ETC1_RGB8
#define MESA_FORMAT_ETC2_RGB8                    PIPE_FORMAT_ETC2_RGB8
#define MESA_FORMAT_ETC2_SRGB8                   PIPE_FORMAT_ETC2_SRGB8
#define MESA_FORMAT_ETC2_RGBA8_EAC               PIPE_FORMAT_ETC2_RGBA8
#define MESA_FORMAT_ETC2_SRGB8_ALPHA8_EAC        PIPE_FORMAT_ETC2_SRGBA8
#define MESA_FORMAT_ETC2_R11_EAC                 PIPE_FORMAT_ETC2_R11_UNORM
#define MESA_FORMAT_ETC2_RG11_EAC                PIPE_FORMAT_ETC2_RG11_UNORM
#define MESA_FORMAT_ETC2_SIGNED_R11_EAC          PIPE_FORMAT_ETC2_R11_SNORM
#define MESA_FORMAT_ETC2_SIGNED_RG11_EAC         PIPE_FORMAT_ETC2_RG11_SNORM
#define MESA_FORMAT_ETC2_RGB8_PUNCHTHROUGH_ALPHA1 PIPE_FORMAT_ETC2_RGB8A1
#define MESA_FORMAT_ETC2_SRGB8_PUNCHTHROUGH_ALPHA1 PIPE_FORMAT_ETC2_SRGB8A1
#define MESA_FORMAT_BPTC_RGBA_UNORM              PIPE_FORMAT_BPTC_RGBA_UNORM
#define MESA_FORMAT_BPTC_SRGB_ALPHA_UNORM        PIPE_FORMAT_BPTC_SRGBA
#define MESA_FORMAT_BPTC_RGB_SIGNED_FLOAT        PIPE_FORMAT_BPTC_RGB_FLOAT
#define MESA_FORMAT_BPTC_RGB_UNSIGNED_FLOAT      PIPE_FORMAT_BPTC_RGB_UFLOAT
#define MESA_FORMAT_RGBA_ASTC_4x4                PIPE_FORMAT_ASTC_4x4
#define MESA_FORMAT_RGBA_ASTC_5x4                PIPE_FORMAT_ASTC_5x4
#define MESA_FORMAT_RGBA_ASTC_5x5                PIPE_FORMAT_ASTC_5x5
#define MESA_FORMAT_RGBA_ASTC_6x5                PIPE_FORMAT_ASTC_6x5
#define MESA_FORMAT_RGBA_ASTC_6x6                PIPE_FORMAT_ASTC_6x6
#define MESA_FORMAT_RGBA_ASTC_8x5                PIPE_FORMAT_ASTC_8x5
#define MESA_FORMAT_RGBA_ASTC_8x6                PIPE_FORMAT_ASTC_8x6
#define MESA_FORMAT_RGBA_ASTC_8x8                PIPE_FORMAT_ASTC_8x8
#define MESA_FORMAT_RGBA_ASTC_10x5               PIPE_FORMAT_ASTC_10x5
#define MESA_FORMAT_RGBA_ASTC_10x6               PIPE_FORMAT_ASTC_10x6
#define MESA_FORMAT_RGBA_ASTC_10x8               PIPE_FORMAT_ASTC_10x8
#define MESA_FORMAT_RGBA_ASTC_10x10              PIPE_FORMAT_ASTC_10x10
#define MESA_FORMAT_RGBA_ASTC_12x10              PIPE_FORMAT_ASTC_12x10
#define MESA_FORMAT_RGBA_ASTC_12x12              PIPE_FORMAT_ASTC_12x12
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4        PIPE_FORMAT_ASTC_4x4_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x4        PIPE_FORMAT_ASTC_5x4_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5        PIPE_FORMAT_ASTC_5x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x5        PIPE_FORMAT_ASTC_6x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6        PIPE_FORMAT_ASTC_6x6_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x5        PIPE_FORMAT_ASTC_8x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x6        PIPE_FORMAT_ASTC_8x6_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x8        PIPE_FORMAT_ASTC_8x8_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x5       PIPE_FORMAT_ASTC_10x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x6       PIPE_FORMAT_ASTC_10x6_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x8       PIPE_FORMAT_ASTC_10x8_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x10      PIPE_FORMAT_ASTC_10x10_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_12x10      PIPE_FORMAT_ASTC_12x10_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_12x12      PIPE_FORMAT_ASTC_12x12_SRGB
#define MESA_FORMAT_RGBA_ASTC_3x3x3              PIPE_FORMAT_ASTC_3x3x3
#define MESA_FORMAT_RGBA_ASTC_4x3x3              PIPE_FORMAT_ASTC_4x3x3
#define MESA_FORMAT_RGBA_ASTC_4x4x3              PIPE_FORMAT_ASTC_4x4x3
#define MESA_FORMAT_RGBA_ASTC_4x4x4              PIPE_FORMAT_ASTC_4x4x4
#define MESA_FORMAT_RGBA_ASTC_5x4x4              PIPE_FORMAT_ASTC_5x4x4
#define MESA_FORMAT_RGBA_ASTC_5x5x4              PIPE_FORMAT_ASTC_5x5x4
#define MESA_FORMAT_RGBA_ASTC_5x5x5              PIPE_FORMAT_ASTC_5x5x5
#define MESA_FORMAT_RGBA_ASTC_6x5x5              PIPE_FORMAT_ASTC_6x5x5
#define MESA_FORMAT_RGBA_ASTC_6x6x5              PIPE_FORMAT_ASTC_6x6x5
#define MESA_FORMAT_RGBA_ASTC_6x6x6              PIPE_FORMAT_ASTC_6x6x6
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_3x3x3      PIPE_FORMAT_ASTC_3x3x3_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x3x3      PIPE_FORMAT_ASTC_4x3x3_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4x3      PIPE_FORMAT_ASTC_4x4x3_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4x4      PIPE_FORMAT_ASTC_4x4x4_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x4x4      PIPE_FORMAT_ASTC_5x4x4_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5x4      PIPE_FORMAT_ASTC_5x5x4_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5x5      PIPE_FORMAT_ASTC_5x5x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x5x5      PIPE_FORMAT_ASTC_6x5x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6x5      PIPE_FORMAT_ASTC_6x6x5_SRGB
#define MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6x6      PIPE_FORMAT_ASTC_6x6x6_SRGB
#define MESA_FORMAT_ATC_RGB                      PIPE_FORMAT_ATC_RGB
#define MESA_FORMAT_ATC_RGBA_EXPLICIT            PIPE_FORMAT_ATC_RGBA_EXPLICIT
#define MESA_FORMAT_ATC_RGBA_INTERPOLATED        PIPE_FORMAT_ATC_RGBA_INTERPOLATED
#define MESA_FORMAT_COUNT                        PIPE_FORMAT_COUNT

static const struct {
    uint32_t    image_format;
    mesa_format mesa_format;
    GLenum internal_format;
} format_mapping[] = {
        {
                .image_format    = __DRI_IMAGE_FORMAT_RGB565,
                .mesa_format     =        MESA_FORMAT_B5G6R5_UNORM,
                .internal_format =        GL_RGB565,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ARGB1555,
                .mesa_format     =        MESA_FORMAT_B5G5R5A1_UNORM,
                .internal_format =        GL_RGB5_A1,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_XRGB8888,
                .mesa_format     =        MESA_FORMAT_B8G8R8X8_UNORM,
                .internal_format =        GL_RGB8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ABGR16161616F,
                .mesa_format     =        MESA_FORMAT_RGBA_FLOAT16,
                .internal_format =        GL_RGBA16F,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_XBGR16161616F,
                .mesa_format     =        MESA_FORMAT_RGBX_FLOAT16,
                .internal_format =        GL_RGBA16F,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ARGB2101010,
                .mesa_format     =        MESA_FORMAT_B10G10R10A2_UNORM,
                .internal_format =        GL_RGB10_A2,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_XRGB2101010,
                .mesa_format     =        MESA_FORMAT_B10G10R10X2_UNORM,
                .internal_format =        GL_RGB10_A2,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ABGR2101010,
                .mesa_format     =        MESA_FORMAT_R10G10B10A2_UNORM,
                .internal_format =        GL_RGB10_A2,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_XBGR2101010,
                .mesa_format     =        MESA_FORMAT_R10G10B10X2_UNORM,
                .internal_format =        GL_RGB10_A2,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ARGB8888,
                .mesa_format     =        MESA_FORMAT_B8G8R8A8_UNORM,
                .internal_format =        GL_RGBA8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_ABGR8888,
                .mesa_format     =        MESA_FORMAT_R8G8B8A8_UNORM,
                .internal_format =        GL_RGBA8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_XBGR8888,
                .mesa_format     =        MESA_FORMAT_R8G8B8X8_UNORM,
                .internal_format =        GL_RGB8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_R8,
                .mesa_format     =        MESA_FORMAT_R_UNORM8,
                .internal_format =        GL_R8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_R8,
                .mesa_format     =        MESA_FORMAT_L_UNORM8,
                .internal_format =        GL_R8,
        },
#if UTIL_ARCH_LITTLE_ENDIAN
        {
      .image_format    = __DRI_IMAGE_FORMAT_GR88,
      .mesa_format     =        MESA_FORMAT_RG_UNORM8,
      .internal_format =        GL_RG8,
   },
   {
      .image_format    = __DRI_IMAGE_FORMAT_GR88,
      .mesa_format     =        MESA_FORMAT_LA_UNORM8,
      .internal_format =        GL_RG8,
   },
#endif
        {
                .image_format    = __DRI_IMAGE_FORMAT_SABGR8,
                .mesa_format     =        MESA_FORMAT_R8G8B8A8_SRGB,
                .internal_format =        GL_SRGB8_ALPHA8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_SARGB8,
                .mesa_format     =        MESA_FORMAT_B8G8R8A8_SRGB,
                .internal_format =        GL_SRGB8_ALPHA8,
        },
        {
                .image_format = __DRI_IMAGE_FORMAT_SXRGB8,
                .mesa_format  =           MESA_FORMAT_B8G8R8X8_SRGB,
                .internal_format =        GL_SRGB8_ALPHA8,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_R16,
                .mesa_format     =        MESA_FORMAT_R_UNORM16,
                .internal_format =        GL_R16,
        },
        {
                .image_format    = __DRI_IMAGE_FORMAT_R16,
                .mesa_format     =        MESA_FORMAT_L_UNORM16,
                .internal_format =        GL_R16,
        },
#if UTIL_ARCH_LITTLE_ENDIAN
        {
      .image_format    = __DRI_IMAGE_FORMAT_GR1616,
      .mesa_format     =        MESA_FORMAT_RG_UNORM16,
      .internal_format =        GL_RG16,
   },
   {
      .image_format    = __DRI_IMAGE_FORMAT_GR1616,
      .mesa_format     =        MESA_FORMAT_LA_UNORM16,
      .internal_format =        GL_RG16,
   },
#endif
};

enum st_manager_param {
    /**
     * The DRI frontend on old libGL's doesn't do the right thing
     * with regards to invalidating the framebuffers.
     *
     * For the GL gallium frontend that means that it needs to invalidate
     * the framebuffer in glViewport itself.
     */
    ST_MANAGER_BROKEN_INVALIDATE
};

enum st_texture_type {
    ST_TEXTURE_1D,
    ST_TEXTURE_2D,
    ST_TEXTURE_3D,
    ST_TEXTURE_RECT
};

struct st_context_iface
{
    /**
     * Available for the gallium frontend and the manager to use.
     */
    void *st_context_private;
    void *st_manager_private;

    /**
     * The frontend manager that manages this object.
     */
    struct st_manager *state_manager;

    /**
     * The CSO context associated with this context in case we need to draw
     * something before swap buffers.
     */
    struct cso_context *cso_context;

    /**
     * The gallium context.
     */
    struct pipe_context *pipe;

    /**
     * Destroy the context.
     */
    void (*destroy)(struct st_context_iface *stctxi);

    /**
     * Flush all drawing from context to the pipe also flushes the pipe.
     */
    void (*flush)(struct st_context_iface *stctxi, unsigned flags,
                  struct pipe_fence_handle **fence,
                  void (*notify_before_flush_cb) (void*),
                  void* notify_before_flush_cb_args);

    /**
     * Replace the texture image of a texture object at the specified level.
     *
     * This function is optional.
     */
    bool (*teximage)(struct st_context_iface *stctxi,
                     enum st_texture_type target,
                     int level, enum pipe_format internal_format,
                     struct pipe_resource *tex, bool mipmap);

    /**
     * Used to implement glXCopyContext.
     */
    void (*copy)(struct st_context_iface *stctxi,
                 struct st_context_iface *stsrci, unsigned mask);

    /**
     * Used to implement wglShareLists.
     */
    bool (*share)(struct st_context_iface *stctxi,
                  struct st_context_iface *stsrci);

    /**
     * Start the thread if the API has a worker thread.
     * Called after the context has been created and fully initialized on both
     * sides (e.g. st/mesa and st/dri).
     */
    void (*start_thread)(struct st_context_iface *stctxi);

    /**
     * If the API is multithreaded, wait for all queued commands to complete.
     * Called from the main thread.
     */
    void (*thread_finish)(struct st_context_iface *stctxi);

    /**
     * Invalidate states to notify the frontend that states have been changed
     * behind its back.
     */
    void (*invalidate_state)(struct st_context_iface *stctxi, unsigned flags);
};

struct dri_context
{
    /* dri */
    __DRIscreen *sPriv;
    __DRIcontext *cPriv;
    __DRIdrawable *dPriv;
    __DRIdrawable *rPriv;

    unsigned int bind_count;

    /**
     * True if the __DRIdrawable's current __DRIimageBufferMask is
     * __DRI_IMAGE_BUFFER_SHARED.
     */
    bool is_shared_buffer_bound;

    /* gallium */
    struct st_api *stapi;
    struct st_context_iface *st;
    struct pp_queue_t *pp;
    struct hud_context *hud;
};

static inline struct dri_context *
dri_context(__DRIcontext * driContextPriv)
{
    if (!driContextPriv)
        return NULL;
    return (struct dri_context *)driContextPriv->driverPrivate;
}

struct list_head
{
    struct list_head *prev;
    struct list_head *next;
};

typedef pthread_cond_t  cnd_t;
typedef pthread_t       thrd_t;
typedef pthread_key_t   tss_t;
typedef pthread_mutex_t mtx_t;
typedef pthread_once_t  once_flag;

struct util_queue {
    char name[14]; /* 13 characters = the thread name without the index */
    mtx_t finish_lock; /* for util_queue_finish and protects threads/num_threads */
    mtx_t lock;
    cnd_t has_queued_cond;
    cnd_t has_space_cond;
    thrd_t *threads;
    unsigned flags;
    int num_queued;
    unsigned max_threads;
    unsigned num_threads; /* decreasing this number will terminate threads */
    int max_jobs;
    int write_idx, read_idx; /* ring buffer pointers */
    size_t total_jobs_size;  /* memory use of all jobs in the queue */
    struct util_queue_job *jobs;

    /* for cleanup at exit(), protected by exit_mutex */
    struct list_head head;
};

struct util_queue_monitoring
{
    /* For querying the thread busyness. */
    struct util_queue *queue;

    /* Counters updated by the user of the queue. */
    unsigned num_offloaded_items;
    unsigned num_direct_items;
    unsigned num_syncs;
};

struct pipe_rt_blend_state
{
    unsigned blend_enable:1;

    unsigned rgb_func:3;          /**< PIPE_BLEND_x */
    unsigned rgb_src_factor:5;    /**< PIPE_BLENDFACTOR_x */
    unsigned rgb_dst_factor:5;    /**< PIPE_BLENDFACTOR_x */

    unsigned alpha_func:3;        /**< PIPE_BLEND_x */
    unsigned alpha_src_factor:5;  /**< PIPE_BLENDFACTOR_x */
    unsigned alpha_dst_factor:5;  /**< PIPE_BLENDFACTOR_x */

    unsigned colormask:4;         /**< bitmask of PIPE_MASK_R/G/B/A */
};

#define PIPE_MAX_COLOR_BUFS        8
#define PIPE_MAX_CLIP_PLANES       8

struct pipe_blend_state
{
    unsigned independent_blend_enable:1;
    unsigned logicop_enable:1;
    unsigned logicop_func:4;      /**< PIPE_LOGICOP_x */
    unsigned dither:1;
    unsigned alpha_to_coverage:1;
    unsigned alpha_to_coverage_dither:1;
    unsigned alpha_to_one:1;
    unsigned max_rt:3;            /* index of max rt, Ie. # of cbufs minus 1 */
    unsigned advanced_blend_func:4;
    struct pipe_rt_blend_state rt[PIPE_MAX_COLOR_BUFS];
};

struct pipe_stencil_state
{
    unsigned enabled:1;  /**< stencil[0]: stencil enabled, stencil[1]: two-side enabled */
    unsigned func:3;     /**< PIPE_FUNC_x */
    unsigned fail_op:3;  /**< PIPE_STENCIL_OP_x */
    unsigned zpass_op:3; /**< PIPE_STENCIL_OP_x */
    unsigned zfail_op:3; /**< PIPE_STENCIL_OP_x */
    unsigned valuemask:8;
    unsigned writemask:8;
};

struct pipe_depth_stencil_alpha_state
{
    struct pipe_stencil_state stencil[2]; /**< [0] = front, [1] = back */

    unsigned alpha_enabled:1;         /**< alpha test enabled? */
    unsigned alpha_func:3;            /**< PIPE_FUNC_x */

    unsigned depth_enabled:1;         /**< depth test enabled? */
    unsigned depth_writemask:1;       /**< allow depth buffer writes? */
    unsigned depth_func:3;            /**< depth test func (PIPE_FUNC_x) */
    unsigned depth_bounds_test:1;     /**< depth bounds test enabled? */

    float alpha_ref_value;            /**< reference value */
    double depth_bounds_min;          /**< minimum depth bound */
    double depth_bounds_max;          /**< maximum depth bound */
};

struct pipe_rasterizer_state
{
    unsigned flatshade:1;
    unsigned light_twoside:1;
    unsigned clamp_vertex_color:1;
    unsigned clamp_fragment_color:1;
    unsigned front_ccw:1;
    unsigned cull_face:2;      /**< PIPE_FACE_x */
    unsigned fill_front:2;     /**< PIPE_POLYGON_MODE_x */
    unsigned fill_back:2;      /**< PIPE_POLYGON_MODE_x */
    unsigned offset_point:1;
    unsigned offset_line:1;
    unsigned offset_tri:1;
    unsigned scissor:1;
    unsigned poly_smooth:1;
    unsigned poly_stipple_enable:1;
    unsigned point_smooth:1;
    unsigned sprite_coord_mode:1;     /**< PIPE_SPRITE_COORD_ */
    unsigned point_quad_rasterization:1; /** points rasterized as quads or points */
    unsigned point_tri_clip:1; /** large points clipped as tris or points */
    unsigned point_size_per_vertex:1; /**< size computed in vertex shader */
    unsigned multisample:1;         /* XXX maybe more ms state in future */
    unsigned no_ms_sample_mask_out:1;
    unsigned force_persample_interp:1;
    unsigned line_smooth:1;
    unsigned line_stipple_enable:1;
    unsigned line_last_pixel:1;
    unsigned conservative_raster_mode:2; /**< PIPE_CONSERVATIVE_RASTER_x */

    /**
     * Use the first vertex of a primitive as the provoking vertex for
     * flat shading.
     */
    unsigned flatshade_first:1;

    unsigned half_pixel_center:1;
    unsigned bottom_edge_rule:1;

    /*
     * Conservative rasterization subpixel precision bias in bits
     */
    unsigned subpixel_precision_x:4;
    unsigned subpixel_precision_y:4;

    /**
     * When true, rasterization is disabled and no pixels are written.
     * This only makes sense with the Stream Out functionality.
     */
    unsigned rasterizer_discard:1;

    /**
     * Exposed by PIPE_CAP_TILE_RASTER_ORDER.  When true,
     * tile_raster_order_increasing_* indicate the order that the rasterizer
     * should render tiles, to meet the requirements of
     * GL_MESA_tile_raster_order.
     */
    unsigned tile_raster_order_fixed:1;
    unsigned tile_raster_order_increasing_x:1;
    unsigned tile_raster_order_increasing_y:1;

    /**
     * When false, depth clipping is disabled and the depth value will be
     * clamped later at the per-pixel level before depth testing.
     * This depends on PIPE_CAP_DEPTH_CLIP_DISABLE.
     *
     * If PIPE_CAP_DEPTH_CLIP_DISABLE_SEPARATE is unsupported, depth_clip_near
     * is equal to depth_clip_far.
     */
    unsigned depth_clip_near:1;
    unsigned depth_clip_far:1;

    /**
     * When true clip space in the z axis goes from [0..1] (D3D).  When false
     * [-1, 1] (GL).
     *
     * NOTE: D3D will always use depth clamping.
     */
    unsigned clip_halfz:1;

    /**
     * When true do not scale offset_units and use same rules for unorm and
     * float depth buffers (D3D9). When false use GL/D3D1X behaviour.
     * This depends on PIPE_CAP_POLYGON_OFFSET_UNITS_UNSCALED.
     */
    unsigned offset_units_unscaled:1;

    /**
     * Enable bits for clipping half-spaces.
     * This applies to both user clip planes and shader clip distances.
     * Note that if the bound shader exports any clip distances, these
     * replace all user clip planes, and clip half-spaces enabled here
     * but not written by the shader count as disabled.
     */
    unsigned clip_plane_enable:PIPE_MAX_CLIP_PLANES;

    unsigned line_stipple_factor:8;  /**< [1..256] actually */
    unsigned line_stipple_pattern:16;

    /**
     * Replace the given TEXCOORD inputs with point coordinates, max. 8 inputs.
     * If TEXCOORD (including PCOORD) are unsupported, replace GENERIC inputs
     * instead. Max. 9 inputs: 8x GENERIC to emulate TEXCOORD, and 1x GENERIC
     * to emulate PCOORD.
     */
    uint16_t sprite_coord_enable; /* 0-7: TEXCOORD/GENERIC, 8: PCOORD */

    float line_width;
    float point_size;           /**< used when no per-vertex size */
    float offset_units;
    float offset_scale;
    float offset_clamp;
    float conservative_raster_dilate;
};

struct pipe_vertex_element
{
    /** Offset of this attribute, in bytes, from the start of the vertex */
    unsigned src_offset:16;

    /** Which vertex_buffer (as given to pipe->set_vertex_buffer()) does
     * this attribute live in?
     */
    unsigned vertex_buffer_index:5;

    enum pipe_format src_format:11;

    /** Instance data rate divisor. 0 means this is per-vertex data,
     *  n means per-instance data used for n consecutive instances (n > 0).
     */
    unsigned instance_divisor;
};

#define PIPE_MAX_ATTRIBS          32

struct cso_velems_state {
    unsigned count;
    struct pipe_vertex_element velems[PIPE_MAX_ATTRIBS];
};

struct util_font {
    struct pipe_resource *texture;
    unsigned glyph_width;
    unsigned glyph_height;
};

union pipe_color_union
{
    float f[4];
    int i[4];
    unsigned int ui[4];
};

struct pipe_sampler_state
{
    unsigned wrap_s:3;            /**< PIPE_TEX_WRAP_x */
    unsigned wrap_t:3;            /**< PIPE_TEX_WRAP_x */
    unsigned wrap_r:3;            /**< PIPE_TEX_WRAP_x */
    unsigned min_img_filter:1;    /**< PIPE_TEX_FILTER_x */
    unsigned min_mip_filter:2;    /**< PIPE_TEX_MIPFILTER_x */
    unsigned mag_img_filter:1;    /**< PIPE_TEX_FILTER_x */
    unsigned compare_mode:1;      /**< PIPE_TEX_COMPARE_x */
    unsigned compare_func:3;      /**< PIPE_FUNC_x */
    unsigned normalized_coords:1; /**< Are coords normalized to [0,1]? */
    unsigned max_anisotropy:5;
    unsigned seamless_cube_map:1;
    unsigned border_color_is_integer:1;
    unsigned reduction_mode:2;    /**< PIPE_TEX_REDUCTION_x */
    float lod_bias;               /**< LOD/lambda bias */
    float min_lod, max_lod;       /**< LOD clamp range, after bias */
    union pipe_color_union border_color;
};

struct pipe_constant_buffer {
    struct pipe_resource *buffer; /**< the actual buffer */
    unsigned buffer_offset; /**< offset to start of data in buffer, in bytes */
    unsigned buffer_size;   /**< how much data can be read in shader */
    const void *user_buffer;  /**< pointer to a user buffer if buffer == NULL */
};

struct pipe_vertex_buffer
{
    uint16_t stride;    /**< stride to same attrib in next vertex, in bytes */
    bool is_user_buffer;
    unsigned buffer_offset;  /**< offset to start of data in buffer, in bytes */

    union {
        struct pipe_resource *resource;  /**< the actual buffer */
        const void *user;  /**< pointer to a user buffer */
    } buffer;
};

struct hud_context {
    int refcount;
    bool simple;

    /* Context where queries are executed. */
    struct pipe_context *record_pipe;

    /* Context where the HUD is drawn: */
    struct pipe_context *pipe;
    struct cso_context *cso;
    struct st_context_iface *st;

    struct hud_batch_query_context *batch_query;
    struct list_head pane_list;

    struct util_queue_monitoring *monitored_queue;

    /* states */
    struct pipe_blend_state no_blend, alpha_blend;
    struct pipe_depth_stencil_alpha_state dsa;
    void *fs_color, *fs_text;
    struct pipe_rasterizer_state rasterizer, rasterizer_aa_lines;
    void *vs_color, *vs_text;
    struct cso_velems_state velems;

    /* font */
    struct util_font font;
    struct pipe_sampler_view *font_sampler_view;
    struct pipe_sampler_state font_sampler_state;

    /* VS constant buffer */
    struct {
        float color[4];
        float two_div_fb_width;
        float two_div_fb_height;
        float translate[2];
        float scale[2];
        float padding[2];
    } constants;
    struct pipe_constant_buffer constbuf;

    unsigned fb_width, fb_height;

    /* vertices for text and background drawing are accumulated here and then
     * drawn all at once */
    struct vertex_queue {
        float *vertices;
        struct pipe_vertex_buffer vbuf;
        unsigned max_num_vertices;
        unsigned num_vertices;
        unsigned buffer_size;
    } text, bg, whitelines;

    bool has_srgb;
};

#define MESA_GIT_SHA1 ""
#define PACKAGE_VERSION ""
#define PACKAGE_BUGREPORT ""

enum st_profile_type
{
    ST_PROFILE_DEFAULT,			/**< OpenGL compatibility profile */
    ST_PROFILE_OPENGL_CORE,		/**< OpenGL 3.2+ core profile */
    ST_PROFILE_OPENGL_ES1,		/**< OpenGL ES 1.x */
    ST_PROFILE_OPENGL_ES2		/**< OpenGL ES 2.0 */
};

enum st_api_feature
{
    ST_API_FEATURE_MS_VISUALS  /**< support for multisample visuals */
};

#define ST_PROFILE_DEFAULT_MASK      (1 << ST_PROFILE_DEFAULT)
#define ST_PROFILE_OPENGL_CORE_MASK  (1 << ST_PROFILE_OPENGL_CORE)
#define ST_PROFILE_OPENGL_ES1_MASK   (1 << ST_PROFILE_OPENGL_ES1)
#define ST_PROFILE_OPENGL_ES2_MASK   (1 << ST_PROFILE_OPENGL_ES2)

#define ST_API_FEATURE_MS_VISUALS_MASK (1 << ST_API_FEATURE_MS_VISUALS)

/**
 * New context flags for GL 3.0 and beyond.
 *
 * Profile information (core vs. compatibilty for OpenGL 3.2+) is communicated
 * through the \c st_profile_type, not through flags.
 */
#define ST_CONTEXT_FLAG_DEBUG               (1 << 0)
#define ST_CONTEXT_FLAG_FORWARD_COMPATIBLE  (1 << 1)
#define ST_CONTEXT_FLAG_ROBUST_ACCESS       (1 << 2)
#define ST_CONTEXT_FLAG_RESET_NOTIFICATION_ENABLED (1 << 3)
#define ST_CONTEXT_FLAG_NO_ERROR            (1 << 4)
#define ST_CONTEXT_FLAG_RELEASE_NONE	    (1 << 5)
#define ST_CONTEXT_FLAG_HIGH_PRIORITY       (1 << 6)
#define ST_CONTEXT_FLAG_LOW_PRIORITY        (1 << 7)

struct pp_filter_t
{
    const char *name;            /* Config name */
    unsigned int inner_tmps;     /* Request how many inner temps */
    unsigned int shaders;        /* Request how many shaders */
    unsigned int verts;          /* How many are vertex shaders */
    pp_init_func init;           /* Init function */
    pp_func main;                /* Run function */
    pp_free_func free;           /* Free function */
};

/*	Order matters. Put new filters in a suitable place. */

static const struct pp_filter_t pp_filters[PP_FILTERS] = {
/*    name			inner	shaders	verts	init			run                       free   */
        { "pp_noblue",		0,	2,	1,	pp_noblue_init,		pp_nocolor,               pp_nocolor_free },
        { "pp_nogreen",		0,	2,	1,	pp_nogreen_init,	pp_nocolor,               pp_nocolor_free },
        { "pp_nored",		0,	2,	1,	pp_nored_init,		pp_nocolor,               pp_nocolor_free },
        { "pp_celshade",		0,	2,	1,	pp_celshade_init,	pp_nocolor,               pp_celshade_free },
        { "pp_jimenezmlaa",		2,	5,	2,	pp_jimenezmlaa_init,	pp_jimenezmlaa,           pp_jimenezmlaa_free },
        { "pp_jimenezmlaa_color",	2,	5,	2,	pp_jimenezmlaa_init_color, pp_jimenezmlaa_color,  pp_jimenezmlaa_free },
};

#define PIPE_BIND_DEPTH_STENCIL        (1 << 0) /* create_surface */
#define PIPE_BIND_RENDER_TARGET        (1 << 1) /* create_surface */
#define PIPE_BIND_BLENDABLE            (1 << 2) /* create_surface */
#define PIPE_BIND_SAMPLER_VIEW         (1 << 3) /* create_sampler_view */
#define PIPE_BIND_VERTEX_BUFFER        (1 << 4) /* set_vertex_buffers */
#define PIPE_BIND_INDEX_BUFFER         (1 << 5) /* draw_elements */
#define PIPE_BIND_CONSTANT_BUFFER      (1 << 6) /* set_constant_buffer */
#define PIPE_BIND_DISPLAY_TARGET       (1 << 7) /* flush_front_buffer */
/* gap */
#define PIPE_BIND_STREAM_OUTPUT        (1 << 10) /* set_stream_output_buffers */
#define PIPE_BIND_CURSOR               (1 << 11) /* mouse cursor */
#define PIPE_BIND_CUSTOM               (1 << 12) /* gallium frontend/winsys usages */
#define PIPE_BIND_GLOBAL               (1 << 13) /* set_global_binding */
#define PIPE_BIND_SHADER_BUFFER        (1 << 14) /* set_shader_buffers */
#define PIPE_BIND_SHADER_IMAGE         (1 << 15) /* set_shader_images */
#define PIPE_BIND_COMPUTE_RESOURCE     (1 << 16) /* set_compute_resources */
#define PIPE_BIND_COMMAND_ARGS_BUFFER  (1 << 17) /* pipe_draw_info.indirect */
#define PIPE_BIND_QUERY_BUFFER         (1 << 18) /* get_query_result_resource */

#define MSAA_VISUAL_MAX_SAMPLES 32

#define MARSHAL_MAX_CMD_SIZE (8 * 1024)

#define MARSHAL_MAX_BATCHES 8

#define VERT_ATTRIB_PRIMITIVE_RESTART_NV -1

struct util_queue_fence {
    mtx_t mutex;
    cnd_t cond;
    int signalled;
};

struct glthread_batch
{
    /** Batch fence for waiting for the execution to finish. */
    struct util_queue_fence fence;

    /** The worker thread will access the context with this. */
    struct gl_context *ctx;

    /**
     * Number of uint64_t elements filled already.
     * This is 0 when it's being filled because glthread::used holds the real
     * value temporarily, and glthread::used is copied to this variable when
     * the batch is submitted.
     */
    unsigned used;

    /** Data contained in the command buffer. */
    uint64_t buffer[MARSHAL_MAX_CMD_SIZE / 8];
};

typedef enum
{
    VERT_ATTRIB_POS,
    VERT_ATTRIB_NORMAL,
    VERT_ATTRIB_COLOR0,
    VERT_ATTRIB_COLOR1,
    VERT_ATTRIB_FOG,
    VERT_ATTRIB_COLOR_INDEX,
    VERT_ATTRIB_EDGEFLAG,
    VERT_ATTRIB_TEX0,
    VERT_ATTRIB_TEX1,
    VERT_ATTRIB_TEX2,
    VERT_ATTRIB_TEX3,
    VERT_ATTRIB_TEX4,
    VERT_ATTRIB_TEX5,
    VERT_ATTRIB_TEX6,
    VERT_ATTRIB_TEX7,
    VERT_ATTRIB_POINT_SIZE,
    VERT_ATTRIB_GENERIC0,
    VERT_ATTRIB_GENERIC1,
    VERT_ATTRIB_GENERIC2,
    VERT_ATTRIB_GENERIC3,
    VERT_ATTRIB_GENERIC4,
    VERT_ATTRIB_GENERIC5,
    VERT_ATTRIB_GENERIC6,
    VERT_ATTRIB_GENERIC7,
    VERT_ATTRIB_GENERIC8,
    VERT_ATTRIB_GENERIC9,
    VERT_ATTRIB_GENERIC10,
    VERT_ATTRIB_GENERIC11,
    VERT_ATTRIB_GENERIC12,
    VERT_ATTRIB_GENERIC13,
    VERT_ATTRIB_GENERIC14,
    VERT_ATTRIB_GENERIC15,
    VERT_ATTRIB_MAX
} gl_vert_attrib;

struct glthread_vao {
    GLuint Name;
    GLuint CurrentElementBufferName;
    GLbitfield UserEnabled; /**< Vertex attribs enabled by the user. */
    GLbitfield Enabled; /**< UserEnabled with POS vs GENERIC0 aliasing resolved. */
    GLbitfield BufferEnabled; /**< "Enabled" converted to buffer bindings. */
    GLbitfield BufferInterleaved; /**< Bitmask of buffers used by multiple attribs. */
    GLbitfield UserPointerMask; /**< Bitmask of buffer bindings. */
    GLbitfield NonZeroDivisorMask; /**< Bitmask of buffer bindings. */

    struct {
        /* Per attrib: */
        GLuint ElementSize;
        GLuint RelativeOffset;
        GLuint BufferIndex; /**< Referring to Attrib[BufferIndex]. */

        /* Per buffer binding: */
        GLsizei Stride;
        GLuint Divisor;
        int EnabledAttribCount; /**< Number of enabled attribs using this buffer. */
        const void *Pointer;
    } Attrib[VERT_ATTRIB_MAX];
};

struct glthread_client_attrib {
    struct glthread_vao VAO;
    GLuint CurrentArrayBufferName;
    int ClientActiveTexture;
    GLuint RestartIndex;
    bool PrimitiveRestart;
    bool PrimitiveRestartFixedIndex;

    /** Whether this element of the client attrib stack contains saved state. */
    bool Valid;
};

#define MAX_TEXTURE_IMAGE_UNITS 32

#define MAX_PROGRAM_ENV_PARAMS         256

#define MAX_PROGRAM_MATRICES           8
#define MAX_PROGRAM_MATRIX_STACK_DEPTH 4
#define MAX_PROGRAM_CALL_DEPTH         8
#define MAX_PROGRAM_TEMPS              256
#define MAX_PROGRAM_ADDRESS_REGS       1
#define MAX_SAMPLERS                   MAX_TEXTURE_IMAGE_UNITS
#define MAX_PROGRAM_INPUTS             32
#define MAX_PROGRAM_OUTPUTS            64

#define MAX_TEXTURE_COORD_UNITS     8
#define MAX_VERTEX_GENERIC_ATTRIBS  16

#define MAX_TEXTURE_UNITS ((MAX_TEXTURE_COORD_UNITS > MAX_TEXTURE_IMAGE_UNITS) ? MAX_TEXTURE_COORD_UNITS : MAX_TEXTURE_IMAGE_UNITS)

typedef enum {
    M_MODELVIEW,
    M_PROJECTION,
    M_PROGRAM0,
    M_PROGRAM_LAST = M_PROGRAM0 + MAX_PROGRAM_MATRICES - 1,
    M_TEXTURE0,
    M_TEXTURE_LAST = M_TEXTURE0 + MAX_TEXTURE_UNITS - 1,
    M_DUMMY, /* used instead of reporting errors */
    M_NUM_MATRIX_STACKS,
} gl_matrix_index;

#define MAX_CLIENT_ATTRIB_STACK_DEPTH 16

struct glthread_attrib_node {
    GLbitfield Mask;
    int ActiveTexture;
    GLenum MatrixMode;
};

#define MAX_ATTRIB_STACK_DEPTH 16

struct glthread_state
{
    /** Multithreaded queue. */
    struct util_queue queue;

    /** This is sent to the driver for framebuffer overlay / HUD. */
    struct util_queue_monitoring stats;

    /** Whether GLThread is enabled. */
    bool enabled;

    /** Display lists. */
    GLenum ListMode; /**< Zero if not inside display list, else list mode. */
    unsigned ListBase;
    unsigned ListCallDepth;

    /** For L3 cache pinning. */
    unsigned pin_thread_counter;

    /** The ring of batches in memory. */
    struct glthread_batch batches[MARSHAL_MAX_BATCHES];

    /** Pointer to the batch currently being filled. */
    struct glthread_batch *next_batch;

    /** Index of the last submitted batch. */
    unsigned last;

    /** Index of the batch being filled and about to be submitted. */
    unsigned next;

    /** Number of uint64_t elements filled already. */
    unsigned used;

    /** Upload buffer. */
    struct gl_buffer_object *upload_buffer;
    uint8_t *upload_ptr;
    unsigned upload_offset;
    int upload_buffer_private_refcount;

    /** Caps. */
    GLboolean SupportsBufferUploads;
    GLboolean SupportsNonVBOUploads;

    /** Primitive restart state. */
    bool PrimitiveRestart;
    bool PrimitiveRestartFixedIndex;
    bool _PrimitiveRestart;
    GLuint RestartIndex;
    GLuint _RestartIndex[4]; /**< Restart index for index_size = 1,2,4. */

    /** Vertex Array objects tracked by glthread independently of Mesa. */
    struct _mesa_HashTable *VAOs;
    struct glthread_vao *CurrentVAO;
    struct glthread_vao *LastLookedUpVAO;
    struct glthread_vao DefaultVAO;
    struct glthread_client_attrib ClientAttribStack[MAX_CLIENT_ATTRIB_STACK_DEPTH];
    int ClientAttribStackTop;
    int ClientActiveTexture;

    /** Currently-bound buffer object IDs. */
    GLuint CurrentArrayBufferName;
    GLuint CurrentDrawIndirectBufferName;
    GLuint CurrentPixelPackBufferName;
    GLuint CurrentPixelUnpackBufferName;

    /**
     * The batch index of the last occurence of glLinkProgram or
     * glDeleteProgram or -1 if there is no such enqueued call.
     */
    int LastProgramChangeBatch;

    /**
     * The batch index of the last occurence of glEndList or
     * glDeleteLists or -1 if there is no such enqueued call.
     */
    int LastDListChangeBatchIndex;

    /** Basic matrix state tracking. */
    int ActiveTexture;
    GLenum MatrixMode;
    gl_matrix_index MatrixIndex;
    struct glthread_attrib_node AttribStack[MAX_ATTRIB_STACK_DEPTH];
    int AttribStackDepth;
    int MatrixStackDepth[M_NUM_MATRIX_STACKS];
};

struct pipe_screen_config {
    struct driOptionCache *options;
    const struct driOptionCache *options_info;
};

struct sw_driver_descriptor
{
    struct pipe_screen *(*create_screen)(struct sw_winsys *ws, bool sw_vk);
    struct {
        const char * const name;
        struct sw_winsys *(*create_winsys)();
    } winsys[];
};

static inline uint64_t
align64(uint64_t value, unsigned alignment)
{
    return (value + alignment - 1) & ~((uint64_t)alignment - 1);
}

#ifndef likely
#  ifdef HAVE___BUILTIN_EXPECT
#    define likely(x)   __builtin_expect(!!(x), 1)
#    define unlikely(x) __builtin_expect(!!(x), 0)
#  else
#    define likely(x)   (x)
#    define unlikely(x) (x)
#  endif
#endif

struct drisw_loader_funcs
{
    void (*get_image) (struct dri_drawable *dri_drawable,
                       int x, int y, unsigned width, unsigned height, unsigned stride,
                       void *data);
    void (*put_image) (struct dri_drawable *dri_drawable,
                       void *data, unsigned width, unsigned height);
    void (*put_image2) (struct dri_drawable *dri_drawable,
                        void *data, int x, int y, unsigned width, unsigned height, unsigned stride);
    void (*put_image_shm) (struct dri_drawable *dri_drawable,
                           int shmid, char *shmaddr, unsigned offset, unsigned offset_x,
                           int x, int y, unsigned width, unsigned height, unsigned stride);
};

struct dri_drawable
{
    struct st_framebuffer_iface base;
    struct st_visual stvis;

    struct dri_screen *screen;

    /* dri */
    __DRIdrawable *dPriv;
    __DRIscreen *sPriv;

    __DRIbuffer old[__DRI_BUFFER_COUNT];
    unsigned old_num;
    unsigned old_w;
    unsigned old_h;

    struct pipe_box *damage_rects;
    unsigned int num_damage_rects;

    struct pipe_resource *textures[ST_ATTACHMENT_COUNT];
    struct pipe_resource *msaa_textures[ST_ATTACHMENT_COUNT];
    unsigned int texture_mask, texture_stamp;

    struct pipe_fence_handle *throttle_fence;
    bool flushing; /* prevents recursion in dri_flush */

    /* hooks filled in by dri2 & drisw */
    void (*allocate_textures)(struct dri_context *ctx,
                              struct dri_drawable *drawable,
                              const enum st_attachment_type *statts,
                              unsigned count);

    void (*update_drawable_info)(struct dri_drawable *drawable);

    bool (*flush_frontbuffer)(struct dri_context *ctx,
                              struct dri_drawable *drawable,
                              enum st_attachment_type statt);

    void (*update_tex_buffer)(struct dri_drawable *drawable,
                              struct dri_context *ctx,
                              struct pipe_resource *res);
    void (*flush_swapbuffers)(struct dri_context *ctx,
                              struct dri_drawable *drawable);
};

#define ralloc_array(ctx, type, count) \
   ((type *) ralloc_array_size(ctx, sizeof(type), count))

#define CANARY 0x5A1106

#define alignof(t) __alignof__(t)

typedef void (*util_dl_proc)(void);

struct sw_winsys
{
    void
    (*destroy)( struct sw_winsys *ws );

    bool
    (*is_displaytarget_format_supported)( struct sw_winsys *ws,
                                          unsigned tex_usage,
                                          enum pipe_format format );

    /**
     * Allocate storage for a render target.
     *
     * Often surfaces which are meant to be blitted to the front screen (i.e.,
     * display targets) must be allocated with special characteristics, memory
     * pools, or obtained directly from the windowing system.
     *
     * This callback is invoked by the pipe_screen when creating a texture marked
     * with the PIPE_BIND_DISPLAY_TARGET flag to get the underlying
     * storage.
     */
    struct sw_displaytarget *
    (*displaytarget_create)( struct sw_winsys *ws,
                             unsigned tex_usage,
                             enum pipe_format format,
                             unsigned width, unsigned height,
                             unsigned alignment,
                             const void *front_private,
                             unsigned *stride );

    /**
     * Used to implement texture_from_handle.
     */
    struct sw_displaytarget *
    (*displaytarget_from_handle)( struct sw_winsys *ws,
                                  const struct pipe_resource *templat,
                                  struct winsys_handle *whandle,
                                  unsigned *stride );

    /**
     * Used to implement texture_get_handle.
     */
    bool
    (*displaytarget_get_handle)( struct sw_winsys *ws,
                                 struct sw_displaytarget *dt,
                                 struct winsys_handle *whandle );

    /**
     * \param flags  bitmask of PIPE_MAP_x flags
     */
    void *
    (*displaytarget_map)( struct sw_winsys *ws,
                          struct sw_displaytarget *dt,
                          unsigned flags );

    void
    (*displaytarget_unmap)( struct sw_winsys *ws,
                            struct sw_displaytarget *dt );

    /**
     * @sa pipe_screen:flush_frontbuffer.
     *
     * This call will likely become asynchronous eventually.
     */
    void
    (*displaytarget_display)( struct sw_winsys *ws,
                              struct sw_displaytarget *dt,
                              void *context_private,
                              struct pipe_box *box );

    void
    (*displaytarget_destroy)( struct sw_winsys *ws,
                              struct sw_displaytarget *dt );
};

#define MODULE_PREFIX "pipe_"
#define UTIL_DL_EXT ".so"
#define UTIL_DL_PREFIX "lib"
#define PIPE_SEARCH_DIR "/data/data/com.vantacom.aarm/files/usr/pipe"

#define pipe_loader_sw_device(dev) ((struct pipe_loader_sw_device *)dev)

#define strchrnul util_strchrnul
static inline char *
util_strchrnul(const char *s, char c)
{
    for (; *s && *s != c; ++s);

    return (char *)s;
}

#define UTIL_MAX_CPUS               1024

typedef uint32_t util_affinity_mask[UTIL_MAX_CPUS / 32];

#define CLAMP( X, MIN, MAX )  ( (X)>(MIN) ? ((X)>(MAX) ? (MAX) : (X)) : (MIN) )

/* Syntax sugar occuring frequently in graphics code */
#define SATURATE( X ) CLAMP(X, 0.0f, 1.0f)

#define MIN2( A, B )   ( (A)<(B) ? (A) : (B) )
#define MAX2( A, B )   ( (A)>(B) ? (A) : (B) )

#define MIN3( A, B, C ) ((A) < (B) ? MIN2(A, C) : MIN2(B, C))
#define MAX3( A, B, C ) ((A) > (B) ? MAX2(A, C) : MAX2(B, C))

#define MIN4( A, B, C, D ) ((A) < (B) ? MIN3(A, C, D) : MIN3(B, C, D))
#define MAX4( A, B, C, D ) ((A) > (B) ? MAX3(A, C, D) : MAX3(B, C, D))

static bool
debug_get_bool_option(const char *name, bool dfault)
{
    const char *str = os_get_option(name);
    bool result;

    if (str == NULL)
        result = dfault;
    else if (!strcmp(str, "n"))
        result = false;
    else if (!strcmp(str, "no"))
        result = false;
    else if (!strcmp(str, "0"))
        result = false;
    else if (!strcmp(str, "f"))
        result = false;
    else if (!strcmp(str, "F"))
        result = false;
    else if (!strcmp(str, "false"))
        result = false;
    else if (!strcmp(str, "FALSE"))
        result = false;
    else
        result = true;

    if (debug_get_option_should_print())
        debug_printf("%s: %s = %s\n", __FUNCTION__, name,
                     result ? "TRUE" : "FALSE");

    return result;
}

#define DEBUG_GET_ONCE_BOOL_OPTION(sufix, name, dfault) \
static bool \
debug_get_option_ ## sufix (void) \
{ \
   static bool first = true; \
   static bool value; \
   if (first) { \
      first = false; \
      value = debug_get_bool_option(name, dfault); \
   } \
   return value; \
}

static inline int
align(int value, int alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

struct util_cpu_caps_t {
    /**
     * Number of CPUs available to the process.
     *
     * This will be less than or equal to \c max_cpus.  This is the number of
     * CPUs that are online and available to the process.
     */
    int16_t nr_cpus;

    /**
     * Maximum number of CPUs that can be online in the system.
     *
     * This will be greater than or equal to \c nr_cpus.  This is the number of
     * CPUs installed in the system.  \c nr_cpus will be less if some CPUs are
     * offline.
     */
    int16_t max_cpus;

    enum cpu_family family;

    /* Feature flags */
    int x86_cpu_type;
    unsigned cacheline;

    unsigned has_intel:1;
    unsigned has_tsc:1;
    unsigned has_mmx:1;
    unsigned has_mmx2:1;
    unsigned has_sse:1;
    unsigned has_sse2:1;
    unsigned has_sse3:1;
    unsigned has_ssse3:1;
    unsigned has_sse4_1:1;
    unsigned has_sse4_2:1;
    unsigned has_popcnt:1;
    unsigned has_avx:1;
    unsigned has_avx2:1;
    unsigned has_f16c:1;
    unsigned has_fma:1;
    unsigned has_3dnow:1;
    unsigned has_3dnow_ext:1;
    unsigned has_xop:1;
    unsigned has_altivec:1;
    unsigned has_vsx:1;
    unsigned has_daz:1;
    unsigned has_neon:1;
    unsigned has_msa:1;

    unsigned has_avx512f:1;
    unsigned has_avx512dq:1;
    unsigned has_avx512ifma:1;
    unsigned has_avx512pf:1;
    unsigned has_avx512er:1;
    unsigned has_avx512cd:1;
    unsigned has_avx512bw:1;
    unsigned has_avx512vl:1;
    unsigned has_avx512vbmi:1;

    unsigned num_L3_caches;
    unsigned num_cpu_mask_bits;

    uint16_t cpu_to_L3[UTIL_MAX_CPUS];
    /* Affinity masks for each L3 cache. */
    util_affinity_mask *L3_affinity_mask;
};

struct util_cpu_caps_t util_cpu_caps;

#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT

static inline void
debug_printf(const char *format, ...) _util_printf_format(1,2);

static inline void
debug_printf(const char *format, ...)
{
    (void) format; /* silence warning */
}

struct gl_context
{
    /** State possibly shared with other contexts in the address space */
    struct gl_shared_state *Shared;

    /** Whether Shared->BufferObjects has already been locked for this context. */
    bool BufferObjectsLocked;
    /** Whether Shared->TexMutex has already been locked for this context. */
    bool TexturesLocked;

    /** \name API function pointer tables */
    /*@{*/
    gl_api API;

    /**
     * The current dispatch table for non-displaylist-saving execution, either
     * BeginEnd or OutsideBeginEnd
     */
    struct _glapi_table *Exec;
    /**
     * The normal dispatch table for non-displaylist-saving, non-begin/end
     */
    struct _glapi_table *OutsideBeginEnd;
    /** The dispatch table used between glNewList() and glEndList() */
    struct _glapi_table *Save;
    /**
     * The dispatch table used between glBegin() and glEnd() (outside of a
     * display list).  Only valid functions between those two are set.
     */
    struct _glapi_table *BeginEnd;
    /**
     * Dispatch table for when a graphics reset has happened.
     */
    struct _glapi_table *ContextLost;
    /**
     * Dispatch table used to marshal API calls from the client program to a
     * separate server thread.
     */
    struct _glapi_table *MarshalExec;
    /**
     * Dispatch table currently in use for fielding API calls from the client
     * program.  If API calls are being marshalled to another thread, this ==
     * MarshalExec.  Otherwise it == CurrentServerDispatch.
     */
    struct _glapi_table *CurrentClientDispatch;

    /**
     * Dispatch table currently in use for performing API calls.  == Save or
     * Exec.
     */
    struct _glapi_table *CurrentServerDispatch;

    /*@}*/

    struct glthread_state GLThread;

    struct gl_config Visual;
    struct gl_framebuffer *DrawBuffer;	/**< buffer for writing */
    struct gl_framebuffer *ReadBuffer;	/**< buffer for reading */
    struct gl_framebuffer *WinSysDrawBuffer;  /**< set with MakeCurrent */
    struct gl_framebuffer *WinSysReadBuffer;  /**< set with MakeCurrent */

    /**
     * Device driver function pointer table
     */
    struct dd_function_table Driver;

    /** Core/Driver constants */
    struct gl_constants Const;

    /**
     * Bitmask of valid primitive types supported by this context type,
     * GL version, and extensions, not taking current states into account.
     * Current states can further reduce the final bitmask at draw time.
     */
    GLbitfield SupportedPrimMask;

    /**
     * Bitmask of valid primitive types depending on current states (such as
     * shaders). This is 0 if the current states should result in
     * GL_INVALID_OPERATION in draw calls.
     */
    GLbitfield ValidPrimMask;

    GLenum16 DrawGLError; /**< GL error to return from draw calls */

    /**
     * Same as ValidPrimMask, but should be applied to glDrawElements*.
     */
    GLbitfield ValidPrimMaskIndexed;

    /**
     * Whether DrawPixels/CopyPixels/Bitmap are valid to render.
     */
    bool DrawPixValid;

    /** \name The various 4x4 matrix stacks */
    /*@{*/
    struct gl_matrix_stack ModelviewMatrixStack;
    struct gl_matrix_stack ProjectionMatrixStack;
    struct gl_matrix_stack TextureMatrixStack[MAX_TEXTURE_UNITS];
    struct gl_matrix_stack ProgramMatrixStack[MAX_PROGRAM_MATRICES];
    struct gl_matrix_stack *CurrentStack; /**< Points to one of the above stacks */
    /*@}*/

    /** Combined modelview and projection matrix */
    GLmatrix _ModelProjectMatrix;

    /** \name Display lists */
    struct gl_dlist_state ListState;

    GLboolean ExecuteFlag;	/**< Execute GL commands? */
    GLboolean CompileFlag;	/**< Compile GL commands into display list? */

    /** Extension information */
    struct gl_extensions Extensions;

    /** GL version integer, for example 31 for GL 3.1, or 20 for GLES 2.0. */
    GLuint Version;
    char *VersionString;

    /** \name State attribute stack (for glPush/PopAttrib) */
    /*@{*/
    GLuint AttribStackDepth;
    struct gl_attrib_node *AttribStack[MAX_ATTRIB_STACK_DEPTH];
    /*@}*/

    /** \name Renderer attribute groups
     *
     * We define a struct for each attribute group to make pushing and popping
     * attributes easy.  Also it's a good organization.
     */
    /*@{*/
    struct gl_accum_attrib	Accum;		/**< Accum buffer attributes */
    struct gl_colorbuffer_attrib	Color;		/**< Color buffer attributes */
    struct gl_current_attrib	Current;	/**< Current attributes */
    struct gl_depthbuffer_attrib	Depth;		/**< Depth buffer attributes */
    struct gl_eval_attrib	Eval;		/**< Eval attributes */
    struct gl_fog_attrib		Fog;		/**< Fog attributes */
    struct gl_hint_attrib	Hint;		/**< Hint attributes */
    struct gl_light_attrib	Light;		/**< Light attributes */
    struct gl_line_attrib	Line;		/**< Line attributes */
    struct gl_list_attrib	List;		/**< List attributes */
    struct gl_multisample_attrib Multisample;
    struct gl_pixel_attrib	Pixel;		/**< Pixel attributes */
    struct gl_point_attrib	Point;		/**< Point attributes */
    struct gl_polygon_attrib	Polygon;	/**< Polygon attributes */
    GLuint PolygonStipple[32];			/**< Polygon stipple */
    struct gl_scissor_attrib	Scissor;	/**< Scissor attributes */
    struct gl_stencil_attrib	Stencil;	/**< Stencil buffer attributes */
    struct gl_texture_attrib	Texture;	/**< Texture attributes */
    struct gl_transform_attrib	Transform;	/**< Transformation attributes */
    struct gl_viewport_attrib	ViewportArray[MAX_VIEWPORTS];	/**< Viewport attributes */
    GLuint SubpixelPrecisionBias[2];	/**< Viewport attributes */
    /*@}*/

    /** \name Client attribute stack */
    /*@{*/
    GLuint ClientAttribStackDepth;
    struct gl_client_attrib_node ClientAttribStack[MAX_CLIENT_ATTRIB_STACK_DEPTH];
    /*@}*/

    /** \name Client attribute groups */
    /*@{*/
    struct gl_array_attrib	Array;	/**< Vertex arrays */
    struct gl_pixelstore_attrib	Pack;	/**< Pixel packing */
    struct gl_pixelstore_attrib	Unpack;	/**< Pixel unpacking */
    struct gl_pixelstore_attrib	DefaultPacking;	/**< Default params */
    /*@}*/

    /** \name Other assorted state (not pushed/popped on attribute stack) */
    /*@{*/
    struct gl_pixelmaps          PixelMaps;

    struct gl_evaluators EvalMap;   /**< All evaluators */
    struct gl_feedback   Feedback;  /**< Feedback */
    struct gl_selection  Select;    /**< Selection */

    struct gl_program_state Program;  /**< general program state */
    struct gl_vertex_program_state VertexProgram;
    struct gl_fragment_program_state FragmentProgram;
    struct gl_geometry_program_state GeometryProgram;
    struct gl_compute_program_state ComputeProgram;
    struct gl_tess_ctrl_program_state TessCtrlProgram;
    struct gl_tess_eval_program_state TessEvalProgram;
    struct gl_ati_fragment_shader_state ATIFragmentShader;

    struct gl_pipeline_shader_state Pipeline; /**< GLSL pipeline shader object state */
    struct gl_pipeline_object Shader; /**< GLSL shader object state */

    /**
     * Current active shader pipeline state
     *
     * Almost all internal users want ::_Shader instead of ::Shader.  The
     * exceptions are bits of legacy GLSL API that do not know about separate
     * shader objects.
     *
     * If a program is active via \c glUseProgram, this will point to
     * \c ::Shader.
     *
     * If a program pipeline is active via \c glBindProgramPipeline, this will
     * point to \c ::Pipeline.Current.
     *
     * If neither a program nor a program pipeline is active, this will point to
     * \c ::Pipeline.Default.  This ensures that \c ::_Shader will never be
     * \c NULL.
     */
    struct gl_pipeline_object *_Shader;

    /**
     * NIR containing the functions that implement software fp64 support.
     */
    struct nir_shader *SoftFP64;

    struct gl_query_state Query;  /**< occlusion, timer queries */

    struct gl_transform_feedback_state TransformFeedback;

    struct gl_perf_monitor_state PerfMonitor;
    struct gl_perf_query_state PerfQuery;

    struct gl_buffer_object *DrawIndirectBuffer; /** < GL_ARB_draw_indirect */
    struct gl_buffer_object *ParameterBuffer; /** < GL_ARB_indirect_parameters */
    struct gl_buffer_object *DispatchIndirectBuffer; /** < GL_ARB_compute_shader */

    struct gl_buffer_object *CopyReadBuffer; /**< GL_ARB_copy_buffer */
    struct gl_buffer_object *CopyWriteBuffer; /**< GL_ARB_copy_buffer */

    struct gl_buffer_object *QueryBuffer; /**< GL_ARB_query_buffer_object */

    /**
     * Current GL_ARB_uniform_buffer_object binding referenced by
     * GL_UNIFORM_BUFFER target for glBufferData, glMapBuffer, etc.
     */
    struct gl_buffer_object *UniformBuffer;

    /**
     * Current GL_ARB_shader_storage_buffer_object binding referenced by
     * GL_SHADER_STORAGE_BUFFER target for glBufferData, glMapBuffer, etc.
     */
    struct gl_buffer_object *ShaderStorageBuffer;

    /**
     * Array of uniform buffers for GL_ARB_uniform_buffer_object and GL 3.1.
     * This is set up using glBindBufferRange() or glBindBufferBase().  They are
     * associated with uniform blocks by glUniformBlockBinding()'s state in the
     * shader program.
     */
    struct gl_buffer_binding
            UniformBufferBindings[MAX_COMBINED_UNIFORM_BUFFERS];

    /**
     * Array of shader storage buffers for ARB_shader_storage_buffer_object
     * and GL 4.3. This is set up using glBindBufferRange() or
     * glBindBufferBase().  They are associated with shader storage blocks by
     * glShaderStorageBlockBinding()'s state in the shader program.
     */
    struct gl_buffer_binding
            ShaderStorageBufferBindings[MAX_COMBINED_SHADER_STORAGE_BUFFERS];

    /**
     * Object currently associated with the GL_ATOMIC_COUNTER_BUFFER
     * target.
     */
    struct gl_buffer_object *AtomicBuffer;

    /**
     * Object currently associated w/ the GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD
     * target.
     */
    struct gl_buffer_object *ExternalVirtualMemoryBuffer;

    /**
     * Array of atomic counter buffer binding points.
     */
    struct gl_buffer_binding
            AtomicBufferBindings[MAX_COMBINED_ATOMIC_BUFFERS];

    /**
     * Array of image units for ARB_shader_image_load_store.
     */
    struct gl_image_unit ImageUnits[MAX_IMAGE_UNITS];

    struct gl_subroutine_index_binding SubroutineIndex[MESA_SHADER_STAGES];
    /*@}*/

    struct gl_meta_state *Meta;  /**< for "meta" operations */

    /* GL_EXT_framebuffer_object */
    struct gl_renderbuffer *CurrentRenderbuffer;

    GLenum16 ErrorValue;      /**< Last error code */

    /**
     * Recognize and silence repeated error debug messages in buggy apps.
     */
    const char *ErrorDebugFmtString;
    GLuint ErrorDebugCount;

    /* GL_ARB_debug_output/GL_KHR_debug */
    simple_mtx_t DebugMutex;
    struct gl_debug_state *Debug;

    GLenum16 RenderMode;      /**< either GL_RENDER, GL_SELECT, GL_FEEDBACK */
    GLbitfield NewState;      /**< bitwise-or of _NEW_* flags */
    GLbitfield PopAttribState; /**< Updated state since glPushAttrib */
    uint64_t NewDriverState;  /**< bitwise-or of flags from DriverFlags */

    struct gl_driver_flags DriverFlags;

    GLboolean ViewportInitialized;  /**< has viewport size been initialized? */
    GLboolean _AllowDrawOutOfOrder;

    /** \name Derived state */
    GLbitfield _ImageTransferState;/**< bitwise-or of IMAGE_*_BIT flags */
    GLfloat _EyeZDir[3];
    GLfloat _ModelViewInvScale; /* may be for model- or eyespace lighting */
    GLfloat _ModelViewInvScaleEyespace; /* always factor defined in spec */
    GLboolean _NeedEyeCoords;

    GLuint TextureStateTimestamp; /**< detect changes to shared state */

    /** \name For debugging/development only */
    /*@{*/
    GLboolean FirstTimeCurrent;
    /*@}*/

    /**
     * False if this context was created without a config. This is needed
     * because the initial state of glDrawBuffers depends on this
     */
    GLboolean HasConfig;

    GLboolean TextureFormatSupported[MESA_FORMAT_COUNT];

    GLboolean RasterDiscard;  /**< GL_RASTERIZER_DISCARD */
    GLboolean IntelConservativeRasterization; /**< GL_CONSERVATIVE_RASTERIZATION_INTEL */
    GLboolean ConservativeRasterization; /**< GL_CONSERVATIVE_RASTERIZATION_NV */
    GLfloat ConservativeRasterDilate;
    GLenum16 ConservativeRasterMode;

    GLboolean IntelBlackholeRender; /**< GL_INTEL_blackhole_render */

    /** Does glVertexAttrib(0) alias glVertex()? */
    bool _AttribZeroAliasesVertex;

    /**
     * When set, TileRasterOrderIncreasingX/Y control the order that a tiled
     * renderer's tiles should be excecuted, to meet the requirements of
     * GL_MESA_tile_raster_order.
     */
    GLboolean TileRasterOrderFixed;
    GLboolean TileRasterOrderIncreasingX;
    GLboolean TileRasterOrderIncreasingY;

    /**
     * \name Hooks for module contexts.
     *
     * These will eventually live in the driver or elsewhere.
     */
    /*@{*/
    struct vbo_context vbo_context;
    struct st_context *st;
    struct pipe_screen *screen;
    struct pipe_context *pipe;
    struct st_config_options *st_opts;
    struct cso_context *cso_context;
    bool has_invalidate_buffer;
    /* On old libGL's for linux we need to invalidate the drawables
     * on glViewpport calls, this is set via a option.
     */
    bool invalidate_on_gl_viewport;

    /*@}*/

    /**
     * \name NV_vdpau_interop
     */
    /*@{*/
    const void *vdpDevice;
    const void *vdpGetProcAddress;
    struct set *vdpSurfaces;
    /*@}*/

    /**
     * Has this context observed a GPU reset in any context in the share group?
     *
     * Once this field becomes true, it is never reset to false.
     */
    GLboolean ShareGroupReset;

    /**
     * \name OES_primitive_bounding_box
     *
     * Stores the arguments to glPrimitiveBoundingBox
     */
    GLfloat PrimitiveBoundingBox[8];

    struct disk_cache *Cache;

    /**
     * \name GL_ARB_bindless_texture
     */
    /*@{*/
    struct hash_table_u64 *ResidentTextureHandles;
    struct hash_table_u64 *ResidentImageHandles;
    /*@}*/

    bool shader_builtin_ref;
};

#define DRI_CONF_OPT_B(_name, def, _desc) {                     \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_BOOL,                                      \
      },                                                        \
      .value = { ._bool = def },                                \
   },

#define DRI_CONF_OPT_I(_name, def, min, max, _desc) {           \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_INT,                                       \
         DRI_CONF_RANGE_I(min, max),                            \
      },                                                        \
      .value = { ._int = def },                                 \
   },

#define DRI_CONF_OPT_F(_name, def, min, max, _desc) {           \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_FLOAT,                                     \
         DRI_CONF_RANGE_F(min, max),                            \
      },                                                        \
      .value = { ._float = def },                               \
   },

#define DRI_CONF_OPT_E(_name, def, min, max, _desc, values) {   \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_ENUM,                                      \
         DRI_CONF_RANGE_I(min, max),                            \
      },                                                        \
      .value = { ._int = def },                                 \
      .enums = { values },                                      \
   },

#define DRI_CONF_OPT_S(_name, def, _desc) {                     \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_STRING,                                    \
      },                                                        \
      .value = { ._string = #def },                             \
   },

#define DRI_CONF_OPT_S_NODEF(_name, _desc) {                    \
      .desc = _desc,                                            \
      .info = {                                                 \
         .name = #_name,                                        \
         .type = DRI_STRING,                                    \
      },                                                        \
      .value = { ._string = "" },                               \
   },

#define DRI_CONF_ADAPTIVE_SYNC(def) \
   DRI_CONF_OPT_B(adaptive_sync,def, \
                  "Adapt the monitor sync to the application performance (when possible)")

#define DRI_CONF_VK_WSI_FORCE_BGRA8_UNORM_FIRST(def) \
   DRI_CONF_OPT_B(vk_wsi_force_bgra8_unorm_first, def, \
                  "Force vkGetPhysicalDeviceSurfaceFormatsKHR to return VK_FORMAT_B8G8R8A8_UNORM as the first format")

#define DRI_CONF_VK_X11_OVERRIDE_MIN_IMAGE_COUNT(def) \
   DRI_CONF_OPT_I(vk_x11_override_min_image_count, def, 0, 999, \
                  "Override the VkSurfaceCapabilitiesKHR::minImageCount (0 = no override)")

#define DRI_CONF_VK_X11_STRICT_IMAGE_COUNT(def) \
   DRI_CONF_OPT_B(vk_x11_strict_image_count, def, \
                  "Force the X11 WSI to create exactly the number of image specified by the application in VkSwapchainCreateInfoKHR::minImageCount")

#define DRI_CONF_VK_X11_ENSURE_MIN_IMAGE_COUNT(def) \
   DRI_CONF_OPT_B(vk_x11_ensure_min_image_count, def, \
                  "Force the X11 WSI to create at least the number of image specified by the driver in VkSurfaceCapabilitiesKHR::minImageCount")

#define DRI_CONF_VK_XWAYLAND_WAIT_READY(def) \
   DRI_CONF_OPT_B(vk_xwayland_wait_ready, def, \
                  "Wait for fences before submitting buffers to Xwayland")

#define DRI_CONF_MESA_GLTHREAD(def) \
   DRI_CONF_OPT_B(mesa_glthread, def, \
                  "Enable offloading GL driver work to a separate thread")

#define DRI_CONF_MESA_NO_ERROR(def) \
   DRI_CONF_OPT_B(mesa_no_error, def, \
                  "Disable GL driver error checking")

static inline struct dri_drawable *
dri_drawable(__DRIdrawable * driDrawPriv)
{
    return (struct dri_drawable *) (driDrawPriv)
           ? driDrawPriv->driverPrivate : NULL;
}

enum st_attachment_type {
    ST_ATTACHMENT_FRONT_LEFT,
    ST_ATTACHMENT_BACK_LEFT,
    ST_ATTACHMENT_FRONT_RIGHT,
    ST_ATTACHMENT_BACK_RIGHT,
    ST_ATTACHMENT_DEPTH_STENCIL,
    ST_ATTACHMENT_ACCUM,

    ST_ATTACHMENT_COUNT,
    ST_ATTACHMENT_INVALID = -1
};

/* for buffer_mask in st_visual */
#define ST_ATTACHMENT_FRONT_LEFT_MASK     (1 << ST_ATTACHMENT_FRONT_LEFT)
#define ST_ATTACHMENT_BACK_LEFT_MASK      (1 << ST_ATTACHMENT_BACK_LEFT)
#define ST_ATTACHMENT_FRONT_RIGHT_MASK    (1 << ST_ATTACHMENT_FRONT_RIGHT)
#define ST_ATTACHMENT_BACK_RIGHT_MASK     (1 << ST_ATTACHMENT_BACK_RIGHT)
#define ST_ATTACHMENT_DEPTH_STENCIL_MASK  (1 << ST_ATTACHMENT_DEPTH_STENCIL)
#define ST_ATTACHMENT_ACCUM_MASK          (1 << ST_ATTACHMENT_ACCUM)

enum pipe_swizzle {
    PIPE_SWIZZLE_X,
    PIPE_SWIZZLE_Y,
    PIPE_SWIZZLE_Z,
    PIPE_SWIZZLE_W,
    PIPE_SWIZZLE_0,
    PIPE_SWIZZLE_1,
    PIPE_SWIZZLE_NONE,
    PIPE_SWIZZLE_MAX, /**< Number of enums counter (must be last) */
};

struct st_context
{
    struct st_context_iface iface;

    struct gl_context *ctx;
    struct pipe_screen *screen;
    struct pipe_context *pipe;
    struct cso_context *cso_context;

    struct draw_context *draw;  /**< For selection/feedback/rastpos only */
    struct draw_stage *feedback_stage;  /**< For GL_FEEDBACK rendermode */
    struct draw_stage *selection_stage;  /**< For GL_SELECT rendermode */
    struct draw_stage *rastpos_stage;  /**< For glRasterPos */

    unsigned pin_thread_counter; /* for L3 thread pinning on AMD Zen */

    GLboolean clamp_frag_color_in_shader;
    GLboolean clamp_vert_color_in_shader;
    boolean has_stencil_export; /**< can do shader stencil export? */
    boolean has_time_elapsed;
    boolean has_etc1;
    boolean has_etc2;
    boolean transcode_etc;
    boolean transcode_astc;
    boolean has_astc_2d_ldr;
    boolean has_astc_5x5_ldr;
    boolean prefer_blit_based_texture_transfer;
    boolean allow_compute_based_texture_transfer;
    boolean force_persample_in_shader;
    boolean has_shareable_shaders;
    boolean has_half_float_packing;
    boolean has_multi_draw_indirect;
    boolean has_single_pipe_stat;
    boolean has_indep_blend_func;
    boolean needs_rgb_dst_alpha_override;
    boolean can_bind_const_buffer_as_vertex;
    boolean lower_flatshade;
    boolean lower_alpha_test;
    boolean lower_point_size;
    boolean lower_two_sided_color;
    boolean lower_ucp;
    boolean prefer_real_buffer_in_constbuf0;
    boolean has_conditional_render;
    boolean lower_texcoord_replace;
    boolean lower_rect_tex;

    /* There are consequences for drivers wanting to call st_finalize_nir
     * twice, once before shader caching and once after lowering for shader
     * variants. If shader variants use lowering passes that are not ready
     * for that, things can blow up.
     *
     * If this is true, st_finalize_nir and pipe_screen::finalize_nir will be
     * called before the result is stored in the shader cache. If lowering for
     * shader variants is invoked, the functions will be called again.
     */
    boolean allow_st_finalize_nir_twice;

    /**
     * If a shader can be created when we get its source.
     * This means it has only 1 variant, not counting glBitmap and
     * glDrawPixels.
     */
    boolean shader_has_one_variant[MESA_SHADER_STAGES];

    boolean needs_texcoord_semantic;
    boolean apply_texture_swizzle_to_border_color;
    boolean emulate_gl_clamp;
    boolean texture_buffer_sampler;

    /* On old libGL's for linux we need to invalidate the drawables
     * on glViewpport calls, this is set via a option.
     */
    boolean invalidate_on_gl_viewport;
    boolean draw_needs_minmax_index;
    boolean has_hw_atomics;


    /* driver supports scissored clears */
    boolean can_scissor_clear;

    /* Some state is contained in constant objects.
     * Other state is just parameter values.
     */
    struct {
        struct pipe_blend_state               blend;
        struct pipe_depth_stencil_alpha_state depth_stencil;
        struct pipe_rasterizer_state          rasterizer;
        struct pipe_sampler_state vert_samplers[PIPE_MAX_SAMPLERS];
        struct pipe_sampler_state frag_samplers[PIPE_MAX_SAMPLERS];
        GLuint num_vert_samplers;
        GLuint num_frag_samplers;
        GLuint num_sampler_views[PIPE_SHADER_TYPES];
        unsigned num_images[PIPE_SHADER_TYPES];
        struct pipe_clip_state clip;
        unsigned constbuf0_enabled_shader_mask;
        unsigned fb_width;
        unsigned fb_height;
        unsigned fb_num_samples;
        unsigned fb_num_layers;
        unsigned fb_num_cb;
        unsigned num_viewports;
        struct pipe_scissor_state scissor[PIPE_MAX_VIEWPORTS];
        struct pipe_viewport_state viewport[PIPE_MAX_VIEWPORTS];
        struct {
            unsigned num;
            boolean include;
            struct pipe_scissor_state rects[PIPE_MAX_WINDOW_RECTANGLES];
        } window_rects;

        GLuint poly_stipple[32];  /**< In OpenGL's bottom-to-top order */

        GLuint fb_orientation;

        bool enable_sample_locations;
        unsigned sample_locations_samples;
        uint8_t sample_locations[
                PIPE_MAX_SAMPLE_LOCATION_GRID_SIZE *
                PIPE_MAX_SAMPLE_LOCATION_GRID_SIZE * 32];
    } state;

    uint64_t dirty; /**< dirty states */

    /** This masks out unused shader resources. Only valid in draw calls. */
    uint64_t active_states;

    /* If true, further analysis of states is required to know if something
     * has changed. Used mainly for shaders.
     */
    bool gfx_shaders_may_be_dirty;
    bool compute_shader_may_be_dirty;

    GLboolean vertdata_edgeflags;
    GLboolean edgeflag_culls_prims;

    /**
     * The number of currently active queries (excluding timer queries).
     * This is used to know if we need to pause any queries for meta ops.
     */
    unsigned active_queries;

    union {
        struct {
            struct st_program *vp;    /**< Currently bound vertex program */
            struct st_program *tcp; /**< Currently bound tess control program */
            struct st_program *tep; /**< Currently bound tess eval program */
            struct st_program *gp;  /**< Currently bound geometry program */
            struct st_program *fp;  /**< Currently bound fragment program */
            struct st_program *cp;   /**< Currently bound compute program */
        };
        struct gl_program *current_program[MESA_SHADER_STAGES];
    };

    struct st_common_variant *vp_variant;

    struct {
        struct pipe_resource *pixelmap_texture;
        struct pipe_sampler_view *pixelmap_sampler_view;
    } pixel_xfer;

    /** for glBitmap */
    struct {
        struct pipe_rasterizer_state rasterizer;
        struct pipe_sampler_state sampler;
        struct pipe_sampler_state atlas_sampler;
        enum pipe_format tex_format;
        struct st_bitmap_cache cache;
    } bitmap;

    /** for glDraw/CopyPixels */
    struct {
        void *zs_shaders[6];
    } drawpix;

    /** Cache of glDrawPixels images */
    struct {
        struct drawpix_cache_entry entries[NUM_DRAWPIX_CACHE_ENTRIES];
        unsigned age;
    } drawpix_cache;

    /** for glReadPixels */
    struct {
        struct pipe_resource *src;
        struct pipe_resource *cache;
        enum pipe_format dst_format;
        unsigned level;
        unsigned layer;
        unsigned hits;
    } readpix_cache;

    /** for glClear */
    struct {
        struct pipe_rasterizer_state raster;
        struct pipe_viewport_state viewport;
        void *vs;
        void *fs;
        void *vs_layered;
        void *gs_layered;
    } clear;

    /* For gl(Compressed)Tex(Sub)Image */
    struct {
        struct pipe_rasterizer_state raster;
        struct pipe_blend_state upload_blend;
        void *vs;
        void *gs;
        void *upload_fs[5][2];
        /**
         * For drivers supporting formatless storing
         * (PIPE_CAP_IMAGE_STORE_FORMATTED) it is a pointer to the download FS;
         * for those not supporting it, it is a pointer to an array of
         * PIPE_FORMAT_COUNT elements, where each element is a pointer to the
         * download FS using that PIPE_FORMAT as the storing format.
         */
        void *download_fs[5][PIPE_MAX_TEXTURE_TYPES][2];
        struct hash_table *shaders;
        bool upload_enabled;
        bool download_enabled;
        bool rgba_only;
        bool layers;
        bool use_gs;
    } pbo;

    /** for drawing with st_util_vertex */
    struct cso_velems_state util_velems;

    /** passthrough vertex shader matching the util_velem attributes */
    void *passthrough_vs;

    enum pipe_texture_target internal_target;

    void *winsys_drawable_handle;

    /* The number of vertex buffers from the last call of validate_arrays. */
    unsigned last_num_vbuffers;
    bool uses_user_vertex_buffers;

    unsigned last_used_atomic_bindings[PIPE_SHADER_TYPES];
    unsigned last_num_ssbos[PIPE_SHADER_TYPES];

    int32_t draw_stamp;
    int32_t read_stamp;

    struct st_config_options options;

    struct st_perf_monitor_group *perfmon;

    enum pipe_reset_status reset_status;

    /* Array of bound texture/image handles which are resident in the context.
     */
    struct st_bound_handles bound_texture_handles[PIPE_SHADER_TYPES];
    struct st_bound_handles bound_image_handles[PIPE_SHADER_TYPES];

    /* Winsys buffers */
    struct list_head winsys_buffers;

    /* Throttling for texture uploads and similar operations to limit memory
     * usage by limiting the number of in-flight operations based on
     * the estimated allocated size needed to execute those operations.
     */
    struct util_throttle throttle;

    struct {
        struct st_zombie_sampler_view_node list;
        simple_mtx_t mutex;
    } zombie_sampler_views;

    struct {
        struct st_zombie_shader_node list;
        simple_mtx_t mutex;
    } zombie_shaders;
};

struct pipe_context {
    struct pipe_screen *screen;

    void *priv;  /**< context private data (for DRI for example) */
    void *draw;  /**< private, for draw module (temporary?) */

    /**
     * Stream uploaders created by the driver. All drivers, gallium frontends, and
     * modules should use them.
     *
     * Use u_upload_alloc or u_upload_data as many times as you want.
     * Once you are done, use u_upload_unmap.
     */
    struct u_upload_mgr *stream_uploader; /* everything but shader constants */
    struct u_upload_mgr *const_uploader;  /* shader constants only */

    void (*destroy)( struct pipe_context * );

    /**
     * VBO drawing
     */
    /*@{*/
    /**
     * Multi draw.
     *
     * For indirect multi draws, num_draws is 1 and indirect->draw_count
     * is used instead.
     *
     * Caps:
     * - Always supported: Direct multi draws
     * - PIPE_CAP_MULTI_DRAW_INDIRECT: Indirect multi draws
     * - PIPE_CAP_MULTI_DRAW_INDIRECT_PARAMS: Indirect draw count
     *
     * Differences against glMultiDraw and glMultiMode:
     * - "info->mode" and "info->index_bias" are always constant due to the lack
     *   of hardware support and CPU performance concerns. Only start and count
     *   vary.
     * - if "info->increment_draw_id" is false, draw_id doesn't change between
     *   draws
     *
     * Direct multi draws are also generated by u_threaded_context, which looks
     * ahead in gallium command buffers and merges single draws.
     *
     * \param pipe          context
     * \param info          draw info
     * \param indirect      indirect multi draws
     * \param draws         array of (start, count) pairs for direct draws
     * \param num_draws     number of direct draws; 1 for indirect multi draws
     */
    void (*draw_vbo)(struct pipe_context *pipe,
                     const struct pipe_draw_info *info,
                     const struct pipe_draw_indirect_info *indirect,
                     const struct pipe_draw_start_count *draws,
                     unsigned num_draws);
    /*@}*/

    /**
     * Predicate subsequent rendering on occlusion query result
     * \param query  the query predicate, or NULL if no predicate
     * \param condition whether to skip on FALSE or TRUE query results
     * \param mode  one of PIPE_RENDER_COND_x
     */
    void (*render_condition)( struct pipe_context *pipe,
                              struct pipe_query *query,
                              bool condition,
                              enum pipe_render_cond_flag mode );

    /**
     * Predicate subsequent rendering on a value in a buffer
     * \param buffer The buffer to query for the value
     * \param offset Offset in the buffer to query 32-bit
     * \param condition whether to skip on FALSE or TRUE query results
     */
    void (*render_condition_mem)( struct pipe_context *pipe,
                                  struct pipe_resource *buffer,
                                  uint32_t offset,
                                  bool condition );
    /**
     * Query objects
     */
    /*@{*/
    struct pipe_query *(*create_query)( struct pipe_context *pipe,
                                        unsigned query_type,
                                        unsigned index );

    /**
     * Create a query object that queries all given query types simultaneously.
     *
     * This can only be used for those query types for which
     * get_driver_query_info indicates that it must be used. Only one batch
     * query object may be active at a time.
     *
     * There may be additional constraints on which query types can be used
     * together, in particular those that are implied by
     * get_driver_query_group_info.
     *
     * \param num_queries the number of query types
     * \param query_types array of \p num_queries query types
     * \return a query object, or NULL on error.
     */
    struct pipe_query *(*create_batch_query)( struct pipe_context *pipe,
                                              unsigned num_queries,
                                              unsigned *query_types );

    void (*destroy_query)(struct pipe_context *pipe,
                          struct pipe_query *q);

    bool (*begin_query)(struct pipe_context *pipe, struct pipe_query *q);
    bool (*end_query)(struct pipe_context *pipe, struct pipe_query *q);

    /**
     * Get results of a query.
     * \param wait  if true, this query will block until the result is ready
     * \return TRUE if results are ready, FALSE otherwise
     */
    bool (*get_query_result)(struct pipe_context *pipe,
                             struct pipe_query *q,
                             bool wait,
                             union pipe_query_result *result);

    /**
     * Get results of a query, storing into resource. Note that this may not
     * be used with batch queries.
     *
     * \param wait  if true, this query will block until the result is ready
     * \param result_type  the type of the value being stored:
     * \param index  for queries that return multiple pieces of data, which
     *               item of that data to store (e.g. for
     *               PIPE_QUERY_PIPELINE_STATISTICS).
     *               When the index is -1, instead of the value of the query
     *               the driver should instead write a 1 or 0 to the appropriate
     *               location with 1 meaning that the query result is available.
     */
    void (*get_query_result_resource)(struct pipe_context *pipe,
                                      struct pipe_query *q,
                                      bool wait,
                                      enum pipe_query_value_type result_type,
                                      int index,
                                      struct pipe_resource *resource,
                                      unsigned offset);

    /**
     * Set whether all current non-driver queries except TIME_ELAPSED are
     * active or paused.
     */
    void (*set_active_query_state)(struct pipe_context *pipe, bool enable);

    /**
     * INTEL Performance Query
     */
    /*@{*/

    unsigned (*init_intel_perf_query_info)(struct pipe_context *pipe);

    void (*get_intel_perf_query_info)(struct pipe_context *pipe,
                                      unsigned query_index,
                                      const char **name,
                                      uint32_t *data_size,
                                      uint32_t *n_counters,
                                      uint32_t *n_active);

    void (*get_intel_perf_query_counter_info)(struct pipe_context *pipe,
                                              unsigned query_index,
                                              unsigned counter_index,
                                              const char **name,
                                              const char **desc,
                                              uint32_t *offset,
                                              uint32_t *data_size,
                                              uint32_t *type_enum,
                                              uint32_t *data_type_enum,
                                              uint64_t *raw_max);

    struct pipe_query *(*new_intel_perf_query_obj)(struct pipe_context *pipe,
                                                   unsigned query_index);

    bool (*begin_intel_perf_query)(struct pipe_context *pipe, struct pipe_query *q);

    void (*end_intel_perf_query)(struct pipe_context *pipe, struct pipe_query *q);

    void (*delete_intel_perf_query)(struct pipe_context *pipe, struct pipe_query *q);

    void (*wait_intel_perf_query)(struct pipe_context *pipe, struct pipe_query *q);

    bool (*is_intel_perf_query_ready)(struct pipe_context *pipe, struct pipe_query *q);

    bool (*get_intel_perf_query_data)(struct pipe_context *pipe,
                                      struct pipe_query *q,
                                      size_t data_size,
                                      uint32_t *data,
                                      uint32_t *bytes_written);

    /*@}*/

    /**
     * State functions (create/bind/destroy state objects)
     */
    /*@{*/
    void * (*create_blend_state)(struct pipe_context *,
                                 const struct pipe_blend_state *);
    void   (*bind_blend_state)(struct pipe_context *, void *);
    void   (*delete_blend_state)(struct pipe_context *, void  *);

    void * (*create_sampler_state)(struct pipe_context *,
                                   const struct pipe_sampler_state *);
    void   (*bind_sampler_states)(struct pipe_context *,
                                  enum pipe_shader_type shader,
                                  unsigned start_slot, unsigned num_samplers,
                                  void **samplers);
    void   (*delete_sampler_state)(struct pipe_context *, void *);

    void * (*create_rasterizer_state)(struct pipe_context *,
                                      const struct pipe_rasterizer_state *);
    void   (*bind_rasterizer_state)(struct pipe_context *, void *);
    void   (*delete_rasterizer_state)(struct pipe_context *, void *);

    void * (*create_depth_stencil_alpha_state)(struct pipe_context *,
                                               const struct pipe_depth_stencil_alpha_state *);
    void   (*bind_depth_stencil_alpha_state)(struct pipe_context *, void *);
    void   (*delete_depth_stencil_alpha_state)(struct pipe_context *, void *);

    void * (*create_fs_state)(struct pipe_context *,
                              const struct pipe_shader_state *);
    void   (*bind_fs_state)(struct pipe_context *, void *);
    void   (*delete_fs_state)(struct pipe_context *, void *);

    void * (*create_vs_state)(struct pipe_context *,
                              const struct pipe_shader_state *);
    void   (*bind_vs_state)(struct pipe_context *, void *);
    void   (*delete_vs_state)(struct pipe_context *, void *);

    void * (*create_gs_state)(struct pipe_context *,
                              const struct pipe_shader_state *);
    void   (*bind_gs_state)(struct pipe_context *, void *);
    void   (*delete_gs_state)(struct pipe_context *, void *);

    void * (*create_tcs_state)(struct pipe_context *,
                               const struct pipe_shader_state *);
    void   (*bind_tcs_state)(struct pipe_context *, void *);
    void   (*delete_tcs_state)(struct pipe_context *, void *);

    void * (*create_tes_state)(struct pipe_context *,
                               const struct pipe_shader_state *);
    void   (*bind_tes_state)(struct pipe_context *, void *);
    void   (*delete_tes_state)(struct pipe_context *, void *);

    void * (*create_vertex_elements_state)(struct pipe_context *,
                                           unsigned num_elements,
                                           const struct pipe_vertex_element *);
    void   (*bind_vertex_elements_state)(struct pipe_context *, void *);
    void   (*delete_vertex_elements_state)(struct pipe_context *, void *);

    /*@}*/

    /**
     * Parameter-like state (or properties)
     */
    /*@{*/
    void (*set_blend_color)( struct pipe_context *,
                             const struct pipe_blend_color * );

    void (*set_stencil_ref)( struct pipe_context *,
                             const struct pipe_stencil_ref ref);

    void (*set_sample_mask)( struct pipe_context *,
                             unsigned sample_mask );

    void (*set_min_samples)( struct pipe_context *,
                             unsigned min_samples );

    void (*set_clip_state)( struct pipe_context *,
                            const struct pipe_clip_state * );

    /**
     * Set constant buffer
     *
     * \param shader           Shader stage
     * \param index            Buffer binding slot index within a shader stage
     * \param take_ownership   The callee takes ownership of the buffer reference.
     *                         (the callee shouldn't increment the ref count)
     * \param buf              Constant buffer parameters
     */
    void (*set_constant_buffer)( struct pipe_context *,
                                 enum pipe_shader_type shader, uint index,
                                 bool take_ownership,
                                 const struct pipe_constant_buffer *buf );

    /**
     * Set inlinable constants for constant buffer 0.
     *
     * These are constants that the driver would like to inline in the IR
     * of the current shader and recompile it. Drivers can determine which
     * constants they prefer to inline in finalize_nir and store that
     * information in shader_info::*inlinable_uniform*. When the state tracker
     * or frontend uploads constants to a constant buffer, it can pass
     * inlinable constants separately via this call.
     *
     * Any set_constant_buffer call invalidates this state, so this function
     * must be called after it. Binding a shader also invalidates this state.
     *
     * There is no PIPE_CAP for this. Drivers shouldn't set the shader_info
     * fields if they don't want this or if they don't implement this.
     */
    void (*set_inlinable_constants)( struct pipe_context *,
                                     enum pipe_shader_type shader,
                                     uint num_values, uint32_t *values );

    void (*set_framebuffer_state)( struct pipe_context *,
                                   const struct pipe_framebuffer_state * );

    /**
     * Set the sample locations used during rasterization. When NULL or sized
     * zero, the default locations are used.
     *
     * Note that get_sample_position() still returns the default locations.
     *
     * The samples are accessed with
     * locations[(pixel_y*grid_w+pixel_x)*ms+i],
     * where:
     * ms      = the sample count
     * grid_w  = the pixel grid width for the sample count
     * grid_w  = the pixel grid height for the sample count
     * pixel_x = the window x coordinate modulo grid_w
     * pixel_y = the window y coordinate modulo grid_w
     * i       = the sample index
     * This gives a result with the x coordinate as the low 4 bits and the y
     * coordinate as the high 4 bits. For each coordinate 0 is the left or top
     * edge of the pixel's rectangle and 16 (not 15) is the right or bottom edge.
     *
     * Out of bounds accesses are return undefined values.
     *
     * The pixel grid is used to vary sample locations across pixels and its
     * size can be queried with get_sample_pixel_grid().
     */
    void (*set_sample_locations)( struct pipe_context *,
                                  size_t size, const uint8_t *locations );

    void (*set_polygon_stipple)( struct pipe_context *,
                                 const struct pipe_poly_stipple * );

    void (*set_scissor_states)( struct pipe_context *,
                                unsigned start_slot,
                                unsigned num_scissors,
                                const struct pipe_scissor_state * );

    void (*set_window_rectangles)( struct pipe_context *,
                                   bool include,
                                   unsigned num_rectangles,
                                   const struct pipe_scissor_state * );

    void (*set_viewport_states)( struct pipe_context *,
                                 unsigned start_slot,
                                 unsigned num_viewports,
                                 const struct pipe_viewport_state *);

    void (*set_sampler_views)(struct pipe_context *,
                              enum pipe_shader_type shader,
                              unsigned start_slot, unsigned num_views,
                              unsigned unbind_num_trailing_slots,
                              struct pipe_sampler_view **views);

    void (*set_tess_state)(struct pipe_context *,
                           const float default_outer_level[4],
                           const float default_inner_level[2]);

    /**
     * Sets the debug callback. If the pointer is null, then no callback is
     * set, otherwise a copy of the data should be made.
     */
    void (*set_debug_callback)(struct pipe_context *,
                               const struct pipe_debug_callback *);

    /**
     * Bind an array of shader buffers that will be used by a shader.
     * Any buffers that were previously bound to the specified range
     * will be unbound.
     *
     * \param shader     selects shader stage
     * \param start_slot first buffer slot to bind.
     * \param count      number of consecutive buffers to bind.
     * \param buffers    array of pointers to the buffers to bind, it
     *                   should contain at least \a count elements
     *                   unless it's NULL, in which case no buffers will
     *                   be bound.
     * \param writable_bitmask  If bit i is not set, buffers[i] will only be
     *                          used with loads. If unsure, set to ~0.
     */
    void (*set_shader_buffers)(struct pipe_context *,
                               enum pipe_shader_type shader,
                               unsigned start_slot, unsigned count,
                               const struct pipe_shader_buffer *buffers,
                               unsigned writable_bitmask);

    /**
     * Bind an array of hw atomic buffers for use by all shaders.
     * And buffers that were previously bound to the specified range
     * will be unbound.
     *
     * \param start_slot first buffer slot to bind.
     * \param count      number of consecutive buffers to bind.
     * \param buffers    array of pointers to the buffers to bind, it
     *                   should contain at least \a count elements
     *                   unless it's NULL, in which case no buffers will
     *                   be bound.
     */
    void (*set_hw_atomic_buffers)(struct pipe_context *,
                                  unsigned start_slot, unsigned count,
                                  const struct pipe_shader_buffer *buffers);

    /**
     * Bind an array of images that will be used by a shader.
     * Any images that were previously bound to the specified range
     * will be unbound.
     *
     * \param shader     selects shader stage
     * \param start_slot first image slot to bind.
     * \param count      number of consecutive images to bind.
     * \param unbind_num_trailing_slots  number of images to unbind after
     *                                   the bound slot
     * \param buffers    array of the images to bind, it
     *                   should contain at least \a count elements
     *                   unless it's NULL, in which case no images will
     *                   be bound.
     */
    void (*set_shader_images)(struct pipe_context *,
                              enum pipe_shader_type shader,
                              unsigned start_slot, unsigned count,
                              unsigned unbind_num_trailing_slots,
                              const struct pipe_image_view *images);

    /**
     * Bind an array of vertex buffers to the specified slots.
     *
     * \param start_slot      first vertex buffer slot
     * \param count           number of consecutive vertex buffers to bind.
     * \param unbind_num_trailing_slots  unbind slots after the bound slots
     * \param take_ownership the caller holds buffer references and they
     *                        should be taken over by the callee. This means
     *                        that drivers shouldn't increment reference counts.
     * \param buffers         array of the buffers to bind
     */
    void (*set_vertex_buffers)( struct pipe_context *,
                                unsigned start_slot,
                                unsigned num_buffers,
                                unsigned unbind_num_trailing_slots,
                                bool take_ownership,
                                const struct pipe_vertex_buffer * );

    /*@}*/

    /**
     * Stream output functions.
     */
    /*@{*/

    struct pipe_stream_output_target *(*create_stream_output_target)(
            struct pipe_context *,
            struct pipe_resource *,
            unsigned buffer_offset,
            unsigned buffer_size);

    void (*stream_output_target_destroy)(struct pipe_context *,
                                         struct pipe_stream_output_target *);

    void (*set_stream_output_targets)(struct pipe_context *,
                                      unsigned num_targets,
                                      struct pipe_stream_output_target **targets,
                                      const unsigned *offsets);

    uint32_t (*stream_output_target_offset)(struct pipe_stream_output_target *target);

    /*@}*/


    /**
     * INTEL_blackhole_render
     */
    /*@{*/

    void (*set_frontend_noop)(struct pipe_context *,
                              bool enable);

    /*@}*/


    /**
     * Resource functions for blit-like functionality
     *
     * If a driver supports multisampling, blit must implement color resolve.
     */
    /*@{*/

    /**
     * Copy a block of pixels from one resource to another.
     * The resource must be of the same format.
     * Resources with nr_samples > 1 are not allowed.
     */
    void (*resource_copy_region)(struct pipe_context *pipe,
                                 struct pipe_resource *dst,
                                 unsigned dst_level,
                                 unsigned dstx, unsigned dsty, unsigned dstz,
                                 struct pipe_resource *src,
                                 unsigned src_level,
                                 const struct pipe_box *src_box);

    /* Optimal hardware path for blitting pixels.
     * Scaling, format conversion, up- and downsampling (resolve) are allowed.
     */
    void (*blit)(struct pipe_context *pipe,
                 const struct pipe_blit_info *info);

    /*@}*/

    /**
     * Clear the specified set of currently bound buffers to specified values.
     * The entire buffers are cleared (no scissor, no colormask, etc).
     *
     * \param buffers  bitfield of PIPE_CLEAR_* values.
     * \param scissor_state  the scissored region to clear
     * \param color  pointer to a union of fiu array for each of r, g, b, a.
     * \param depth  depth clear value in [0,1].
     * \param stencil  stencil clear value
     */
    void (*clear)(struct pipe_context *pipe,
                  unsigned buffers,
                  const struct pipe_scissor_state *scissor_state,
                  const union pipe_color_union *color,
                  double depth,
                  unsigned stencil);

    /**
     * Clear a color rendertarget surface.
     * \param color  pointer to an union of fiu array for each of r, g, b, a.
     */
    void (*clear_render_target)(struct pipe_context *pipe,
                                struct pipe_surface *dst,
                                const union pipe_color_union *color,
                                unsigned dstx, unsigned dsty,
                                unsigned width, unsigned height,
                                bool render_condition_enabled);

    /**
     * Clear a depth-stencil surface.
     * \param clear_flags  bitfield of PIPE_CLEAR_DEPTH/STENCIL values.
     * \param depth  depth clear value in [0,1].
     * \param stencil  stencil clear value
     */
    void (*clear_depth_stencil)(struct pipe_context *pipe,
                                struct pipe_surface *dst,
                                unsigned clear_flags,
                                double depth,
                                unsigned stencil,
                                unsigned dstx, unsigned dsty,
                                unsigned width, unsigned height,
                                bool render_condition_enabled);

    /**
     * Clear the texture with the specified texel. Not guaranteed to be a
     * renderable format. Data provided in the resource's format.
     */
    void (*clear_texture)(struct pipe_context *pipe,
                          struct pipe_resource *res,
                          unsigned level,
                          const struct pipe_box *box,
                          const void *data);

    /**
     * Clear a buffer. Runs a memset over the specified region with the element
     * value passed in through clear_value of size clear_value_size.
     */
    void (*clear_buffer)(struct pipe_context *pipe,
                         struct pipe_resource *res,
                         unsigned offset,
                         unsigned size,
                         const void *clear_value,
                         int clear_value_size);

    /**
     * If a depth buffer is rendered with different sample location state than
     * what is current at the time of reading, the values may differ because
     * depth buffer compression can depend the sample locations.
     *
     * This function is a hint to decompress the current depth buffer to avoid
     * such problems.
     */
    void (*evaluate_depth_buffer)(struct pipe_context *pipe);

    /**
     * Flush draw commands.
     *
     * This guarantees that the new fence (if any) will finish in finite time,
     * unless PIPE_FLUSH_DEFERRED is used.
     *
     * Subsequent operations on other contexts of the same screen are guaranteed
     * to execute after the flushed commands, unless PIPE_FLUSH_ASYNC is used.
     *
     * NOTE: use screen->fence_reference() (or equivalent) to transfer
     * new fence ref to **fence, to ensure that previous fence is unref'd
     *
     * \param fence  if not NULL, an old fence to unref and transfer a
     *    new fence reference to
     * \param flags  bitfield of enum pipe_flush_flags values.
     */
    void (*flush)(struct pipe_context *pipe,
                  struct pipe_fence_handle **fence,
                  unsigned flags);

    /**
     * Create a fence from a fd.
     *
     * This is used for importing a foreign/external fence fd.
     *
     * \param fence  if not NULL, an old fence to unref and transfer a
     *    new fence reference to
     * \param fd     fd representing the fence object
     * \param type   indicates which fence types backs fd
     */
    void (*create_fence_fd)(struct pipe_context *pipe,
                            struct pipe_fence_handle **fence,
                            int fd,
                            enum pipe_fd_type type);

    /**
     * Insert commands to have GPU wait for fence to be signaled.
     */
    void (*fence_server_sync)(struct pipe_context *pipe,
                              struct pipe_fence_handle *fence);

    /**
     * Insert commands to have the GPU signal a fence.
     */
    void (*fence_server_signal)(struct pipe_context *pipe,
                                struct pipe_fence_handle *fence);

    /**
     * Create a view on a texture to be used by a shader stage.
     */
    struct pipe_sampler_view * (*create_sampler_view)(struct pipe_context *ctx,
                                                      struct pipe_resource *texture,
                                                      const struct pipe_sampler_view *templat);

    /**
     * Destroy a view on a texture.
     *
     * \param ctx the current context
     * \param view the view to be destroyed
     *
     * \note The current context may not be the context in which the view was
     *       created (view->context). However, the caller must guarantee that
     *       the context which created the view is still alive.
     */
    void (*sampler_view_destroy)(struct pipe_context *ctx,
                                 struct pipe_sampler_view *view);


    /**
     * Get a surface which is a "view" into a resource, used by
     * render target / depth stencil stages.
     */
    struct pipe_surface *(*create_surface)(struct pipe_context *ctx,
                                           struct pipe_resource *resource,
                                           const struct pipe_surface *templat);

    void (*surface_destroy)(struct pipe_context *ctx,
                            struct pipe_surface *);


    /**
     * Map a resource.
     *
     * Transfers are (by default) context-private and allow uploads to be
     * interleaved with rendering.
     *
     * out_transfer will contain the transfer object that must be passed
     * to all the other transfer functions. It also contains useful
     * information (like texture strides).
     */
    void *(*transfer_map)(struct pipe_context *,
                          struct pipe_resource *resource,
                          unsigned level,
                          unsigned usage,  /* a combination of PIPE_MAP_x */
                          const struct pipe_box *,
                          struct pipe_transfer **out_transfer);

    /* If transfer was created with WRITE|FLUSH_EXPLICIT, only the
     * regions specified with this call are guaranteed to be written to
     * the resource.
     */
    void (*transfer_flush_region)( struct pipe_context *,
                                   struct pipe_transfer *transfer,
                                   const struct pipe_box *);

    void (*transfer_unmap)(struct pipe_context *,
                           struct pipe_transfer *transfer);

    /* One-shot transfer operation with data supplied in a user
     * pointer.
     */
    void (*buffer_subdata)(struct pipe_context *,
                           struct pipe_resource *,
                           unsigned usage, /* a combination of PIPE_MAP_x */
                           unsigned offset,
                           unsigned size,
                           const void *data);

    void (*texture_subdata)(struct pipe_context *,
                            struct pipe_resource *,
                            unsigned level,
                            unsigned usage, /* a combination of PIPE_MAP_x */
                            const struct pipe_box *,
                            const void *data,
                            unsigned stride,
                            unsigned layer_stride);

    /**
     * Flush any pending framebuffer writes and invalidate texture caches.
     */
    void (*texture_barrier)(struct pipe_context *, unsigned flags);

    /**
     * Flush caches according to flags.
     */
    void (*memory_barrier)(struct pipe_context *, unsigned flags);

    /**
     * Change the commitment status of a part of the given resource, which must
     * have been created with the PIPE_RESOURCE_FLAG_SPARSE bit.
     *
     * \param level The texture level whose commitment should be changed.
     * \param box The region of the resource whose commitment should be changed.
     * \param commit Whether memory should be committed or un-committed.
     *
     * \return false if out of memory, true on success.
     */
    bool (*resource_commit)(struct pipe_context *, struct pipe_resource *,
                            unsigned level, struct pipe_box *box, bool commit);

    /**
     * Creates a video codec for a specific video format/profile
     */
    struct pipe_video_codec *(*create_video_codec)( struct pipe_context *context,
                                                    const struct pipe_video_codec *templat );

    /**
     * Creates a video buffer as decoding target
     */
    struct pipe_video_buffer *(*create_video_buffer)( struct pipe_context *context,
                                                      const struct pipe_video_buffer *templat );

    /**
     * Compute kernel execution
     */
    /*@{*/
    /**
     * Define the compute program and parameters to be used by
     * pipe_context::launch_grid.
     */
    void *(*create_compute_state)(struct pipe_context *context,
                                  const struct pipe_compute_state *);
    void (*bind_compute_state)(struct pipe_context *, void *);
    void (*delete_compute_state)(struct pipe_context *, void *);

    /**
     * Bind an array of shader resources that will be used by the
     * compute program.  Any resources that were previously bound to
     * the specified range will be unbound after this call.
     *
     * \param start      first resource to bind.
     * \param count      number of consecutive resources to bind.
     * \param resources  array of pointers to the resources to bind, it
     *                   should contain at least \a count elements
     *                   unless it's NULL, in which case no new
     *                   resources will be bound.
     */
    void (*set_compute_resources)(struct pipe_context *,
                                  unsigned start, unsigned count,
                                  struct pipe_surface **resources);

    /**
     * Bind an array of buffers to be mapped into the address space of
     * the GLOBAL resource.  Any buffers that were previously bound
     * between [first, first + count - 1] are unbound after this call.
     *
     * \param first      first buffer to map.
     * \param count      number of consecutive buffers to map.
     * \param resources  array of pointers to the buffers to map, it
     *                   should contain at least \a count elements
     *                   unless it's NULL, in which case no new
     *                   resources will be bound.
     * \param handles    array of pointers to the memory locations that
     *                   will be updated with the address each buffer
     *                   will be mapped to.  The base memory address of
     *                   each of the buffers will be added to the value
     *                   pointed to by its corresponding handle to form
     *                   the final address argument.  It should contain
     *                   at least \a count elements, unless \a
     *                   resources is NULL in which case \a handles
     *                   should be NULL as well.
     *
     * Note that the driver isn't required to make any guarantees about
     * the contents of the \a handles array being valid anytime except
     * during the subsequent calls to pipe_context::launch_grid.  This
     * means that the only sensible location handles[i] may point to is
     * somewhere within the INPUT buffer itself.  This is so to
     * accommodate implementations that lack virtual memory but
     * nevertheless migrate buffers on the fly, leading to resource
     * base addresses that change on each kernel invocation or are
     * unknown to the pipe driver.
     */
    void (*set_global_binding)(struct pipe_context *context,
                               unsigned first, unsigned count,
                               struct pipe_resource **resources,
                               uint32_t **handles);

    /**
     * Launch the compute kernel starting from instruction \a pc of the
     * currently bound compute program.
     */
    void (*launch_grid)(struct pipe_context *context,
                        const struct pipe_grid_info *info);
    /*@}*/

    /**
     * SVM (Share Virtual Memory) helpers
     */
    /*@{*/
    /**
     * Migrate range of virtual address to device or host memory.
     *
     * \param to_device - true if the virtual memory is migrated to the device
     *                    false if the virtual memory is migrated to the host
     * \param migrate_content - whether the content should be migrated as well
     */
    void (*svm_migrate)(struct pipe_context *context, unsigned num_ptrs,
                        const void* const* ptrs, const size_t *sizes,
                        bool to_device, bool migrate_content);
    /*@}*/

    /**
     * Get the default sample position for an individual sample point.
     *
     * \param sample_count - total number of samples
     * \param sample_index - sample to get the position values for
     * \param out_value - return value of 2 floats for x and y position for
     *                    requested sample.
     */
    void (*get_sample_position)(struct pipe_context *context,
                                unsigned sample_count,
                                unsigned sample_index,
                                float *out_value);

    /**
     * Query a timestamp in nanoseconds.  This is completely equivalent to
     * pipe_screen::get_timestamp() but takes a context handle for drivers
     * that require a context.
     */
    uint64_t (*get_timestamp)(struct pipe_context *);

    /**
     * Flush the resource cache, so that the resource can be used
     * by an external client. Possible usage:
     * - flushing a resource before presenting it on the screen
     * - flushing a resource if some other process or device wants to use it
     * This shouldn't be used to flush caches if the resource is only managed
     * by a single pipe_screen and is not shared with another process.
     * (i.e. you shouldn't use it to flush caches explicitly if you want to e.g.
     * use the resource for texturing)
     */
    void (*flush_resource)(struct pipe_context *ctx,
                           struct pipe_resource *resource);

    /**
     * Invalidate the contents of the resource. This is used to
     *
     * (1) implement EGL's semantic of undefined depth/stencil
     * contents after a swapbuffers.  This allows a tiled renderer (for
     * example) to not store the depth buffer.
     *
     * (2) implement GL's InvalidateBufferData. For backwards compatibility,
     * you must only rely on the usability for this purpose when
     * PIPE_CAP_INVALIDATE_BUFFER is enabled.
     */
    void (*invalidate_resource)(struct pipe_context *ctx,
                                struct pipe_resource *resource);

    /**
     * Return information about unexpected device resets.
     */
    enum pipe_reset_status (*get_device_reset_status)(struct pipe_context *ctx);

    /**
     * Sets the reset status callback. If the pointer is null, then no callback
     * is set, otherwise a copy of the data should be made.
     */
    void (*set_device_reset_callback)(struct pipe_context *ctx,
                                      const struct pipe_device_reset_callback *cb);

    /**
     * Dump driver-specific debug information into a stream. This is
     * used by debugging tools.
     *
     * \param ctx        pipe context
     * \param stream     where the output should be written to
     * \param flags      a mask of PIPE_DUMP_* flags
     */
    void (*dump_debug_state)(struct pipe_context *ctx, FILE *stream,
                             unsigned flags);

    /**
     * Set the log context to which the driver should write internal debug logs
     * (internal states, command streams).
     *
     * The caller must ensure that the log context is destroyed and reset to
     * NULL before the pipe context is destroyed, and that log context functions
     * are only called from the driver thread.
     *
     * \param ctx pipe context
     * \param log logging context
     */
    void (*set_log_context)(struct pipe_context *ctx, struct u_log_context *log);

    /**
     * Emit string marker in cmdstream
     */
    void (*emit_string_marker)(struct pipe_context *ctx,
                               const char *string,
                               int len);

    /**
     * Generate mipmap.
     * \return TRUE if mipmap generation succeeds, FALSE otherwise
     */
    bool (*generate_mipmap)(struct pipe_context *ctx,
                            struct pipe_resource *resource,
                            enum pipe_format format,
                            unsigned base_level,
                            unsigned last_level,
                            unsigned first_layer,
                            unsigned last_layer);

    /**
     * Create a 64-bit texture handle.
     *
     * \param ctx        pipe context
     * \param view       pipe sampler view object
     * \param state      pipe sampler state template
     * \return           a 64-bit texture handle if success, 0 otherwise
     */
    uint64_t (*create_texture_handle)(struct pipe_context *ctx,
                                      struct pipe_sampler_view *view,
                                      const struct pipe_sampler_state *state);

    /**
     * Delete a texture handle.
     *
     * \param ctx        pipe context
     * \param handle     64-bit texture handle
     */
    void (*delete_texture_handle)(struct pipe_context *ctx, uint64_t handle);

    /**
     * Make a texture handle resident.
     *
     * \param ctx        pipe context
     * \param handle     64-bit texture handle
     * \param resident   TRUE for resident, FALSE otherwise
     */
    void (*make_texture_handle_resident)(struct pipe_context *ctx,
                                         uint64_t handle, bool resident);

    /**
     * Create a 64-bit image handle.
     *
     * \param ctx        pipe context
     * \param image      pipe image view template
     * \return           a 64-bit image handle if success, 0 otherwise
     */
    uint64_t (*create_image_handle)(struct pipe_context *ctx,
                                    const struct pipe_image_view *image);

    /**
     * Delete an image handle.
     *
     * \param ctx        pipe context
     * \param handle     64-bit image handle
     */
    void (*delete_image_handle)(struct pipe_context *ctx, uint64_t handle);

    /**
     * Make an image handle resident.
     *
     * \param ctx        pipe context
     * \param handle     64-bit image handle
     * \param access     GL_READ_ONLY, GL_WRITE_ONLY or GL_READ_WRITE
     * \param resident   TRUE for resident, FALSE otherwise
     */
    void (*make_image_handle_resident)(struct pipe_context *ctx, uint64_t handle,
                                       unsigned access, bool resident);

    /**
     * Call the given function from the driver thread.
     *
     * This is set by threaded contexts for use by debugging wrappers.
     *
     * \param asap if true, run the callback immediately if there are no pending
     *             commands to be processed by the driver thread
     */
    void (*callback)(struct pipe_context *ctx, void (*fn)(void *), void *data,
                     bool asap);

    /**
     * Set a context parameter See enum pipe_context_param for more details.
     */
    void (*set_context_param)(struct pipe_context *ctx,
                              enum pipe_context_param param,
                              unsigned value);
};

struct hash_entry {
    uint32_t hash;
    const void *key;
    void *data;
};

struct hash_table {
    struct hash_entry *table;
    uint32_t (*key_hash_function)(const void *key);
    bool (*key_equals_function)(const void *a, const void *b);
    const void *deleted_key;
    uint32_t size;
    uint32_t rehash;
    uint64_t size_magic;
    uint64_t rehash_magic;
    uint32_t max_entries;
    uint32_t size_index;
    uint32_t entries;
    uint32_t deleted_entries;
};

static inline uint32_t
uint_hash(GLuint id)
{
    return id;
}

static inline void *
uint_key(GLuint id)
{
    return (void *)(uintptr_t) id;
}

#define DELETED_KEY_VALUE 1

struct _mesa_HashTable {
    struct hash_table *ht;
    GLuint MaxKey;                        /**< highest key inserted so far */
    simple_mtx_t Mutex;                   /**< mutual exclusion lock */
    /* Used when name reuse is enabled */
    struct util_idalloc* id_alloc;

    /** Value that would be in the table for DELETED_KEY_VALUE. */
    void *deleted_key_data;
#ifndef NDEBUG
    GLboolean InDeleteAll;                /**< Debug check */
#endif
};

struct gl_renderbuffer
{
    GLuint ClassID;        /**< Useful for drivers */
    GLuint Name;
    GLchar *Label;         /**< GL_KHR_debug */
    GLint RefCount;
    GLuint Width, Height;
    GLuint Depth;
    GLboolean Purgeable;  /**< Is the buffer purgeable under memory pressure? */
    GLboolean AttachedAnytime; /**< TRUE if it was attached to a framebuffer */
    GLubyte NumSamples;    /**< zero means not multisampled */
    GLubyte NumStorageSamples; /**< for AMD_framebuffer_multisample_advanced */
    GLenum16 InternalFormat; /**< The user-specified format */
    GLenum16 _BaseFormat;    /**< Either GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT or
                               GL_STENCIL_INDEX. */
    mesa_format Format;      /**< The actual renderbuffer memory format */
    /**
     * Pointer to the texture image if this renderbuffer wraps a texture,
     * otherwise NULL.
     *
     * Note that the reference on the gl_texture_object containing this
     * TexImage is held by the gl_renderbuffer_attachment.
     */
    struct gl_texture_image *TexImage;

    /** Delete this renderbuffer */
    void (*Delete)(struct gl_context *ctx, struct gl_renderbuffer *rb);

    /** Allocate new storage for this renderbuffer */
    GLboolean (*AllocStorage)(struct gl_context *ctx,
                              struct gl_renderbuffer *rb,
                              GLenum internalFormat,
                              GLuint width, GLuint height);

    struct pipe_resource *texture;
    /* This points to either "surface_linear" or "surface_srgb".
     * It doesn't hold the pipe_surface reference. The other two do.
     */
    struct pipe_surface *surface;
    struct pipe_surface *surface_linear;
    struct pipe_surface *surface_srgb;
    GLboolean defined;        /**< defined contents? */

    struct pipe_transfer *transfer; /**< only used when mapping the resource */

    /**
     * Used only when hardware accumulation buffers are not supported.
     */
    boolean software;
    void *data;

    bool use_readpix_cache;

    /* Inputs from Driver.RenderTexture, don't use directly. */
    boolean is_rtt; /**< whether Driver.RenderTexture was called */
    unsigned rtt_face, rtt_slice;
    boolean rtt_layered; /**< whether glFramebufferTexture was called */
    unsigned rtt_nr_samples; /**< from FramebufferTexture2DMultisampleEXT */
};


struct gl_shared_state
{
    simple_mtx_t Mutex;		   /**< for thread safety */
    GLint RefCount;			   /**< Reference count */
    bool DisplayListsAffectGLThread;

    struct _mesa_HashTable *DisplayList;	   /**< Display lists hash table */
    struct _mesa_HashTable *BitmapAtlas;    /**< For optimized glBitmap text */
    struct _mesa_HashTable *TexObjects;	   /**< Texture objects hash table */

    /** Default texture objects (shared by all texture units) */
    struct gl_texture_object *DefaultTex[NUM_TEXTURE_TARGETS];

    /** Fallback texture used when a bound texture is incomplete */
    struct gl_texture_object *FallbackTex[NUM_TEXTURE_TARGETS];

    /**
     * \name Thread safety and statechange notification for texture
     * objects.
     *
     * \todo Improve the granularity of locking.
     */
    /*@{*/
    simple_mtx_t TexMutex;		/**< texobj thread safety */
    GLuint TextureStateStamp;	        /**< state notification for shared tex */
    /*@}*/

    /**
     * \name Vertex/geometry/fragment programs
     */
    /*@{*/
    struct _mesa_HashTable *Programs; /**< All vertex/fragment programs */
    struct gl_program *DefaultVertexProgram;
    struct gl_program *DefaultFragmentProgram;
    /*@}*/

    /* GL_ATI_fragment_shader */
    struct _mesa_HashTable *ATIShaders;
    struct ati_fragment_shader *DefaultFragmentShader;

    struct _mesa_HashTable *BufferObjects;

    /* Buffer objects released by a different context than the one that
     * created them. Since the creating context holds one global buffer
     * reference for each buffer it created and skips reference counting,
     * deleting a buffer by another context can't touch the buffer reference
     * held by the context that created it. Only the creating context can
     * remove its global buffer reference.
     *
     * This list contains all buffers that were deleted by a different context
     * than the one that created them. This list should be probed by all
     * contexts regularly and remove references of those buffers that they own.
     */
    struct set *ZombieBufferObjects;

    /** Table of both gl_shader and gl_shader_program objects */
    struct _mesa_HashTable *ShaderObjects;

    /* GL_EXT_framebuffer_object */
    struct _mesa_HashTable *RenderBuffers;
    struct _mesa_HashTable *FrameBuffers;

    /* GL_ARB_sync */
    struct set *SyncObjects;

    /** GL_ARB_sampler_objects */
    struct _mesa_HashTable *SamplerObjects;

    /* GL_ARB_bindless_texture */
    struct hash_table_u64 *TextureHandles;
    struct hash_table_u64 *ImageHandles;
    mtx_t HandlesMutex; /**< For texture/image handles safety */

    /* GL_ARB_shading_language_include */
    struct shader_includes *ShaderIncludes;
    /* glCompileShaderInclude expects ShaderIncludes not to change while it is
     * in progress.
     */
    simple_mtx_t ShaderIncludeMutex;

    /**
     * Some context in this share group was affected by a GPU reset
     *
     * On the next call to \c glGetGraphicsResetStatus, contexts that have not
     * been affected by a GPU reset must also return
     * \c GL_INNOCENT_CONTEXT_RESET_ARB.
     *
     * Once this field becomes true, it is never reset to false.
     */
    bool ShareGroupReset;

    /** EXT_external_objects */
    struct _mesa_HashTable *MemoryObjects;

    /** EXT_semaphore */
    struct _mesa_HashTable *SemaphoreObjects;

    /**
     * Some context in this share group was affected by a disjoint
     * operation. This operation can be anything that has effects on
     * values of timer queries in such manner that they become invalid for
     * performance metrics. As example gpu reset, counter overflow or gpu
     * frequency changes.
     */
    bool DisjointOperation;

    /**
     * Whether at least one image has been imported or exported, excluding
     * the default framebuffer. If this is false, glFlush can be executed
     * asynchronously because there is no invisible dependency on external
     * users.
     */
    bool HasExternallySharedImages;

    /* Small display list storage */
    struct {
        union gl_dlist_node *ptr;
        struct util_idalloc free_idx;
        unsigned size;
    } small_dlist_store;
};

struct gl_texture_object
{
    GLint RefCount;             /**< reference count */
    GLuint Name;                /**< the user-visible texture object ID */
    GLenum16 Target;            /**< GL_TEXTURE_1D, GL_TEXTURE_2D, etc. */
    GLchar *Label;              /**< GL_KHR_debug */

    struct gl_sampler_object Sampler;
    struct gl_texture_object_attrib Attrib;  /**< State saved by glPushAttrib */

    gl_texture_index TargetIndex; /**< The gl_texture_unit::CurrentTex index.
                                      Only valid when Target is valid. */
    GLbyte _MaxLevel;           /**< actual max mipmap level (q in the spec) */
    GLfloat _MaxLambda;         /**< = _MaxLevel - BaseLevel (q - p in spec) */
    GLint CropRect[4];          /**< GL_OES_draw_texture */
    GLboolean _BaseComplete;    /**< Is the base texture level valid? */
    GLboolean _MipmapComplete;  /**< Is the whole mipmap valid? */
    GLboolean _IsIntegerFormat; /**< Does the texture store integer values? */
    GLboolean _RenderToTexture; /**< Any rendering to this texture? */
    GLboolean Purgeable;        /**< Is the buffer purgeable under memory
                                    pressure? */
    GLboolean Immutable;        /**< GL_ARB_texture_storage */
    GLboolean _IsFloat;         /**< GL_OES_float_texture */
    GLboolean _IsHalfFloat;     /**< GL_OES_half_float_texture */
    bool HandleAllocated;       /**< GL_ARB_bindless_texture */

    /* This should not be restored by glPopAttrib: */
    bool StencilSampling;       /**< Should we sample stencil instead of depth? */

    /** GL_OES_EGL_image_external */
    GLboolean External;
    GLubyte RequiredTextureImageUnits;

    /** GL_EXT_memory_object */
    GLenum16 TextureTiling;

    /** GL_ARB_texture_buffer_object */
    GLenum16 BufferObjectFormat;
    /** Equivalent Mesa format for BufferObjectFormat. */
    mesa_format _BufferObjectFormat;
    /* TODO: BufferObject->Name should be restored by glPopAttrib(GL_TEXTURE_BIT); */
    struct gl_buffer_object *BufferObject;

    /** GL_ARB_texture_buffer_range */
    GLintptr BufferOffset;
    GLsizeiptr BufferSize; /**< if this is -1, use BufferObject->Size instead */

    /** Actual texture images, indexed by [cube face] and [mipmap level] */
    struct gl_texture_image *Image[MAX_FACES][MAX_TEXTURE_LEVELS];

    /** GL_ARB_bindless_texture */
    struct util_dynarray SamplerHandles;
    struct util_dynarray ImageHandles;

    /** GL_ARB_sparse_texture */
    GLboolean IsSparse;
    GLint VirtualPageSizeIndex;
    GLint NumSparseLevels;

    /* The texture must include at levels [0..lastLevel] once validated:
     */
    GLuint lastLevel;

    unsigned int validated_first_level;
    unsigned int validated_last_level;

    /* On validation any active images held in main memory or in other
     * textures will be copied to this texture and the old storage freed.
     */
    struct pipe_resource *pt;

    /* Protect modifications of the sampler_views array */
    simple_mtx_t validate_mutex;

    /* Container of sampler views (one per context) attached to this texture
     * object. Created lazily on first binding in context.
     *
     * Purely read-only accesses to the current context's own sampler view
     * require no locking. Another thread may simultaneously replace the
     * container object in order to grow the array, but the old container will
     * be kept alive.
     *
     * Writing to the container (even for modifying the current context's own
     * sampler view) always requires taking the validate_mutex to protect against
     * concurrent container switches.
     *
     * NULL'ing another context's sampler view is allowed only while
     * implementing an API call that modifies the texture: an application which
     * calls those while simultaneously reading the texture in another context
     * invokes undefined behavior. (TODO: a dubious violation of this rule is
     * st_finalize_texture, which is a lazy operation that corresponds to a
     * texture modification.)
     */
    struct st_sampler_views *sampler_views;

    /* Old sampler views container objects that have not been freed yet because
     * other threads/contexts may still be reading from them.
     */
    struct st_sampler_views *sampler_views_old;

    /* True if this texture comes from the window system. Such a texture
     * cannot be reallocated and the format can only be changed with a sampler
     * view or a surface.
     */
    GLboolean surface_based;

    /* If surface_based is true, this format should be used for all sampler
     * views and surfaces instead of pt->format.
     */
    enum pipe_format surface_format;

    /* When non-negative, samplers should use this level instead of the level
     * range specified by the GL state.
     *
     * This is used for EGL images, which may correspond to a single level out
     * of an imported pipe_resources with multiple mip levels.
     */
    int level_override;

    /* When non-negative, samplers should use this layer instead of the one
     * specified by the GL state.
     *
     * This is used for EGL images and VDPAU interop, where imported
     * pipe_resources may be cube, 3D, or array textures (containing layers
     * with different fields in the case of VDPAU) even though the GL state
     * describes one non-array texture per field.
     */
    int layer_override;

    /**
     * Set when the texture images of this texture object might not all be in
     * the pipe_resource *pt above.
     */
    bool needs_validation;
};



struct st_texture_object
{
    struct gl_texture_object base;       /* The "parent" object */

    /* The texture must include at levels [0..lastLevel] once validated:
     */
    GLuint lastLevel;

    unsigned int validated_first_level;
    unsigned int validated_last_level;

    /* On validation any active images held in main memory or in other
     * textures will be copied to this texture and the old storage freed.
     */
    struct pipe_resource *pt;

    /* Protect modifications of the sampler_views array */
    simple_mtx_t validate_mutex;

    /* Container of sampler views (one per context) attached to this texture
     * object. Created lazily on first binding in context.
     *
     * Purely read-only accesses to the current context's own sampler view
     * require no locking. Another thread may simultaneously replace the
     * container object in order to grow the array, but the old container will
     * be kept alive.
     *
     * Writing to the container (even for modifying the current context's own
     * sampler view) always requires taking the validate_mutex to protect against
     * concurrent container switches.
     *
     * NULL'ing another context's sampler view is allowed only while
     * implementing an API call that modifies the texture: an application which
     * calls those while simultaneously reading the texture in another context
     * invokes undefined behavior. (TODO: a dubious violation of this rule is
     * st_finalize_texture, which is a lazy operation that corresponds to a
     * texture modification.)
     */
    struct st_sampler_views *sampler_views;

    /* Old sampler views container objects that have not been freed yet because
     * other threads/contexts may still be reading from them.
     */
    struct st_sampler_views *sampler_views_old;

    /* True if this texture comes from the window system. Such a texture
     * cannot be reallocated and the format can only be changed with a sampler
     * view or a surface.
     */
    GLboolean surface_based;

    /* If surface_based is true, this format should be used for all sampler
     * views and surfaces instead of pt->format.
     */
    enum pipe_format surface_format;

    /* When non-negative, samplers should use this level instead of the level
     * range specified by the GL state.
     *
     * This is used for EGL images, which may correspond to a single level out
     * of an imported pipe_resources with multiple mip levels.
     */
    int level_override;

    /* When non-negative, samplers should use this layer instead of the one
     * specified by the GL state.
     *
     * This is used for EGL images and VDPAU interop, where imported
     * pipe_resources may be cube, 3D, or array textures (containing layers
     * with different fields in the case of VDPAU) even though the GL state
     * describes one non-array texture per field.
     */
    int layer_override;

    /**
     * Set when the texture images of this texture object might not all be in
     * the pipe_resource *pt above.
     */
    bool needs_validation;
};

enum _debug
{
    DEBUG_SILENT                 = (1 << 0),
    DEBUG_ALWAYS_FLUSH		= (1 << 1),
    DEBUG_INCOMPLETE_TEXTURE     = (1 << 2),
    DEBUG_INCOMPLETE_FBO         = (1 << 3),
    DEBUG_CONTEXT                = (1 << 4)
};

#define MAX_TEXTURE_LEVELS 15

struct gl_texture_image
{
    GLint InternalFormat;	/**< Internal format as given by the user */
    GLenum16 _BaseFormat;	/**< Either GL_RGB, GL_RGBA, GL_ALPHA,
                                 *   GL_LUMINANCE, GL_LUMINANCE_ALPHA,
                                 *   GL_INTENSITY, GL_DEPTH_COMPONENT or
                                 *   GL_DEPTH_STENCIL_EXT only. Used for
                                 *   choosing TexEnv arithmetic.
                                 */
    mesa_format TexFormat;         /**< The actual texture memory format */

    GLuint Border;		/**< 0 or 1 */
    GLuint Width;		/**< = 2^WidthLog2 + 2*Border */
    GLuint Height;		/**< = 2^HeightLog2 + 2*Border */
    GLuint Depth;		/**< = 2^DepthLog2 + 2*Border */
    GLuint Width2;		/**< = Width - 2*Border */
    GLuint Height2;		/**< = Height - 2*Border */
    GLuint Depth2;		/**< = Depth - 2*Border */
    GLuint WidthLog2;		/**< = log2(Width2) */
    GLuint HeightLog2;		/**< = log2(Height2) */
    GLuint DepthLog2;		/**< = log2(Depth2) */
    GLuint MaxNumLevels;		/**< = maximum possible number of mipmap
                                       levels, computed from the dimensions */

    struct gl_texture_object *TexObject;  /**< Pointer back to parent object */
    GLuint Level;                /**< Which mipmap level am I? */
    /** Cube map face: index into gl_texture_object::Image[] array */
    GLuint Face;

    /** GL_ARB_texture_multisample */
    GLuint NumSamples;            /**< Sample count, or 0 for non-multisample */
    GLboolean FixedSampleLocations; /**< Same sample locations for all pixels? */
};



struct mesa_extension {
    /** Name of extension, such as "GL_ARB_depth_clamp". */
    const char *name;

    /** Offset (in bytes) of the corresponding member in struct gl_extensions. */
    size_t offset;

    /** Minimum version the extension requires for the given API
     * (see gl_api defined in mtypes.h). The value is equal to:
     * 10 * major_version + minor_version
     */
    uint8_t version[API_OPENGL_LAST + 1];

    /** Year the extension was proposed or approved.  Used to sort the
     * extension string chronologically. */
    uint16_t year;
};

extern const struct mesa_extension _mesa_extension_table[];

/* Generate enums for the functions below */
enum {
#define EXT(name_str, ...) MESA_EXTENSION_##name_str,
#include "extensions_table.h"
#undef EXT
    MESA_EXTENSION_COUNT
};


/** Checks if the context supports a user-facing extension */
#define EXT(name_str, driver_cap, ...) \
static inline bool \
_mesa_has_##name_str(const struct gl_context *ctx) \
{ \
   return ctx->Extensions.driver_cap && (ctx->Extensions.Version >= \
          _mesa_extension_table[MESA_EXTENSION_##name_str].version[ctx->API]); \
}
#include "extensions_table.h"
#undef EXT

#define ST_FLUSH_FRONT                    (1 << 0)
#define ST_FLUSH_END_OF_FRAME             (1 << 1)
#define ST_FLUSH_WAIT                     (1 << 2)
#define ST_FLUSH_FENCE_FD                 (1 << 3)

struct st_framebuffer_iface
{
    /**
     * Atomic stamp which changes when framebuffers need to be updated.
     */
    int32_t stamp;

    /**
     * Identifier that uniquely identifies the framebuffer interface object.
     */
    uint32_t ID;

    /**
     * The frontend manager that manages this object.
     */
    struct st_manager *state_manager;

    /**
     * Available for the frontend manager to use.
     */
    void *st_manager_private;

    /**
     * The visual of a framebuffer.
     */
    const struct st_visual *visual;

    /**
     * Flush the front buffer.
     *
     * On some window systems, changes to the front buffers are not immediately
     * visible.  They need to be flushed.
     *
     * @att is one of the front buffer attachments.
     */
    bool (*flush_front)(struct st_context_iface *stctx,
                        struct st_framebuffer_iface *stfbi,
                        enum st_attachment_type statt);

    /**
     * the gallium frontend asks for the textures it needs.
     *
     * It should try to only ask for attachments that it currently renders
     * to, thus allowing the winsys to delay the allocation of textures not
     * needed. For example front buffer attachments are not needed if you
     * only do back buffer rendering.
     *
     * The implementor of this function needs to also ensure
     * thread safty as this call might be done from multiple threads.
     *
     * The returned textures are owned by the caller.  They should be
     * unreferenced when no longer used.  If this function is called multiple
     * times with different sets of attachments, those buffers not included in
     * the last call might be destroyed.  This behavior might change in the
     * future.
     */
    bool (*validate)(struct st_context_iface *stctx,
                     struct st_framebuffer_iface *stfbi,
                     const enum st_attachment_type *statts,
                     unsigned count,
                     struct pipe_resource **out);
    bool (*flush_swapbuffers) (struct st_context_iface *stctx,
                               struct st_framebuffer_iface *stfbi);
};

#define GET_CURRENT_CONTEXT(C)  struct gl_context *C = (struct gl_context *) \
     (likely(_glapi_Context) ? _glapi_Context : _glapi_get_context())

#ifdef USE_ELF_TLS
#define u_current_table _glapi_tls_Dispatch
#define u_current_context _glapi_tls_Context
#else
#define u_current_table _glapi_Dispatch
#define u_current_context _glapi_Context
#endif

enum pipe_capf
{
    PIPE_CAPF_MIN_LINE_WIDTH,
    PIPE_CAPF_MIN_LINE_WIDTH_AA,
    PIPE_CAPF_MAX_LINE_WIDTH,
    PIPE_CAPF_MAX_LINE_WIDTH_AA,
    PIPE_CAPF_LINE_WIDTH_GRANULARITY,
    PIPE_CAPF_MIN_POINT_SIZE,
    PIPE_CAPF_MIN_POINT_SIZE_AA,
    PIPE_CAPF_MAX_POINT_SIZE,
    PIPE_CAPF_MAX_POINT_SIZE_AA,
    PIPE_CAPF_POINT_SIZE_GRANULARITY,
    PIPE_CAPF_MAX_TEXTURE_ANISOTROPY,
    PIPE_CAPF_MAX_TEXTURE_LOD_BIAS,
    PIPE_CAPF_MIN_CONSERVATIVE_RASTER_DILATE,
    PIPE_CAPF_MAX_CONSERVATIVE_RASTER_DILATE,
    PIPE_CAPF_CONSERVATIVE_RASTER_DILATE_GRANULARITY,
};

typedef struct nir_shader_compiler_options {
    bool lower_fdiv;
    bool lower_ffma16;
    bool lower_ffma32;
    bool lower_ffma64;
    bool fuse_ffma16;
    bool fuse_ffma32;
    bool fuse_ffma64;
    bool lower_flrp16;
    bool lower_flrp32;
    /** Lowers flrp when it does not support doubles */
    bool lower_flrp64;
    bool lower_fpow;
    bool lower_fsat;
    bool lower_fsqrt;
    bool lower_sincos;
    bool lower_fmod;
    /** Lowers ibitfield_extract/ubitfield_extract to ibfe/ubfe. */
    bool lower_bitfield_extract;
    /** Lowers ibitfield_extract/ubitfield_extract to compares, shifts. */
    bool lower_bitfield_extract_to_shifts;
    /** Lowers bitfield_insert to bfi/bfm */
    bool lower_bitfield_insert;
    /** Lowers bitfield_insert to compares, and shifts. */
    bool lower_bitfield_insert_to_shifts;
    /** Lowers bitfield_insert to bfm/bitfield_select. */
    bool lower_bitfield_insert_to_bitfield_select;
    /** Lowers bitfield_reverse to shifts. */
    bool lower_bitfield_reverse;
    /** Lowers bit_count to shifts. */
    bool lower_bit_count;
    /** Lowers ifind_msb to compare and ufind_msb */
    bool lower_ifind_msb;
    /** Lowers ifind_msb and ufind_msb to reverse variants */
    bool lower_find_msb_to_reverse;
    /** Lowers find_lsb to ufind_msb and logic ops */
    bool lower_find_lsb;
    bool lower_uadd_carry;
    bool lower_usub_borrow;
    /** Lowers imul_high/umul_high to 16-bit multiplies and carry operations. */
    bool lower_mul_high;
    /** lowers fneg to fmul(x, -1.0). Driver must call nir_opt_algebraic_late() */
    bool lower_fneg;
    /** lowers ineg to isub. Driver must call nir_opt_algebraic_late(). */
    bool lower_ineg;

    /* lower {slt,sge,seq,sne} to {flt,fge,feq,fneu} + b2f: */
    bool lower_scmp;

    /* lower b/fall_equalN/b/fany_nequalN (ex:fany_nequal4 to sne+fdot4+fsat) */
    bool lower_vector_cmp;

    /** enable rules to avoid bit ops */
    bool lower_bitops;

    /** enables rules to lower isign to imin+imax */
    bool lower_isign;

    /** enables rules to lower fsign to fsub and flt */
    bool lower_fsign;

    /** enables rules to lower iabs to ineg+imax */
    bool lower_iabs;

    /** enable rules that avoid generating umax from signed integer ops */
    bool lower_umax;

    /** enable rules that avoid generating umin from signed integer ops */
    bool lower_umin;

    /* lower fdph to fdot4 */
    bool lower_fdph;

    /** lower fdot to fmul and fsum/fadd. */
    bool lower_fdot;

    /* Does the native fdot instruction replicate its result for four
     * components?  If so, then opt_algebraic_late will turn all fdotN
     * instructions into fdotN_replicated instructions.
     */
    bool fdot_replicates;

    /** lowers ffloor to fsub+ffract: */
    bool lower_ffloor;

    /** lowers ffract to fsub+ffloor: */
    bool lower_ffract;

    /** lowers fceil to fneg+ffloor+fneg: */
    bool lower_fceil;

    bool lower_ftrunc;

    bool lower_ldexp;

    bool lower_pack_half_2x16;
    bool lower_pack_unorm_2x16;
    bool lower_pack_snorm_2x16;
    bool lower_pack_unorm_4x8;
    bool lower_pack_snorm_4x8;
    bool lower_pack_64_2x32;
    bool lower_pack_64_4x16;
    bool lower_pack_32_2x16;
    bool lower_pack_64_2x32_split;
    bool lower_pack_32_2x16_split;
    bool lower_unpack_half_2x16;
    bool lower_unpack_unorm_2x16;
    bool lower_unpack_snorm_2x16;
    bool lower_unpack_unorm_4x8;
    bool lower_unpack_snorm_4x8;
    bool lower_unpack_64_2x32_split;
    bool lower_unpack_32_2x16_split;

    bool lower_pack_split;

    bool lower_extract_byte;
    bool lower_extract_word;

    bool lower_all_io_to_temps;
    bool lower_all_io_to_elements;

    /* Indicates that the driver only has zero-based vertex id */
    bool vertex_id_zero_based;

    /**
     * If enabled, gl_BaseVertex will be lowered as:
     * is_indexed_draw (~0/0) & firstvertex
     */
    bool lower_base_vertex;

    /**
     * If enabled, gl_HelperInvocation will be lowered as:
     *
     *   !((1 << sample_id) & sample_mask_in))
     *
     * This depends on some possibly hw implementation details, which may
     * not be true for all hw.  In particular that the FS is only executed
     * for covered samples or for helper invocations.  So, do not blindly
     * enable this option.
     *
     * Note: See also issue #22 in ARB_shader_image_load_store
     */
    bool lower_helper_invocation;

    /**
     * Convert gl_SampleMaskIn to gl_HelperInvocation as follows:
     *
     *   gl_SampleMaskIn == 0 ---> gl_HelperInvocation
     *   gl_SampleMaskIn != 0 ---> !gl_HelperInvocation
     */
    bool optimize_sample_mask_in;

    bool lower_cs_local_index_from_id;
    bool lower_cs_local_id_from_index;

    /* Prevents lowering global_invocation_id to be in terms of work_group_id */
    bool has_cs_global_id;

    bool lower_device_index_to_zero;

    /* Set if nir_lower_pntc_ytransform() should invert gl_PointCoord.
     * Either when frame buffer is flipped or GL_POINT_SPRITE_COORD_ORIGIN
     * is GL_LOWER_LEFT.
     */
    bool lower_wpos_pntc;

    /**
     * Set if nir_op_[iu]hadd and nir_op_[iu]rhadd instructions should be
     * lowered to simple arithmetic.
     *
     * If this flag is set, the lowering will be applied to all bit-sizes of
     * these instructions.
     *
     * \sa ::lower_hadd64
     */
    bool lower_hadd;

    /**
     * Set if only 64-bit nir_op_[iu]hadd and nir_op_[iu]rhadd instructions
     * should be lowered to simple arithmetic.
     *
     * If this flag is set, the lowering will be applied to only 64-bit
     * versions of these instructions.
     *
     * \sa ::lower_hadd
     */
    bool lower_hadd64;

    /**
     * Set if nir_op_add_sat and nir_op_usub_sat should be lowered to simple
     * arithmetic.
     *
     * If this flag is set, the lowering will be applied to all bit-sizes of
     * these instructions.
     *
     * \sa ::lower_usub_sat64
     */
    bool lower_add_sat;

    /**
     * Set if only 64-bit nir_op_usub_sat should be lowered to simple
     * arithmetic.
     *
     * \sa ::lower_add_sat
     */
    bool lower_usub_sat64;

    /**
     * Should IO be re-vectorized?  Some scalar ISAs still operate on vec4's
     * for IO purposes and would prefer loads/stores be vectorized.
     */
    bool vectorize_io;
    bool lower_to_scalar;

    /**
     * Whether nir_opt_vectorize should only create 16-bit 2D vectors.
     */
    bool vectorize_vec2_16bit;

    /**
     * Should the linker unify inputs_read/outputs_written between adjacent
     * shader stages which are linked into a single program?
     */
    bool unify_interfaces;

    /**
     * Should nir_lower_io() create load_interpolated_input intrinsics?
     *
     * If not, it generates regular load_input intrinsics and interpolation
     * information must be inferred from the list of input nir_variables.
     */
    bool use_interpolated_input_intrinsics;


    /**
     * Whether nir_lower_io() will lower interpolateAt functions to
     * load_interpolated_input intrinsics.
     *
     * Unlike use_interpolated_input_intrinsics this will only lower these
     * functions and leave input load intrinsics untouched.
     */
    bool lower_interpolate_at;

    /* Lowers when 32x32->64 bit multiplication is not supported */
    bool lower_mul_2x32_64;

    /* Lowers when rotate instruction is not supported */
    bool lower_rotate;

    /**
     * Backend supports imul24, and would like to use it (when possible)
     * for address/offset calculation.  If true, driver should call
     * nir_lower_amul().  (If not set, amul will automatically be lowered
     * to imul.)
     */
    bool has_imul24;

    /** Backend supports umul24, if not set  umul24 will automatically be lowered
     * to imul with masked inputs */
    bool has_umul24;

    /** Backend supports umad24, if not set  umad24 will automatically be lowered
     * to imul with masked inputs and iadd */
    bool has_umad24;

    /* Backend supports fused comapre against zero and csel */
    bool has_fused_comp_and_csel;

    /** Backend supports fsub, if not set fsub will automatically be lowered to
     * fadd(x, fneg(y)). If true, driver should call nir_opt_algebraic_late(). */
    bool has_fsub;

    /** Backend supports isub, if not set isub will automatically be lowered to
     * iadd(x, ineg(y)). If true, driver should call nir_opt_algebraic_late(). */
    bool has_isub;

    /** Backend supports txs, if not nir_lower_tex(..) uses txs-free variants
     * for rect texture lowering. */
    bool has_txs;

    /* Whether to generate only scoped_barrier intrinsics instead of the set of
     * memory and control barrier intrinsics based on GLSL.
     */
    bool use_scoped_barrier;

    /**
     * Is this the Intel vec4 backend?
     *
     * Used to inhibit algebraic optimizations that are known to be harmful on
     * the Intel vec4 backend.  This is generally applicable to any
     * optimization that might cause more immediate values to be used in
     * 3-source (e.g., ffma and flrp) instructions.
     */
    bool intel_vec4;

    /** Lower nir_op_ibfe and nir_op_ubfe that have two constant sources. */
    bool lower_bfe_with_two_constants;

    /** Whether 8-bit ALU is supported. */
    bool support_8bit_alu;

    /** Whether 16-bit ALU is supported. */
    bool support_16bit_alu;

    unsigned max_unroll_iterations;
    unsigned max_unroll_iterations_aggressive;

    bool lower_uniforms_to_ubo;

    nir_lower_int64_options lower_int64_options;
    nir_lower_doubles_options lower_doubles_options;
    nir_divergence_options divergence_analysis_options;
} nir_shader_compiler_options;

typedef enum
{
    VARYING_SLOT_POS,
    VARYING_SLOT_COL0, /* COL0 and COL1 must be contiguous */
    VARYING_SLOT_COL1,
    VARYING_SLOT_FOGC,
    VARYING_SLOT_TEX0, /* TEX0-TEX7 must be contiguous */
    VARYING_SLOT_TEX1,
    VARYING_SLOT_TEX2,
    VARYING_SLOT_TEX3,
    VARYING_SLOT_TEX4,
    VARYING_SLOT_TEX5,
    VARYING_SLOT_TEX6,
    VARYING_SLOT_TEX7,
    VARYING_SLOT_PSIZ, /* Does not appear in FS */
    VARYING_SLOT_BFC0, /* Does not appear in FS */
    VARYING_SLOT_BFC1, /* Does not appear in FS */
    VARYING_SLOT_EDGE, /* Does not appear in FS */
    VARYING_SLOT_CLIP_VERTEX, /* Does not appear in FS */
    VARYING_SLOT_CLIP_DIST0,
    VARYING_SLOT_CLIP_DIST1,
    VARYING_SLOT_CULL_DIST0,
    VARYING_SLOT_CULL_DIST1,
    VARYING_SLOT_PRIMITIVE_ID, /* Does not appear in VS */
    VARYING_SLOT_LAYER, /* Appears as VS or GS output */
    VARYING_SLOT_VIEWPORT, /* Appears as VS or GS output */
    VARYING_SLOT_FACE, /* FS only */
    VARYING_SLOT_PNTC, /* FS only */
    VARYING_SLOT_TESS_LEVEL_OUTER, /* Only appears as TCS output. */
    VARYING_SLOT_TESS_LEVEL_INNER, /* Only appears as TCS output. */
    VARYING_SLOT_BOUNDING_BOX0, /* Only appears as TCS output. */
    VARYING_SLOT_BOUNDING_BOX1, /* Only appears as TCS output. */
    VARYING_SLOT_VIEW_INDEX,
    VARYING_SLOT_VIEWPORT_MASK, /* Does not appear in FS */
    VARYING_SLOT_PRIMITIVE_SHADING_RATE = VARYING_SLOT_FACE, /* Does not appear in FS. */
    VARYING_SLOT_VAR0 = 32, /* First generic varying slot */
    /* the remaining are simply for the benefit of gl_varying_slot_name()
     * and not to be construed as an upper bound:
     */
    VARYING_SLOT_VAR1,
    VARYING_SLOT_VAR2,
    VARYING_SLOT_VAR3,
    VARYING_SLOT_VAR4,
    VARYING_SLOT_VAR5,
    VARYING_SLOT_VAR6,
    VARYING_SLOT_VAR7,
    VARYING_SLOT_VAR8,
    VARYING_SLOT_VAR9,
    VARYING_SLOT_VAR10,
    VARYING_SLOT_VAR11,
    VARYING_SLOT_VAR12,
    VARYING_SLOT_VAR13,
    VARYING_SLOT_VAR14,
    VARYING_SLOT_VAR15,
    VARYING_SLOT_VAR16,
    VARYING_SLOT_VAR17,
    VARYING_SLOT_VAR18,
    VARYING_SLOT_VAR19,
    VARYING_SLOT_VAR20,
    VARYING_SLOT_VAR21,
    VARYING_SLOT_VAR22,
    VARYING_SLOT_VAR23,
    VARYING_SLOT_VAR24,
    VARYING_SLOT_VAR25,
    VARYING_SLOT_VAR26,
    VARYING_SLOT_VAR27,
    VARYING_SLOT_VAR28,
    VARYING_SLOT_VAR29,
    VARYING_SLOT_VAR30,
    VARYING_SLOT_VAR31,
    /* Per-patch varyings for tessellation. */
    VARYING_SLOT_PATCH0,
    VARYING_SLOT_PATCH1,
    VARYING_SLOT_PATCH2,
    VARYING_SLOT_PATCH3,
    VARYING_SLOT_PATCH4,
    VARYING_SLOT_PATCH5,
    VARYING_SLOT_PATCH6,
    VARYING_SLOT_PATCH7,
    VARYING_SLOT_PATCH8,
    VARYING_SLOT_PATCH9,
    VARYING_SLOT_PATCH10,
    VARYING_SLOT_PATCH11,
    VARYING_SLOT_PATCH12,
    VARYING_SLOT_PATCH13,
    VARYING_SLOT_PATCH14,
    VARYING_SLOT_PATCH15,
    VARYING_SLOT_PATCH16,
    VARYING_SLOT_PATCH17,
    VARYING_SLOT_PATCH18,
    VARYING_SLOT_PATCH19,
    VARYING_SLOT_PATCH20,
    VARYING_SLOT_PATCH21,
    VARYING_SLOT_PATCH22,
    VARYING_SLOT_PATCH23,
    VARYING_SLOT_PATCH24,
    VARYING_SLOT_PATCH25,
    VARYING_SLOT_PATCH26,
    VARYING_SLOT_PATCH27,
    VARYING_SLOT_PATCH28,
    VARYING_SLOT_PATCH29,
    VARYING_SLOT_PATCH30,
    VARYING_SLOT_PATCH31,
    /* 32 16-bit vec4 slots packed in 16 32-bit vec4 slots for GLES/mediump.
     * They are really just additional generic slots used for 16-bit data to
     * prevent conflicts between neighboring mediump and non-mediump varyings
     * that can't be packed without breaking one or the other, which is
     * a limitation of separate shaders. This allows linking shaders in 32 bits
     * and then get an optimally packed 16-bit varyings by remapping the IO
     * locations to these slots. The remapping can also be undone trivially.
     *
     * nir_io_semantics::high_16bit determines which half of the slot is
     * accessed. The low and high halves share the same IO "base" number.
     * Drivers can treat these as 32-bit slots everywhere except for FP16
     * interpolation.
     */
    VARYING_SLOT_VAR0_16BIT,
    VARYING_SLOT_VAR1_16BIT,
    VARYING_SLOT_VAR2_16BIT,
    VARYING_SLOT_VAR3_16BIT,
    VARYING_SLOT_VAR4_16BIT,
    VARYING_SLOT_VAR5_16BIT,
    VARYING_SLOT_VAR6_16BIT,
    VARYING_SLOT_VAR7_16BIT,
    VARYING_SLOT_VAR8_16BIT,
    VARYING_SLOT_VAR9_16BIT,
    VARYING_SLOT_VAR10_16BIT,
    VARYING_SLOT_VAR11_16BIT,
    VARYING_SLOT_VAR12_16BIT,
    VARYING_SLOT_VAR13_16BIT,
    VARYING_SLOT_VAR14_16BIT,
    VARYING_SLOT_VAR15_16BIT,

    NUM_TOTAL_VARYING_SLOTS,
} gl_varying_slot;

#define BITFIELD64_BIT(b)      (1ull << (b))

#define VARYING_BIT_POS BITFIELD64_BIT(VARYING_SLOT_POS)
#define VARYING_BIT_PSIZ BITFIELD64_BIT(VARYING_SLOT_PSIZ)

#define MAX_VARYING 32

static inline unsigned
fui( float f )
{
    union fi fi;
    fi.f = f;
    return fi.ui;
}

#define BITFIELD_BIT(b)      (1u << (b))

#define DEBUG_MESA            BITFIELD_BIT(0)
#define DEBUG_PRINT_IR        BITFIELD_BIT(1)
#define DEBUG_FALLBACK        BITFIELD_BIT(2)
#define DEBUG_BUFFER          BITFIELD_BIT(3)
#define DEBUG_WIREFRAME       BITFIELD_BIT(4)
#define DEBUG_GREMEDY         BITFIELD_BIT(5)
#define DEBUG_NOREADPIXCACHE  BITFIELD_BIT(6)

typedef enum {
    nir_lower_drcp = (1 << 0),
    nir_lower_dsqrt = (1 << 1),
    nir_lower_drsq = (1 << 2),
    nir_lower_dtrunc = (1 << 3),
    nir_lower_dfloor = (1 << 4),
    nir_lower_dceil = (1 << 5),
    nir_lower_dfract = (1 << 6),
    nir_lower_dround_even = (1 << 7),
    nir_lower_dmod = (1 << 8),
    nir_lower_dsub = (1 << 9),
    nir_lower_ddiv = (1 << 10),
    nir_lower_fp64_full_software = (1 << 11),
} nir_lower_doubles_options;

#define list_container_of(ptr, sample, member)				\
    (void *)((char *)(ptr)						\
	     - ((char *)&(sample)->member - (char *)(sample)))

#define LIST_FOR_EACH_ENTRY(pos, head, member)				\
   for (pos = NULL, pos = list_container_of((head)->next, pos, member);	\
	&pos->member != (head);						\
	pos = list_container_of(pos->member.next, pos, member))

struct gl_framebuffer
{
    simple_mtx_t Mutex;  /**< for thread safety */
    /**
     * If zero, this is a window system framebuffer.  If non-zero, this
     * is a FBO framebuffer; note that for some devices (i.e. those with
     * a natural pixel coordinate system for FBOs that differs from the
     * OpenGL/Mesa coordinate system), this means that the viewport,
     * polygon face orientation, and polygon stipple will have to be inverted.
     */
    GLuint Name;
    GLint RefCount;

    GLchar *Label;       /**< GL_KHR_debug */

    GLboolean DeletePending;

    /**
     * The framebuffer's visual. Immutable if this is a window system buffer.
     * Computed from attachments if user-made FBO.
     */
    struct gl_config Visual;

    /**
     * Size of frame buffer in pixels. If there are no attachments, then both
     * of these are 0.
     */
    GLuint Width, Height;

    /**
     * In the case that the framebuffer has no attachment (i.e.
     * GL_ARB_framebuffer_no_attachments) then the geometry of
     * the framebuffer is specified by the default values.
     */
    struct {
        GLuint Width, Height, Layers, NumSamples;
        GLboolean FixedSampleLocations;
        /* Derived from NumSamples by the driver so that it can choose a valid
         * value for the hardware.
         */
        GLuint _NumSamples;
    } DefaultGeometry;

    /** \name  Drawing bounds (Intersection of buffer size and scissor box)
     * The drawing region is given by [_Xmin, _Xmax) x [_Ymin, _Ymax),
     * (inclusive for _Xmin and _Ymin while exclusive for _Xmax and _Ymax)
     */
    /*@{*/
    GLint _Xmin, _Xmax;
    GLint _Ymin, _Ymax;
    /*@}*/

    /** \name  Derived Z buffer stuff */
    /*@{*/
    GLuint _DepthMax;	/**< Max depth buffer value */
    GLfloat _DepthMaxF;	/**< Float max depth buffer value */
    GLfloat _MRD;	/**< minimum resolvable difference in Z values */
    /*@}*/

    /** One of the GL_FRAMEBUFFER_(IN)COMPLETE_* tokens */
    GLenum16 _Status;

    /** Whether one of Attachment has Type != GL_NONE
     * NOTE: the values for Width and Height are set to 0 in case of having
     * no attachments, a backend driver supporting the extension
     * GL_ARB_framebuffer_no_attachments must check for the flag _HasAttachments
     * and if GL_FALSE, must then use the values in DefaultGeometry to initialize
     * its viewport, scissor and so on (in particular _Xmin, _Xmax, _Ymin and
     * _Ymax do NOT take into account _HasAttachments being false). To get the
     * geometry of the framebuffer, the  helper functions
     *   _mesa_geometric_width(),
     *   _mesa_geometric_height(),
     *   _mesa_geometric_samples() and
     *   _mesa_geometric_layers()
     * are available that check _HasAttachments.
     */
    bool _HasAttachments;

    GLbitfield _IntegerBuffers;  /**< Which color buffers are integer valued */
    GLbitfield _RGBBuffers;  /**< Which color buffers have baseformat == RGB */
    GLbitfield _FP32Buffers; /**< Which color buffers are FP32 */

    /* ARB_color_buffer_float */
    GLboolean _AllColorBuffersFixedPoint; /* no integer, no float */
    GLboolean _HasSNormOrFloatColorBuffer;

    /**
     * The maximum number of layers in the framebuffer, or 0 if the framebuffer
     * is not layered.  For cube maps and cube map arrays, each cube face
     * counts as a layer. As the case for Width, Height a backend driver
     * supporting GL_ARB_framebuffer_no_attachments must use DefaultGeometry
     * in the case that _HasAttachments is false
     */
    GLuint MaxNumLayers;

    /** Array of all renderbuffer attachments, indexed by BUFFER_* tokens. */
    struct gl_renderbuffer_attachment Attachment[BUFFER_COUNT];

    /* In unextended OpenGL these vars are part of the GL_COLOR_BUFFER
     * attribute group and GL_PIXEL attribute group, respectively.
     */
    GLenum16 ColorDrawBuffer[MAX_DRAW_BUFFERS];
    GLenum16 ColorReadBuffer;

    /* GL_ARB_sample_locations */
    GLfloat *SampleLocationTable; /**< If NULL, no table has been specified */
    GLboolean ProgrammableSampleLocations;
    GLboolean SampleLocationPixelGrid;

    /** Computed from ColorDraw/ReadBuffer above */
    GLuint _NumColorDrawBuffers;
    gl_buffer_index _ColorDrawBufferIndexes[MAX_DRAW_BUFFERS];
    gl_buffer_index _ColorReadBufferIndex;
    struct gl_renderbuffer *_ColorDrawBuffers[MAX_DRAW_BUFFERS];
    struct gl_renderbuffer *_ColorReadBuffer;

    /* GL_MESA_framebuffer_flip_y */
    bool FlipY;


    /** Delete this framebuffer */
    void (*Delete)(struct gl_framebuffer *fb);

    struct st_framebuffer_iface *iface;
    enum st_attachment_type statts[ST_ATTACHMENT_COUNT];
    unsigned num_statts;
    int32_t stamp;
    int32_t iface_stamp;
    uint32_t iface_ID;

    /* list of framebuffer objects */
    struct list_head head;
};

typedef enum
{
    /* the four standard color buffers */
    BUFFER_FRONT_LEFT,
    BUFFER_BACK_LEFT,
    BUFFER_FRONT_RIGHT,
    BUFFER_BACK_RIGHT,
    BUFFER_DEPTH,
    BUFFER_STENCIL,
    BUFFER_ACCUM,
    /* generic renderbuffers */
    BUFFER_COLOR0,
    BUFFER_COLOR1,
    BUFFER_COLOR2,
    BUFFER_COLOR3,
    BUFFER_COLOR4,
    BUFFER_COLOR5,
    BUFFER_COLOR6,
    BUFFER_COLOR7,
    BUFFER_COUNT,
    BUFFER_NONE = -1,
} gl_buffer_index;

enum util_format_swizzle {
    UTIL_FORMAT_SWIZZLE_X = 0,
    UTIL_FORMAT_SWIZZLE_Y = 1,
    UTIL_FORMAT_SWIZZLE_Z = 2,
    UTIL_FORMAT_SWIZZLE_W = 3,
    UTIL_FORMAT_SWIZZLE_0 = 4,
    UTIL_FORMAT_SWIZZLE_1 = 5,
    UTIL_FORMAT_SWIZZLE_NONE = 6,
    UTIL_FORMAT_SWIZZLE_MAX = 7  /**< Number of enums counter (must be last) */
};

enum {
    mtx_plain     = 0,
    mtx_try       = 1,
    mtx_timed     = 2,
    mtx_recursive = 4
};

struct pipe_sampler_view
{
    struct pipe_reference reference;
    enum pipe_format format:15;      /**< typed PIPE_FORMAT_x */
    enum pipe_texture_target target:5; /**< PIPE_TEXTURE_x */
    unsigned swizzle_r:3;         /**< PIPE_SWIZZLE_x for red component */
    unsigned swizzle_g:3;         /**< PIPE_SWIZZLE_x for green component */
    unsigned swizzle_b:3;         /**< PIPE_SWIZZLE_x for blue component */
    unsigned swizzle_a:3;         /**< PIPE_SWIZZLE_x for alpha component */
    struct pipe_resource *texture; /**< texture into which this is a view  */
    struct pipe_context *context; /**< context this view belongs to */
    union {
        struct {
            unsigned first_layer:16;  /**< first layer to use for array textures */
            unsigned last_layer:16;   /**< last layer to use for array textures */
            unsigned first_level:8;   /**< first mipmap level to use */
            unsigned last_level:8;    /**< last mipmap level to use */
        } tex;
        struct {
            unsigned offset;   /**< offset in bytes */
            unsigned size;     /**< size of the readable sub-range in bytes */
        } buf;
    } u;
};

#define os_malloc(_size)  malloc(_size)
#define MALLOC(_size)  os_malloc(_size)
#define MALLOC_STRUCT(T)   (struct T *) MALLOC(sizeof(struct T))

struct gl_texture_handle_object
{
    struct gl_texture_object *texObj;
    struct gl_sampler_object *sampObj;
    GLuint64 handle;
};

struct gl_sampler_object
{
    simple_mtx_t Mutex;
    GLuint Name;
    GLchar *Label;               /**< GL_KHR_debug */
    GLint RefCount;

    struct gl_sampler_attrib Attrib;  /**< State saved by glPushAttrib */

    /** GL_ARB_bindless_texture */
    bool HandleAllocated;
    struct util_dynarray Handles;
};

struct hash_table_u64 {
    struct hash_table *table;
    void *freed_key_data;
    void *deleted_key_data;
};

struct gl_image_handle_object
{
    struct gl_image_unit imgObj;
    GLuint64 handle;
};

struct gl_buffer_object
{
    GLint RefCount;
    GLuint Name;
    GLchar *Label;       /**< GL_KHR_debug */

    /**
     * The context that holds a global buffer reference for the lifetime of
     * the GL buffer ID to skip refcounting for all its private bind points.
     * Other contexts must still do refcounting as usual. Shared binding points
     * like TBO within gl_texture_object are always refcounted.
     *
     * Implementation details:
     * - Only the context that creates the buffer ("creating context") skips
     *   refcounting.
     * - Only buffers represented by an OpenGL buffer ID skip refcounting.
     *   Other internal buffers don't. (glthread requires refcounting for
     *   internal buffers, etc.)
     * - glDeleteBuffers removes the global buffer reference and increments
     *   RefCount for all private bind points where the deleted buffer is bound
     *   (e.g. unbound VAOs that are not changed by glDeleteBuffers),
     *   effectively enabling refcounting for that context. This is the main
     *   point where the global buffer reference is removed.
     * - glDeleteBuffers called from a different context adds the buffer into
     *   the ZombieBufferObjects list, which is a way to notify the creating
     *   context that it should remove its global buffer reference to allow
     *   freeing the buffer. The creating context walks over that list in a few
     *   GL functions.
     * - xxxDestroyContext walks over all buffers and removes its global
     *   reference from those buffers that it created.
     */
    struct gl_context *Ctx;
    GLint CtxRefCount;   /**< Non-atomic references held by Ctx. */

    GLenum16 Usage;      /**< GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, etc. */
    GLbitfield StorageFlags; /**< GL_MAP_PERSISTENT_BIT, etc. */
    GLsizeiptrARB Size;  /**< Size of buffer storage in bytes */
    GLubyte *Data;       /**< Location of storage either in RAM or VRAM. */
    GLboolean DeletePending;   /**< true if buffer object is removed from the hash */
    GLboolean Written;   /**< Ever written to? (for debugging) */
    GLboolean Purgeable; /**< Is the buffer purgeable under memory pressure? */
    GLboolean Immutable; /**< GL_ARB_buffer_storage */
    gl_buffer_usage UsageHistory; /**< How has this buffer been used so far? */

    /** Counters used for buffer usage warnings */
    GLuint NumSubDataCalls;
    GLuint NumMapBufferWriteCalls;

    struct gl_buffer_mapping Mappings[MAP_COUNT];

    /** Memoization of min/max index computations for static index buffers */
    simple_mtx_t MinMaxCacheMutex;
    struct hash_table *MinMaxCache;
    unsigned MinMaxCacheHitIndices;
    unsigned MinMaxCacheMissIndices;
    bool MinMaxCacheDirty;

    bool HandleAllocated; /**< GL_ARB_bindless_texture */
};

struct pipe_surface
{
    struct pipe_reference reference;
    enum pipe_format format:16;
    unsigned writable:1;          /**< writable shader resource */
    struct pipe_resource *texture; /**< resource into which this is a view  */
    struct pipe_context *context; /**< context this surface belongs to */

    /* XXX width/height should be removed */
    uint16_t width;               /**< logical width in pixels */
    uint16_t height;              /**< logical height in pixels */

    /**
     * Number of samples for the surface.  This will be 0 if rendering
     * should use the resource's nr_samples, or another value if the resource
     * is bound using FramebufferTexture2DMultisampleEXT.
     */
    unsigned nr_samples:8;

    union pipe_surface_desc u;
};

#define _NEW_MODELVIEW         (1u << 0)   /**< gl_context::ModelView */
#define _NEW_PROJECTION        (1u << 1)   /**< gl_context::Projection */
#define _NEW_TEXTURE_MATRIX    (1u << 2)   /**< gl_context::TextureMatrix */
#define _NEW_COLOR             (1u << 3)   /**< gl_context::Color */
#define _NEW_DEPTH             (1u << 4)   /**< gl_context::Depth */
#define _NEW_TNL_SPACES        (1u << 5)  /**< _mesa_update_tnl_spaces */
#define _NEW_FOG               (1u << 6)   /**< gl_context::Fog */
#define _NEW_HINT              (1u << 7)   /**< gl_context::Hint */
#define _NEW_LIGHT_CONSTANTS   (1u << 8)   /**< gl_context::Light */
#define _NEW_LINE              (1u << 9)   /**< gl_context::Line */
#define _NEW_PIXEL             (1u << 10)  /**< gl_context::Pixel */
#define _NEW_POINT             (1u << 11)  /**< gl_context::Point */
#define _NEW_POLYGON           (1u << 12)  /**< gl_context::Polygon */
#define _NEW_POLYGONSTIPPLE    (1u << 13)  /**< gl_context::PolygonStipple */
#define _NEW_SCISSOR           (1u << 14)  /**< gl_context::Scissor */
#define _NEW_STENCIL           (1u << 15)  /**< gl_context::Stencil */
#define _NEW_TEXTURE_OBJECT    (1u << 16)  /**< gl_context::Texture (bindings only) */
#define _NEW_TRANSFORM         (1u << 17)  /**< gl_context::Transform */
#define _NEW_VIEWPORT          (1u << 18)  /**< gl_context::Viewport */
#define _NEW_TEXTURE_STATE     (1u << 19)  /**< gl_context::Texture (states only) */
#define _NEW_LIGHT_STATE       (1u << 20)  /**< gl_context::Light */
#define _NEW_RENDERMODE        (1u << 21)  /**< gl_context::RenderMode, etc */
#define _NEW_BUFFERS           (1u << 22)  /**< gl_context::Visual, DrawBuffer, */
#define _NEW_CURRENT_ATTRIB    (1u << 23)  /**< gl_context::Current */
#define _NEW_MULTISAMPLE       (1u << 24)  /**< gl_context::Multisample */
#define _NEW_TRACK_MATRIX      (1u << 25)  /**< gl_context::VertexProgram */
#define _NEW_PROGRAM           (1u << 26)  /**< New program/shader state */
#define _NEW_PROGRAM_CONSTANTS (1u << 27)
#define _NEW_FF_VERT_PROGRAM   (1u << 28)
#define _NEW_FRAG_CLAMP        (1u << 29)
#define _NEW_MATERIAL          (1u << 30)  /**< gl_context::Light.Material */
#define _NEW_FF_FRAG_PROGRAM   (1u << 31)
#define _NEW_ALL ~0

#ifndef MESA_VERBOSE
int MESA_VERBOSE = 0;
#endif

#ifndef MESA_DEBUG_FLAGS
int MESA_DEBUG_FLAGS = 0;
#endif

enum _verbose
{
    VERBOSE_VARRAY		= 0x0001,
    VERBOSE_TEXTURE		= 0x0002,
    VERBOSE_MATERIAL		= 0x0004,
    VERBOSE_PIPELINE		= 0x0008,
    VERBOSE_DRIVER		= 0x0010,
    VERBOSE_STATE		= 0x0020,
    VERBOSE_API			= 0x0040,
    VERBOSE_DISPLAY_LIST		= 0x0100,
    VERBOSE_LIGHTING		= 0x0200,
    VERBOSE_PRIMS		= 0x0400,
    VERBOSE_VERTS		= 0x0800,
    VERBOSE_DISASSEM		= 0x1000,
    VERBOSE_SWAPBUFFERS          = 0x4000
};

#define FLUSH_VERTICES(ctx, newstate, pop_attrib_mask)          \
do {								\
   if (MESA_VERBOSE & VERBOSE_STATE)				\
      _mesa_debug(ctx, "FLUSH_VERTICES in %s\n", __func__);	\
   if (ctx->Driver.NeedFlush & FLUSH_STORED_VERTICES)		\
      vbo_exec_FlushVertices(ctx, FLUSH_STORED_VERTICES);	\
   ctx->NewState |= newstate;					\
   ctx->PopAttribState |= pop_attrib_mask;                      \
} while (0)

struct vbo_context {
    struct gl_vertex_buffer_binding binding;
    struct gl_array_attributes current[VBO_ATTRIB_MAX];

    struct gl_vertex_array_object *VAO;

    struct vbo_exec_context exec;
    struct vbo_save_context save;
};

static inline struct vbo_context *
vbo_context(struct gl_context *ctx)
{
    return &ctx->vbo_context;
}

struct vbo_exec_context
{
    GLvertexformat vtxfmt;
    GLvertexformat vtxfmt_noop;

    struct {
        /* Multi draw where the mode can vary between draws. */
        struct pipe_draw_info info;
        struct pipe_draw_start_count draw[VBO_MAX_PRIM];
        GLubyte mode[VBO_MAX_PRIM];            /**< primitive modes per draw */
        struct vbo_markers markers[VBO_MAX_PRIM];
        unsigned prim_count;

        struct gl_buffer_object *bufferobj;

        GLuint vertex_size;       /* in dwords */
        GLuint vertex_size_no_pos;

        fi_type *buffer_map;
        fi_type *buffer_ptr;              /* cursor, points into buffer */
        GLuint   buffer_used;             /* in bytes */
        unsigned buffer_offset;           /* only for persistent mappings */
        fi_type vertex[VBO_ATTRIB_MAX*4]; /* current vertex */

        GLuint vert_count;   /**< Number of vertices currently in buffer */
        GLuint max_vert;     /**< Max number of vertices allowed in buffer */
        struct vbo_exec_copied_vtx copied;

        GLbitfield64 enabled;             /**< mask of enabled vbo arrays. */

        /* Keep these packed in a structure for faster access. */
        struct {
            GLenum16 type;       /**< GL_FLOAT, GL_DOUBLE, GL_INT, etc */
            GLubyte active_size; /**< number of components, but can shrink */
            GLubyte size;        /**< number of components (1..4) */
        } attr[VBO_ATTRIB_MAX];

        /** pointers into the current 'vertex' array, declared above */
        fi_type *attrptr[VBO_ATTRIB_MAX];
    } vtx;

    struct {
        GLboolean recalculate_maps;
        struct vbo_exec_eval1_map map1[VERT_ATTRIB_MAX];
        struct vbo_exec_eval2_map map2[VERT_ATTRIB_MAX];
    } eval;

#ifndef NDEBUG
    GLint flush_call_depth;
#endif
};

#define PRIM_MAX                 GL_PATCHES
#define PRIM_OUTSIDE_BEGIN_END   (PRIM_MAX + 1)
#define PRIM_UNKNOWN             (PRIM_MAX + 2)

#ifndef __GNUC__
/* a grown-up compiler is required for the extra type checking: */
#  define container_of(ptr, type, member)                               \
      (type*)((uint8_t *)ptr - offsetof(type, member))
#else
#  define __same_type(a, b) \
      __builtin_types_compatible_p(__typeof__(a), __typeof__(b))
#  define container_of(ptr, type, member) ({                            \
         uint8_t *__mptr = (uint8_t *)(ptr);                            \
         STATIC_ASSERT(__same_type(*(ptr), ((type *)0)->member) ||      \
                       __same_type(*(ptr), void) ||                     \
                       !"pointer type mismatch in container_of()");     \
         ((type *)(__mptr - offsetof(type, member)));                   \
      })
#endif

typedef union { float f; int i; unsigned u; } fi_type;

#endif //AWIMUL_DRI_PRIVATE_H

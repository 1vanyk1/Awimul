#ifndef GLSLextAMD_H
#define GLSLextAMD_H

enum BuiltIn;
enum Capability;
enum Decoration;
enum Op;

static const int GLSLextAMDVersion = 100;
static const int GLSLextAMDRevision = 6;

// SPV_AMD_shader_ballot
static const char* const E_SPV_AMD_shader_ballot = "SPV_AMD_shader_ballot";

enum ShaderBallotAMD {
    ShaderBallotBadAMD = 0, // Don't use

    SwizzleInvocationsAMD = 1,
    SwizzleInvocationsMaskedAMD = 2,
    WriteInvocationAMD = 3,
    MbcntAMD = 4,

    ShaderBallotCountAMD
};

// SPV_AMD_shader_trinary_minmax
static const char* const E_SPV_AMD_shader_trinary_minmax = "SPV_AMD_shader_trinary_minmax";

enum ShaderTrinaryMinMaxAMD {
    ShaderTrinaryMinMaxBadAMD = 0, // Don't use

    FMin3AMD = 1,
    UMin3AMD = 2,
    SMin3AMD = 3,
    FMax3AMD = 4,
    UMax3AMD = 5,
    SMax3AMD = 6,
    FMid3AMD = 7,
    UMid3AMD = 8,
    SMid3AMD = 9,

    ShaderTrinaryMinMaxCountAMD
};

// SPV_AMD_shader_explicit_vertex_parameter
static const char* const E_SPV_AMD_shader_explicit_vertex_parameter = "SPV_AMD_shader_explicit_vertex_parameter";

enum ShaderExplicitVertexParameterAMD {
    ShaderExplicitVertexParameterBadAMD = 0, // Don't use

    InterpolateAtVertexAMD = 1,

    ShaderExplicitVertexParameterCountAMD
};

// SPV_AMD_gcn_shader
static const char* const E_SPV_AMD_gcn_shader = "SPV_AMD_gcn_shader";

enum GcnShaderAMD {
    GcnShaderBadAMD = 0, // Don't use

    CubeFaceIndexAMD = 1,
    CubeFaceCoordAMD = 2,
    TimeAMD = 3,

    GcnShaderCountAMD
};

// SPV_AMD_gpu_shader_half_float
static const char* const E_SPV_AMD_gpu_shader_half_float = "SPV_AMD_gpu_shader_half_float";

// SPV_AMD_texture_gather_bias_lod
static const char* const E_SPV_AMD_texture_gather_bias_lod = "SPV_AMD_texture_gather_bias_lod";

// SPV_AMD_gpu_shader_int16
static const char* const E_SPV_AMD_gpu_shader_int16 = "SPV_AMD_gpu_shader_int16";

// SPV_AMD_shader_image_load_store_lod
static const char* const E_SPV_AMD_shader_image_load_store_lod = "SPV_AMD_shader_image_load_store_lod";

// SPV_AMD_shader_fragment_mask
static const char* const E_SPV_AMD_shader_fragment_mask = "SPV_AMD_shader_fragment_mask";

#endif  // #ifndef GLSLextAMD_H

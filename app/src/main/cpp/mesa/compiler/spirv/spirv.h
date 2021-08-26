#ifndef spirv_H
#define spirv_H

typedef unsigned int SpvId;

#define SPV_VERSION 0x10500
#define SPV_REVISION 4

static const unsigned int SpvMagicNumber = 0x07230203;
static const unsigned int SpvVersion = 0x00010500;
static const unsigned int SpvRevision = 4;
static const unsigned int SpvOpCodeMask = 0xffff;
static const unsigned int SpvWordCountShift = 16;

typedef enum SpvSourceLanguage_ {
    SpvSourceLanguageUnknown = 0,
    SpvSourceLanguageESSL = 1,
    SpvSourceLanguageGLSL = 2,
    SpvSourceLanguageOpenCL_C = 3,
    SpvSourceLanguageOpenCL_CPP = 4,
    SpvSourceLanguageHLSL = 5,
    SpvSourceLanguageMax = 0x7fffffff,
} SpvSourceLanguage;

typedef enum SpvExecutionModel_ {
    SpvExecutionModelVertex = 0,
    SpvExecutionModelTessellationControl = 1,
    SpvExecutionModelTessellationEvaluation = 2,
    SpvExecutionModelGeometry = 3,
    SpvExecutionModelFragment = 4,
    SpvExecutionModelGLCompute = 5,
    SpvExecutionModelKernel = 6,
    SpvExecutionModelTaskNV = 5267,
    SpvExecutionModelMeshNV = 5268,
    SpvExecutionModelRayGenerationKHR = 5313,
    SpvExecutionModelRayGenerationNV = 5313,
    SpvExecutionModelIntersectionKHR = 5314,
    SpvExecutionModelIntersectionNV = 5314,
    SpvExecutionModelAnyHitKHR = 5315,
    SpvExecutionModelAnyHitNV = 5315,
    SpvExecutionModelClosestHitKHR = 5316,
    SpvExecutionModelClosestHitNV = 5316,
    SpvExecutionModelMissKHR = 5317,
    SpvExecutionModelMissNV = 5317,
    SpvExecutionModelCallableKHR = 5318,
    SpvExecutionModelCallableNV = 5318,
    SpvExecutionModelMax = 0x7fffffff,
} SpvExecutionModel;

typedef enum SpvAddressingModel_ {
    SpvAddressingModelLogical = 0,
    SpvAddressingModelPhysical32 = 1,
    SpvAddressingModelPhysical64 = 2,
    SpvAddressingModelPhysicalStorageBuffer64 = 5348,
    SpvAddressingModelPhysicalStorageBuffer64EXT = 5348,
    SpvAddressingModelMax = 0x7fffffff,
} SpvAddressingModel;

typedef enum SpvMemoryModel_ {
    SpvMemoryModelSimple = 0,
    SpvMemoryModelGLSL450 = 1,
    SpvMemoryModelOpenCL = 2,
    SpvMemoryModelVulkan = 3,
    SpvMemoryModelVulkanKHR = 3,
    SpvMemoryModelMax = 0x7fffffff,
} SpvMemoryModel;

typedef enum SpvExecutionMode_ {
    SpvExecutionModeInvocations = 0,
    SpvExecutionModeSpacingEqual = 1,
    SpvExecutionModeSpacingFractionalEven = 2,
    SpvExecutionModeSpacingFractionalOdd = 3,
    SpvExecutionModeVertexOrderCw = 4,
    SpvExecutionModeVertexOrderCcw = 5,
    SpvExecutionModePixelCenterInteger = 6,
    SpvExecutionModeOriginUpperLeft = 7,
    SpvExecutionModeOriginLowerLeft = 8,
    SpvExecutionModeEarlyFragmentTests = 9,
    SpvExecutionModePointMode = 10,
    SpvExecutionModeXfb = 11,
    SpvExecutionModeDepthReplacing = 12,
    SpvExecutionModeDepthGreater = 14,
    SpvExecutionModeDepthLess = 15,
    SpvExecutionModeDepthUnchanged = 16,
    SpvExecutionModeLocalSize = 17,
    SpvExecutionModeLocalSizeHint = 18,
    SpvExecutionModeInputPoints = 19,
    SpvExecutionModeInputLines = 20,
    SpvExecutionModeInputLinesAdjacency = 21,
    SpvExecutionModeTriangles = 22,
    SpvExecutionModeInputTrianglesAdjacency = 23,
    SpvExecutionModeQuads = 24,
    SpvExecutionModeIsolines = 25,
    SpvExecutionModeOutputVertices = 26,
    SpvExecutionModeOutputPoints = 27,
    SpvExecutionModeOutputLineStrip = 28,
    SpvExecutionModeOutputTriangleStrip = 29,
    SpvExecutionModeVecTypeHint = 30,
    SpvExecutionModeContractionOff = 31,
    SpvExecutionModeInitializer = 33,
    SpvExecutionModeFinalizer = 34,
    SpvExecutionModeSubgroupSize = 35,
    SpvExecutionModeSubgroupsPerWorkgroup = 36,
    SpvExecutionModeSubgroupsPerWorkgroupId = 37,
    SpvExecutionModeLocalSizeId = 38,
    SpvExecutionModeLocalSizeHintId = 39,
    SpvExecutionModePostDepthCoverage = 4446,
    SpvExecutionModeDenormPreserve = 4459,
    SpvExecutionModeDenormFlushToZero = 4460,
    SpvExecutionModeSignedZeroInfNanPreserve = 4461,
    SpvExecutionModeRoundingModeRTE = 4462,
    SpvExecutionModeRoundingModeRTZ = 4463,
    SpvExecutionModeStencilRefReplacingEXT = 5027,
    SpvExecutionModeOutputLinesNV = 5269,
    SpvExecutionModeOutputPrimitivesNV = 5270,
    SpvExecutionModeDerivativeGroupQuadsNV = 5289,
    SpvExecutionModeDerivativeGroupLinearNV = 5290,
    SpvExecutionModeOutputTrianglesNV = 5298,
    SpvExecutionModePixelInterlockOrderedEXT = 5366,
    SpvExecutionModePixelInterlockUnorderedEXT = 5367,
    SpvExecutionModeSampleInterlockOrderedEXT = 5368,
    SpvExecutionModeSampleInterlockUnorderedEXT = 5369,
    SpvExecutionModeShadingRateInterlockOrderedEXT = 5370,
    SpvExecutionModeShadingRateInterlockUnorderedEXT = 5371,
    SpvExecutionModeSharedLocalMemorySizeINTEL = 5618,
    SpvExecutionModeRoundingModeRTPINTEL = 5620,
    SpvExecutionModeRoundingModeRTNINTEL = 5621,
    SpvExecutionModeFloatingPointModeALTINTEL = 5622,
    SpvExecutionModeFloatingPointModeIEEEINTEL = 5623,
    SpvExecutionModeMaxWorkgroupSizeINTEL = 5893,
    SpvExecutionModeMaxWorkDimINTEL = 5894,
    SpvExecutionModeNoGlobalOffsetINTEL = 5895,
    SpvExecutionModeNumSIMDWorkitemsINTEL = 5896,
    SpvExecutionModeSchedulerTargetFmaxMhzINTEL = 5903,
    SpvExecutionModeMax = 0x7fffffff,
} SpvExecutionMode;

typedef enum SpvStorageClass_ {
    SpvStorageClassUniformConstant = 0,
    SpvStorageClassInput = 1,
    SpvStorageClassUniform = 2,
    SpvStorageClassOutput = 3,
    SpvStorageClassWorkgroup = 4,
    SpvStorageClassCrossWorkgroup = 5,
    SpvStorageClassPrivate = 6,
    SpvStorageClassFunction = 7,
    SpvStorageClassGeneric = 8,
    SpvStorageClassPushConstant = 9,
    SpvStorageClassAtomicCounter = 10,
    SpvStorageClassImage = 11,
    SpvStorageClassStorageBuffer = 12,
    SpvStorageClassCallableDataKHR = 5328,
    SpvStorageClassCallableDataNV = 5328,
    SpvStorageClassIncomingCallableDataKHR = 5329,
    SpvStorageClassIncomingCallableDataNV = 5329,
    SpvStorageClassRayPayloadKHR = 5338,
    SpvStorageClassRayPayloadNV = 5338,
    SpvStorageClassHitAttributeKHR = 5339,
    SpvStorageClassHitAttributeNV = 5339,
    SpvStorageClassIncomingRayPayloadKHR = 5342,
    SpvStorageClassIncomingRayPayloadNV = 5342,
    SpvStorageClassShaderRecordBufferKHR = 5343,
    SpvStorageClassShaderRecordBufferNV = 5343,
    SpvStorageClassPhysicalStorageBuffer = 5349,
    SpvStorageClassPhysicalStorageBufferEXT = 5349,
    SpvStorageClassCodeSectionINTEL = 5605,
    SpvStorageClassDeviceOnlyINTEL = 5936,
    SpvStorageClassHostOnlyINTEL = 5937,
    SpvStorageClassMax = 0x7fffffff,
} SpvStorageClass;

typedef enum SpvDim_ {
    SpvDim1D = 0,
    SpvDim2D = 1,
    SpvDim3D = 2,
    SpvDimCube = 3,
    SpvDimRect = 4,
    SpvDimBuffer = 5,
    SpvDimSubpassData = 6,
    SpvDimMax = 0x7fffffff,
} SpvDim;

typedef enum SpvSamplerAddressingMode_ {
    SpvSamplerAddressingModeNone = 0,
    SpvSamplerAddressingModeClampToEdge = 1,
    SpvSamplerAddressingModeClamp = 2,
    SpvSamplerAddressingModeRepeat = 3,
    SpvSamplerAddressingModeRepeatMirrored = 4,
    SpvSamplerAddressingModeMax = 0x7fffffff,
} SpvSamplerAddressingMode;

typedef enum SpvSamplerFilterMode_ {
    SpvSamplerFilterModeNearest = 0,
    SpvSamplerFilterModeLinear = 1,
    SpvSamplerFilterModeMax = 0x7fffffff,
} SpvSamplerFilterMode;

typedef enum SpvImageFormat_ {
    SpvImageFormatUnknown = 0,
    SpvImageFormatRgba32f = 1,
    SpvImageFormatRgba16f = 2,
    SpvImageFormatR32f = 3,
    SpvImageFormatRgba8 = 4,
    SpvImageFormatRgba8Snorm = 5,
    SpvImageFormatRg32f = 6,
    SpvImageFormatRg16f = 7,
    SpvImageFormatR11fG11fB10f = 8,
    SpvImageFormatR16f = 9,
    SpvImageFormatRgba16 = 10,
    SpvImageFormatRgb10A2 = 11,
    SpvImageFormatRg16 = 12,
    SpvImageFormatRg8 = 13,
    SpvImageFormatR16 = 14,
    SpvImageFormatR8 = 15,
    SpvImageFormatRgba16Snorm = 16,
    SpvImageFormatRg16Snorm = 17,
    SpvImageFormatRg8Snorm = 18,
    SpvImageFormatR16Snorm = 19,
    SpvImageFormatR8Snorm = 20,
    SpvImageFormatRgba32i = 21,
    SpvImageFormatRgba16i = 22,
    SpvImageFormatRgba8i = 23,
    SpvImageFormatR32i = 24,
    SpvImageFormatRg32i = 25,
    SpvImageFormatRg16i = 26,
    SpvImageFormatRg8i = 27,
    SpvImageFormatR16i = 28,
    SpvImageFormatR8i = 29,
    SpvImageFormatRgba32ui = 30,
    SpvImageFormatRgba16ui = 31,
    SpvImageFormatRgba8ui = 32,
    SpvImageFormatR32ui = 33,
    SpvImageFormatRgb10a2ui = 34,
    SpvImageFormatRg32ui = 35,
    SpvImageFormatRg16ui = 36,
    SpvImageFormatRg8ui = 37,
    SpvImageFormatR16ui = 38,
    SpvImageFormatR8ui = 39,
    SpvImageFormatR64ui = 40,
    SpvImageFormatR64i = 41,
    SpvImageFormatMax = 0x7fffffff,
} SpvImageFormat;

typedef enum SpvImageChannelOrder_ {
    SpvImageChannelOrderR = 0,
    SpvImageChannelOrderA = 1,
    SpvImageChannelOrderRG = 2,
    SpvImageChannelOrderRA = 3,
    SpvImageChannelOrderRGB = 4,
    SpvImageChannelOrderRGBA = 5,
    SpvImageChannelOrderBGRA = 6,
    SpvImageChannelOrderARGB = 7,
    SpvImageChannelOrderIntensity = 8,
    SpvImageChannelOrderLuminance = 9,
    SpvImageChannelOrderRx = 10,
    SpvImageChannelOrderRGx = 11,
    SpvImageChannelOrderRGBx = 12,
    SpvImageChannelOrderDepth = 13,
    SpvImageChannelOrderDepthStencil = 14,
    SpvImageChannelOrdersRGB = 15,
    SpvImageChannelOrdersRGBx = 16,
    SpvImageChannelOrdersRGBA = 17,
    SpvImageChannelOrdersBGRA = 18,
    SpvImageChannelOrderABGR = 19,
    SpvImageChannelOrderMax = 0x7fffffff,
} SpvImageChannelOrder;

typedef enum SpvImageChannelDataType_ {
    SpvImageChannelDataTypeSnormInt8 = 0,
    SpvImageChannelDataTypeSnormInt16 = 1,
    SpvImageChannelDataTypeUnormInt8 = 2,
    SpvImageChannelDataTypeUnormInt16 = 3,
    SpvImageChannelDataTypeUnormShort565 = 4,
    SpvImageChannelDataTypeUnormShort555 = 5,
    SpvImageChannelDataTypeUnormInt101010 = 6,
    SpvImageChannelDataTypeSignedInt8 = 7,
    SpvImageChannelDataTypeSignedInt16 = 8,
    SpvImageChannelDataTypeSignedInt32 = 9,
    SpvImageChannelDataTypeUnsignedInt8 = 10,
    SpvImageChannelDataTypeUnsignedInt16 = 11,
    SpvImageChannelDataTypeUnsignedInt32 = 12,
    SpvImageChannelDataTypeHalfFloat = 13,
    SpvImageChannelDataTypeFloat = 14,
    SpvImageChannelDataTypeUnormInt24 = 15,
    SpvImageChannelDataTypeUnormInt101010_2 = 16,
    SpvImageChannelDataTypeMax = 0x7fffffff,
} SpvImageChannelDataType;

typedef enum SpvImageOperandsShift_ {
    SpvImageOperandsBiasShift = 0,
    SpvImageOperandsLodShift = 1,
    SpvImageOperandsGradShift = 2,
    SpvImageOperandsConstOffsetShift = 3,
    SpvImageOperandsOffsetShift = 4,
    SpvImageOperandsConstOffsetsShift = 5,
    SpvImageOperandsSampleShift = 6,
    SpvImageOperandsMinLodShift = 7,
    SpvImageOperandsMakeTexelAvailableShift = 8,
    SpvImageOperandsMakeTexelAvailableKHRShift = 8,
    SpvImageOperandsMakeTexelVisibleShift = 9,
    SpvImageOperandsMakeTexelVisibleKHRShift = 9,
    SpvImageOperandsNonPrivateTexelShift = 10,
    SpvImageOperandsNonPrivateTexelKHRShift = 10,
    SpvImageOperandsVolatileTexelShift = 11,
    SpvImageOperandsVolatileTexelKHRShift = 11,
    SpvImageOperandsSignExtendShift = 12,
    SpvImageOperandsZeroExtendShift = 13,
    SpvImageOperandsMax = 0x7fffffff,
} SpvImageOperandsShift;

typedef enum SpvImageOperandsMask_ {
    SpvImageOperandsMaskNone = 0,
    SpvImageOperandsBiasMask = 0x00000001,
    SpvImageOperandsLodMask = 0x00000002,
    SpvImageOperandsGradMask = 0x00000004,
    SpvImageOperandsConstOffsetMask = 0x00000008,
    SpvImageOperandsOffsetMask = 0x00000010,
    SpvImageOperandsConstOffsetsMask = 0x00000020,
    SpvImageOperandsSampleMask = 0x00000040,
    SpvImageOperandsMinLodMask = 0x00000080,
    SpvImageOperandsMakeTexelAvailableMask = 0x00000100,
    SpvImageOperandsMakeTexelAvailableKHRMask = 0x00000100,
    SpvImageOperandsMakeTexelVisibleMask = 0x00000200,
    SpvImageOperandsMakeTexelVisibleKHRMask = 0x00000200,
    SpvImageOperandsNonPrivateTexelMask = 0x00000400,
    SpvImageOperandsNonPrivateTexelKHRMask = 0x00000400,
    SpvImageOperandsVolatileTexelMask = 0x00000800,
    SpvImageOperandsVolatileTexelKHRMask = 0x00000800,
    SpvImageOperandsSignExtendMask = 0x00001000,
    SpvImageOperandsZeroExtendMask = 0x00002000,
} SpvImageOperandsMask;

typedef enum SpvFPFastMathModeShift_ {
    SpvFPFastMathModeNotNaNShift = 0,
    SpvFPFastMathModeNotInfShift = 1,
    SpvFPFastMathModeNSZShift = 2,
    SpvFPFastMathModeAllowRecipShift = 3,
    SpvFPFastMathModeFastShift = 4,
    SpvFPFastMathModeAllowContractFastINTELShift = 16,
    SpvFPFastMathModeAllowReassocINTELShift = 17,
    SpvFPFastMathModeMax = 0x7fffffff,
} SpvFPFastMathModeShift;

typedef enum SpvFPFastMathModeMask_ {
    SpvFPFastMathModeMaskNone = 0,
    SpvFPFastMathModeNotNaNMask = 0x00000001,
    SpvFPFastMathModeNotInfMask = 0x00000002,
    SpvFPFastMathModeNSZMask = 0x00000004,
    SpvFPFastMathModeAllowRecipMask = 0x00000008,
    SpvFPFastMathModeFastMask = 0x00000010,
    SpvFPFastMathModeAllowContractFastINTELMask = 0x00010000,
    SpvFPFastMathModeAllowReassocINTELMask = 0x00020000,
} SpvFPFastMathModeMask;

typedef enum SpvFPRoundingMode_ {
    SpvFPRoundingModeRTE = 0,
    SpvFPRoundingModeRTZ = 1,
    SpvFPRoundingModeRTP = 2,
    SpvFPRoundingModeRTN = 3,
    SpvFPRoundingModeMax = 0x7fffffff,
} SpvFPRoundingMode;

typedef enum SpvLinkageType_ {
    SpvLinkageTypeExport = 0,
    SpvLinkageTypeImport = 1,
    SpvLinkageTypeLinkOnceODR = 2,
    SpvLinkageTypeMax = 0x7fffffff,
} SpvLinkageType;

typedef enum SpvAccessQualifier_ {
    SpvAccessQualifierReadOnly = 0,
    SpvAccessQualifierWriteOnly = 1,
    SpvAccessQualifierReadWrite = 2,
    SpvAccessQualifierMax = 0x7fffffff,
} SpvAccessQualifier;

typedef enum SpvFunctionParameterAttribute_ {
    SpvFunctionParameterAttributeZext = 0,
    SpvFunctionParameterAttributeSext = 1,
    SpvFunctionParameterAttributeByVal = 2,
    SpvFunctionParameterAttributeSret = 3,
    SpvFunctionParameterAttributeNoAlias = 4,
    SpvFunctionParameterAttributeNoCapture = 5,
    SpvFunctionParameterAttributeNoWrite = 6,
    SpvFunctionParameterAttributeNoReadWrite = 7,
    SpvFunctionParameterAttributeMax = 0x7fffffff,
} SpvFunctionParameterAttribute;

typedef enum SpvDecoration_ {
    SpvDecorationRelaxedPrecision = 0,
    SpvDecorationSpecId = 1,
    SpvDecorationBlock = 2,
    SpvDecorationBufferBlock = 3,
    SpvDecorationRowMajor = 4,
    SpvDecorationColMajor = 5,
    SpvDecorationArrayStride = 6,
    SpvDecorationMatrixStride = 7,
    SpvDecorationGLSLShared = 8,
    SpvDecorationGLSLPacked = 9,
    SpvDecorationCPacked = 10,
    SpvDecorationBuiltIn = 11,
    SpvDecorationNoPerspective = 13,
    SpvDecorationFlat = 14,
    SpvDecorationPatch = 15,
    SpvDecorationCentroid = 16,
    SpvDecorationSample = 17,
    SpvDecorationInvariant = 18,
    SpvDecorationRestrict = 19,
    SpvDecorationAliased = 20,
    SpvDecorationVolatile = 21,
    SpvDecorationConstant = 22,
    SpvDecorationCoherent = 23,
    SpvDecorationNonWritable = 24,
    SpvDecorationNonReadable = 25,
    SpvDecorationUniform = 26,
    SpvDecorationUniformId = 27,
    SpvDecorationSaturatedConversion = 28,
    SpvDecorationStream = 29,
    SpvDecorationLocation = 30,
    SpvDecorationComponent = 31,
    SpvDecorationIndex = 32,
    SpvDecorationBinding = 33,
    SpvDecorationDescriptorSet = 34,
    SpvDecorationOffset = 35,
    SpvDecorationXfbBuffer = 36,
    SpvDecorationXfbStride = 37,
    SpvDecorationFuncParamAttr = 38,
    SpvDecorationFPRoundingMode = 39,
    SpvDecorationFPFastMathMode = 40,
    SpvDecorationLinkageAttributes = 41,
    SpvDecorationNoContraction = 42,
    SpvDecorationInputAttachmentIndex = 43,
    SpvDecorationAlignment = 44,
    SpvDecorationMaxByteOffset = 45,
    SpvDecorationAlignmentId = 46,
    SpvDecorationMaxByteOffsetId = 47,
    SpvDecorationNoSignedWrap = 4469,
    SpvDecorationNoUnsignedWrap = 4470,
    SpvDecorationExplicitInterpAMD = 4999,
    SpvDecorationOverrideCoverageNV = 5248,
    SpvDecorationPassthroughNV = 5250,
    SpvDecorationViewportRelativeNV = 5252,
    SpvDecorationSecondaryViewportRelativeNV = 5256,
    SpvDecorationPerPrimitiveNV = 5271,
    SpvDecorationPerViewNV = 5272,
    SpvDecorationPerTaskNV = 5273,
    SpvDecorationPerVertexNV = 5285,
    SpvDecorationNonUniform = 5300,
    SpvDecorationNonUniformEXT = 5300,
    SpvDecorationRestrictPointer = 5355,
    SpvDecorationRestrictPointerEXT = 5355,
    SpvDecorationAliasedPointer = 5356,
    SpvDecorationAliasedPointerEXT = 5356,
    SpvDecorationSIMTCallINTEL = 5599,
    SpvDecorationReferencedIndirectlyINTEL = 5602,
    SpvDecorationClobberINTEL = 5607,
    SpvDecorationSideEffectsINTEL = 5608,
    SpvDecorationVectorComputeVariableINTEL = 5624,
    SpvDecorationFuncParamIOKindINTEL = 5625,
    SpvDecorationVectorComputeFunctionINTEL = 5626,
    SpvDecorationStackCallINTEL = 5627,
    SpvDecorationGlobalVariableOffsetINTEL = 5628,
    SpvDecorationCounterBuffer = 5634,
    SpvDecorationHlslCounterBufferGOOGLE = 5634,
    SpvDecorationHlslSemanticGOOGLE = 5635,
    SpvDecorationUserSemantic = 5635,
    SpvDecorationUserTypeGOOGLE = 5636,
    SpvDecorationFunctionRoundingModeINTEL = 5822,
    SpvDecorationFunctionDenormModeINTEL = 5823,
    SpvDecorationRegisterINTEL = 5825,
    SpvDecorationMemoryINTEL = 5826,
    SpvDecorationNumbanksINTEL = 5827,
    SpvDecorationBankwidthINTEL = 5828,
    SpvDecorationMaxPrivateCopiesINTEL = 5829,
    SpvDecorationSinglepumpINTEL = 5830,
    SpvDecorationDoublepumpINTEL = 5831,
    SpvDecorationMaxReplicatesINTEL = 5832,
    SpvDecorationSimpleDualPortINTEL = 5833,
    SpvDecorationMergeINTEL = 5834,
    SpvDecorationBankBitsINTEL = 5835,
    SpvDecorationForcePow2DepthINTEL = 5836,
    SpvDecorationBurstCoalesceINTEL = 5899,
    SpvDecorationCacheSizeINTEL = 5900,
    SpvDecorationDontStaticallyCoalesceINTEL = 5901,
    SpvDecorationPrefetchINTEL = 5902,
    SpvDecorationStallEnableINTEL = 5905,
    SpvDecorationFuseLoopsInFunctionINTEL = 5907,
    SpvDecorationBufferLocationINTEL = 5921,
    SpvDecorationIOPipeStorageINTEL = 5944,
    SpvDecorationFunctionFloatingPointModeINTEL = 6080,
    SpvDecorationSingleElementVectorINTEL = 6085,
    SpvDecorationVectorComputeCallableFunctionINTEL = 6087,
    SpvDecorationMax = 0x7fffffff,
} SpvDecoration;

typedef enum SpvBuiltIn_ {
    SpvBuiltInPosition = 0,
    SpvBuiltInPointSize = 1,
    SpvBuiltInClipDistance = 3,
    SpvBuiltInCullDistance = 4,
    SpvBuiltInVertexId = 5,
    SpvBuiltInInstanceId = 6,
    SpvBuiltInPrimitiveId = 7,
    SpvBuiltInInvocationId = 8,
    SpvBuiltInLayer = 9,
    SpvBuiltInViewportIndex = 10,
    SpvBuiltInTessLevelOuter = 11,
    SpvBuiltInTessLevelInner = 12,
    SpvBuiltInTessCoord = 13,
    SpvBuiltInPatchVertices = 14,
    SpvBuiltInFragCoord = 15,
    SpvBuiltInPointCoord = 16,
    SpvBuiltInFrontFacing = 17,
    SpvBuiltInSampleId = 18,
    SpvBuiltInSamplePosition = 19,
    SpvBuiltInSampleMask = 20,
    SpvBuiltInFragDepth = 22,
    SpvBuiltInHelperInvocation = 23,
    SpvBuiltInNumWorkgroups = 24,
    SpvBuiltInWorkgroupSize = 25,
    SpvBuiltInWorkgroupId = 26,
    SpvBuiltInLocalInvocationId = 27,
    SpvBuiltInGlobalInvocationId = 28,
    SpvBuiltInLocalInvocationIndex = 29,
    SpvBuiltInWorkDim = 30,
    SpvBuiltInGlobalSize = 31,
    SpvBuiltInEnqueuedWorkgroupSize = 32,
    SpvBuiltInGlobalOffset = 33,
    SpvBuiltInGlobalLinearId = 34,
    SpvBuiltInSubgroupSize = 36,
    SpvBuiltInSubgroupMaxSize = 37,
    SpvBuiltInNumSubgroups = 38,
    SpvBuiltInNumEnqueuedSubgroups = 39,
    SpvBuiltInSubgroupId = 40,
    SpvBuiltInSubgroupLocalInvocationId = 41,
    SpvBuiltInVertexIndex = 42,
    SpvBuiltInInstanceIndex = 43,
    SpvBuiltInSubgroupEqMask = 4416,
    SpvBuiltInSubgroupEqMaskKHR = 4416,
    SpvBuiltInSubgroupGeMask = 4417,
    SpvBuiltInSubgroupGeMaskKHR = 4417,
    SpvBuiltInSubgroupGtMask = 4418,
    SpvBuiltInSubgroupGtMaskKHR = 4418,
    SpvBuiltInSubgroupLeMask = 4419,
    SpvBuiltInSubgroupLeMaskKHR = 4419,
    SpvBuiltInSubgroupLtMask = 4420,
    SpvBuiltInSubgroupLtMaskKHR = 4420,
    SpvBuiltInBaseVertex = 4424,
    SpvBuiltInBaseInstance = 4425,
    SpvBuiltInDrawIndex = 4426,
    SpvBuiltInPrimitiveShadingRateKHR = 4432,
    SpvBuiltInDeviceIndex = 4438,
    SpvBuiltInViewIndex = 4440,
    SpvBuiltInShadingRateKHR = 4444,
    SpvBuiltInBaryCoordNoPerspAMD = 4992,
    SpvBuiltInBaryCoordNoPerspCentroidAMD = 4993,
    SpvBuiltInBaryCoordNoPerspSampleAMD = 4994,
    SpvBuiltInBaryCoordSmoothAMD = 4995,
    SpvBuiltInBaryCoordSmoothCentroidAMD = 4996,
    SpvBuiltInBaryCoordSmoothSampleAMD = 4997,
    SpvBuiltInBaryCoordPullModelAMD = 4998,
    SpvBuiltInFragStencilRefEXT = 5014,
    SpvBuiltInViewportMaskNV = 5253,
    SpvBuiltInSecondaryPositionNV = 5257,
    SpvBuiltInSecondaryViewportMaskNV = 5258,
    SpvBuiltInPositionPerViewNV = 5261,
    SpvBuiltInViewportMaskPerViewNV = 5262,
    SpvBuiltInFullyCoveredEXT = 5264,
    SpvBuiltInTaskCountNV = 5274,
    SpvBuiltInPrimitiveCountNV = 5275,
    SpvBuiltInPrimitiveIndicesNV = 5276,
    SpvBuiltInClipDistancePerViewNV = 5277,
    SpvBuiltInCullDistancePerViewNV = 5278,
    SpvBuiltInLayerPerViewNV = 5279,
    SpvBuiltInMeshViewCountNV = 5280,
    SpvBuiltInMeshViewIndicesNV = 5281,
    SpvBuiltInBaryCoordNV = 5286,
    SpvBuiltInBaryCoordNoPerspNV = 5287,
    SpvBuiltInFragSizeEXT = 5292,
    SpvBuiltInFragmentSizeNV = 5292,
    SpvBuiltInFragInvocationCountEXT = 5293,
    SpvBuiltInInvocationsPerPixelNV = 5293,
    SpvBuiltInLaunchIdKHR = 5319,
    SpvBuiltInLaunchIdNV = 5319,
    SpvBuiltInLaunchSizeKHR = 5320,
    SpvBuiltInLaunchSizeNV = 5320,
    SpvBuiltInWorldRayOriginKHR = 5321,
    SpvBuiltInWorldRayOriginNV = 5321,
    SpvBuiltInWorldRayDirectionKHR = 5322,
    SpvBuiltInWorldRayDirectionNV = 5322,
    SpvBuiltInObjectRayOriginKHR = 5323,
    SpvBuiltInObjectRayOriginNV = 5323,
    SpvBuiltInObjectRayDirectionKHR = 5324,
    SpvBuiltInObjectRayDirectionNV = 5324,
    SpvBuiltInRayTminKHR = 5325,
    SpvBuiltInRayTminNV = 5325,
    SpvBuiltInRayTmaxKHR = 5326,
    SpvBuiltInRayTmaxNV = 5326,
    SpvBuiltInInstanceCustomIndexKHR = 5327,
    SpvBuiltInInstanceCustomIndexNV = 5327,
    SpvBuiltInObjectToWorldKHR = 5330,
    SpvBuiltInObjectToWorldNV = 5330,
    SpvBuiltInWorldToObjectKHR = 5331,
    SpvBuiltInWorldToObjectNV = 5331,
    SpvBuiltInHitTNV = 5332,
    SpvBuiltInHitKindKHR = 5333,
    SpvBuiltInHitKindNV = 5333,
    SpvBuiltInIncomingRayFlagsKHR = 5351,
    SpvBuiltInIncomingRayFlagsNV = 5351,
    SpvBuiltInRayGeometryIndexKHR = 5352,
    SpvBuiltInWarpsPerSMNV = 5374,
    SpvBuiltInSMCountNV = 5375,
    SpvBuiltInWarpIDNV = 5376,
    SpvBuiltInSMIDNV = 5377,
    SpvBuiltInMax = 0x7fffffff,
} SpvBuiltIn;

typedef enum SpvSelectionControlShift_ {
    SpvSelectionControlFlattenShift = 0,
    SpvSelectionControlDontFlattenShift = 1,
    SpvSelectionControlMax = 0x7fffffff,
} SpvSelectionControlShift;

typedef enum SpvSelectionControlMask_ {
    SpvSelectionControlMaskNone = 0,
    SpvSelectionControlFlattenMask = 0x00000001,
    SpvSelectionControlDontFlattenMask = 0x00000002,
} SpvSelectionControlMask;

typedef enum SpvLoopControlShift_ {
    SpvLoopControlUnrollShift = 0,
    SpvLoopControlDontUnrollShift = 1,
    SpvLoopControlDependencyInfiniteShift = 2,
    SpvLoopControlDependencyLengthShift = 3,
    SpvLoopControlMinIterationsShift = 4,
    SpvLoopControlMaxIterationsShift = 5,
    SpvLoopControlIterationMultipleShift = 6,
    SpvLoopControlPeelCountShift = 7,
    SpvLoopControlPartialCountShift = 8,
    SpvLoopControlInitiationIntervalINTELShift = 16,
    SpvLoopControlMaxConcurrencyINTELShift = 17,
    SpvLoopControlDependencyArrayINTELShift = 18,
    SpvLoopControlPipelineEnableINTELShift = 19,
    SpvLoopControlLoopCoalesceINTELShift = 20,
    SpvLoopControlMaxInterleavingINTELShift = 21,
    SpvLoopControlSpeculatedIterationsINTELShift = 22,
    SpvLoopControlNoFusionINTELShift = 23,
    SpvLoopControlMax = 0x7fffffff,
} SpvLoopControlShift;

typedef enum SpvLoopControlMask_ {
    SpvLoopControlMaskNone = 0,
    SpvLoopControlUnrollMask = 0x00000001,
    SpvLoopControlDontUnrollMask = 0x00000002,
    SpvLoopControlDependencyInfiniteMask = 0x00000004,
    SpvLoopControlDependencyLengthMask = 0x00000008,
    SpvLoopControlMinIterationsMask = 0x00000010,
    SpvLoopControlMaxIterationsMask = 0x00000020,
    SpvLoopControlIterationMultipleMask = 0x00000040,
    SpvLoopControlPeelCountMask = 0x00000080,
    SpvLoopControlPartialCountMask = 0x00000100,
    SpvLoopControlInitiationIntervalINTELMask = 0x00010000,
    SpvLoopControlMaxConcurrencyINTELMask = 0x00020000,
    SpvLoopControlDependencyArrayINTELMask = 0x00040000,
    SpvLoopControlPipelineEnableINTELMask = 0x00080000,
    SpvLoopControlLoopCoalesceINTELMask = 0x00100000,
    SpvLoopControlMaxInterleavingINTELMask = 0x00200000,
    SpvLoopControlSpeculatedIterationsINTELMask = 0x00400000,
    SpvLoopControlNoFusionINTELMask = 0x00800000,
} SpvLoopControlMask;

typedef enum SpvFunctionControlShift_ {
    SpvFunctionControlInlineShift = 0,
    SpvFunctionControlDontInlineShift = 1,
    SpvFunctionControlPureShift = 2,
    SpvFunctionControlConstShift = 3,
    SpvFunctionControlMax = 0x7fffffff,
} SpvFunctionControlShift;

typedef enum SpvFunctionControlMask_ {
    SpvFunctionControlMaskNone = 0,
    SpvFunctionControlInlineMask = 0x00000001,
    SpvFunctionControlDontInlineMask = 0x00000002,
    SpvFunctionControlPureMask = 0x00000004,
    SpvFunctionControlConstMask = 0x00000008,
} SpvFunctionControlMask;

typedef enum SpvMemorySemanticsShift_ {
    SpvMemorySemanticsAcquireShift = 1,
    SpvMemorySemanticsReleaseShift = 2,
    SpvMemorySemanticsAcquireReleaseShift = 3,
    SpvMemorySemanticsSequentiallyConsistentShift = 4,
    SpvMemorySemanticsUniformMemoryShift = 6,
    SpvMemorySemanticsSubgroupMemoryShift = 7,
    SpvMemorySemanticsWorkgroupMemoryShift = 8,
    SpvMemorySemanticsCrossWorkgroupMemoryShift = 9,
    SpvMemorySemanticsAtomicCounterMemoryShift = 10,
    SpvMemorySemanticsImageMemoryShift = 11,
    SpvMemorySemanticsOutputMemoryShift = 12,
    SpvMemorySemanticsOutputMemoryKHRShift = 12,
    SpvMemorySemanticsMakeAvailableShift = 13,
    SpvMemorySemanticsMakeAvailableKHRShift = 13,
    SpvMemorySemanticsMakeVisibleShift = 14,
    SpvMemorySemanticsMakeVisibleKHRShift = 14,
    SpvMemorySemanticsVolatileShift = 15,
    SpvMemorySemanticsMax = 0x7fffffff,
} SpvMemorySemanticsShift;

typedef enum SpvMemorySemanticsMask_ {
    SpvMemorySemanticsMaskNone = 0,
    SpvMemorySemanticsAcquireMask = 0x00000002,
    SpvMemorySemanticsReleaseMask = 0x00000004,
    SpvMemorySemanticsAcquireReleaseMask = 0x00000008,
    SpvMemorySemanticsSequentiallyConsistentMask = 0x00000010,
    SpvMemorySemanticsUniformMemoryMask = 0x00000040,
    SpvMemorySemanticsSubgroupMemoryMask = 0x00000080,
    SpvMemorySemanticsWorkgroupMemoryMask = 0x00000100,
    SpvMemorySemanticsCrossWorkgroupMemoryMask = 0x00000200,
    SpvMemorySemanticsAtomicCounterMemoryMask = 0x00000400,
    SpvMemorySemanticsImageMemoryMask = 0x00000800,
    SpvMemorySemanticsOutputMemoryMask = 0x00001000,
    SpvMemorySemanticsOutputMemoryKHRMask = 0x00001000,
    SpvMemorySemanticsMakeAvailableMask = 0x00002000,
    SpvMemorySemanticsMakeAvailableKHRMask = 0x00002000,
    SpvMemorySemanticsMakeVisibleMask = 0x00004000,
    SpvMemorySemanticsMakeVisibleKHRMask = 0x00004000,
    SpvMemorySemanticsVolatileMask = 0x00008000,
} SpvMemorySemanticsMask;

typedef enum SpvMemoryAccessShift_ {
    SpvMemoryAccessVolatileShift = 0,
    SpvMemoryAccessAlignedShift = 1,
    SpvMemoryAccessNontemporalShift = 2,
    SpvMemoryAccessMakePointerAvailableShift = 3,
    SpvMemoryAccessMakePointerAvailableKHRShift = 3,
    SpvMemoryAccessMakePointerVisibleShift = 4,
    SpvMemoryAccessMakePointerVisibleKHRShift = 4,
    SpvMemoryAccessNonPrivatePointerShift = 5,
    SpvMemoryAccessNonPrivatePointerKHRShift = 5,
    SpvMemoryAccessMax = 0x7fffffff,
} SpvMemoryAccessShift;

typedef enum SpvMemoryAccessMask_ {
    SpvMemoryAccessMaskNone = 0,
    SpvMemoryAccessVolatileMask = 0x00000001,
    SpvMemoryAccessAlignedMask = 0x00000002,
    SpvMemoryAccessNontemporalMask = 0x00000004,
    SpvMemoryAccessMakePointerAvailableMask = 0x00000008,
    SpvMemoryAccessMakePointerAvailableKHRMask = 0x00000008,
    SpvMemoryAccessMakePointerVisibleMask = 0x00000010,
    SpvMemoryAccessMakePointerVisibleKHRMask = 0x00000010,
    SpvMemoryAccessNonPrivatePointerMask = 0x00000020,
    SpvMemoryAccessNonPrivatePointerKHRMask = 0x00000020,
} SpvMemoryAccessMask;

typedef enum SpvScope_ {
    SpvScopeCrossDevice = 0,
    SpvScopeDevice = 1,
    SpvScopeWorkgroup = 2,
    SpvScopeSubgroup = 3,
    SpvScopeInvocation = 4,
    SpvScopeQueueFamily = 5,
    SpvScopeQueueFamilyKHR = 5,
    SpvScopeShaderCallKHR = 6,
    SpvScopeMax = 0x7fffffff,
} SpvScope;

typedef enum SpvGroupOperation_ {
    SpvGroupOperationReduce = 0,
    SpvGroupOperationInclusiveScan = 1,
    SpvGroupOperationExclusiveScan = 2,
    SpvGroupOperationClusteredReduce = 3,
    SpvGroupOperationPartitionedReduceNV = 6,
    SpvGroupOperationPartitionedInclusiveScanNV = 7,
    SpvGroupOperationPartitionedExclusiveScanNV = 8,
    SpvGroupOperationMax = 0x7fffffff,
} SpvGroupOperation;

typedef enum SpvKernelEnqueueFlags_ {
    SpvKernelEnqueueFlagsNoWait = 0,
    SpvKernelEnqueueFlagsWaitKernel = 1,
    SpvKernelEnqueueFlagsWaitWorkGroup = 2,
    SpvKernelEnqueueFlagsMax = 0x7fffffff,
} SpvKernelEnqueueFlags;

typedef enum SpvKernelProfilingInfoShift_ {
    SpvKernelProfilingInfoCmdExecTimeShift = 0,
    SpvKernelProfilingInfoMax = 0x7fffffff,
} SpvKernelProfilingInfoShift;

typedef enum SpvKernelProfilingInfoMask_ {
    SpvKernelProfilingInfoMaskNone = 0,
    SpvKernelProfilingInfoCmdExecTimeMask = 0x00000001,
} SpvKernelProfilingInfoMask;

typedef enum SpvCapability_ {
    SpvCapabilityMatrix = 0,
    SpvCapabilityShader = 1,
    SpvCapabilityGeometry = 2,
    SpvCapabilityTessellation = 3,
    SpvCapabilityAddresses = 4,
    SpvCapabilityLinkage = 5,
    SpvCapabilityKernel = 6,
    SpvCapabilityVector16 = 7,
    SpvCapabilityFloat16Buffer = 8,
    SpvCapabilityFloat16 = 9,
    SpvCapabilityFloat64 = 10,
    SpvCapabilityInt64 = 11,
    SpvCapabilityInt64Atomics = 12,
    SpvCapabilityImageBasic = 13,
    SpvCapabilityImageReadWrite = 14,
    SpvCapabilityImageMipmap = 15,
    SpvCapabilityPipes = 17,
    SpvCapabilityGroups = 18,
    SpvCapabilityDeviceEnqueue = 19,
    SpvCapabilityLiteralSampler = 20,
    SpvCapabilityAtomicStorage = 21,
    SpvCapabilityInt16 = 22,
    SpvCapabilityTessellationPointSize = 23,
    SpvCapabilityGeometryPointSize = 24,
    SpvCapabilityImageGatherExtended = 25,
    SpvCapabilityStorageImageMultisample = 27,
    SpvCapabilityUniformBufferArrayDynamicIndexing = 28,
    SpvCapabilitySampledImageArrayDynamicIndexing = 29,
    SpvCapabilityStorageBufferArrayDynamicIndexing = 30,
    SpvCapabilityStorageImageArrayDynamicIndexing = 31,
    SpvCapabilityClipDistance = 32,
    SpvCapabilityCullDistance = 33,
    SpvCapabilityImageCubeArray = 34,
    SpvCapabilitySampleRateShading = 35,
    SpvCapabilityImageRect = 36,
    SpvCapabilitySampledRect = 37,
    SpvCapabilityGenericPointer = 38,
    SpvCapabilityInt8 = 39,
    SpvCapabilityInputAttachment = 40,
    SpvCapabilitySparseResidency = 41,
    SpvCapabilityMinLod = 42,
    SpvCapabilitySampled1D = 43,
    SpvCapabilityImage1D = 44,
    SpvCapabilitySampledCubeArray = 45,
    SpvCapabilitySampledBuffer = 46,
    SpvCapabilityImageBuffer = 47,
    SpvCapabilityImageMSArray = 48,
    SpvCapabilityStorageImageExtendedFormats = 49,
    SpvCapabilityImageQuery = 50,
    SpvCapabilityDerivativeControl = 51,
    SpvCapabilityInterpolationFunction = 52,
    SpvCapabilityTransformFeedback = 53,
    SpvCapabilityGeometryStreams = 54,
    SpvCapabilityStorageImageReadWithoutFormat = 55,
    SpvCapabilityStorageImageWriteWithoutFormat = 56,
    SpvCapabilityMultiViewport = 57,
    SpvCapabilitySubgroupDispatch = 58,
    SpvCapabilityNamedBarrier = 59,
    SpvCapabilityPipeStorage = 60,
    SpvCapabilityGroupNonUniform = 61,
    SpvCapabilityGroupNonUniformVote = 62,
    SpvCapabilityGroupNonUniformArithmetic = 63,
    SpvCapabilityGroupNonUniformBallot = 64,
    SpvCapabilityGroupNonUniformShuffle = 65,
    SpvCapabilityGroupNonUniformShuffleRelative = 66,
    SpvCapabilityGroupNonUniformClustered = 67,
    SpvCapabilityGroupNonUniformQuad = 68,
    SpvCapabilityShaderLayer = 69,
    SpvCapabilityShaderViewportIndex = 70,
    SpvCapabilityFragmentShadingRateKHR = 4422,
    SpvCapabilitySubgroupBallotKHR = 4423,
    SpvCapabilityDrawParameters = 4427,
    SpvCapabilityWorkgroupMemoryExplicitLayoutKHR = 4428,
    SpvCapabilityWorkgroupMemoryExplicitLayout8BitAccessKHR = 4429,
    SpvCapabilityWorkgroupMemoryExplicitLayout16BitAccessKHR = 4430,
    SpvCapabilitySubgroupVoteKHR = 4431,
    SpvCapabilityStorageBuffer16BitAccess = 4433,
    SpvCapabilityStorageUniformBufferBlock16 = 4433,
    SpvCapabilityStorageUniform16 = 4434,
    SpvCapabilityUniformAndStorageBuffer16BitAccess = 4434,
    SpvCapabilityStoragePushConstant16 = 4435,
    SpvCapabilityStorageInputOutput16 = 4436,
    SpvCapabilityDeviceGroup = 4437,
    SpvCapabilityMultiView = 4439,
    SpvCapabilityVariablePointersStorageBuffer = 4441,
    SpvCapabilityVariablePointers = 4442,
    SpvCapabilityAtomicStorageOps = 4445,
    SpvCapabilitySampleMaskPostDepthCoverage = 4447,
    SpvCapabilityStorageBuffer8BitAccess = 4448,
    SpvCapabilityUniformAndStorageBuffer8BitAccess = 4449,
    SpvCapabilityStoragePushConstant8 = 4450,
    SpvCapabilityDenormPreserve = 4464,
    SpvCapabilityDenormFlushToZero = 4465,
    SpvCapabilitySignedZeroInfNanPreserve = 4466,
    SpvCapabilityRoundingModeRTE = 4467,
    SpvCapabilityRoundingModeRTZ = 4468,
    SpvCapabilityRayQueryProvisionalKHR = 4471,
    SpvCapabilityRayQueryKHR = 4472,
    SpvCapabilityRayTraversalPrimitiveCullingKHR = 4478,
    SpvCapabilityRayTracingKHR = 4479,
    SpvCapabilityFloat16ImageAMD = 5008,
    SpvCapabilityImageGatherBiasLodAMD = 5009,
    SpvCapabilityFragmentMaskAMD = 5010,
    SpvCapabilityStencilExportEXT = 5013,
    SpvCapabilityImageReadWriteLodAMD = 5015,
    SpvCapabilityInt64ImageEXT = 5016,
    SpvCapabilityShaderClockKHR = 5055,
    SpvCapabilitySampleMaskOverrideCoverageNV = 5249,
    SpvCapabilityGeometryShaderPassthroughNV = 5251,
    SpvCapabilityShaderViewportIndexLayerEXT = 5254,
    SpvCapabilityShaderViewportIndexLayerNV = 5254,
    SpvCapabilityShaderViewportMaskNV = 5255,
    SpvCapabilityShaderStereoViewNV = 5259,
    SpvCapabilityPerViewAttributesNV = 5260,
    SpvCapabilityFragmentFullyCoveredEXT = 5265,
    SpvCapabilityMeshShadingNV = 5266,
    SpvCapabilityImageFootprintNV = 5282,
    SpvCapabilityFragmentBarycentricNV = 5284,
    SpvCapabilityComputeDerivativeGroupQuadsNV = 5288,
    SpvCapabilityFragmentDensityEXT = 5291,
    SpvCapabilityShadingRateNV = 5291,
    SpvCapabilityGroupNonUniformPartitionedNV = 5297,
    SpvCapabilityShaderNonUniform = 5301,
    SpvCapabilityShaderNonUniformEXT = 5301,
    SpvCapabilityRuntimeDescriptorArray = 5302,
    SpvCapabilityRuntimeDescriptorArrayEXT = 5302,
    SpvCapabilityInputAttachmentArrayDynamicIndexing = 5303,
    SpvCapabilityInputAttachmentArrayDynamicIndexingEXT = 5303,
    SpvCapabilityUniformTexelBufferArrayDynamicIndexing = 5304,
    SpvCapabilityUniformTexelBufferArrayDynamicIndexingEXT = 5304,
    SpvCapabilityStorageTexelBufferArrayDynamicIndexing = 5305,
    SpvCapabilityStorageTexelBufferArrayDynamicIndexingEXT = 5305,
    SpvCapabilityUniformBufferArrayNonUniformIndexing = 5306,
    SpvCapabilityUniformBufferArrayNonUniformIndexingEXT = 5306,
    SpvCapabilitySampledImageArrayNonUniformIndexing = 5307,
    SpvCapabilitySampledImageArrayNonUniformIndexingEXT = 5307,
    SpvCapabilityStorageBufferArrayNonUniformIndexing = 5308,
    SpvCapabilityStorageBufferArrayNonUniformIndexingEXT = 5308,
    SpvCapabilityStorageImageArrayNonUniformIndexing = 5309,
    SpvCapabilityStorageImageArrayNonUniformIndexingEXT = 5309,
    SpvCapabilityInputAttachmentArrayNonUniformIndexing = 5310,
    SpvCapabilityInputAttachmentArrayNonUniformIndexingEXT = 5310,
    SpvCapabilityUniformTexelBufferArrayNonUniformIndexing = 5311,
    SpvCapabilityUniformTexelBufferArrayNonUniformIndexingEXT = 5311,
    SpvCapabilityStorageTexelBufferArrayNonUniformIndexing = 5312,
    SpvCapabilityStorageTexelBufferArrayNonUniformIndexingEXT = 5312,
    SpvCapabilityRayTracingNV = 5340,
    SpvCapabilityVulkanMemoryModel = 5345,
    SpvCapabilityVulkanMemoryModelKHR = 5345,
    SpvCapabilityVulkanMemoryModelDeviceScope = 5346,
    SpvCapabilityVulkanMemoryModelDeviceScopeKHR = 5346,
    SpvCapabilityPhysicalStorageBufferAddresses = 5347,
    SpvCapabilityPhysicalStorageBufferAddressesEXT = 5347,
    SpvCapabilityComputeDerivativeGroupLinearNV = 5350,
    SpvCapabilityRayTracingProvisionalKHR = 5353,
    SpvCapabilityCooperativeMatrixNV = 5357,
    SpvCapabilityFragmentShaderSampleInterlockEXT = 5363,
    SpvCapabilityFragmentShaderShadingRateInterlockEXT = 5372,
    SpvCapabilityShaderSMBuiltinsNV = 5373,
    SpvCapabilityFragmentShaderPixelInterlockEXT = 5378,
    SpvCapabilityDemoteToHelperInvocationEXT = 5379,
    SpvCapabilitySubgroupShuffleINTEL = 5568,
    SpvCapabilitySubgroupBufferBlockIOINTEL = 5569,
    SpvCapabilitySubgroupImageBlockIOINTEL = 5570,
    SpvCapabilitySubgroupImageMediaBlockIOINTEL = 5579,
    SpvCapabilityRoundToInfinityINTEL = 5582,
    SpvCapabilityFloatingPointModeINTEL = 5583,
    SpvCapabilityIntegerFunctions2INTEL = 5584,
    SpvCapabilityFunctionPointersINTEL = 5603,
    SpvCapabilityIndirectReferencesINTEL = 5604,
    SpvCapabilityAsmINTEL = 5606,
    SpvCapabilityAtomicFloat32MinMaxEXT = 5612,
    SpvCapabilityAtomicFloat64MinMaxEXT = 5613,
    SpvCapabilityAtomicFloat16MinMaxEXT = 5616,
    SpvCapabilityVectorComputeINTEL = 5617,
    SpvCapabilityVectorAnyINTEL = 5619,
    SpvCapabilityExpectAssumeKHR = 5629,
    SpvCapabilitySubgroupAvcMotionEstimationINTEL = 5696,
    SpvCapabilitySubgroupAvcMotionEstimationIntraINTEL = 5697,
    SpvCapabilitySubgroupAvcMotionEstimationChromaINTEL = 5698,
    SpvCapabilityVariableLengthArrayINTEL = 5817,
    SpvCapabilityFunctionFloatControlINTEL = 5821,
    SpvCapabilityFPGAMemoryAttributesINTEL = 5824,
    SpvCapabilityFPFastMathModeINTEL = 5837,
    SpvCapabilityArbitraryPrecisionIntegersINTEL = 5844,
    SpvCapabilityUnstructuredLoopControlsINTEL = 5886,
    SpvCapabilityFPGALoopControlsINTEL = 5888,
    SpvCapabilityKernelAttributesINTEL = 5892,
    SpvCapabilityFPGAKernelAttributesINTEL = 5897,
    SpvCapabilityFPGAMemoryAccessesINTEL = 5898,
    SpvCapabilityFPGAClusterAttributesINTEL = 5904,
    SpvCapabilityLoopFuseINTEL = 5906,
    SpvCapabilityFPGABufferLocationINTEL = 5920,
    SpvCapabilityUSMStorageClassesINTEL = 5935,
    SpvCapabilityIOPipesINTEL = 5943,
    SpvCapabilityBlockingPipesINTEL = 5945,
    SpvCapabilityFPGARegINTEL = 5948,
    SpvCapabilityAtomicFloat32AddEXT = 6033,
    SpvCapabilityAtomicFloat64AddEXT = 6034,
    SpvCapabilityLongConstantCompositeINTEL = 6089,
    SpvCapabilityMax = 0x7fffffff,
} SpvCapability;

typedef enum SpvRayFlagsShift_ {
    SpvRayFlagsOpaqueKHRShift = 0,
    SpvRayFlagsNoOpaqueKHRShift = 1,
    SpvRayFlagsTerminateOnFirstHitKHRShift = 2,
    SpvRayFlagsSkipClosestHitShaderKHRShift = 3,
    SpvRayFlagsCullBackFacingTrianglesKHRShift = 4,
    SpvRayFlagsCullFrontFacingTrianglesKHRShift = 5,
    SpvRayFlagsCullOpaqueKHRShift = 6,
    SpvRayFlagsCullNoOpaqueKHRShift = 7,
    SpvRayFlagsSkipTrianglesKHRShift = 8,
    SpvRayFlagsSkipAABBsKHRShift = 9,
    SpvRayFlagsMax = 0x7fffffff,
} SpvRayFlagsShift;

typedef enum SpvRayFlagsMask_ {
    SpvRayFlagsMaskNone = 0,
    SpvRayFlagsOpaqueKHRMask = 0x00000001,
    SpvRayFlagsNoOpaqueKHRMask = 0x00000002,
    SpvRayFlagsTerminateOnFirstHitKHRMask = 0x00000004,
    SpvRayFlagsSkipClosestHitShaderKHRMask = 0x00000008,
    SpvRayFlagsCullBackFacingTrianglesKHRMask = 0x00000010,
    SpvRayFlagsCullFrontFacingTrianglesKHRMask = 0x00000020,
    SpvRayFlagsCullOpaqueKHRMask = 0x00000040,
    SpvRayFlagsCullNoOpaqueKHRMask = 0x00000080,
    SpvRayFlagsSkipTrianglesKHRMask = 0x00000100,
    SpvRayFlagsSkipAABBsKHRMask = 0x00000200,
} SpvRayFlagsMask;

typedef enum SpvRayQueryIntersection_ {
    SpvRayQueryIntersectionRayQueryCandidateIntersectionKHR = 0,
    SpvRayQueryIntersectionRayQueryCommittedIntersectionKHR = 1,
    SpvRayQueryIntersectionMax = 0x7fffffff,
} SpvRayQueryIntersection;

typedef enum SpvRayQueryCommittedIntersectionType_ {
    SpvRayQueryCommittedIntersectionTypeRayQueryCommittedIntersectionNoneKHR = 0,
    SpvRayQueryCommittedIntersectionTypeRayQueryCommittedIntersectionTriangleKHR = 1,
    SpvRayQueryCommittedIntersectionTypeRayQueryCommittedIntersectionGeneratedKHR = 2,
    SpvRayQueryCommittedIntersectionTypeMax = 0x7fffffff,
} SpvRayQueryCommittedIntersectionType;

typedef enum SpvRayQueryCandidateIntersectionType_ {
    SpvRayQueryCandidateIntersectionTypeRayQueryCandidateIntersectionTriangleKHR = 0,
    SpvRayQueryCandidateIntersectionTypeRayQueryCandidateIntersectionAABBKHR = 1,
    SpvRayQueryCandidateIntersectionTypeMax = 0x7fffffff,
} SpvRayQueryCandidateIntersectionType;

typedef enum SpvFragmentShadingRateShift_ {
    SpvFragmentShadingRateVertical2PixelsShift = 0,
    SpvFragmentShadingRateVertical4PixelsShift = 1,
    SpvFragmentShadingRateHorizontal2PixelsShift = 2,
    SpvFragmentShadingRateHorizontal4PixelsShift = 3,
    SpvFragmentShadingRateMax = 0x7fffffff,
} SpvFragmentShadingRateShift;

typedef enum SpvFragmentShadingRateMask_ {
    SpvFragmentShadingRateMaskNone = 0,
    SpvFragmentShadingRateVertical2PixelsMask = 0x00000001,
    SpvFragmentShadingRateVertical4PixelsMask = 0x00000002,
    SpvFragmentShadingRateHorizontal2PixelsMask = 0x00000004,
    SpvFragmentShadingRateHorizontal4PixelsMask = 0x00000008,
} SpvFragmentShadingRateMask;

typedef enum SpvFPDenormMode_ {
    SpvFPDenormModePreserve = 0,
    SpvFPDenormModeFlushToZero = 1,
    SpvFPDenormModeMax = 0x7fffffff,
} SpvFPDenormMode;

typedef enum SpvFPOperationMode_ {
    SpvFPOperationModeIEEE = 0,
    SpvFPOperationModeALT = 1,
    SpvFPOperationModeMax = 0x7fffffff,
} SpvFPOperationMode;

typedef enum SpvOp_ {
    SpvOpNop = 0,
    SpvOpUndef = 1,
    SpvOpSourceContinued = 2,
    SpvOpSource = 3,
    SpvOpSourceExtension = 4,
    SpvOpName = 5,
    SpvOpMemberName = 6,
    SpvOpString = 7,
    SpvOpLine = 8,
    SpvOpExtension = 10,
    SpvOpExtInstImport = 11,
    SpvOpExtInst = 12,
    SpvOpMemoryModel = 14,
    SpvOpEntryPoint = 15,
    SpvOpExecutionMode = 16,
    SpvOpCapability = 17,
    SpvOpTypeVoid = 19,
    SpvOpTypeBool = 20,
    SpvOpTypeInt = 21,
    SpvOpTypeFloat = 22,
    SpvOpTypeVector = 23,
    SpvOpTypeMatrix = 24,
    SpvOpTypeImage = 25,
    SpvOpTypeSampler = 26,
    SpvOpTypeSampledImage = 27,
    SpvOpTypeArray = 28,
    SpvOpTypeRuntimeArray = 29,
    SpvOpTypeStruct = 30,
    SpvOpTypeOpaque = 31,
    SpvOpTypePointer = 32,
    SpvOpTypeFunction = 33,
    SpvOpTypeEvent = 34,
    SpvOpTypeDeviceEvent = 35,
    SpvOpTypeReserveId = 36,
    SpvOpTypeQueue = 37,
    SpvOpTypePipe = 38,
    SpvOpTypeForwardPointer = 39,
    SpvOpConstantTrue = 41,
    SpvOpConstantFalse = 42,
    SpvOpConstant = 43,
    SpvOpConstantComposite = 44,
    SpvOpConstantSampler = 45,
    SpvOpConstantNull = 46,
    SpvOpSpecConstantTrue = 48,
    SpvOpSpecConstantFalse = 49,
    SpvOpSpecConstant = 50,
    SpvOpSpecConstantComposite = 51,
    SpvOpSpecConstantOp = 52,
    SpvOpFunction = 54,
    SpvOpFunctionParameter = 55,
    SpvOpFunctionEnd = 56,
    SpvOpFunctionCall = 57,
    SpvOpVariable = 59,
    SpvOpImageTexelPointer = 60,
    SpvOpLoad = 61,
    SpvOpStore = 62,
    SpvOpCopyMemory = 63,
    SpvOpCopyMemorySized = 64,
    SpvOpAccessChain = 65,
    SpvOpInBoundsAccessChain = 66,
    SpvOpPtrAccessChain = 67,
    SpvOpArrayLength = 68,
    SpvOpGenericPtrMemSemantics = 69,
    SpvOpInBoundsPtrAccessChain = 70,
    SpvOpDecorate = 71,
    SpvOpMemberDecorate = 72,
    SpvOpDecorationGroup = 73,
    SpvOpGroupDecorate = 74,
    SpvOpGroupMemberDecorate = 75,
    SpvOpVectorExtractDynamic = 77,
    SpvOpVectorInsertDynamic = 78,
    SpvOpVectorShuffle = 79,
    SpvOpCompositeConstruct = 80,
    SpvOpCompositeExtract = 81,
    SpvOpCompositeInsert = 82,
    SpvOpCopyObject = 83,
    SpvOpTranspose = 84,
    SpvOpSampledImage = 86,
    SpvOpImageSampleImplicitLod = 87,
    SpvOpImageSampleExplicitLod = 88,
    SpvOpImageSampleDrefImplicitLod = 89,
    SpvOpImageSampleDrefExplicitLod = 90,
    SpvOpImageSampleProjImplicitLod = 91,
    SpvOpImageSampleProjExplicitLod = 92,
    SpvOpImageSampleProjDrefImplicitLod = 93,
    SpvOpImageSampleProjDrefExplicitLod = 94,
    SpvOpImageFetch = 95,
    SpvOpImageGather = 96,
    SpvOpImageDrefGather = 97,
    SpvOpImageRead = 98,
    SpvOpImageWrite = 99,
    SpvOpImage = 100,
    SpvOpImageQueryFormat = 101,
    SpvOpImageQueryOrder = 102,
    SpvOpImageQuerySizeLod = 103,
    SpvOpImageQuerySize = 104,
    SpvOpImageQueryLod = 105,
    SpvOpImageQueryLevels = 106,
    SpvOpImageQuerySamples = 107,
    SpvOpConvertFToU = 109,
    SpvOpConvertFToS = 110,
    SpvOpConvertSToF = 111,
    SpvOpConvertUToF = 112,
    SpvOpUConvert = 113,
    SpvOpSConvert = 114,
    SpvOpFConvert = 115,
    SpvOpQuantizeToF16 = 116,
    SpvOpConvertPtrToU = 117,
    SpvOpSatConvertSToU = 118,
    SpvOpSatConvertUToS = 119,
    SpvOpConvertUToPtr = 120,
    SpvOpPtrCastToGeneric = 121,
    SpvOpGenericCastToPtr = 122,
    SpvOpGenericCastToPtrExplicit = 123,
    SpvOpBitcast = 124,
    SpvOpSNegate = 126,
    SpvOpFNegate = 127,
    SpvOpIAdd = 128,
    SpvOpFAdd = 129,
    SpvOpISub = 130,
    SpvOpFSub = 131,
    SpvOpIMul = 132,
    SpvOpFMul = 133,
    SpvOpUDiv = 134,
    SpvOpSDiv = 135,
    SpvOpFDiv = 136,
    SpvOpUMod = 137,
    SpvOpSRem = 138,
    SpvOpSMod = 139,
    SpvOpFRem = 140,
    SpvOpFMod = 141,
    SpvOpVectorTimesScalar = 142,
    SpvOpMatrixTimesScalar = 143,
    SpvOpVectorTimesMatrix = 144,
    SpvOpMatrixTimesVector = 145,
    SpvOpMatrixTimesMatrix = 146,
    SpvOpOuterProduct = 147,
    SpvOpDot = 148,
    SpvOpIAddCarry = 149,
    SpvOpISubBorrow = 150,
    SpvOpUMulExtended = 151,
    SpvOpSMulExtended = 152,
    SpvOpAny = 154,
    SpvOpAll = 155,
    SpvOpIsNan = 156,
    SpvOpIsInf = 157,
    SpvOpIsFinite = 158,
    SpvOpIsNormal = 159,
    SpvOpSignBitSet = 160,
    SpvOpLessOrGreater = 161,
    SpvOpOrdered = 162,
    SpvOpUnordered = 163,
    SpvOpLogicalEqual = 164,
    SpvOpLogicalNotEqual = 165,
    SpvOpLogicalOr = 166,
    SpvOpLogicalAnd = 167,
    SpvOpLogicalNot = 168,
    SpvOpSelect = 169,
    SpvOpIEqual = 170,
    SpvOpINotEqual = 171,
    SpvOpUGreaterThan = 172,
    SpvOpSGreaterThan = 173,
    SpvOpUGreaterThanEqual = 174,
    SpvOpSGreaterThanEqual = 175,
    SpvOpULessThan = 176,
    SpvOpSLessThan = 177,
    SpvOpULessThanEqual = 178,
    SpvOpSLessThanEqual = 179,
    SpvOpFOrdEqual = 180,
    SpvOpFUnordEqual = 181,
    SpvOpFOrdNotEqual = 182,
    SpvOpFUnordNotEqual = 183,
    SpvOpFOrdLessThan = 184,
    SpvOpFUnordLessThan = 185,
    SpvOpFOrdGreaterThan = 186,
    SpvOpFUnordGreaterThan = 187,
    SpvOpFOrdLessThanEqual = 188,
    SpvOpFUnordLessThanEqual = 189,
    SpvOpFOrdGreaterThanEqual = 190,
    SpvOpFUnordGreaterThanEqual = 191,
    SpvOpShiftRightLogical = 194,
    SpvOpShiftRightArithmetic = 195,
    SpvOpShiftLeftLogical = 196,
    SpvOpBitwiseOr = 197,
    SpvOpBitwiseXor = 198,
    SpvOpBitwiseAnd = 199,
    SpvOpNot = 200,
    SpvOpBitFieldInsert = 201,
    SpvOpBitFieldSExtract = 202,
    SpvOpBitFieldUExtract = 203,
    SpvOpBitReverse = 204,
    SpvOpBitCount = 205,
    SpvOpDPdx = 207,
    SpvOpDPdy = 208,
    SpvOpFwidth = 209,
    SpvOpDPdxFine = 210,
    SpvOpDPdyFine = 211,
    SpvOpFwidthFine = 212,
    SpvOpDPdxCoarse = 213,
    SpvOpDPdyCoarse = 214,
    SpvOpFwidthCoarse = 215,
    SpvOpEmitVertex = 218,
    SpvOpEndPrimitive = 219,
    SpvOpEmitStreamVertex = 220,
    SpvOpEndStreamPrimitive = 221,
    SpvOpControlBarrier = 224,
    SpvOpMemoryBarrier = 225,
    SpvOpAtomicLoad = 227,
    SpvOpAtomicStore = 228,
    SpvOpAtomicExchange = 229,
    SpvOpAtomicCompareExchange = 230,
    SpvOpAtomicCompareExchangeWeak = 231,
    SpvOpAtomicIIncrement = 232,
    SpvOpAtomicIDecrement = 233,
    SpvOpAtomicIAdd = 234,
    SpvOpAtomicISub = 235,
    SpvOpAtomicSMin = 236,
    SpvOpAtomicUMin = 237,
    SpvOpAtomicSMax = 238,
    SpvOpAtomicUMax = 239,
    SpvOpAtomicAnd = 240,
    SpvOpAtomicOr = 241,
    SpvOpAtomicXor = 242,
    SpvOpPhi = 245,
    SpvOpLoopMerge = 246,
    SpvOpSelectionMerge = 247,
    SpvOpLabel = 248,
    SpvOpBranch = 249,
    SpvOpBranchConditional = 250,
    SpvOpSwitch = 251,
    SpvOpKill = 252,
    SpvOpReturn = 253,
    SpvOpReturnValue = 254,
    SpvOpUnreachable = 255,
    SpvOpLifetimeStart = 256,
    SpvOpLifetimeStop = 257,
    SpvOpGroupAsyncCopy = 259,
    SpvOpGroupWaitEvents = 260,
    SpvOpGroupAll = 261,
    SpvOpGroupAny = 262,
    SpvOpGroupBroadcast = 263,
    SpvOpGroupIAdd = 264,
    SpvOpGroupFAdd = 265,
    SpvOpGroupFMin = 266,
    SpvOpGroupUMin = 267,
    SpvOpGroupSMin = 268,
    SpvOpGroupFMax = 269,
    SpvOpGroupUMax = 270,
    SpvOpGroupSMax = 271,
    SpvOpReadPipe = 274,
    SpvOpWritePipe = 275,
    SpvOpReservedReadPipe = 276,
    SpvOpReservedWritePipe = 277,
    SpvOpReserveReadPipePackets = 278,
    SpvOpReserveWritePipePackets = 279,
    SpvOpCommitReadPipe = 280,
    SpvOpCommitWritePipe = 281,
    SpvOpIsValidReserveId = 282,
    SpvOpGetNumPipePackets = 283,
    SpvOpGetMaxPipePackets = 284,
    SpvOpGroupReserveReadPipePackets = 285,
    SpvOpGroupReserveWritePipePackets = 286,
    SpvOpGroupCommitReadPipe = 287,
    SpvOpGroupCommitWritePipe = 288,
    SpvOpEnqueueMarker = 291,
    SpvOpEnqueueKernel = 292,
    SpvOpGetKernelNDrangeSubGroupCount = 293,
    SpvOpGetKernelNDrangeMaxSubGroupSize = 294,
    SpvOpGetKernelWorkGroupSize = 295,
    SpvOpGetKernelPreferredWorkGroupSizeMultiple = 296,
    SpvOpRetainEvent = 297,
    SpvOpReleaseEvent = 298,
    SpvOpCreateUserEvent = 299,
    SpvOpIsValidEvent = 300,
    SpvOpSetUserEventStatus = 301,
    SpvOpCaptureEventProfilingInfo = 302,
    SpvOpGetDefaultQueue = 303,
    SpvOpBuildNDRange = 304,
    SpvOpImageSparseSampleImplicitLod = 305,
    SpvOpImageSparseSampleExplicitLod = 306,
    SpvOpImageSparseSampleDrefImplicitLod = 307,
    SpvOpImageSparseSampleDrefExplicitLod = 308,
    SpvOpImageSparseSampleProjImplicitLod = 309,
    SpvOpImageSparseSampleProjExplicitLod = 310,
    SpvOpImageSparseSampleProjDrefImplicitLod = 311,
    SpvOpImageSparseSampleProjDrefExplicitLod = 312,
    SpvOpImageSparseFetch = 313,
    SpvOpImageSparseGather = 314,
    SpvOpImageSparseDrefGather = 315,
    SpvOpImageSparseTexelsResident = 316,
    SpvOpNoLine = 317,
    SpvOpAtomicFlagTestAndSet = 318,
    SpvOpAtomicFlagClear = 319,
    SpvOpImageSparseRead = 320,
    SpvOpSizeOf = 321,
    SpvOpTypePipeStorage = 322,
    SpvOpConstantPipeStorage = 323,
    SpvOpCreatePipeFromPipeStorage = 324,
    SpvOpGetKernelLocalSizeForSubgroupCount = 325,
    SpvOpGetKernelMaxNumSubgroups = 326,
    SpvOpTypeNamedBarrier = 327,
    SpvOpNamedBarrierInitialize = 328,
    SpvOpMemoryNamedBarrier = 329,
    SpvOpModuleProcessed = 330,
    SpvOpExecutionModeId = 331,
    SpvOpDecorateId = 332,
    SpvOpGroupNonUniformElect = 333,
    SpvOpGroupNonUniformAll = 334,
    SpvOpGroupNonUniformAny = 335,
    SpvOpGroupNonUniformAllEqual = 336,
    SpvOpGroupNonUniformBroadcast = 337,
    SpvOpGroupNonUniformBroadcastFirst = 338,
    SpvOpGroupNonUniformBallot = 339,
    SpvOpGroupNonUniformInverseBallot = 340,
    SpvOpGroupNonUniformBallotBitExtract = 341,
    SpvOpGroupNonUniformBallotBitCount = 342,
    SpvOpGroupNonUniformBallotFindLSB = 343,
    SpvOpGroupNonUniformBallotFindMSB = 344,
    SpvOpGroupNonUniformShuffle = 345,
    SpvOpGroupNonUniformShuffleXor = 346,
    SpvOpGroupNonUniformShuffleUp = 347,
    SpvOpGroupNonUniformShuffleDown = 348,
    SpvOpGroupNonUniformIAdd = 349,
    SpvOpGroupNonUniformFAdd = 350,
    SpvOpGroupNonUniformIMul = 351,
    SpvOpGroupNonUniformFMul = 352,
    SpvOpGroupNonUniformSMin = 353,
    SpvOpGroupNonUniformUMin = 354,
    SpvOpGroupNonUniformFMin = 355,
    SpvOpGroupNonUniformSMax = 356,
    SpvOpGroupNonUniformUMax = 357,
    SpvOpGroupNonUniformFMax = 358,
    SpvOpGroupNonUniformBitwiseAnd = 359,
    SpvOpGroupNonUniformBitwiseOr = 360,
    SpvOpGroupNonUniformBitwiseXor = 361,
    SpvOpGroupNonUniformLogicalAnd = 362,
    SpvOpGroupNonUniformLogicalOr = 363,
    SpvOpGroupNonUniformLogicalXor = 364,
    SpvOpGroupNonUniformQuadBroadcast = 365,
    SpvOpGroupNonUniformQuadSwap = 366,
    SpvOpCopyLogical = 400,
    SpvOpPtrEqual = 401,
    SpvOpPtrNotEqual = 402,
    SpvOpPtrDiff = 403,
    SpvOpTerminateInvocation = 4416,
    SpvOpSubgroupBallotKHR = 4421,
    SpvOpSubgroupFirstInvocationKHR = 4422,
    SpvOpSubgroupAllKHR = 4428,
    SpvOpSubgroupAnyKHR = 4429,
    SpvOpSubgroupAllEqualKHR = 4430,
    SpvOpSubgroupReadInvocationKHR = 4432,
    SpvOpTraceRayKHR = 4445,
    SpvOpExecuteCallableKHR = 4446,
    SpvOpConvertUToAccelerationStructureKHR = 4447,
    SpvOpIgnoreIntersectionKHR = 4448,
    SpvOpTerminateRayKHR = 4449,
    SpvOpTypeRayQueryKHR = 4472,
    SpvOpRayQueryInitializeKHR = 4473,
    SpvOpRayQueryTerminateKHR = 4474,
    SpvOpRayQueryGenerateIntersectionKHR = 4475,
    SpvOpRayQueryConfirmIntersectionKHR = 4476,
    SpvOpRayQueryProceedKHR = 4477,
    SpvOpRayQueryGetIntersectionTypeKHR = 4479,
    SpvOpGroupIAddNonUniformAMD = 5000,
    SpvOpGroupFAddNonUniformAMD = 5001,
    SpvOpGroupFMinNonUniformAMD = 5002,
    SpvOpGroupUMinNonUniformAMD = 5003,
    SpvOpGroupSMinNonUniformAMD = 5004,
    SpvOpGroupFMaxNonUniformAMD = 5005,
    SpvOpGroupUMaxNonUniformAMD = 5006,
    SpvOpGroupSMaxNonUniformAMD = 5007,
    SpvOpFragmentMaskFetchAMD = 5011,
    SpvOpFragmentFetchAMD = 5012,
    SpvOpReadClockKHR = 5056,
    SpvOpImageSampleFootprintNV = 5283,
    SpvOpGroupNonUniformPartitionNV = 5296,
    SpvOpWritePackedPrimitiveIndices4x8NV = 5299,
    SpvOpReportIntersectionKHR = 5334,
    SpvOpReportIntersectionNV = 5334,
    SpvOpIgnoreIntersectionNV = 5335,
    SpvOpTerminateRayNV = 5336,
    SpvOpTraceNV = 5337,
    SpvOpTypeAccelerationStructureKHR = 5341,
    SpvOpTypeAccelerationStructureNV = 5341,
    SpvOpExecuteCallableNV = 5344,
    SpvOpTypeCooperativeMatrixNV = 5358,
    SpvOpCooperativeMatrixLoadNV = 5359,
    SpvOpCooperativeMatrixStoreNV = 5360,
    SpvOpCooperativeMatrixMulAddNV = 5361,
    SpvOpCooperativeMatrixLengthNV = 5362,
    SpvOpBeginInvocationInterlockEXT = 5364,
    SpvOpEndInvocationInterlockEXT = 5365,
    SpvOpDemoteToHelperInvocationEXT = 5380,
    SpvOpIsHelperInvocationEXT = 5381,
    SpvOpSubgroupShuffleINTEL = 5571,
    SpvOpSubgroupShuffleDownINTEL = 5572,
    SpvOpSubgroupShuffleUpINTEL = 5573,
    SpvOpSubgroupShuffleXorINTEL = 5574,
    SpvOpSubgroupBlockReadINTEL = 5575,
    SpvOpSubgroupBlockWriteINTEL = 5576,
    SpvOpSubgroupImageBlockReadINTEL = 5577,
    SpvOpSubgroupImageBlockWriteINTEL = 5578,
    SpvOpSubgroupImageMediaBlockReadINTEL = 5580,
    SpvOpSubgroupImageMediaBlockWriteINTEL = 5581,
    SpvOpUCountLeadingZerosINTEL = 5585,
    SpvOpUCountTrailingZerosINTEL = 5586,
    SpvOpAbsISubINTEL = 5587,
    SpvOpAbsUSubINTEL = 5588,
    SpvOpIAddSatINTEL = 5589,
    SpvOpUAddSatINTEL = 5590,
    SpvOpIAverageINTEL = 5591,
    SpvOpUAverageINTEL = 5592,
    SpvOpIAverageRoundedINTEL = 5593,
    SpvOpUAverageRoundedINTEL = 5594,
    SpvOpISubSatINTEL = 5595,
    SpvOpUSubSatINTEL = 5596,
    SpvOpIMul32x16INTEL = 5597,
    SpvOpUMul32x16INTEL = 5598,
    SpvOpConstFunctionPointerINTEL = 5600,
    SpvOpFunctionPointerCallINTEL = 5601,
    SpvOpAsmTargetINTEL = 5609,
    SpvOpAsmINTEL = 5610,
    SpvOpAsmCallINTEL = 5611,
    SpvOpAtomicFMinEXT = 5614,
    SpvOpAtomicFMaxEXT = 5615,
    SpvOpAssumeTrueKHR = 5630,
    SpvOpExpectKHR = 5631,
    SpvOpDecorateString = 5632,
    SpvOpDecorateStringGOOGLE = 5632,
    SpvOpMemberDecorateString = 5633,
    SpvOpMemberDecorateStringGOOGLE = 5633,
    SpvOpVmeImageINTEL = 5699,
    SpvOpTypeVmeImageINTEL = 5700,
    SpvOpTypeAvcImePayloadINTEL = 5701,
    SpvOpTypeAvcRefPayloadINTEL = 5702,
    SpvOpTypeAvcSicPayloadINTEL = 5703,
    SpvOpTypeAvcMcePayloadINTEL = 5704,
    SpvOpTypeAvcMceResultINTEL = 5705,
    SpvOpTypeAvcImeResultINTEL = 5706,
    SpvOpTypeAvcImeResultSingleReferenceStreamoutINTEL = 5707,
    SpvOpTypeAvcImeResultDualReferenceStreamoutINTEL = 5708,
    SpvOpTypeAvcImeSingleReferenceStreaminINTEL = 5709,
    SpvOpTypeAvcImeDualReferenceStreaminINTEL = 5710,
    SpvOpTypeAvcRefResultINTEL = 5711,
    SpvOpTypeAvcSicResultINTEL = 5712,
    SpvOpSubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL = 5713,
    SpvOpSubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL = 5714,
    SpvOpSubgroupAvcMceGetDefaultInterShapePenaltyINTEL = 5715,
    SpvOpSubgroupAvcMceSetInterShapePenaltyINTEL = 5716,
    SpvOpSubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL = 5717,
    SpvOpSubgroupAvcMceSetInterDirectionPenaltyINTEL = 5718,
    SpvOpSubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL = 5719,
    SpvOpSubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL = 5720,
    SpvOpSubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL = 5721,
    SpvOpSubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL = 5722,
    SpvOpSubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL = 5723,
    SpvOpSubgroupAvcMceSetMotionVectorCostFunctionINTEL = 5724,
    SpvOpSubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL = 5725,
    SpvOpSubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL = 5726,
    SpvOpSubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL = 5727,
    SpvOpSubgroupAvcMceSetAcOnlyHaarINTEL = 5728,
    SpvOpSubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL = 5729,
    SpvOpSubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL = 5730,
    SpvOpSubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL = 5731,
    SpvOpSubgroupAvcMceConvertToImePayloadINTEL = 5732,
    SpvOpSubgroupAvcMceConvertToImeResultINTEL = 5733,
    SpvOpSubgroupAvcMceConvertToRefPayloadINTEL = 5734,
    SpvOpSubgroupAvcMceConvertToRefResultINTEL = 5735,
    SpvOpSubgroupAvcMceConvertToSicPayloadINTEL = 5736,
    SpvOpSubgroupAvcMceConvertToSicResultINTEL = 5737,
    SpvOpSubgroupAvcMceGetMotionVectorsINTEL = 5738,
    SpvOpSubgroupAvcMceGetInterDistortionsINTEL = 5739,
    SpvOpSubgroupAvcMceGetBestInterDistortionsINTEL = 5740,
    SpvOpSubgroupAvcMceGetInterMajorShapeINTEL = 5741,
    SpvOpSubgroupAvcMceGetInterMinorShapeINTEL = 5742,
    SpvOpSubgroupAvcMceGetInterDirectionsINTEL = 5743,
    SpvOpSubgroupAvcMceGetInterMotionVectorCountINTEL = 5744,
    SpvOpSubgroupAvcMceGetInterReferenceIdsINTEL = 5745,
    SpvOpSubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL = 5746,
    SpvOpSubgroupAvcImeInitializeINTEL = 5747,
    SpvOpSubgroupAvcImeSetSingleReferenceINTEL = 5748,
    SpvOpSubgroupAvcImeSetDualReferenceINTEL = 5749,
    SpvOpSubgroupAvcImeRefWindowSizeINTEL = 5750,
    SpvOpSubgroupAvcImeAdjustRefOffsetINTEL = 5751,
    SpvOpSubgroupAvcImeConvertToMcePayloadINTEL = 5752,
    SpvOpSubgroupAvcImeSetMaxMotionVectorCountINTEL = 5753,
    SpvOpSubgroupAvcImeSetUnidirectionalMixDisableINTEL = 5754,
    SpvOpSubgroupAvcImeSetEarlySearchTerminationThresholdINTEL = 5755,
    SpvOpSubgroupAvcImeSetWeightedSadINTEL = 5756,
    SpvOpSubgroupAvcImeEvaluateWithSingleReferenceINTEL = 5757,
    SpvOpSubgroupAvcImeEvaluateWithDualReferenceINTEL = 5758,
    SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL = 5759,
    SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL = 5760,
    SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL = 5761,
    SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL = 5762,
    SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL = 5763,
    SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL = 5764,
    SpvOpSubgroupAvcImeConvertToMceResultINTEL = 5765,
    SpvOpSubgroupAvcImeGetSingleReferenceStreaminINTEL = 5766,
    SpvOpSubgroupAvcImeGetDualReferenceStreaminINTEL = 5767,
    SpvOpSubgroupAvcImeStripSingleReferenceStreamoutINTEL = 5768,
    SpvOpSubgroupAvcImeStripDualReferenceStreamoutINTEL = 5769,
    SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL = 5770,
    SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL = 5771,
    SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL = 5772,
    SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL = 5773,
    SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL = 5774,
    SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL = 5775,
    SpvOpSubgroupAvcImeGetBorderReachedINTEL = 5776,
    SpvOpSubgroupAvcImeGetTruncatedSearchIndicationINTEL = 5777,
    SpvOpSubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL = 5778,
    SpvOpSubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL = 5779,
    SpvOpSubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL = 5780,
    SpvOpSubgroupAvcFmeInitializeINTEL = 5781,
    SpvOpSubgroupAvcBmeInitializeINTEL = 5782,
    SpvOpSubgroupAvcRefConvertToMcePayloadINTEL = 5783,
    SpvOpSubgroupAvcRefSetBidirectionalMixDisableINTEL = 5784,
    SpvOpSubgroupAvcRefSetBilinearFilterEnableINTEL = 5785,
    SpvOpSubgroupAvcRefEvaluateWithSingleReferenceINTEL = 5786,
    SpvOpSubgroupAvcRefEvaluateWithDualReferenceINTEL = 5787,
    SpvOpSubgroupAvcRefEvaluateWithMultiReferenceINTEL = 5788,
    SpvOpSubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL = 5789,
    SpvOpSubgroupAvcRefConvertToMceResultINTEL = 5790,
    SpvOpSubgroupAvcSicInitializeINTEL = 5791,
    SpvOpSubgroupAvcSicConfigureSkcINTEL = 5792,
    SpvOpSubgroupAvcSicConfigureIpeLumaINTEL = 5793,
    SpvOpSubgroupAvcSicConfigureIpeLumaChromaINTEL = 5794,
    SpvOpSubgroupAvcSicGetMotionVectorMaskINTEL = 5795,
    SpvOpSubgroupAvcSicConvertToMcePayloadINTEL = 5796,
    SpvOpSubgroupAvcSicSetIntraLumaShapePenaltyINTEL = 5797,
    SpvOpSubgroupAvcSicSetIntraLumaModeCostFunctionINTEL = 5798,
    SpvOpSubgroupAvcSicSetIntraChromaModeCostFunctionINTEL = 5799,
    SpvOpSubgroupAvcSicSetBilinearFilterEnableINTEL = 5800,
    SpvOpSubgroupAvcSicSetSkcForwardTransformEnableINTEL = 5801,
    SpvOpSubgroupAvcSicSetBlockBasedRawSkipSadINTEL = 5802,
    SpvOpSubgroupAvcSicEvaluateIpeINTEL = 5803,
    SpvOpSubgroupAvcSicEvaluateWithSingleReferenceINTEL = 5804,
    SpvOpSubgroupAvcSicEvaluateWithDualReferenceINTEL = 5805,
    SpvOpSubgroupAvcSicEvaluateWithMultiReferenceINTEL = 5806,
    SpvOpSubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL = 5807,
    SpvOpSubgroupAvcSicConvertToMceResultINTEL = 5808,
    SpvOpSubgroupAvcSicGetIpeLumaShapeINTEL = 5809,
    SpvOpSubgroupAvcSicGetBestIpeLumaDistortionINTEL = 5810,
    SpvOpSubgroupAvcSicGetBestIpeChromaDistortionINTEL = 5811,
    SpvOpSubgroupAvcSicGetPackedIpeLumaModesINTEL = 5812,
    SpvOpSubgroupAvcSicGetIpeChromaModeINTEL = 5813,
    SpvOpSubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL = 5814,
    SpvOpSubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL = 5815,
    SpvOpSubgroupAvcSicGetInterRawSadsINTEL = 5816,
    SpvOpVariableLengthArrayINTEL = 5818,
    SpvOpSaveMemoryINTEL = 5819,
    SpvOpRestoreMemoryINTEL = 5820,
    SpvOpLoopControlINTEL = 5887,
    SpvOpPtrCastToCrossWorkgroupINTEL = 5934,
    SpvOpCrossWorkgroupCastToPtrINTEL = 5938,
    SpvOpReadPipeBlockingINTEL = 5946,
    SpvOpWritePipeBlockingINTEL = 5947,
    SpvOpFPGARegINTEL = 5949,
    SpvOpRayQueryGetRayTMinKHR = 6016,
    SpvOpRayQueryGetRayFlagsKHR = 6017,
    SpvOpRayQueryGetIntersectionTKHR = 6018,
    SpvOpRayQueryGetIntersectionInstanceCustomIndexKHR = 6019,
    SpvOpRayQueryGetIntersectionInstanceIdKHR = 6020,
    SpvOpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR = 6021,
    SpvOpRayQueryGetIntersectionGeometryIndexKHR = 6022,
    SpvOpRayQueryGetIntersectionPrimitiveIndexKHR = 6023,
    SpvOpRayQueryGetIntersectionBarycentricsKHR = 6024,
    SpvOpRayQueryGetIntersectionFrontFaceKHR = 6025,
    SpvOpRayQueryGetIntersectionCandidateAABBOpaqueKHR = 6026,
    SpvOpRayQueryGetIntersectionObjectRayDirectionKHR = 6027,
    SpvOpRayQueryGetIntersectionObjectRayOriginKHR = 6028,
    SpvOpRayQueryGetWorldRayDirectionKHR = 6029,
    SpvOpRayQueryGetWorldRayOriginKHR = 6030,
    SpvOpRayQueryGetIntersectionObjectToWorldKHR = 6031,
    SpvOpRayQueryGetIntersectionWorldToObjectKHR = 6032,
    SpvOpAtomicFAddEXT = 6035,
    SpvOpTypeBufferSurfaceINTEL = 6086,
    SpvOpTypeStructContinuedINTEL = 6090,
    SpvOpConstantCompositeContinuedINTEL = 6091,
    SpvOpSpecConstantCompositeContinuedINTEL = 6092,
    SpvOpMax = 0x7fffffff,
} SpvOp;

#ifdef SPV_ENABLE_UTILITY_CODE
inline void SpvHasResultAndType(SpvOp opcode, bool *hasResult, bool *hasResultType) {
    *hasResult = *hasResultType = false;
    switch (opcode) {
    default: /* unknown opcode */ break;
    case SpvOpNop: *hasResult = false; *hasResultType = false; break;
    case SpvOpUndef: *hasResult = true; *hasResultType = true; break;
    case SpvOpSourceContinued: *hasResult = false; *hasResultType = false; break;
    case SpvOpSource: *hasResult = false; *hasResultType = false; break;
    case SpvOpSourceExtension: *hasResult = false; *hasResultType = false; break;
    case SpvOpName: *hasResult = false; *hasResultType = false; break;
    case SpvOpMemberName: *hasResult = false; *hasResultType = false; break;
    case SpvOpString: *hasResult = true; *hasResultType = false; break;
    case SpvOpLine: *hasResult = false; *hasResultType = false; break;
    case SpvOpExtension: *hasResult = false; *hasResultType = false; break;
    case SpvOpExtInstImport: *hasResult = true; *hasResultType = false; break;
    case SpvOpExtInst: *hasResult = true; *hasResultType = true; break;
    case SpvOpMemoryModel: *hasResult = false; *hasResultType = false; break;
    case SpvOpEntryPoint: *hasResult = false; *hasResultType = false; break;
    case SpvOpExecutionMode: *hasResult = false; *hasResultType = false; break;
    case SpvOpCapability: *hasResult = false; *hasResultType = false; break;
    case SpvOpTypeVoid: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeBool: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeInt: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeFloat: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeVector: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeMatrix: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeImage: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeSampler: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeSampledImage: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeArray: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeRuntimeArray: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeStruct: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeOpaque: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypePointer: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeFunction: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeEvent: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeDeviceEvent: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeReserveId: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeQueue: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypePipe: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeForwardPointer: *hasResult = false; *hasResultType = false; break;
    case SpvOpConstantTrue: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstantFalse: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstant: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstantComposite: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstantSampler: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstantNull: *hasResult = true; *hasResultType = true; break;
    case SpvOpSpecConstantTrue: *hasResult = true; *hasResultType = true; break;
    case SpvOpSpecConstantFalse: *hasResult = true; *hasResultType = true; break;
    case SpvOpSpecConstant: *hasResult = true; *hasResultType = true; break;
    case SpvOpSpecConstantComposite: *hasResult = true; *hasResultType = true; break;
    case SpvOpSpecConstantOp: *hasResult = true; *hasResultType = true; break;
    case SpvOpFunction: *hasResult = true; *hasResultType = true; break;
    case SpvOpFunctionParameter: *hasResult = true; *hasResultType = true; break;
    case SpvOpFunctionEnd: *hasResult = false; *hasResultType = false; break;
    case SpvOpFunctionCall: *hasResult = true; *hasResultType = true; break;
    case SpvOpVariable: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageTexelPointer: *hasResult = true; *hasResultType = true; break;
    case SpvOpLoad: *hasResult = true; *hasResultType = true; break;
    case SpvOpStore: *hasResult = false; *hasResultType = false; break;
    case SpvOpCopyMemory: *hasResult = false; *hasResultType = false; break;
    case SpvOpCopyMemorySized: *hasResult = false; *hasResultType = false; break;
    case SpvOpAccessChain: *hasResult = true; *hasResultType = true; break;
    case SpvOpInBoundsAccessChain: *hasResult = true; *hasResultType = true; break;
    case SpvOpPtrAccessChain: *hasResult = true; *hasResultType = true; break;
    case SpvOpArrayLength: *hasResult = true; *hasResultType = true; break;
    case SpvOpGenericPtrMemSemantics: *hasResult = true; *hasResultType = true; break;
    case SpvOpInBoundsPtrAccessChain: *hasResult = true; *hasResultType = true; break;
    case SpvOpDecorate: *hasResult = false; *hasResultType = false; break;
    case SpvOpMemberDecorate: *hasResult = false; *hasResultType = false; break;
    case SpvOpDecorationGroup: *hasResult = true; *hasResultType = false; break;
    case SpvOpGroupDecorate: *hasResult = false; *hasResultType = false; break;
    case SpvOpGroupMemberDecorate: *hasResult = false; *hasResultType = false; break;
    case SpvOpVectorExtractDynamic: *hasResult = true; *hasResultType = true; break;
    case SpvOpVectorInsertDynamic: *hasResult = true; *hasResultType = true; break;
    case SpvOpVectorShuffle: *hasResult = true; *hasResultType = true; break;
    case SpvOpCompositeConstruct: *hasResult = true; *hasResultType = true; break;
    case SpvOpCompositeExtract: *hasResult = true; *hasResultType = true; break;
    case SpvOpCompositeInsert: *hasResult = true; *hasResultType = true; break;
    case SpvOpCopyObject: *hasResult = true; *hasResultType = true; break;
    case SpvOpTranspose: *hasResult = true; *hasResultType = true; break;
    case SpvOpSampledImage: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleDrefImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleDrefExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleProjImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleProjExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleProjDrefImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleProjDrefExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageFetch: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageGather: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageDrefGather: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageRead: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageWrite: *hasResult = false; *hasResultType = false; break;
    case SpvOpImage: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQueryFormat: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQueryOrder: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQuerySizeLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQuerySize: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQueryLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQueryLevels: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageQuerySamples: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertFToU: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertFToS: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertSToF: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertUToF: *hasResult = true; *hasResultType = true; break;
    case SpvOpUConvert: *hasResult = true; *hasResultType = true; break;
    case SpvOpSConvert: *hasResult = true; *hasResultType = true; break;
    case SpvOpFConvert: *hasResult = true; *hasResultType = true; break;
    case SpvOpQuantizeToF16: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertPtrToU: *hasResult = true; *hasResultType = true; break;
    case SpvOpSatConvertSToU: *hasResult = true; *hasResultType = true; break;
    case SpvOpSatConvertUToS: *hasResult = true; *hasResultType = true; break;
    case SpvOpConvertUToPtr: *hasResult = true; *hasResultType = true; break;
    case SpvOpPtrCastToGeneric: *hasResult = true; *hasResultType = true; break;
    case SpvOpGenericCastToPtr: *hasResult = true; *hasResultType = true; break;
    case SpvOpGenericCastToPtrExplicit: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitcast: *hasResult = true; *hasResultType = true; break;
    case SpvOpSNegate: *hasResult = true; *hasResultType = true; break;
    case SpvOpFNegate: *hasResult = true; *hasResultType = true; break;
    case SpvOpIAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpFAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpISub: *hasResult = true; *hasResultType = true; break;
    case SpvOpFSub: *hasResult = true; *hasResultType = true; break;
    case SpvOpIMul: *hasResult = true; *hasResultType = true; break;
    case SpvOpFMul: *hasResult = true; *hasResultType = true; break;
    case SpvOpUDiv: *hasResult = true; *hasResultType = true; break;
    case SpvOpSDiv: *hasResult = true; *hasResultType = true; break;
    case SpvOpFDiv: *hasResult = true; *hasResultType = true; break;
    case SpvOpUMod: *hasResult = true; *hasResultType = true; break;
    case SpvOpSRem: *hasResult = true; *hasResultType = true; break;
    case SpvOpSMod: *hasResult = true; *hasResultType = true; break;
    case SpvOpFRem: *hasResult = true; *hasResultType = true; break;
    case SpvOpFMod: *hasResult = true; *hasResultType = true; break;
    case SpvOpVectorTimesScalar: *hasResult = true; *hasResultType = true; break;
    case SpvOpMatrixTimesScalar: *hasResult = true; *hasResultType = true; break;
    case SpvOpVectorTimesMatrix: *hasResult = true; *hasResultType = true; break;
    case SpvOpMatrixTimesVector: *hasResult = true; *hasResultType = true; break;
    case SpvOpMatrixTimesMatrix: *hasResult = true; *hasResultType = true; break;
    case SpvOpOuterProduct: *hasResult = true; *hasResultType = true; break;
    case SpvOpDot: *hasResult = true; *hasResultType = true; break;
    case SpvOpIAddCarry: *hasResult = true; *hasResultType = true; break;
    case SpvOpISubBorrow: *hasResult = true; *hasResultType = true; break;
    case SpvOpUMulExtended: *hasResult = true; *hasResultType = true; break;
    case SpvOpSMulExtended: *hasResult = true; *hasResultType = true; break;
    case SpvOpAny: *hasResult = true; *hasResultType = true; break;
    case SpvOpAll: *hasResult = true; *hasResultType = true; break;
    case SpvOpIsNan: *hasResult = true; *hasResultType = true; break;
    case SpvOpIsInf: *hasResult = true; *hasResultType = true; break;
    case SpvOpIsFinite: *hasResult = true; *hasResultType = true; break;
    case SpvOpIsNormal: *hasResult = true; *hasResultType = true; break;
    case SpvOpSignBitSet: *hasResult = true; *hasResultType = true; break;
    case SpvOpLessOrGreater: *hasResult = true; *hasResultType = true; break;
    case SpvOpOrdered: *hasResult = true; *hasResultType = true; break;
    case SpvOpUnordered: *hasResult = true; *hasResultType = true; break;
    case SpvOpLogicalEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpLogicalNotEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpLogicalOr: *hasResult = true; *hasResultType = true; break;
    case SpvOpLogicalAnd: *hasResult = true; *hasResultType = true; break;
    case SpvOpLogicalNot: *hasResult = true; *hasResultType = true; break;
    case SpvOpSelect: *hasResult = true; *hasResultType = true; break;
    case SpvOpIEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpINotEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpUGreaterThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpSGreaterThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpUGreaterThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpSGreaterThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpULessThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpSLessThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpULessThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpSLessThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdNotEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordNotEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdLessThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordLessThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdGreaterThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordGreaterThan: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdLessThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordLessThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFOrdGreaterThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpFUnordGreaterThanEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpShiftRightLogical: *hasResult = true; *hasResultType = true; break;
    case SpvOpShiftRightArithmetic: *hasResult = true; *hasResultType = true; break;
    case SpvOpShiftLeftLogical: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitwiseOr: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitwiseXor: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitwiseAnd: *hasResult = true; *hasResultType = true; break;
    case SpvOpNot: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitFieldInsert: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitFieldSExtract: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitFieldUExtract: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitReverse: *hasResult = true; *hasResultType = true; break;
    case SpvOpBitCount: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdx: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdy: *hasResult = true; *hasResultType = true; break;
    case SpvOpFwidth: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdxFine: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdyFine: *hasResult = true; *hasResultType = true; break;
    case SpvOpFwidthFine: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdxCoarse: *hasResult = true; *hasResultType = true; break;
    case SpvOpDPdyCoarse: *hasResult = true; *hasResultType = true; break;
    case SpvOpFwidthCoarse: *hasResult = true; *hasResultType = true; break;
    case SpvOpEmitVertex: *hasResult = false; *hasResultType = false; break;
    case SpvOpEndPrimitive: *hasResult = false; *hasResultType = false; break;
    case SpvOpEmitStreamVertex: *hasResult = false; *hasResultType = false; break;
    case SpvOpEndStreamPrimitive: *hasResult = false; *hasResultType = false; break;
    case SpvOpControlBarrier: *hasResult = false; *hasResultType = false; break;
    case SpvOpMemoryBarrier: *hasResult = false; *hasResultType = false; break;
    case SpvOpAtomicLoad: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicStore: *hasResult = false; *hasResultType = false; break;
    case SpvOpAtomicExchange: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicCompareExchange: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicCompareExchangeWeak: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicIIncrement: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicIDecrement: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicIAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicISub: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicSMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicUMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicSMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicUMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicAnd: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicOr: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicXor: *hasResult = true; *hasResultType = true; break;
    case SpvOpPhi: *hasResult = true; *hasResultType = true; break;
    case SpvOpLoopMerge: *hasResult = false; *hasResultType = false; break;
    case SpvOpSelectionMerge: *hasResult = false; *hasResultType = false; break;
    case SpvOpLabel: *hasResult = true; *hasResultType = false; break;
    case SpvOpBranch: *hasResult = false; *hasResultType = false; break;
    case SpvOpBranchConditional: *hasResult = false; *hasResultType = false; break;
    case SpvOpSwitch: *hasResult = false; *hasResultType = false; break;
    case SpvOpKill: *hasResult = false; *hasResultType = false; break;
    case SpvOpReturn: *hasResult = false; *hasResultType = false; break;
    case SpvOpReturnValue: *hasResult = false; *hasResultType = false; break;
    case SpvOpUnreachable: *hasResult = false; *hasResultType = false; break;
    case SpvOpLifetimeStart: *hasResult = false; *hasResultType = false; break;
    case SpvOpLifetimeStop: *hasResult = false; *hasResultType = false; break;
    case SpvOpGroupAsyncCopy: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupWaitEvents: *hasResult = false; *hasResultType = false; break;
    case SpvOpGroupAll: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupAny: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupBroadcast: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupIAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupUMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupSMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupUMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupSMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpReadPipe: *hasResult = true; *hasResultType = true; break;
    case SpvOpWritePipe: *hasResult = true; *hasResultType = true; break;
    case SpvOpReservedReadPipe: *hasResult = true; *hasResultType = true; break;
    case SpvOpReservedWritePipe: *hasResult = true; *hasResultType = true; break;
    case SpvOpReserveReadPipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpReserveWritePipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpCommitReadPipe: *hasResult = false; *hasResultType = false; break;
    case SpvOpCommitWritePipe: *hasResult = false; *hasResultType = false; break;
    case SpvOpIsValidReserveId: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetNumPipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetMaxPipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupReserveReadPipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupReserveWritePipePackets: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupCommitReadPipe: *hasResult = false; *hasResultType = false; break;
    case SpvOpGroupCommitWritePipe: *hasResult = false; *hasResultType = false; break;
    case SpvOpEnqueueMarker: *hasResult = true; *hasResultType = true; break;
    case SpvOpEnqueueKernel: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelNDrangeSubGroupCount: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelNDrangeMaxSubGroupSize: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelWorkGroupSize: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelPreferredWorkGroupSizeMultiple: *hasResult = true; *hasResultType = true; break;
    case SpvOpRetainEvent: *hasResult = false; *hasResultType = false; break;
    case SpvOpReleaseEvent: *hasResult = false; *hasResultType = false; break;
    case SpvOpCreateUserEvent: *hasResult = true; *hasResultType = true; break;
    case SpvOpIsValidEvent: *hasResult = true; *hasResultType = true; break;
    case SpvOpSetUserEventStatus: *hasResult = false; *hasResultType = false; break;
    case SpvOpCaptureEventProfilingInfo: *hasResult = false; *hasResultType = false; break;
    case SpvOpGetDefaultQueue: *hasResult = true; *hasResultType = true; break;
    case SpvOpBuildNDRange: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleDrefImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleDrefExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleProjImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleProjExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleProjDrefImplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseSampleProjDrefExplicitLod: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseFetch: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseGather: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseDrefGather: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSparseTexelsResident: *hasResult = true; *hasResultType = true; break;
    case SpvOpNoLine: *hasResult = false; *hasResultType = false; break;
    case SpvOpAtomicFlagTestAndSet: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicFlagClear: *hasResult = false; *hasResultType = false; break;
    case SpvOpImageSparseRead: *hasResult = true; *hasResultType = true; break;
    case SpvOpSizeOf: *hasResult = true; *hasResultType = true; break;
    case SpvOpTypePipeStorage: *hasResult = true; *hasResultType = false; break;
    case SpvOpConstantPipeStorage: *hasResult = true; *hasResultType = true; break;
    case SpvOpCreatePipeFromPipeStorage: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelLocalSizeForSubgroupCount: *hasResult = true; *hasResultType = true; break;
    case SpvOpGetKernelMaxNumSubgroups: *hasResult = true; *hasResultType = true; break;
    case SpvOpTypeNamedBarrier: *hasResult = true; *hasResultType = false; break;
    case SpvOpNamedBarrierInitialize: *hasResult = true; *hasResultType = true; break;
    case SpvOpMemoryNamedBarrier: *hasResult = false; *hasResultType = false; break;
    case SpvOpModuleProcessed: *hasResult = false; *hasResultType = false; break;
    case SpvOpExecutionModeId: *hasResult = false; *hasResultType = false; break;
    case SpvOpDecorateId: *hasResult = false; *hasResultType = false; break;
    case SpvOpGroupNonUniformElect: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformAll: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformAny: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformAllEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBroadcast: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBroadcastFirst: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBallot: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformInverseBallot: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBallotBitExtract: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBallotBitCount: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBallotFindLSB: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBallotFindMSB: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformShuffle: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformShuffleXor: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformShuffleUp: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformShuffleDown: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformIAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformFAdd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformIMul: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformFMul: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformSMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformUMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformFMin: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformSMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformUMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformFMax: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBitwiseAnd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBitwiseOr: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformBitwiseXor: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformLogicalAnd: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformLogicalOr: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformLogicalXor: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformQuadBroadcast: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformQuadSwap: *hasResult = true; *hasResultType = true; break;
    case SpvOpCopyLogical: *hasResult = true; *hasResultType = true; break;
    case SpvOpPtrEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpPtrNotEqual: *hasResult = true; *hasResultType = true; break;
    case SpvOpPtrDiff: *hasResult = true; *hasResultType = true; break;
    case SpvOpTerminateInvocation: *hasResult = false; *hasResultType = false; break;
    case SpvOpSubgroupBallotKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupFirstInvocationKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAllKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAnyKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAllEqualKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupReadInvocationKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpTraceRayKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpExecuteCallableKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpConvertUToAccelerationStructureKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpIgnoreIntersectionKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpTerminateRayKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpTypeRayQueryKHR: *hasResult = true; *hasResultType = false; break;
    case SpvOpRayQueryInitializeKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpRayQueryTerminateKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpRayQueryGenerateIntersectionKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpRayQueryConfirmIntersectionKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpRayQueryProceedKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionTypeKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupIAddNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFAddNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFMinNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupUMinNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupSMinNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupFMaxNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupUMaxNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupSMaxNonUniformAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpFragmentMaskFetchAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpFragmentFetchAMD: *hasResult = true; *hasResultType = true; break;
    case SpvOpReadClockKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpImageSampleFootprintNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpGroupNonUniformPartitionNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpWritePackedPrimitiveIndices4x8NV: *hasResult = false; *hasResultType = false; break;
    case SpvOpReportIntersectionNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpIgnoreIntersectionNV: *hasResult = false; *hasResultType = false; break;
    case SpvOpTerminateRayNV: *hasResult = false; *hasResultType = false; break;
    case SpvOpTraceNV: *hasResult = false; *hasResultType = false; break;
    case SpvOpTypeAccelerationStructureNV: *hasResult = true; *hasResultType = false; break;
    case SpvOpExecuteCallableNV: *hasResult = false; *hasResultType = false; break;
    case SpvOpTypeCooperativeMatrixNV: *hasResult = true; *hasResultType = false; break;
    case SpvOpCooperativeMatrixLoadNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpCooperativeMatrixStoreNV: *hasResult = false; *hasResultType = false; break;
    case SpvOpCooperativeMatrixMulAddNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpCooperativeMatrixLengthNV: *hasResult = true; *hasResultType = true; break;
    case SpvOpBeginInvocationInterlockEXT: *hasResult = false; *hasResultType = false; break;
    case SpvOpEndInvocationInterlockEXT: *hasResult = false; *hasResultType = false; break;
    case SpvOpDemoteToHelperInvocationEXT: *hasResult = false; *hasResultType = false; break;
    case SpvOpIsHelperInvocationEXT: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupShuffleINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupShuffleDownINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupShuffleUpINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupShuffleXorINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupBlockReadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupBlockWriteINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpSubgroupImageBlockReadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupImageBlockWriteINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpSubgroupImageMediaBlockReadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupImageMediaBlockWriteINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpUCountLeadingZerosINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUCountTrailingZerosINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAbsISubINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAbsUSubINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpIAddSatINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUAddSatINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpIAverageINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUAverageINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpIAverageRoundedINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUAverageRoundedINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpISubSatINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUSubSatINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpIMul32x16INTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpUMul32x16INTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpConstFunctionPointerINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpFunctionPointerCallINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAsmTargetINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAsmINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAsmCallINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicFMinEXT: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicFMaxEXT: *hasResult = true; *hasResultType = true; break;
    case SpvOpAssumeTrueKHR: *hasResult = false; *hasResultType = false; break;
    case SpvOpExpectKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpDecorateString: *hasResult = false; *hasResultType = false; break;
    case SpvOpMemberDecorateString: *hasResult = false; *hasResultType = false; break;
    case SpvOpVmeImageINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpTypeVmeImageINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImePayloadINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcRefPayloadINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcSicPayloadINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcMcePayloadINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcMceResultINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImeResultINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImeResultSingleReferenceStreamoutINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImeResultDualReferenceStreamoutINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImeSingleReferenceStreaminINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcImeDualReferenceStreaminINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcRefResultINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeAvcSicResultINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpSubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultInterShapePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetInterShapePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetInterDirectionPenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetMotionVectorCostFunctionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetAcOnlyHaarINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToImePayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToImeResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToRefPayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToRefResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToSicPayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceConvertToSicResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetMotionVectorsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterDistortionsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetBestInterDistortionsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterMajorShapeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterMinorShapeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterDirectionsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterMotionVectorCountINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterReferenceIdsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeInitializeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetSingleReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetDualReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeRefWindowSizeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeAdjustRefOffsetINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeConvertToMcePayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetMaxMotionVectorCountINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetUnidirectionalMixDisableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetEarlySearchTerminationThresholdINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeSetWeightedSadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithSingleReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithDualReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeConvertToMceResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetSingleReferenceStreaminINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetDualReferenceStreaminINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeStripSingleReferenceStreamoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeStripDualReferenceStreamoutINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetBorderReachedINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetTruncatedSearchIndicationINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcFmeInitializeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcBmeInitializeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefConvertToMcePayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefSetBidirectionalMixDisableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefSetBilinearFilterEnableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefEvaluateWithSingleReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefEvaluateWithDualReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefEvaluateWithMultiReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcRefConvertToMceResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicInitializeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicConfigureSkcINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicConfigureIpeLumaINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicConfigureIpeLumaChromaINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetMotionVectorMaskINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicConvertToMcePayloadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetIntraLumaShapePenaltyINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetIntraLumaModeCostFunctionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetIntraChromaModeCostFunctionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetBilinearFilterEnableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetSkcForwardTransformEnableINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicSetBlockBasedRawSkipSadINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicEvaluateIpeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicEvaluateWithSingleReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicEvaluateWithDualReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicEvaluateWithMultiReferenceINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicConvertToMceResultINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetIpeLumaShapeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetBestIpeLumaDistortionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetBestIpeChromaDistortionINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetPackedIpeLumaModesINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetIpeChromaModeINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSubgroupAvcSicGetInterRawSadsINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpVariableLengthArrayINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpSaveMemoryINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpRestoreMemoryINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpLoopControlINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpPtrCastToCrossWorkgroupINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpCrossWorkgroupCastToPtrINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpReadPipeBlockingINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpWritePipeBlockingINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpFPGARegINTEL: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetRayTMinKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetRayFlagsKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionTKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionInstanceCustomIndexKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionInstanceIdKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionGeometryIndexKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionPrimitiveIndexKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionBarycentricsKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionFrontFaceKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionCandidateAABBOpaqueKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionObjectRayDirectionKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionObjectRayOriginKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetWorldRayDirectionKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetWorldRayOriginKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionObjectToWorldKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpRayQueryGetIntersectionWorldToObjectKHR: *hasResult = true; *hasResultType = true; break;
    case SpvOpAtomicFAddEXT: *hasResult = true; *hasResultType = true; break;
    case SpvOpTypeBufferSurfaceINTEL: *hasResult = true; *hasResultType = false; break;
    case SpvOpTypeStructContinuedINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpConstantCompositeContinuedINTEL: *hasResult = false; *hasResultType = false; break;
    case SpvOpSpecConstantCompositeContinuedINTEL: *hasResult = false; *hasResultType = false; break;
    }
}
#endif /* SPV_ENABLE_UTILITY_CODE */

#endif


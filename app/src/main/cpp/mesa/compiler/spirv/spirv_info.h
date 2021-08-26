#ifndef _SPIRV_INFO_H_
#define _SPIRV_INFO_H_

#include "spirv.h"

const char *spirv_addressingmodel_to_string(SpvAddressingModel model);
const char *spirv_builtin_to_string(SpvBuiltIn builtin);
const char *spirv_capability_to_string(SpvCapability cap);
const char *spirv_decoration_to_string(SpvDecoration dec);
const char *spirv_dim_to_string(SpvDim dim);
const char *spirv_executionmode_to_string(SpvExecutionMode mode);
const char *spirv_executionmodel_to_string(SpvExecutionModel model);
const char *spirv_imageformat_to_string(SpvImageFormat format);
const char *spirv_imageoperands_to_string(SpvImageOperandsMask op);
const char *spirv_memorymodel_to_string(SpvMemoryModel cap);
const char *spirv_op_to_string(SpvOp op);
const char *spirv_storageclass_to_string(SpvStorageClass sc);
const char *spirv_fproundingmode_to_string(SpvFPRoundingMode sc);

#endif /* SPIRV_INFO_H */

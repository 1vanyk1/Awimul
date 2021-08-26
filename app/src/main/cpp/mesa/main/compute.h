#ifndef COMPUTE_H
#define COMPUTE_H


#include "glheader.h"

extern void GLAPIENTRY
_mesa_DispatchCompute_no_error(GLuint num_groups_x, GLuint num_groups_y,
                               GLuint num_groups_z);
extern void GLAPIENTRY
_mesa_DispatchCompute(GLuint num_groups_x,
                      GLuint num_groups_y,
                      GLuint num_groups_z);

extern void GLAPIENTRY
_mesa_DispatchComputeIndirect_no_error(GLintptr indirect);
extern void GLAPIENTRY
_mesa_DispatchComputeIndirect(GLintptr indirect);

extern void GLAPIENTRY
_mesa_DispatchComputeGroupSizeARB_no_error(GLuint num_groups_x,
                                           GLuint num_groups_y,
                                           GLuint num_groups_z,
                                           GLuint group_size_x,
                                           GLuint group_size_y,
                                           GLuint group_size_z);
extern void GLAPIENTRY
_mesa_DispatchComputeGroupSizeARB(GLuint num_groups_x, GLuint num_groups_y,
                                  GLuint num_groups_z, GLuint group_size_x,
                                  GLuint group_size_y, GLuint group_size_z);

#endif

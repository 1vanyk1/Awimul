#ifndef DRI_DEBUG_H
#define DRI_DEBUG_H

#include "../../../../GL/gl.h"
#include "../../../../GL/internal/dri_interface.h"
#include "../../../main/context.h"

struct __DRIconfigRec {
    struct gl_config modes;
};

extern __DRIconfig **
driCreateConfigs(mesa_format format,
                 const uint8_t * depth_bits, const uint8_t * stencil_bits,
                 unsigned num_depth_stencil_bits,
                 const GLenum * db_modes, unsigned num_db_modes,
                 const uint8_t * msaa_samples, unsigned num_msaa_modes,
                 GLboolean enable_accum, GLboolean color_depth_match);

__DRIconfig **driConcatConfigs(__DRIconfig **a,
                               __DRIconfig **b);

int
driGetConfigAttrib(const __DRIconfig *config,
                   unsigned int attrib, unsigned int *value);
int
driIndexConfigAttrib(const __DRIconfig *config, int index,
                     unsigned int *attrib, unsigned int *value);

int
driQueryRendererIntegerCommon(__DRIscreen *psp, int param, unsigned int *value);

#endif /* DRI_DEBUG_H */

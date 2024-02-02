#ifndef VIRGLRENDERER_HW_H
#define VIRGLRENDERER_HW_H

#include "venus_hw.h"
#include "virgl_hw.h"

#ifdef VIRGL_RENDERER_UNSTABLE_APIS
enum virgl_renderer_capset {
   VIRGL_RENDERER_CAPSET_VIRGL                   = 1,
   VIRGL_RENDERER_CAPSET_VIRGL2                  = 2,
   /* 3 is reserved for gfxstream */
   VIRGL_RENDERER_CAPSET_VENUS                   = 4,
};
#endif

#endif /* VIRGLRENDERER_HW_H */

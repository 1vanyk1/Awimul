#ifndef BARRIER_H
#define BARRIER_H

#include "glheader.h"

struct dd_function_table;

extern void
_mesa_init_barrier_functions(struct dd_function_table *driver);

extern void GLAPIENTRY
_mesa_TextureBarrierNV(void);

void GLAPIENTRY
_mesa_MemoryBarrier(GLbitfield barriers);

void GLAPIENTRY
_mesa_MemoryBarrierByRegion_no_error(GLbitfield barriers);

void GLAPIENTRY
_mesa_MemoryBarrierByRegion(GLbitfield barriers);

void GLAPIENTRY
_mesa_BlendBarrier(void);

void GLAPIENTRY
_mesa_FramebufferFetchBarrierEXT(void);

#endif /* BARRIER_H */

#ifndef _VTXFMT_H_
#define _VTXFMT_H_

#include "dd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;

extern void _mesa_install_exec_vtxfmt( struct gl_context *ctx, const GLvertexformat *vfmt );
extern void _mesa_install_save_vtxfmt( struct gl_context *ctx, const GLvertexformat *vfmt );
extern void _mesa_initialize_vbo_vtxfmt(struct gl_context *ctx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _VTXFMT_H_ */

#ifndef VBO_NOOP_H
#define VBO_NOOP_H

#include "../main/dd.h"

struct _glapi_table;

extern void
_mesa_noop_vtxfmt_init(struct gl_context *ctx, GLvertexformat *vfmt);

extern GLboolean
_mesa_using_noop_vtxfmt(const struct _glapi_table *dispatch);


#endif /* VBO_NOOP_H */

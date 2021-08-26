#ifndef VBO_EXEC_H
#define VBO_EXEC_H

#include "../main/dd.h"
#include "../main/mesa_private.h"
#include "vbo.h"
#include "vbo_attrib.h"

void
vbo_exec_init(struct gl_context *ctx, bool use_buffer_objects);

void
vbo_exec_destroy(struct gl_context *ctx);

void
vbo_exec_vtx_init(struct vbo_exec_context *exec, bool use_buffer_objects);

void
vbo_exec_vtx_destroy(struct vbo_exec_context *exec);

void
vbo_exec_vtx_flush(struct vbo_exec_context *exec);

void
vbo_exec_vtx_map(struct vbo_exec_context *exec);

void
vbo_exec_eval_update(struct vbo_exec_context *exec);

void
vbo_exec_do_EvalCoord2f(struct vbo_exec_context *exec, GLfloat u, GLfloat v);

void
vbo_exec_do_EvalCoord1f(struct vbo_exec_context *exec, GLfloat u);

#endif

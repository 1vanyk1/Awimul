#ifndef S_ZOOM_H
#define S_ZOOM_H

#include "../main/mtypes.h"
#include "s_span.h"


extern void
_swrast_write_zoomed_rgba_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                               const SWspan *span, const GLvoid *rgba);

extern void
_swrast_write_zoomed_rgb_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                              const SWspan *span, const GLvoid *rgb);

extern void
_swrast_write_zoomed_depth_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                                const SWspan *span);


extern void
_swrast_write_zoomed_stencil_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                                  GLint width, GLint spanX, GLint spanY,
                                  const GLubyte stencil[]);

extern void
_swrast_write_zoomed_z_span(struct gl_context *ctx, GLint imgX, GLint imgY,
                            GLint width, GLint spanX, GLint spanY,
                            const GLuint *zVals);


#endif

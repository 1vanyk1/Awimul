#ifndef BLIT_H
#define BLIT_H

#include "glheader.h"

extern bool
_mesa_regions_overlap(int srcX0, int srcY0,
                      int srcX1, int srcY1,
                      int dstX0, int dstY0,
                      int dstX1, int dstY1);

void GLAPIENTRY
_mesa_BlitFramebuffer_no_error(GLint srcX0, GLint srcY0, GLint srcX1,
                               GLint srcY1, GLint dstX0, GLint dstY0,
                               GLint dstX1, GLint dstY1,
                               GLbitfield mask, GLenum filter);

extern void GLAPIENTRY
_mesa_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                      GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                      GLbitfield mask, GLenum filter);

void GLAPIENTRY
_mesa_BlitNamedFramebuffer_no_error(GLuint readFramebuffer,
                                    GLuint drawFramebuffer,
                                    GLint srcX0, GLint srcY0,
                                    GLint srcX1, GLint srcY1,
                                    GLint dstX0, GLint dstY0,
                                    GLint dstX1, GLint dstY1,
                                    GLbitfield mask, GLenum filter);

extern void GLAPIENTRY
_mesa_BlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer,
                           GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                           GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                           GLbitfield mask, GLenum filter);


#endif /* BLIT_H */

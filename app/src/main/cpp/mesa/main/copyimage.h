#ifndef COPYIMAGE_H
#define COPYIMAGE_H

#include "glheader.h"

#ifdef __cplusplus
extern "C" {
#endif

void GLAPIENTRY
_mesa_CopyImageSubData_no_error(GLuint srcName, GLenum srcTarget, GLint srcLevel,
                                GLint srcX, GLint srcY, GLint srcZ,
                                GLuint destName, GLenum destTarget, GLint destLevel,
                                GLint destX, GLint destY, GLint destZ,
                                GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

extern void GLAPIENTRY
_mesa_CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel,
                       GLint srcX, GLint srcY, GLint srcZ,
                       GLuint destName, GLenum destTarget, GLint destLevel,
                       GLint destX, GLint destY, GLint destZ,
                       GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

void GLAPIENTRY
_mesa_CopyImageSubDataNV_no_error(GLuint srcName, GLenum srcTarget, GLint srcLevel,
                                  GLint srcX, GLint srcY, GLint srcZ,
                                  GLuint destName, GLenum destTarget, GLint destLevel,
                                  GLint destX, GLint destY, GLint destZ,
                                  GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

extern void GLAPIENTRY
_mesa_CopyImageSubDataNV(GLuint srcName, GLenum srcTarget, GLint srcLevel,
                         GLint srcX, GLint srcY, GLint srcZ,
                         GLuint destName, GLenum destTarget, GLint destLevel,
                         GLint destX, GLint destY, GLint destZ,
                         GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

#ifdef __cplusplus
}
#endif

#endif /* COPYIMAGE_H */

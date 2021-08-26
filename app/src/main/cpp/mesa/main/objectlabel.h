#ifndef OBJECTLABEL_H
#define OBJECTLABEL_H


#include "glheader.h"


#ifdef __cplusplus
extern "C" {
#endif

void GLAPIENTRY
_mesa_ObjectLabel(GLenum identifier, GLuint name, GLsizei length,
                  const GLchar *label);
void GLAPIENTRY
_mesa_GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize,
                     GLsizei *length, GLchar *label);
void GLAPIENTRY
_mesa_ObjectPtrLabel(const void *ptr, GLsizei length, const GLchar *label);
void GLAPIENTRY
_mesa_GetObjectPtrLabel(const void *ptr, GLsizei bufSize, GLsizei *length,
                        GLchar *label);

#ifdef __cplusplus
}
#endif


#endif /* OBJECTLABEL_H */

#ifndef ARBPROGRAM_H
#define ARBPROGRAM_H


#include "glheader.h"


extern void GLAPIENTRY
_mesa_BindProgramARB(GLenum target, GLuint id);

extern void GLAPIENTRY
_mesa_DeleteProgramsARB(GLsizei n, const GLuint *ids);

extern void GLAPIENTRY
_mesa_GenProgramsARB(GLsizei n, GLuint *ids);


extern GLboolean GLAPIENTRY
_mesa_IsProgramARB(GLuint id);


extern void GLAPIENTRY
_mesa_ProgramStringARB(GLenum target, GLenum format, GLsizei len,
                       const GLvoid *string);

extern void GLAPIENTRY
_mesa_NamedProgramStringEXT(GLuint program, GLenum target, GLenum format,
                            GLsizei len, const GLvoid *string);

extern void GLAPIENTRY
_mesa_ProgramEnvParameter4dARB(GLenum target, GLuint index,
                               GLdouble x, GLdouble y, GLdouble z, GLdouble w);


extern void GLAPIENTRY
_mesa_ProgramEnvParameter4dvARB(GLenum target, GLuint index,
                                const GLdouble *params);


extern void GLAPIENTRY
_mesa_ProgramEnvParameter4fARB(GLenum target, GLuint index,
                               GLfloat x, GLfloat y, GLfloat z, GLfloat w);


extern void GLAPIENTRY
_mesa_ProgramEnvParameter4fvARB(GLenum target, GLuint index,
                                const GLfloat *params);


extern void GLAPIENTRY
_mesa_ProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count,
                                 const GLfloat *params);


extern void GLAPIENTRY
_mesa_ProgramLocalParameter4dARB(GLenum target, GLuint index,
                                 GLdouble x, GLdouble y,
                                 GLdouble z, GLdouble w);


extern void GLAPIENTRY
_mesa_ProgramLocalParameter4dvARB(GLenum target, GLuint index,
                                  const GLdouble *params);


extern void GLAPIENTRY
_mesa_ProgramLocalParameter4fARB(GLenum target, GLuint index,
                                 GLfloat x, GLfloat y, GLfloat z, GLfloat w);


extern void GLAPIENTRY
_mesa_NamedProgramLocalParameter4fvEXT(GLuint program, GLenum target,
                                       GLuint index, const GLfloat *params);

extern void GLAPIENTRY
_mesa_NamedProgramLocalParameter4dEXT(GLuint program, GLenum target,
                                      GLuint index, GLdouble x, GLdouble y,
                                      GLdouble z, GLdouble w);

extern void GLAPIENTRY
_mesa_NamedProgramLocalParameter4dvEXT(GLuint program, GLenum target,
                                       GLuint index, const GLdouble *params);


extern void GLAPIENTRY
_mesa_NamedProgramLocalParameter4fEXT(GLuint program, GLenum target,
                                      GLuint index, GLfloat x, GLfloat y,
                                      GLfloat z, GLfloat w);


extern void GLAPIENTRY
_mesa_ProgramLocalParameter4fvARB(GLenum target, GLuint index,
                                  const GLfloat *params);

extern void GLAPIENTRY
_mesa_ProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count,
                                   const GLfloat *params);

extern void GLAPIENTRY
_mesa_NamedProgramLocalParameters4fvEXT(GLuint program, GLenum target, GLuint index,
                                        GLsizei count, const GLfloat *params);

extern void GLAPIENTRY
_mesa_GetProgramEnvParameterdvARB(GLenum target, GLuint index,
                                  GLdouble *params);


extern void GLAPIENTRY
_mesa_GetProgramEnvParameterfvARB(GLenum target, GLuint index,
                                  GLfloat *params);


extern void GLAPIENTRY
_mesa_GetProgramLocalParameterdvARB(GLenum target, GLuint index,
                                    GLdouble *params);

extern void GLAPIENTRY
_mesa_GetNamedProgramLocalParameterdvEXT(GLuint program, GLenum target,
                                         GLuint index, GLdouble *params);

extern void GLAPIENTRY
_mesa_GetProgramLocalParameterfvARB(GLenum target, GLuint index,
                                    GLfloat *params);

extern void GLAPIENTRY
_mesa_GetNamedProgramLocalParameterfvEXT(GLuint program, GLenum target,
                                         GLuint index, GLfloat *params);

extern void GLAPIENTRY
_mesa_GetProgramivARB(GLenum target, GLenum pname, GLint *params);

extern void GLAPIENTRY
_mesa_GetNamedProgramivEXT(GLuint program, GLenum target, GLenum pname,
                           GLint *params);

extern void GLAPIENTRY
_mesa_GetProgramStringARB(GLenum target, GLenum pname, GLvoid *string);

extern void GLAPIENTRY
_mesa_GetNamedProgramStringEXT(GLuint program, GLenum target,
                               GLenum pname, GLvoid *string);

#endif

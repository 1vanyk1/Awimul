#ifndef VDPAU_H
#define VDPAU_H

extern void GLAPIENTRY
_mesa_VDPAUInitNV(const GLvoid *vdpDevice, const GLvoid *getProcAddress);

extern void GLAPIENTRY
_mesa_VDPAUFiniNV(void);

extern GLintptr GLAPIENTRY
_mesa_VDPAURegisterVideoSurfaceNV(const GLvoid *vdpSurface, GLenum target,
                                  GLsizei numTextureNames,
                                  const GLuint *textureNames);

extern GLintptr GLAPIENTRY
_mesa_VDPAURegisterOutputSurfaceNV(const GLvoid *vdpSurface, GLenum target,
                                   GLsizei numTextureNames,
                                   const GLuint *textureNames);

extern GLboolean GLAPIENTRY
_mesa_VDPAUIsSurfaceNV(GLintptr surface);

extern void GLAPIENTRY
_mesa_VDPAUUnregisterSurfaceNV(GLintptr surface);

extern void GLAPIENTRY
_mesa_VDPAUGetSurfaceivNV(GLintptr surface, GLenum pname, GLsizei bufSize,
GLsizei *length, GLint *values);

extern void GLAPIENTRY
_mesa_VDPAUSurfaceAccessNV(GLintptr surface, GLenum access);

extern void GLAPIENTRY
_mesa_VDPAUMapSurfacesNV(GLsizei numSurfaces, const GLintptr *surfaces);

extern void GLAPIENTRY
_mesa_VDPAUUnmapSurfacesNV(GLsizei numSurfaces, const GLintptr *surfaces);

#endif /* VDPAU_H */

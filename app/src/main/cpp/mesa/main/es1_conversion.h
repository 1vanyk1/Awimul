#ifndef ES1_CONVERSION_H
#define ES1_CONVERSION_H

#ifndef GL_APIENTRY
#define GL_APIENTRY GLAPIENTRY
#endif

void GL_APIENTRY
_mesa_AlphaFuncx(GLenum func, GLclampx ref);

void GL_APIENTRY
_mesa_ClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);

void GL_APIENTRY
_mesa_ClearDepthx(GLclampx depth);

void GL_APIENTRY
_mesa_ClipPlanef(GLenum plane, const GLfloat *equation);

void GL_APIENTRY
_mesa_ClipPlanex(GLenum plane, const GLfixed *equation);

void GL_APIENTRY
_es_Color4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);

void GL_APIENTRY
_mesa_Color4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);

void GL_APIENTRY
_mesa_DepthRangex(GLclampx zNear, GLclampx zFar);

void GL_APIENTRY
_mesa_DrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed w, GLfixed h);

void GL_APIENTRY
_mesa_DrawTexxvOES(const GLfixed *coords);

void GL_APIENTRY
_mesa_Fogx(GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_Fogxv(GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_Frustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
        GLfloat zNear, GLfloat zFar);

void GL_APIENTRY
_mesa_Frustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
        GLfixed zNear, GLfixed zFar);

void GL_APIENTRY
_mesa_GetClipPlanef(GLenum plane, GLfloat *equation);

void GL_APIENTRY
_mesa_GetClipPlanex(GLenum plane, GLfixed *equation);

void GL_APIENTRY
_mesa_GetLightxv(GLenum light, GLenum pname, GLfixed *params);

void GL_APIENTRY
_mesa_GetMaterialxv(GLenum face, GLenum pname, GLfixed *params);

void GL_APIENTRY
_check_GetTexGenivOES(GLenum coord, GLenum pname, GLint *params);

void GL_APIENTRY
_mesa_GetTexEnvxv(GLenum target, GLenum pname, GLfixed *params);

void GL_APIENTRY
_mesa_GetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params);

void GL_APIENTRY
_mesa_GetTexParameterxv(GLenum target, GLenum pname, GLfixed *params);

void GL_APIENTRY
_mesa_LightModelx(GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_LightModelxv(GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_Lightx(GLenum light, GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_Lightxv(GLenum light, GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_LineWidthx(GLfixed width);

void GL_APIENTRY
_mesa_LoadMatrixx(const GLfixed *m);

void GL_APIENTRY
_mesa_Materialx(GLenum face, GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_Materialxv(GLenum face, GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_MultMatrixx(const GLfixed *m);

void GL_APIENTRY
_mesa_MultiTexCoord4x(GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q);

void GL_APIENTRY
_mesa_Normal3x(GLfixed nx, GLfixed ny, GLfixed nz);

void GL_APIENTRY
_mesa_Orthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
        GLfloat zNear, GLfloat zFar);

void GL_APIENTRY
_mesa_Orthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
        GLfixed zNear, GLfixed zFar);

void GL_APIENTRY
_mesa_PointParameterx(GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_PointParameterxv(GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_PointSizex(GLfixed size);

void GL_APIENTRY
_mesa_PolygonOffsetx(GLfixed factor, GLfixed units);

void GL_APIENTRY
_mesa_Rotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);

void GL_APIENTRY
_mesa_SampleCoveragex(GLclampx value, GLboolean invert);

void GL_APIENTRY
_mesa_Scalex(GLfixed x, GLfixed y, GLfixed z);

void GL_APIENTRY
_mesa_TexEnvx(GLenum target, GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_TexEnvxv(GLenum target, GLenum pname, const GLfixed *params);

void GL_APIENTRY
_check_TexGeniOES(GLenum coord, GLenum pname, GLint param);

void GL_APIENTRY
_check_TexGenivOES(GLenum coord, GLenum pname, const GLint *params);

void GL_APIENTRY
_mesa_TexGenxOES(GLenum coord, GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_TexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_TexParameterx(GLenum target, GLenum pname, GLfixed param);

void GL_APIENTRY
_mesa_TexParameterxv(GLenum target, GLenum pname, const GLfixed *params);

void GL_APIENTRY
_mesa_Translatex(GLfixed x, GLfixed y, GLfixed z);

#endif /* ES1_CONVERSION_H */

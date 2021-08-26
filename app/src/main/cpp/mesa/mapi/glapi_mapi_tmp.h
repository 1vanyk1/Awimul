#ifndef _GLAPI_TMP_H_
#define _GLAPI_TMP_H_
#define GLAPI_PREFIX(func)  gl##func
#define GLAPI_PREFIX_STR(func)  "gl"#func

typedef int GLclampx;
#endif /* _GLAPI_TMP_H_ */

#ifdef MAPI_TMP_DEFINES
#define GL_GLEXT_PROTOTYPES
#include "../../GL/gl.h"
#include "../../GL/glext.h"

#ifdef MemoryBarrier
#undef MemoryBarrier
#endif

GLAPI void APIENTRY GLAPI_PREFIX(NewList)(GLuint list, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(EndList)(void);
GLAPI void APIENTRY GLAPI_PREFIX(CallList)(GLuint list);
GLAPI void APIENTRY GLAPI_PREFIX(CallLists)(GLsizei n, GLenum type, const GLvoid *lists);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteLists)(GLuint list, GLsizei range);
GLAPI GLuint APIENTRY GLAPI_PREFIX(GenLists)(GLsizei range);
GLAPI void APIENTRY GLAPI_PREFIX(ListBase)(GLuint base);
GLAPI void APIENTRY GLAPI_PREFIX(Begin)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(Bitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
GLAPI void APIENTRY GLAPI_PREFIX(Color3b)(GLbyte red, GLbyte green, GLbyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3bv)(const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3d)(GLdouble red, GLdouble green, GLdouble blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3f)(GLfloat red, GLfloat green, GLfloat blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3i)(GLint red, GLint green, GLint blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3s)(GLshort red, GLshort green, GLshort blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3ub)(GLubyte red, GLubyte green, GLubyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3ubv)(const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3ui)(GLuint red, GLuint green, GLuint blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3uiv)(const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color3us)(GLushort red, GLushort green, GLushort blue);
GLAPI void APIENTRY GLAPI_PREFIX(Color3usv)(const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);

GLAPI void APIENTRY GLAPI_PREFIX(Color4bv)(const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4i)(GLint red, GLint green, GLint blue, GLint alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4ubv)(const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4uiv)(const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Color4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
GLAPI void APIENTRY GLAPI_PREFIX(Color4usv)(const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(EdgeFlag)(GLboolean flag);
GLAPI void APIENTRY GLAPI_PREFIX(EdgeFlagv)(const GLboolean *flag);
GLAPI void APIENTRY GLAPI_PREFIX(End)(void);
GLAPI void APIENTRY GLAPI_PREFIX(Indexd)(GLdouble c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexdv)(const GLdouble *c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexf)(GLfloat c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexfv)(const GLfloat *c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexi)(GLint c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexiv)(const GLint *c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexs)(GLshort c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexsv)(const GLshort *c);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3bv)(const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3i)(GLint nx, GLint ny, GLint nz);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3s)(GLshort nx, GLshort ny, GLshort nz);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2d)(GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2f)(GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2i)(GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2s)(GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos2sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3i)(GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3s)(GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4i)(GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void APIENTRY GLAPI_PREFIX(RasterPos4sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Rectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
GLAPI void APIENTRY GLAPI_PREFIX(Rectdv)(const GLdouble *v1, const GLdouble *v2);
GLAPI void APIENTRY GLAPI_PREFIX(Rectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLAPI void APIENTRY GLAPI_PREFIX(Rectfv)(const GLfloat *v1, const GLfloat *v2);
GLAPI void APIENTRY GLAPI_PREFIX(Recti)(GLint x1, GLint y1, GLint x2, GLint y2);
GLAPI void APIENTRY GLAPI_PREFIX(Rectiv)(const GLint *v1, const GLint *v2);
GLAPI void APIENTRY GLAPI_PREFIX(Rects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
GLAPI void APIENTRY GLAPI_PREFIX(Rectsv)(const GLshort *v1, const GLshort *v2);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1d)(GLdouble s);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1f)(GLfloat s);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1i)(GLint s);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1s)(GLshort s);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord1sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2d)(GLdouble s, GLdouble t);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2f)(GLfloat s, GLfloat t);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2i)(GLint s, GLint t);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2s)(GLshort s, GLshort t);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord2sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3i)(GLint s, GLint t, GLint r);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3s)(GLshort s, GLshort t, GLshort r);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4i)(GLint s, GLint t, GLint r, GLint q);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoord4sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2d)(GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2f)(GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2i)(GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2s)(GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex2sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3d)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3f)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3i)(GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3s)(GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4i)(GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void APIENTRY GLAPI_PREFIX(Vertex4sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(ClipPlane)(GLenum plane, const GLdouble *equation);
GLAPI void APIENTRY GLAPI_PREFIX(ColorMaterial)(GLenum face, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(CullFace)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(Fogf)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(Fogfv)(GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(Fogi)(GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(Fogiv)(GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(FrontFace)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(Hint)(GLenum target, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(Lightf)(GLenum light, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(Lightfv)(GLenum light, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(Lighti)(GLenum light, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(Lightiv)(GLenum light, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(LightModelf)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(LightModelfv)(GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(LightModeli)(GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(LightModeliv)(GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(LineStipple)(GLint factor, GLushort pattern);
GLAPI void APIENTRY GLAPI_PREFIX(LineWidth)(GLfloat width);
GLAPI void APIENTRY GLAPI_PREFIX(Materialf)(GLenum face, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(Materialfv)(GLenum face, GLenum pname, const GLfloat *params);

GLAPI void APIENTRY GLAPI_PREFIX(Materiali)(GLenum face, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(Materialiv)(GLenum face, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(PointSize)(GLfloat size);
GLAPI void APIENTRY GLAPI_PREFIX(PolygonMode)(GLenum face, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(PolygonStipple)(const GLubyte *mask);
GLAPI void APIENTRY GLAPI_PREFIX(Scissor)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(ShadeModel)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterf)(GLenum target, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameteri)(GLenum target, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameteriv)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

GLAPI void APIENTRY GLAPI_PREFIX(TexEnvf)(GLenum target, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(TexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);

GLAPI void APIENTRY GLAPI_PREFIX(TexEnvi)(GLenum target, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(TexEnviv)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexGend)(GLenum coord, GLenum pname, GLdouble param);
GLAPI void APIENTRY GLAPI_PREFIX(TexGendv)(GLenum coord, GLenum pname, const GLdouble *params);

GLAPI void APIENTRY GLAPI_PREFIX(TexGenf)(GLenum coord, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(TexGenfv)(GLenum coord, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexGeni)(GLenum coord, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(TexGeniv)(GLenum coord, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(FeedbackBuffer)(GLsizei size, GLenum type, GLfloat *buffer);
GLAPI void APIENTRY GLAPI_PREFIX(SelectBuffer)(GLsizei size, GLuint *buffer);
GLAPI GLint APIENTRY GLAPI_PREFIX(RenderMode)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(InitNames)(void);
GLAPI void APIENTRY GLAPI_PREFIX(LoadName)(GLuint name);
GLAPI void APIENTRY GLAPI_PREFIX(PassThrough)(GLfloat token);
GLAPI void APIENTRY GLAPI_PREFIX(PopName)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PushName)(GLuint name);
GLAPI void APIENTRY GLAPI_PREFIX(DrawBuffer)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(Clear)(GLbitfield mask);
GLAPI void APIENTRY GLAPI_PREFIX(ClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY GLAPI_PREFIX(ClearIndex)(GLfloat c);
GLAPI void APIENTRY GLAPI_PREFIX(ClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void APIENTRY GLAPI_PREFIX(ClearStencil)(GLint s);
GLAPI void APIENTRY GLAPI_PREFIX(ClearDepth)(GLclampd depth);
GLAPI void APIENTRY GLAPI_PREFIX(StencilMask)(GLuint mask);
GLAPI void APIENTRY GLAPI_PREFIX(ColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI void APIENTRY GLAPI_PREFIX(DepthMask)(GLboolean flag);
GLAPI void APIENTRY GLAPI_PREFIX(IndexMask)(GLuint mask);
GLAPI void APIENTRY GLAPI_PREFIX(Accum)(GLenum op, GLfloat value);
GLAPI void APIENTRY GLAPI_PREFIX(Disable)(GLenum cap);
GLAPI void APIENTRY GLAPI_PREFIX(Enable)(GLenum cap);
GLAPI void APIENTRY GLAPI_PREFIX(Finish)(void);
GLAPI void APIENTRY GLAPI_PREFIX(Flush)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PopAttrib)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PushAttrib)(GLbitfield mask);
GLAPI void APIENTRY GLAPI_PREFIX(Map1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
GLAPI void APIENTRY GLAPI_PREFIX(Map1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
GLAPI void APIENTRY GLAPI_PREFIX(Map2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride,GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
GLAPI void APIENTRY GLAPI_PREFIX(Map2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
GLAPI void APIENTRY GLAPI_PREFIX(MapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
GLAPI void APIENTRY GLAPI_PREFIX(MapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
GLAPI void APIENTRY GLAPI_PREFIX(MapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
GLAPI void APIENTRY GLAPI_PREFIX(MapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord1d)(GLdouble u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord1dv)(const GLdouble *u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord1f)(GLfloat u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord1fv)(const GLfloat *u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord2d)(GLdouble u, GLdouble v);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord2dv)(const GLdouble *u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord2f)(GLfloat u, GLfloat v);
GLAPI void APIENTRY GLAPI_PREFIX(EvalCoord2fv)(const GLfloat *u);
GLAPI void APIENTRY GLAPI_PREFIX(EvalMesh1)(GLenum mode, GLint i1, GLint i2);
GLAPI void APIENTRY GLAPI_PREFIX(EvalPoint1)(GLint i);
GLAPI void APIENTRY GLAPI_PREFIX(EvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
GLAPI void APIENTRY GLAPI_PREFIX(EvalPoint2)(GLint i, GLint j);
GLAPI void APIENTRY GLAPI_PREFIX(AlphaFunc)(GLenum func, GLclampf ref);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFunc)(GLenum sfactor, GLenum dfactor);
GLAPI void APIENTRY GLAPI_PREFIX(LogicOp)(GLenum opcode);
GLAPI void APIENTRY GLAPI_PREFIX(StencilFunc)(GLenum func, GLint ref, GLuint mask);
GLAPI void APIENTRY GLAPI_PREFIX(StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
GLAPI void APIENTRY GLAPI_PREFIX(DepthFunc)(GLenum func);
GLAPI void APIENTRY GLAPI_PREFIX(PixelZoom)(GLfloat xfactor, GLfloat yfactor);
GLAPI void APIENTRY GLAPI_PREFIX(PixelTransferf)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(PixelTransferi)(GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(PixelStoref)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(PixelStorei)(GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(PixelMapfv)(GLenum map, GLsizei mapsize, const GLfloat *values);
GLAPI void APIENTRY GLAPI_PREFIX(PixelMapuiv)(GLenum map, GLsizei mapsize, const GLuint *values);
GLAPI void APIENTRY GLAPI_PREFIX(PixelMapusv)(GLenum map, GLsizei mapsize, const GLushort *values);
GLAPI void APIENTRY GLAPI_PREFIX(ReadBuffer)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(CopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
GLAPI void APIENTRY GLAPI_PREFIX(ReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(DrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(GetBooleanv)(GLenum pname, GLboolean *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetClipPlane)(GLenum plane, GLdouble *equation);
GLAPI void APIENTRY GLAPI_PREFIX(GetDoublev)(GLenum pname, GLdouble *params);
GLAPI GLenum APIENTRY GLAPI_PREFIX(GetError)(void);
GLAPI void APIENTRY GLAPI_PREFIX(GetFloatv)(GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetIntegerv)(GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetLightfv)(GLenum light, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetLightiv)(GLenum light, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetMapdv)(GLenum target, GLenum query, GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetMapfv)(GLenum target, GLenum query, GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetMapiv)(GLenum target, GLenum query, GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetMaterialiv)(GLenum face, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetPixelMapfv)(GLenum map, GLfloat *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetPixelMapuiv)(GLenum map, GLuint *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetPixelMapusv)(GLenum map, GLushort *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetPolygonStipple)(GLubyte *mask);
GLAPI const GLubyte * APIENTRY GLAPI_PREFIX(GetString)(GLenum name);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexEnvfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexEnviv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexGendv)(GLenum coord, GLenum pname, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexGenfv)(GLenum coord, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexGeniv)(GLenum coord, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsEnabled)(GLenum cap);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsList)(GLuint list);
GLAPI void APIENTRY GLAPI_PREFIX(DepthRange)(GLclampd zNear, GLclampd zFar);
GLAPI void APIENTRY GLAPI_PREFIX(Frustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI void APIENTRY GLAPI_PREFIX(LoadIdentity)(void);
GLAPI void APIENTRY GLAPI_PREFIX(LoadMatrixf)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(LoadMatrixd)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(MatrixMode)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(MultMatrixf)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultMatrixd)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(Ortho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI void APIENTRY GLAPI_PREFIX(PopMatrix)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PushMatrix)(void);
GLAPI void APIENTRY GLAPI_PREFIX(Rotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(Rotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(Scaled)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(Scalef)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(Translated)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(Translatef)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(Viewport)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(ArrayElement)(GLint i);
GLAPI void APIENTRY GLAPI_PREFIX(ArrayElementEXT)(GLint i);
GLAPI void APIENTRY GLAPI_PREFIX(BindTexture)(GLenum target, GLuint texture);
GLAPI void APIENTRY GLAPI_PREFIX(BindTextureEXT)(GLenum target, GLuint texture);
GLAPI void APIENTRY GLAPI_PREFIX(ColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(DisableClientState)(GLenum array);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArrays)(GLenum mode, GLint first, GLsizei count);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysEXT)(GLenum mode, GLint first, GLsizei count);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void APIENTRY GLAPI_PREFIX(EdgeFlagPointer)(GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(EnableClientState)(GLenum array);
GLAPI void APIENTRY GLAPI_PREFIX(IndexPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(Indexub)(GLubyte c);
GLAPI void APIENTRY GLAPI_PREFIX(Indexubv)(const GLubyte *c);
GLAPI void APIENTRY GLAPI_PREFIX(InterleavedArrays)(GLenum format, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(NormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(PolygonOffset)(GLfloat factor, GLfloat units);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(VertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(AreTexturesResident)(GLsizei n, const GLuint *textures, GLboolean *residences);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(AreTexturesResidentEXT)(GLsizei n, const GLuint *textures, GLboolean *residences);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteTextures)(GLsizei n, const GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteTexturesEXT)(GLsizei n, const GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(GenTextures)(GLsizei n, GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(GenTexturesEXT)(GLsizei n, GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(GetPointerv)(GLenum pname, GLvoid **params);
GLAPI void APIENTRY GLAPI_PREFIX(GetPointervEXT)(GLenum pname, GLvoid **params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsTexture)(GLuint texture);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsTextureEXT)(GLuint texture);
GLAPI void APIENTRY GLAPI_PREFIX(PrioritizeTextures)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLAPI void APIENTRY GLAPI_PREFIX(PrioritizeTexturesEXT)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLAPI void APIENTRY GLAPI_PREFIX(TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(PopClientAttrib)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PushClientAttrib)(GLbitfield mask);
GLAPI void APIENTRY GLAPI_PREFIX(BlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void APIENTRY GLAPI_PREFIX(BlendColorEXT)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquation)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationEXT)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(DrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void APIENTRY GLAPI_PREFIX(DrawRangeElementsEXT)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void APIENTRY GLAPI_PREFIX(ColorTable)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
GLAPI void APIENTRY GLAPI_PREFIX(ColorTableParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(ColorTableParameteriv)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(CopyColorTable)(GLenum target, GLenum internalformat, GLint x,GLint y, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(GetColorTable)(GLenum target, GLenum format, GLenum type, GLvoid *table);
GLAPI void APIENTRY GLAPI_PREFIX(GetColorTableParameterfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetColorTableParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(ColorSubTable)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CopyColorSubTable)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionFilter1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionParameterf)(GLenum target, GLenum pname, GLfloat params);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionParameteri)(GLenum target, GLenum pname, GLint params);
GLAPI void APIENTRY GLAPI_PREFIX(ConvolutionParameteriv)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(CopyConvolutionFilter1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(CopyConvolutionFilter2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(GetConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLvoid *image);
GLAPI void APIENTRY GLAPI_PREFIX(GetConvolutionParameterfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetConvolutionParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetSeparableFilter)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
GLAPI void APIENTRY GLAPI_PREFIX(SeparableFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

GLAPI void APIENTRY GLAPI_PREFIX(GetHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetHistogramParameterfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetHistogramParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetMinmaxParameterfv)(GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetMinmaxParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(Histogram)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
GLAPI void APIENTRY GLAPI_PREFIX(Minmax)(GLenum target, GLenum internalformat, GLboolean sink);

GLAPI void APIENTRY GLAPI_PREFIX(ResetHistogram)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(ResetMinmax)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(TexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(CopyTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(ActiveTexture)(GLenum texture);
GLAPI void APIENTRY GLAPI_PREFIX(ActiveTextureARB)(GLenum texture);
GLAPI void APIENTRY GLAPI_PREFIX(ClientActiveTexture)(GLenum texture);
GLAPI void APIENTRY GLAPI_PREFIX(ClientActiveTextureARB)(GLenum texture);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1d)(GLenum target, GLdouble s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1dARB)(GLenum target, GLdouble s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1dv)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1dvARB)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1fARB)(GLenum target, GLfloat s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1f)(GLenum target, GLfloat s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1fvARB)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1fv)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1i)(GLenum target, GLint s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1iARB)(GLenum target, GLint s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1iv)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1ivARB)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1s)(GLenum target, GLshort s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1sARB)(GLenum target, GLshort s);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1sv)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord1svARB)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2d)(GLenum target, GLdouble s, GLdouble t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2dARB)(GLenum target, GLdouble s, GLdouble t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2dv)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2dvARB)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2fARB)(GLenum target, GLfloat s, GLfloat t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2fvARB)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2fv)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2i)(GLenum target, GLint s, GLint t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2iARB)(GLenum target, GLint s, GLint t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2iv)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2ivARB)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2s)(GLenum target, GLshort s, GLshort t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2sARB)(GLenum target, GLshort s, GLshort t);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2sv)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord2svARB)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3d)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3dv)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3dvARB)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3f)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3fvARB)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3fv)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3i)(GLenum target, GLint s, GLint t, GLint r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3iARB)(GLenum target, GLint s, GLint t, GLint r);

GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3iv)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3ivARB)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3s)(GLenum target, GLshort s, GLshort t, GLshort r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3sARB)(GLenum target, GLshort s, GLshort t, GLshort r);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3sv)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord3svARB)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4d)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4dv)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4dvARB)(GLenum target, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4fvARB)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4fv)(GLenum target, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4i)(GLenum target, GLint s, GLint t, GLint r, GLint q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4iARB)(GLenum target, GLint s, GLint t, GLint r, GLint q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4iv)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4ivARB)(GLenum target, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4s)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4sARB)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4sv)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4svARB)(GLenum target, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);

GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(CompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);
GLAPI void APIENTRY GLAPI_PREFIX(GetCompressedTexImageARB)(GLenum target, GLint level, GLvoid *img);
GLAPI void APIENTRY GLAPI_PREFIX(LoadTransposeMatrixd)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(LoadTransposeMatrixdARB)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(LoadTransposeMatrixf)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(LoadTransposeMatrixfARB)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultTransposeMatrixd)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultTransposeMatrixdARB)(const GLdouble *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultTransposeMatrixf)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultTransposeMatrixfARB)(const GLfloat *m);
GLAPI void APIENTRY GLAPI_PREFIX(SampleCoverage)(GLclampf value, GLboolean invert);
GLAPI void APIENTRY GLAPI_PREFIX(SampleCoverageARB)(GLclampf value, GLboolean invert);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFuncSeparateEXT)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordd)(GLdouble coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoorddEXT)(GLdouble coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoorddv)(const GLdouble *coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoorddvEXT)(const GLdouble *coord);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawArraysEXT)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterf)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterfARB)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterfEXT)(GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterfv)(GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterfvARB)(GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterfvEXT)(GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameteri)(GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameteriv)(GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3b)(GLbyte red, GLbyte green, GLbyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3bEXT)(GLbyte red, GLbyte green, GLbyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3bv)(const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3bvEXT)(const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3d)(GLdouble red, GLdouble green, GLdouble blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3dEXT)(GLdouble red, GLdouble green, GLdouble blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3dvEXT)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3i)(GLint red, GLint green, GLint blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3iEXT)(GLint red, GLint green, GLint blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ivEXT)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3s)(GLshort red, GLshort green, GLshort blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3sEXT)(GLshort red, GLshort green, GLshort blue)
;
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3svEXT)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ubEXT)(GLubyte red, GLubyte green, GLubyte blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ubv)(const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ubvEXT)(const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3ui)(GLuint red, GLuint green, GLuint blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3uiEXT)(GLuint red, GLuint green, GLuint blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3uiv)(const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3uivEXT)(const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3us)(GLushort red, GLushort green, GLushort blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3usEXT)(GLushort red, GLushort green, GLushort blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3usv)(const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3usvEXT)(const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColorPointerEXT)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2d)(GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2dARB)(GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2dvARB)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2f)(GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2fARB)(GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2fvARB)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2i)(GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2iARB)(GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2ivARB)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2s)(GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2sARB)(GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos2svARB)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3d)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3dARB)(GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3dv)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3dvARB)(const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3f)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3fARB)(GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3fvARB)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3i)(GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3iARB)(GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3iv)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3ivARB)(const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3s)(GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3sARB)(GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3sv)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(WindowPos3svARB)(const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(BeginQuery)(GLenum target, GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(BeginQueryARB)(GLenum target, GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(BindBuffer)(GLenum target, GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(BindBufferARB)(GLenum target, GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(BufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
GLAPI void APIENTRY GLAPI_PREFIX(BufferDataARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
GLAPI void APIENTRY GLAPI_PREFIX(BufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(BufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteBuffers)(GLsizei n, const GLuint *buffer);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteBuffersARB)(GLsizei n, const GLuint *buffer);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteQueries)(GLsizei n, const GLuint *ids);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteQueriesARB)(GLsizei n, const GLuint *ids);
GLAPI void APIENTRY GLAPI_PREFIX(EndQuery)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(EndQueryARB)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(GenBuffers)(GLsizei n, GLuint *buffer);
GLAPI void APIENTRY GLAPI_PREFIX(GenBuffersARB)(GLsizei n, GLuint *buffer);
GLAPI void APIENTRY GLAPI_PREFIX(GenQueries)(GLsizei n, GLuint *ids);
GLAPI void APIENTRY GLAPI_PREFIX(GenQueriesARB)(GLsizei n, GLuint *ids);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferPointerv)(GLenum target, GLenum pname, GLvoid **params);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferPointervARB)(GLenum target, GLenum pname, GLvoid **params);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryiv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryivARB)(GLenum target, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsBuffer)(GLuint buffer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsBufferARB)(GLuint buffer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsQuery)(GLuint id);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsQueryARB)(GLuint id);
GLAPI GLvoid * APIENTRY GLAPI_PREFIX(MapBuffer)(GLenum target, GLenum access);
GLAPI GLvoid * APIENTRY GLAPI_PREFIX(MapBufferARB)(GLenum target, GLenum access);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(UnmapBuffer)(GLenum target);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(UnmapBufferARB)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(AttachShader)(GLuint program, GLuint shader);
GLAPI void APIENTRY GLAPI_PREFIX(BindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(BindAttribLocationARB)(GLhandleARB program, GLuint index, const GLcharARB *name);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationSeparate)(GLenum modeRGB, GLenum modeA);
GLAPI void APIENTRY GLAPI_PREFIX(CompileShader)(GLuint shader);
GLAPI void APIENTRY GLAPI_PREFIX(CompileShaderARB)(GLhandleARB shader);
GLAPI GLuint APIENTRY GLAPI_PREFIX(CreateProgram)(void);
GLAPI GLuint APIENTRY GLAPI_PREFIX(CreateShader)(GLenum type);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteProgram)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteShader)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(DetachShader)(GLuint program, GLuint shader);
GLAPI void APIENTRY GLAPI_PREFIX(DisableVertexAttribArray)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(DisableVertexAttribArrayARB)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(DrawBuffers)(GLsizei n, const GLenum *bufs);
GLAPI void APIENTRY GLAPI_PREFIX(DrawBuffersARB)(GLsizei n, const GLenum *bufs);
GLAPI void APIENTRY GLAPI_PREFIX(DrawBuffersATI)(GLsizei n, const GLenum *bufs);
GLAPI void APIENTRY GLAPI_PREFIX(EnableVertexAttribArray)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(EnableVertexAttribArrayARB)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveAttribARB)(GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniformARB)(GLhandleARB program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetAttribLocation)(GLuint program, const GLchar *name);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetAttribLocationARB)(GLhandleARB program, const GLcharARB *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramiv)(GLuint program, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void APIENTRY GLAPI_PREFIX(GetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLAPI void APIENTRY GLAPI_PREFIX(GetShaderSourceARB)(GLhandleARB shader, GLsizei bufSize, GLsizei *length, GLcharARB *source);
GLAPI void APIENTRY GLAPI_PREFIX(GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetUniformLocation)(GLuint program, const GLchar *name);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetUniformLocationARB)(GLhandleARB program, const GLcharARB *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformfv)(GLuint program, GLint location, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformfvARB)(GLhandleARB program, GLint location, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformiv)(GLuint program, GLint location, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformivARB)(GLhandleARB program, GLint location, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid **pointer);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);

GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsProgram)(GLuint program);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsShader)(GLuint shader);
GLAPI void APIENTRY GLAPI_PREFIX(LinkProgram)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(LinkProgramARB)(GLhandleARB program);
GLAPI void APIENTRY GLAPI_PREFIX(ShaderSource)(GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length);
GLAPI void APIENTRY GLAPI_PREFIX(ShaderSourceARB)(GLhandleARB shader, GLsizei count, const GLcharARB **string, const GLint *length);
GLAPI void APIENTRY GLAPI_PREFIX(StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
GLAPI void APIENTRY GLAPI_PREFIX(StencilMaskSeparate)(GLenum face, GLuint mask);
GLAPI void APIENTRY GLAPI_PREFIX(StencilOpSeparate)(GLenum face, GLenum sfail, GLenum zfail, GLenum zpass);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1f)(GLint location, GLfloat v0);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1fARB)(GLint location, GLfloat v0);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1fv)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1fvARB)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1i)(GLint location, GLint v0);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1iARB)(GLint location, GLint v0);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1iv)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1ivARB)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2fv)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2fvARB)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2i)(GLint location, GLint v0, GLint v1);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2iARB)(GLint location, GLint v0, GLint v1);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2iv)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2ivARB)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3fv)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3fvARB)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3iv)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3ivARB)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4fv)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4fvARB)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4iv)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4ivARB)(GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UseProgram)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(UseProgramObjectARB)(GLhandleARB program);
GLAPI void APIENTRY GLAPI_PREFIX(ValidateProgram)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(ValidateProgramARB)(GLhandleARB program);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1d)(GLuint index, GLdouble x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1dARB)(GLuint index, GLdouble x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1dv)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1dvARB)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1s)(GLuint index, GLshort x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1sARB)(GLuint index, GLshort x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1sv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1svARB)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2dARB)(GLuint index, GLdouble x, GLdouble y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2dv)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2dvARB)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2s)(GLuint index, GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2sARB)(GLuint index, GLshort x, GLshort y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2sv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2svARB)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3dv)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3dvARB)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z)
;
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3sARB)(GLuint index, GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3sv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3svARB)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nbv)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NbvARB)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Niv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NivARB)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nsv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NsvARB)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nubv)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NubvARB)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nuiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NuivARB)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4Nusv)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4NusvARB)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4bv)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4bvARB)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4dv)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4dvARB)(GLuint index, const GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4iv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4ivARB)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4sARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4sv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4svARB)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4ubv)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4ubvARB)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4uiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4uivARB)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4usv)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4usvARB)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribPointerARB)(GLuint index, GLint size, GLenum type,GLboolean normalized, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(BeginConditionalRender)(GLuint query, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BeginConditionalRenderNV)(GLuint query, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BeginTransformFeedback)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BindBufferBase)(GLenum target, GLuint index, GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(BindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY GLAPI_PREFIX(BindFragDataLocation)(GLuint program, GLuint colorNumber, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(BindFragDataLocationEXT)(GLuint program, GLuint colorNumber, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(ClampColor)(GLenum target, GLenum clamp);
GLAPI void APIENTRY GLAPI_PREFIX(ClampColorARB)(GLenum target, GLenum clamp);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ColorMaski)(GLuint buf, GLboolean r, GLboolean g, GLboolean b,GLboolean a);
GLAPI void APIENTRY GLAPI_PREFIX(ColorMaskIndexedEXT)(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI void APIENTRY GLAPI_PREFIX(Disablei)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(DisableIndexedEXT)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(Enablei)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(EnableIndexedEXT)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(EndConditionalRender)(void);
GLAPI void APIENTRY GLAPI_PREFIX(EndConditionalRenderNV)(void);
GLAPI void APIENTRY GLAPI_PREFIX(EndTransformFeedback)(void);
GLAPI void APIENTRY GLAPI_PREFIX(GetBooleani_v)(GLenum value, GLuint index, GLboolean *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetBooleanIndexedvEXT)(GLenum value, GLuint index, GLboolean *data);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetFragDataLocation)(GLuint program, const GLchar *name);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetFragDataLocationEXT)(GLuint program, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetIntegeri_v)(GLenum value, GLuint index, GLint *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetIntegerIndexedvEXT)(GLenum value, GLuint index, GLint *data);
GLAPI const GLubyte * APIENTRY GLAPI_PREFIX(GetStringi)(GLenum name, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterIivEXT)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterIuivEXT)(GLenum target, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformuiv)(GLuint program, GLint location, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformuivEXT)(GLuint program, GLint location, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribIivEXT)(GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetVertexAttribIuivEXT)(GLuint index, GLenum pname, GLuint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsEnabledi)(GLenum target, GLuint index);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsEnabledIndexedEXT)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterIivEXT)(GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterIuivEXT)(GLenum target, GLenum pname, const GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar * const *varyings, GLenum bufferMode);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1ui)(GLint location, GLuint x);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1uiEXT)(GLint location, GLuint x);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1uiv)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform1uivEXT)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2ui)(GLint location, GLuint x, GLuint y);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2uiEXT)(GLint location, GLuint x, GLuint y);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2uiv)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform2uivEXT)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3ui)(GLint location, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3uiEXT)(GLint location, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3uiv)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform3uivEXT)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4ui)(GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4uiEXT)(GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4uiv)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(Uniform4uivEXT)(GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1iv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1ivEXT)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1uiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1uivEXT)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4bv)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4bvEXT)(GLuint index, const GLbyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4sv)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4svEXT)(GLuint index, const GLshort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4ubv)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4ubvEXT)(GLuint index, const GLubyte *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4usv)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4usvEXT)(GLuint index, const GLushort *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribIPointerEXT)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(PrimitiveRestartIndex)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(PrimitiveRestartIndexNV)(GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(TexBuffer)(GLenum target, GLenum internalFormat, GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(TexBufferARB)(GLenum target, GLenum internalFormat, GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(GetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetInteger64i_v)(GLenum cap, GLuint index, GLint64 *data);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribDivisor)(GLuint index, GLuint divisor);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribDivisorARB)(GLuint index, GLuint divisor);
GLAPI void APIENTRY GLAPI_PREFIX(MinSampleShading)(GLfloat value);
GLAPI void APIENTRY GLAPI_PREFIX(MinSampleShadingARB)(GLfloat value);
GLAPI void APIENTRY GLAPI_PREFIX(MemoryBarrierByRegion)(GLbitfield barriers);
GLAPI void APIENTRY GLAPI_PREFIX(BindProgramARB)(GLenum target, GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteProgramsARB)(GLsizei n, const GLuint *programs);
GLAPI void APIENTRY GLAPI_PREFIX(GenProgramsARB)(GLsizei n, GLuint *programs);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramivARB)(GLenum target, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsProgramARB)(GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1fARB)(GLuint index, GLfloat x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1f)(GLuint index, GLfloat x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1fvARB)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib1fv)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2fARB)(GLuint index, GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2fvARB)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib2fv)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3fvARB)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib3fv)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4fvARB)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttrib4fv)(GLuint index, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(AttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
GLAPI GLhandleARB APIENTRY GLAPI_PREFIX(CreateProgramObjectARB)(void);
GLAPI GLhandleARB APIENTRY GLAPI_PREFIX(CreateShaderObjectARB)(GLenum shaderType);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteObjectARB)(GLhandleARB obj);
GLAPI void APIENTRY GLAPI_PREFIX(DetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
GLAPI void APIENTRY GLAPI_PREFIX(GetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxLength, GLsizei *length, GLhandleARB *infoLog);
GLAPI GLhandleARB APIENTRY GLAPI_PREFIX(GetHandleARB)(GLenum pname);
GLAPI void APIENTRY GLAPI_PREFIX(GetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
GLAPI void APIENTRY GLAPI_PREFIX(GetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysInstancedARB)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysInstancedEXT)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstancedARB)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstancedEXT)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(BindFramebuffer)(GLenum target, GLuint framebuffer);
GLAPI void APIENTRY GLAPI_PREFIX(BindRenderbuffer)(GLenum target, GLuint renderbuffer);
GLAPI void APIENTRY GLAPI_PREFIX(BlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI GLenum APIENTRY GLAPI_PREFIX(CheckFramebufferStatus)(GLenum target);
GLAPI GLenum APIENTRY GLAPI_PREFIX(CheckFramebufferStatusEXT)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);

GLAPI void APIENTRY GLAPI_PREFIX(DeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteRenderbuffersEXT)(GLsizei n, const GLuint *renderbuffers);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferRenderbufferEXT)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture1DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTexture3DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferTextureLayerEXT)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI void APIENTRY GLAPI_PREFIX(GenFramebuffers)(GLsizei n, GLuint *framebuffers);
GLAPI void APIENTRY GLAPI_PREFIX(GenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
GLAPI void APIENTRY GLAPI_PREFIX(GenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
GLAPI void APIENTRY GLAPI_PREFIX(GenRenderbuffersEXT)(GLsizei n, GLuint *renderbuffers);
GLAPI void APIENTRY GLAPI_PREFIX(GenerateMipmap)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(GenerateMipmapEXT)(GLenum target);
GLAPI void APIENTRY GLAPI_PREFIX(GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetFramebufferAttachmentParameterivEXT)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetRenderbufferParameterivEXT)(GLenum target, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsFramebuffer)(GLuint framebuffer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsFramebufferEXT)(GLuint framebuffer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsRenderbuffer)(GLuint renderbuffer);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsRenderbufferEXT)(GLuint renderbuffer);
GLAPI void APIENTRY GLAPI_PREFIX(RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(RenderbufferStorageEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(RenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(RenderbufferStorageMultisampleEXT)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(FlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
GLAPI GLvoid * APIENTRY GLAPI_PREFIX(MapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI void APIENTRY GLAPI_PREFIX(BindVertexArray)(GLuint array);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteVertexArrays)(GLsizei n, const GLuint *arrays);
GLAPI void APIENTRY GLAPI_PREFIX(GenVertexArrays)(GLsizei n, GLuint *arrays);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsVertexArray)(GLuint array);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
GLAPI GLuint APIENTRY GLAPI_PREFIX(GetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
GLAPI void APIENTRY GLAPI_PREFIX(GetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar * const *uniformNames, GLuint *uniformIndices);
GLAPI void APIENTRY GLAPI_PREFIX(UniformBlockBinding)(GLuint program, GLuint uniformBlockIndex,GLuint uniformBlockBinding);
GLAPI void APIENTRY GLAPI_PREFIX(CopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLAPI GLenum APIENTRY GLAPI_PREFIX(ClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteSync)(GLsync sync);
GLAPI GLsync APIENTRY GLAPI_PREFIX(FenceSync)(GLenum condition, GLbitfield flags);
GLAPI void APIENTRY GLAPI_PREFIX(GetInteger64v)(GLenum pname, GLint64 *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsSync)(GLsync sync);
GLAPI void APIENTRY GLAPI_PREFIX(WaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
GLAPI void APIENTRY GLAPI_PREFIX(DrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, const GLint *basevertex);
GLAPI void APIENTRY GLAPI_PREFIX(ProvokingVertex)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(ProvokingVertexEXT)(GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(GetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
GLAPI void APIENTRY GLAPI_PREFIX(SampleMaski)(GLuint index, GLbitfield mask);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI void APIENTRY GLAPI_PREFIX(TexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationSeparateiARB)(GLuint buf, GLenum modeRGB, GLenum modeA);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeA);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationiARB)(GLuint buf, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BlendEquationi)(GLuint buf, GLenum mode);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFuncSeparateiARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcA, GLenum dstA);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFuncSeparatei)(GLuint buf, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFunciARB)(GLuint buf, GLenum src, GLenum dst);
GLAPI void APIENTRY GLAPI_PREFIX(BlendFunci)(GLuint buf, GLenum sfactor, GLenum dfactor);
GLAPI void APIENTRY GLAPI_PREFIX(BindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetFragDataIndex)(GLuint program, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(BindSampler)(GLuint unit, GLuint sampler);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteSamplers)(GLsizei count, const GLuint *samplers);
GLAPI void APIENTRY GLAPI_PREFIX(GenSamplers)(GLsizei count, GLuint *samplers);
GLAPI void APIENTRY GLAPI_PREFIX(GetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsSampler)(GLuint sampler);
GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameteri)(GLuint sampler, GLenum pname, GLint param);

GLAPI void APIENTRY GLAPI_PREFIX(SamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(ColorP3ui)(GLenum type, GLuint color);
GLAPI void APIENTRY GLAPI_PREFIX(ColorP3uiv)(GLenum type, const GLuint *color);
GLAPI void APIENTRY GLAPI_PREFIX(ColorP4ui)(GLenum type, GLuint color);
GLAPI void APIENTRY GLAPI_PREFIX(ColorP4uiv)(GLenum type, const GLuint *color);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(NormalP3ui)(GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(NormalP3uiv)(GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColorP3ui)(GLenum type, GLuint color);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColorP3uiv)(GLenum type, const GLuint *color);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP1ui)(GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP1uiv)(GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP2ui)(GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP2uiv)(GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP3ui)(GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP3uiv)(GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP4ui)(GLenum type, GLuint coords);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordP4uiv)(GLenum type, const GLuint *coords);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP2ui)(GLenum type, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP2uiv)(GLenum type, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP3ui)(GLenum type, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP3uiv)(GLenum type, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP4ui)(GLenum type, GLuint value);
GLAPI void APIENTRY GLAPI_PREFIX(VertexP4uiv)(GLenum type, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysIndirect)(GLenum mode, const GLvoid *indirect);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsIndirect)(GLenum mode, GLenum type, const GLvoid *indirect);
GLAPI void APIENTRY GLAPI_PREFIX(PatchParameteri)(GLenum pname, GLint value);
GLAPI void APIENTRY GLAPI_PREFIX(BindTransformFeedback)(GLenum target, GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
GLAPI void APIENTRY GLAPI_PREFIX(DrawTransformFeedback)(GLenum mode, GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(GenTransformFeedbacks)(GLsizei n, GLuint *ids);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsTransformFeedback)(GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(PauseTransformFeedback)(void);
GLAPI void APIENTRY GLAPI_PREFIX(ResumeTransformFeedback)(void);
GLAPI void APIENTRY GLAPI_PREFIX(BeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
GLAPI void APIENTRY GLAPI_PREFIX(DrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
GLAPI void APIENTRY GLAPI_PREFIX(EndQueryIndexed)(GLenum target, GLuint index);
GLAPI void APIENTRY GLAPI_PREFIX(GetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(ClearDepthf)(GLclampf depth);
GLAPI void APIENTRY GLAPI_PREFIX(DepthRangef)(GLclampf zNear, GLclampf zFar);
GLAPI void APIENTRY GLAPI_PREFIX(GetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
GLAPI void APIENTRY GLAPI_PREFIX(ReleaseShaderCompiler)(void);
GLAPI void APIENTRY GLAPI_PREFIX(ShaderBinary)(GLsizei n, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramBinary)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramParameteri)(GLuint program, GLenum pname, GLint value);

GLAPI void APIENTRY GLAPI_PREFIX(DepthRangeArrayv)(GLuint first, GLsizei count, const GLclampd *v);
GLAPI void APIENTRY GLAPI_PREFIX(DepthRangeIndexed)(GLuint index, GLclampd n, GLclampd f);
GLAPI void APIENTRY GLAPI_PREFIX(GetDoublei_v)(GLenum target, GLuint index, GLdouble *data);
GLAPI void APIENTRY GLAPI_PREFIX(GetFloati_v)(GLenum target, GLuint index, GLfloat *data);
GLAPI void APIENTRY GLAPI_PREFIX(ScissorArrayv)(GLuint first, GLsizei count, const int *v);
GLAPI void APIENTRY GLAPI_PREFIX(ScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(ScissorIndexedv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(ViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(ViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
GLAPI void APIENTRY GLAPI_PREFIX(ViewportIndexedfv)(GLuint index, const GLfloat *v);
GLAPI GLenum APIENTRY GLAPI_PREFIX(GetGraphicsResetStatusARB)(void);
GLAPI GLenum APIENTRY GLAPI_PREFIX(GetGraphicsResetStatus)(void);
GLAPI void APIENTRY GLAPI_PREFIX(GetnColorTableARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *table);
GLAPI void APIENTRY GLAPI_PREFIX(GetnCompressedTexImageARB)(GLenum target, GLint lod, GLsizei bufSize, GLvoid *img);
GLAPI void APIENTRY GLAPI_PREFIX(GetnConvolutionFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *image);
GLAPI void APIENTRY GLAPI_PREFIX(GetnHistogramARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetnMapdvARB)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetnMapfvARB)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetnMapivARB)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(GetnMinmaxARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetnPixelMapfvARB)(GLenum map, GLsizei bufSize, GLfloat *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetnPixelMapuivARB)(GLenum map, GLsizei bufSize, GLuint *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetnPixelMapusvARB)(GLenum map, GLsizei bufSize, GLushort *values);
GLAPI void APIENTRY GLAPI_PREFIX(GetnPolygonStippleARB)(GLsizei bufSize, GLubyte *pattern);
GLAPI void APIENTRY GLAPI_PREFIX(GetnSeparableFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid *row, GLsizei columnBufSize, GLvoid *column, GLvoid *span);
GLAPI void APIENTRY GLAPI_PREFIX(GetnTexImageARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *img);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformdvARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformfvARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformivARB)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformuivARB)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
GLAPI void APIENTRY GLAPI_PREFIX(ReadnPixelsARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(ReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height,GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(DrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count,GLenum type, const GLvoid *indices, GLsizei primcount, GLuint baseinstance);
GLAPI void APIENTRY GLAPI_PREFIX(DrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
GLAPI void APIENTRY GLAPI_PREFIX(DrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(DrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id,GLuint stream, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(GetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(BindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GLAPI void APIENTRY GLAPI_PREFIX(MemoryBarrier)(GLbitfield barriers);
GLAPI void APIENTRY GLAPI_PREFIX(TexStorage1D)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(TexStorage2D)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(TexStorage3D)(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY GLAPI_PREFIX(TextureStorage1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width);
GLAPI void APIENTRY GLAPI_PREFIX(TextureStorage2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(TextureStorage3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(ClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(DispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GLAPI void APIENTRY GLAPI_PREFIX(DispatchComputeIndirect)(GLintptr indirect);
GLAPI void APIENTRY GLAPI_PREFIX(CopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
GLAPI void APIENTRY GLAPI_PREFIX(TextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

GLAPI void APIENTRY GLAPI_PREFIX(BindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribBinding)(GLuint attribindex, GLuint bindingindex);

GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GLAPI void APIENTRY GLAPI_PREFIX(VertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
GLAPI void APIENTRY GLAPI_PREFIX(FramebufferParameteri)(GLenum target, GLenum pname, GLint param);
GLAPI void APIENTRY GLAPI_PREFIX(GetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawArraysIndirect)(GLenum mode, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawElementsIndirect)(GLenum mode, GLenum type, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
GLAPI GLuint APIENTRY GLAPI_PREFIX(GetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI GLint APIENTRY GLAPI_PREFIX(GetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY GLAPI_PREFIX(TexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI void APIENTRY GLAPI_PREFIX(TexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI void APIENTRY GLAPI_PREFIX(BufferStorage)(GLenum target, GLsizeiptr size, const GLvoid *data, GLbitfield flags);
GLAPI void APIENTRY GLAPI_PREFIX(ClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(ClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data);
GLAPI void APIENTRY GLAPI_PREFIX(BindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
GLAPI void APIENTRY GLAPI_PREFIX(BindBuffersRange)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
GLAPI void APIENTRY GLAPI_PREFIX(BindImageTextures)(GLuint first, GLsizei count, const GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(BindSamplers)(GLuint first, GLsizei count, const GLuint *samplers);
GLAPI void APIENTRY GLAPI_PREFIX(BindTextures)(GLuint first, GLsizei count, const GLuint *textures);
GLAPI void APIENTRY GLAPI_PREFIX(BindVertexBuffers)(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateBufferData)(GLuint buffer);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateTexImage)(GLuint texture, GLint level);
GLAPI void APIENTRY GLAPI_PREFIX(InvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
GLAPI void APIENTRY GLAPI_PREFIX(PointSizePointerOES)(GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(ColorPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(EdgeFlagPointerEXT)(GLsizei stride, GLsizei count, const GLboolean *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(IndexPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(NormalPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(TexCoordPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(VertexPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
GLAPI void APIENTRY GLAPI_PREFIX(ActiveShaderProgram)(GLuint pipeline, GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(BindProgramPipeline)(GLuint pipeline);
GLAPI GLuint APIENTRY GLAPI_PREFIX(CreateShaderProgramv)(GLenum type, GLsizei count, const GLchar * const *strings);
GLAPI void APIENTRY GLAPI_PREFIX(DeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
GLAPI void APIENTRY GLAPI_PREFIX(GenProgramPipelines)(GLsizei n, GLuint *pipelines);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void APIENTRY GLAPI_PREFIX(GetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
GLAPI GLboolean APIENTRY GLAPI_PREFIX(IsProgramPipeline)(GLuint pipeline);
GLAPI void APIENTRY GLAPI_PREFIX(LockArraysEXT)(GLint first, GLsizei count);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1f)(GLuint program, GLint location, GLfloat x);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1i)(GLuint program, GLint location, GLint x);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1ui)(GLuint program, GLint location, GLuint x);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2f)(GLuint program, GLint location, GLfloat x, GLfloat y);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2i)(GLuint program, GLint location, GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2ui)(GLuint program, GLint location, GLuint x, GLuint y);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3f)(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3i)(GLuint program, GLint location, GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3ui)(GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4f)(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4i)(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4ui)(GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(ProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY GLAPI_PREFIX(UnlockArraysEXT)(void);
GLAPI void APIENTRY GLAPI_PREFIX(UseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
GLAPI void APIENTRY GLAPI_PREFIX(ValidateProgramPipeline)(GLuint pipeline);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageCallback)(GLDEBUGPROC callback, const GLvoid *userParam);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageCallbackARB)(GLDEBUGPROCARB callback, const GLvoid *userParam);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageControlARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GLAPI void APIENTRY GLAPI_PREFIX(DebugMessageInsertARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLcharARB *buf);
GLAPI GLuint APIENTRY GLAPI_PREFIX(GetDebugMessageLog)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
GLAPI GLuint APIENTRY GLAPI_PREFIX(GetDebugMessageLogARB)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLcharARB *messageLog);
GLAPI void APIENTRY GLAPI_PREFIX(GetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI void APIENTRY GLAPI_PREFIX(GetObjectPtrLabel)(const GLvoid *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
GLAPI void APIENTRY GLAPI_PREFIX(ObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
GLAPI void APIENTRY GLAPI_PREFIX(ObjectPtrLabel)(const GLvoid *ptr, GLsizei length, const GLchar *label);
GLAPI void APIENTRY GLAPI_PREFIX(PopDebugGroup)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3fEXT)(GLfloat red, GLfloat green, GLfloat blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3f)(GLfloat red, GLfloat green, GLfloat blue);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3fvEXT)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(SecondaryColor3fv)(const GLfloat *v);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawElementsEXT)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(MultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordfEXT)(GLfloat coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordf)(GLfloat coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordfvEXT)(const GLfloat *coord);
GLAPI void APIENTRY GLAPI_PREFIX(FogCoordfv)(const GLfloat *coord);
GLAPI void APIENTRY GLAPI_PREFIX(PrimitiveRestartNV)(void);
GLAPI void APIENTRY GLAPI_PREFIX(BindFramebufferEXT)(GLenum target, GLuint framebuffer);
GLAPI void APIENTRY GLAPI_PREFIX(BindRenderbufferEXT)(GLenum target, GLuint renderbuffer);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1iEXT)(GLuint index, GLint x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1i)(GLuint index, GLint x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1uiEXT)(GLuint index, GLuint x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI1ui)(GLuint index, GLuint x);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2iEXT)(GLuint index, GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2i)(GLuint index, GLint x, GLint y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2ivEXT)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2iv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2uiEXT)(GLuint index, GLuint x, GLuint y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2uivEXT)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI2uiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3iEXT)(GLuint index, GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3ivEXT)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3iv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3uiEXT)(GLuint index, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);

GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3uivEXT)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI3uiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4iEXT)(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4ivEXT)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4iv)(GLuint index, const GLint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4uiEXT)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z,GLuint w);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4uivEXT)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(VertexAttribI4uiv)(GLuint index, const GLuint *v);
GLAPI void APIENTRY GLAPI_PREFIX(ClearColorIiEXT)(GLint r, GLint g, GLint b, GLint a);
GLAPI void APIENTRY GLAPI_PREFIX(ClearColorIuiEXT)(GLuint r, GLuint g, GLuint b, GLuint a);
GLAPI void APIENTRY GLAPI_PREFIX(TextureBarrierNV)(void);
GLAPI void APIENTRY GLAPI_PREFIX(AlphaFuncx)(GLenum func, GLclampx ref);
GLAPI void APIENTRY GLAPI_PREFIX(ClearColorx)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
GLAPI void APIENTRY GLAPI_PREFIX(ClearDepthx)(GLclampx depth);
GLAPI void APIENTRY GLAPI_PREFIX(Color4x)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
GLAPI void APIENTRY GLAPI_PREFIX(DepthRangex)(GLclampx zNear, GLclampx zFar);
GLAPI void APIENTRY GLAPI_PREFIX(Fogx)(GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(Fogxv)(GLenum pname, const GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(Frustumf)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
GLAPI void APIENTRY GLAPI_PREFIX(Frustumx)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
GLAPI void APIENTRY GLAPI_PREFIX(LightModelx)(GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(LightModelxv)(GLenum pname, const GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(Lightx)(GLenum light, GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(Lightxv)(GLenum light, GLenum pname, const GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(LineWidthx)(GLfixed width);
GLAPI void APIENTRY GLAPI_PREFIX(LoadMatrixx)(const GLfixed *m);
GLAPI void APIENTRY GLAPI_PREFIX(Materialx)(GLenum face, GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(Materialxv)(GLenum face, GLenum pname, const GLfixed *params);

GLAPI void APIENTRY GLAPI_PREFIX(MultMatrixx)(const GLfixed *m);
GLAPI void APIENTRY GLAPI_PREFIX(MultiTexCoord4x)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
GLAPI void APIENTRY GLAPI_PREFIX(Normal3x)(GLfixed nx, GLfixed ny, GLfixed nz);
GLAPI void APIENTRY GLAPI_PREFIX(Orthof)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
GLAPI void APIENTRY GLAPI_PREFIX(Orthox)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
GLAPI void APIENTRY GLAPI_PREFIX(PointSizex)(GLfixed size);
GLAPI void APIENTRY GLAPI_PREFIX(PolygonOffsetx)(GLfixed factor, GLfixed units);
GLAPI void APIENTRY GLAPI_PREFIX(Rotatex)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY GLAPI_PREFIX(SampleCoveragex)(GLclampx value, GLboolean invert);
GLAPI void APIENTRY GLAPI_PREFIX(Scalex)(GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY GLAPI_PREFIX(TexEnvx)(GLenum target, GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(TexEnvxv)(GLenum target, GLenum pname, const GLfixed *params);

GLAPI void APIENTRY GLAPI_PREFIX(TexParameterx)(GLenum target, GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(Translatex)(GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY GLAPI_PREFIX(ClipPlanef)(GLenum plane, const GLfloat *equation);
GLAPI void APIENTRY GLAPI_PREFIX(ClipPlanex)(GLenum plane, const GLfixed *equation);
GLAPI void APIENTRY GLAPI_PREFIX(GetClipPlanef)(GLenum plane, GLfloat *equation);
GLAPI void APIENTRY GLAPI_PREFIX(GetClipPlanex)(GLenum plane, GLfixed *equation);
GLAPI void APIENTRY GLAPI_PREFIX(GetFixedv)(GLenum pname, GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetLightxv)(GLenum light, GLenum pname, GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetMaterialxv)(GLenum face, GLenum pname, GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexEnvxv)(GLenum target, GLenum pname, GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(GetTexParameterxv)(GLenum target, GLenum pname, GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterx)(GLenum pname, GLfixed param);
GLAPI void APIENTRY GLAPI_PREFIX(PointParameterxv)(GLenum pname, const GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(TexParameterxv)(GLenum target, GLenum pname, const GLfixed *params);
GLAPI void APIENTRY GLAPI_PREFIX(BlendBarrier)(void);
GLAPI void APIENTRY GLAPI_PREFIX(PrimitiveBoundingBox)(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
#undef MAPI_TMP_DEFINES
#endif /* MAPI_TMP_DEFINES */

#ifdef MAPI_TMP_PUBLIC_ENTRIES_NO_HIDDEN
((void (APIENTRY *)(GLuint program, GLuint colorNumber, const GLchar *name)) _func)(program,
 colorNumber, name);
}

GLAPI void APIENTRY GLAPI_PREFIX(ClampColor)(GLenum target, GLenum clamp)
{
   const struct _glapi_table *_tbl = entry_current_get();
   mapi_func _func = ((const mapi_func *) _tbl)[575];
   ((void (APIENTRY *)(GLenum target, GLenum clamp)) _func)(target, clamp);
}

GLAPI void APIENTRY GLAPI_PREFIX(ClampColorARB)(GLenum target, GLenum clamp)
{
   const struct _glapi_table *_tbl = entry_current_get();
   mapi_func _func = ((const mapi_func *) _tbl)[575];
   ((void (APIENTRY *)(GLenum target, GLenum clamp
"\t"STUB_ASM_CODE("739")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP2ui))"\n"
"\t"STUB_ASM_CODE("740")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP2uiv))"\n"
"\t"STUB_ASM_CODE("741")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP3ui))"\n"
"\t"STUB_ASM_CODE("742")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP3uiv))"\n"
"\t"STUB_ASM_CODE("743")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP4ui))"\n"
"\t"STUB_ASM_CODE("744")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoordP4uiv))"\n"
"\t"STUB_ASM_CODE("745")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(NormalP3ui))"\n"
"\t"STUB_ASM_CODE("746")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(NormalP3uiv))"\n"
"\t"STUB_ASM_CODE("747")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(SecondaryColorP3ui))"\n"
"\t"STUB_ASM_CODE("748")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(SecondaryColorP3uiv))"\n"
"\t"STUB_ASM_CODE("749")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP1ui))"\n"
"\t"STUB_ASM_CODE("750")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP1uiv))"\n"
"\t"STUB_ASM_CODE("751")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP2ui))"\n"
"\t"STUB_ASM_CODE("752")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP2uiv))"\n"
"\t"STUB_ASM_CODE("753")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP3ui))"\n"
"\t"STUB_ASM_CODE("754")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP3uiv))"\n"
"\t"STUB_ASM_CODE("755")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP4ui))"\n"
"\t"STUB_ASM_CODE("756")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordP4uiv))"\n"
"\t"STUB_ASM_CODE("757")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP1ui))"\n"
"\t"STUB_ASM_CODE("758")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP1uiv))"\n"
"\t"STUB_ASM_CODE("759")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP2ui))"\n"
"\t"STUB_ASM_CODE("760")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP2uiv))"\n"
"\t"STUB_ASM_CODE("761")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP3ui))"\n"
"\t"STUB_ASM_CODE("762")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP3uiv))"\n"
"\t"STUB_ASM_CODE("763")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP4ui))"\n"
"\t"STUB_ASM_CODE("764")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribP4uiv))"\n"
"\t"STUB_ASM_CODE("765")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP2ui))"\n"
"\t"STUB_ASM_CODE("766")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP2uiv))"\n"
"\t"STUB_ASM_CODE("767")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP3ui))"\n"
"\t"STUB_ASM_CODE("768")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP3uiv))"\n"
"\t"STUB_ASM_CODE("769")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP4ui))"\n"
"\t"STUB_ASM_CODE("770")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexP4uiv))"\n"
"\t"STUB_ASM_CODE("771")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawArraysIndirect))"\n"
"\t"STUB_ASM_CODE("772")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawElementsIndirect))"\n"
"\t"STUB_ASM_CODE("773")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PatchParameteri))"\n"
"\t"STUB_ASM_CODE("801")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindTransformFeedback))"\n"
"\t"STUB_ASM_CODE("802")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DeleteTransformFeedbacks))"\n"
"\t"STUB_ASM_CODE("803")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawTransformFeedback))"\n"
"\t"STUB_ASM_CODE("804")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GenTransformFeedbacks))"\n"
"\t"STUB_ASM_CODE("805")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(IsTransformFeedback))"\n"
"\t"STUB_ASM_CODE("806")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PauseTransformFeedback))"\n"
"\t"STUB_ASM_CODE("807")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ResumeTransformFeedback))"\n"
"\t"STUB_ASM_CODE("808")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BeginQueryIndexed))"\n"
"\t"STUB_ASM_CODE("809")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawTransformFeedbackStream))"\n"
"\t"STUB_ASM_CODE("810")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(EndQueryIndexed))"\n"
"\t"STUB_ASM_CODE("811")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetQueryIndexediv))"\n"
"\t"STUB_ASM_CODE("812")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearDepthf))"\n"
"\t"STUB_ASM_CODE("813")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DepthRangef))"\n"
"\t"STUB_ASM_CODE("814")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetShaderPrecisionFormat))"\n"
"\t"STUB_ASM_CODE("815")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ReleaseShaderCompiler))"\n"
"\t"STUB_ASM_CODE("816")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ShaderBinary))"\n"
"\t"STUB_ASM_CODE("817")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramBinary))"\n"
"\t"STUB_ASM_CODE("818")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramBinary))"\n"
"\t"STUB_ASM_CODE("819")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramParameteri))"\n"
"\t"STUB_ASM_CODE("820")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DepthRangeArrayv))"\n"
"\t"STUB_ASM_CODE("831")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DepthRangeIndexed))"\n"
"\t"STUB_ASM_CODE("832")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetDoublei_v))"\n"
"\t"STUB_ASM_CODE("833")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetFloati_v))"\n"
"\t"STUB_ASM_CODE("834")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ScissorArrayv))"\n"
"\t"STUB_ASM_CODE("835")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ScissorIndexed))"\n"
"\t"STUB_ASM_CODE("836")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ScissorIndexedv))"\n"
"\t"STUB_ASM_CODE("837")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ViewportArrayv))"\n"
"\t"STUB_ASM_CODE("838")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ViewportIndexedf))"\n"
"\t"STUB_ASM_CODE("839")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ViewportIndexedfv))"\n"
"\t"STUB_ASM_CODE("840")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetGraphicsResetStatusARB))"\n"
"\t"STUB_ASM_CODE("841")"\n"

".globl "GLAPI_PREFIX_STR(GetGraphicsResetStatus)"\n"
".set "GLAPI_PREFIX_STR(GetGraphicsResetStatus)", "GLAPI_PREFIX_STR(GetGraphicsResetStatusARB)"
\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnColorTableARB))"\n"
"\t"STUB_ASM_CODE("842")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnCompressedTexImageARB))"\n"
"\t"STUB_ASM_CODE("843")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnConvolutionFilterARB))"\n"
"\t"STUB_ASM_CODE("844")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnHistogramARB))"\n"
"\t"STUB_ASM_CODE("845")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnMapdvARB))"\n"
"\t"STUB_ASM_CODE("846")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnMapfvARB))"\n"
"\t"STUB_ASM_CODE("847")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnMapivARB))"\n"
"\t"STUB_ASM_CODE("848")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnMinmaxARB))"\n"
"\t"STUB_ASM_CODE("849")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnPixelMapfvARB))"\n"
"\t"STUB_ASM_CODE("850")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnPixelMapuivARB))"\n"
"\t"STUB_ASM_CODE("851")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnPixelMapusvARB))"\n"
"\t"STUB_ASM_CODE("852")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnPolygonStippleARB))"\n"
"\t"STUB_ASM_CODE("853")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnSeparableFilterARB))"\n"
"\t"STUB_ASM_CODE("854")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnTexImageARB))"\n"
"\t"STUB_ASM_CODE("855")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnUniformdvARB))"\n"
"\t"STUB_ASM_CODE("856")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnUniformfvARB))"\n"
"\t"STUB_ASM_CODE("857")"\n"

".globl "GLAPI_PREFIX_STR(GetnUniformfv)"\n"
".set "GLAPI_PREFIX_STR(GetnUniformfv)", "GLAPI_PREFIX_STR(GetnUniformfvARB)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnUniformivARB))"\n"
"\t"STUB_ASM_CODE("858")"\n"

".globl "GLAPI_PREFIX_STR(GetnUniformiv)"\n"
".set "GLAPI_PREFIX_STR(GetnUniformiv)", "GLAPI_PREFIX_STR(GetnUniformivARB)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetnUniformuivARB))"\n"
"\t"STUB_ASM_CODE("859")"\n"

".globl "GLAPI_PREFIX_STR(GetnUniformuiv)"\n"
".set "GLAPI_PREFIX_STR(GetnUniformuiv)", "GLAPI_PREFIX_STR(GetnUniformuivARB)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ReadnPixelsARB))"\n"
"\t"STUB_ASM_CODE("860")"\n"

".globl "GLAPI_PREFIX_STR(ReadnPixels)"\n"
".set "GLAPI_PREFIX_STR(ReadnPixels)", "GLAPI_PREFIX_STR(ReadnPixelsARB)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawArraysInstancedBaseInstance))"\n"
"\t"STUB_ASM_CODE("861")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawElementsInstancedBaseInstance))"\n"
"\t"STUB_ASM_CODE("862")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawElementsInstancedBaseVertexBaseInstance))"\n"
"\t"STUB_ASM_CODE("863")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawTransformFeedbackInstanced))"\n"
"\t"STUB_ASM_CODE("864")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DrawTransformFeedbackStreamInstanced))"\n"
"\t"STUB_ASM_CODE("865")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetActiveAtomicCounterBufferiv))"\n"
"\t"STUB_ASM_CODE("867")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindImageTexture))"\n"
"\t"STUB_ASM_CODE("868")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MemoryBarrier))"\n"
"\t"STUB_ASM_CODE("869")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexStorage1D))"\n"
"\t"STUB_ASM_CODE("870")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexStorage2D))"\n"
"\t"STUB_ASM_CODE("871")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexStorage3D))"\n"
"\t"STUB_ASM_CODE("872")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TextureStorage1DEXT))"\n"
"\t"STUB_ASM_CODE("873")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TextureStorage2DEXT))"\n"
"\t"STUB_ASM_CODE("874")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TextureStorage3DEXT))"\n"
"\t"STUB_ASM_CODE("875")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearBufferData))"\n"
"\t"STUB_ASM_CODE("876")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearBufferSubData))"\n"
"\t"STUB_ASM_CODE("877")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DispatchCompute))"\n"
"\t"STUB_ASM_CODE("878")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DispatchComputeIndirect))"\n"
"\t"STUB_ASM_CODE("879")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(CopyImageSubData))"\n"
"\t"STUB_ASM_CODE("880")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TextureView))"\n"
"\t"STUB_ASM_CODE("881")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindVertexBuffer))"\n"
"\t"STUB_ASM_CODE("882")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribBinding))"\n"
"\t"STUB_ASM_CODE("883")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribFormat))"\n"
"\t"STUB_ASM_CODE("884")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribIFormat))"\n"
"\t"STUB_ASM_CODE("885")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribLFormat))"\n"
"\t"STUB_ASM_CODE("886")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexBindingDivisor))"\n"
"\t"STUB_ASM_CODE("887")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(FramebufferParameteri))"\n"
"\t"STUB_ASM_CODE("888")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetFramebufferParameteriv))"\n"
"\t"STUB_ASM_CODE("889")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiDrawArraysIndirect))"\n"
"\t"STUB_ASM_CODE("891")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiDrawElementsIndirect))"\n"
"\t"STUB_ASM_CODE("892")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramInterfaceiv))"\n"
"\t"STUB_ASM_CODE("893")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramResourceIndex))"\n"
"\t"STUB_ASM_CODE("894")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramResourceLocation))"\n"
"\t"STUB_ASM_CODE("895")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramResourceName))"\n"
"\t"STUB_ASM_CODE("897")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramResourceiv))"\n"
"\t"STUB_ASM_CODE("898")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexBufferRange))"\n"
"\t"STUB_ASM_CODE("900")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexStorage2DMultisample))"\n"
"\t"STUB_ASM_CODE("901")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexStorage3DMultisample))"\n"
"\t"STUB_ASM_CODE("902")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BufferStorage))"\n"
"\t"STUB_ASM_CODE("903")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearTexImage))"\n"
"\t"STUB_ASM_CODE("904")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearTexSubImage))"\n"
"\t"STUB_ASM_CODE("905")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindBuffersBase))"\n"
"\t"STUB_ASM_CODE("906")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindBuffersRange))"\n"
"\t"STUB_ASM_CODE("907")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindImageTextures))"\n"
"\t"STUB_ASM_CODE("908")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindSamplers))"\n"
"\t"STUB_ASM_CODE("909")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindTextures))"\n"
"\t"STUB_ASM_CODE("910")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindVertexBuffers))"\n"
"\t"STUB_ASM_CODE("911")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateBufferData))"\n"
"\t"STUB_ASM_CODE("1073")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateBufferSubData))"\n"
"\t"STUB_ASM_CODE("1074")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateFramebuffer))"\n"
"\t"STUB_ASM_CODE("1075")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateSubFramebuffer))"\n"
"\t"STUB_ASM_CODE("1076")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateTexImage))"\n"
"\t"STUB_ASM_CODE("1077")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(InvalidateTexSubImage))"\n"
"\t"STUB_ASM_CODE("1078")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PointSizePointerOES))"\n"
"\t"STUB_ASM_CODE("1087")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ColorPointerEXT))"\n"
"\t"STUB_ASM_CODE("1091")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(EdgeFlagPointerEXT))"\n"
"\t"STUB_ASM_CODE("1092")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(IndexPointerEXT))"\n"
"\t"STUB_ASM_CODE("1093")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(NormalPointerEXT))"\n"
"\t"STUB_ASM_CODE("1094")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexCoordPointerEXT))"\n"
"\t"STUB_ASM_CODE("1095")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexPointerEXT))"\n"
"\t"STUB_ASM_CODE("1096")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ActiveShaderProgram))"\n"
"\t"STUB_ASM_CODE("1098")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindProgramPipeline))"\n"
"\t"STUB_ASM_CODE("1099")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(CreateShaderProgramv))"\n"
"\t"STUB_ASM_CODE("1100")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DeleteProgramPipelines))"\n"
"\t"STUB_ASM_CODE("1101")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GenProgramPipelines))"\n"
"\t"STUB_ASM_CODE("1102")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramPipelineInfoLog))"\n"
"\t"STUB_ASM_CODE("1103")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetProgramPipelineiv))"\n"
"\t"STUB_ASM_CODE("1104")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(IsProgramPipeline))"\n"
"\t"STUB_ASM_CODE("1105")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(LockArraysEXT))"\n"
"\t"STUB_ASM_CODE("1106")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1f))"\n"
"\t"STUB_ASM_CODE("1109")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1fv))"\n"
"\t"STUB_ASM_CODE("1110")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1i))"\n"
"\t"STUB_ASM_CODE("1111")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1iv))"\n"
"\t"STUB_ASM_CODE("1112")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1ui))"\n"
"\t"STUB_ASM_CODE("1113")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform1uiv))"\n"
"\t"STUB_ASM_CODE("1114")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2f))"\n"
"\t"STUB_ASM_CODE("1117")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2fv))"\n"
"\t"STUB_ASM_CODE("1118")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2i))"\n"
"\t"STUB_ASM_CODE("1119")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2iv))"\n"
"\t"STUB_ASM_CODE("1120")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2ui))"\n"
"\t"STUB_ASM_CODE("1121")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform2uiv))"\n"
"\t"STUB_ASM_CODE("1122")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3f))"\n"
"\t"STUB_ASM_CODE("1125")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3fv))"\n"
"\t"STUB_ASM_CODE("1126")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3i))"\n"
"\t"STUB_ASM_CODE("1127")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3iv))"\n"
"\t"STUB_ASM_CODE("1128")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3ui))"\n"
"\t"STUB_ASM_CODE("1129")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform3uiv))"\n"
"\t"STUB_ASM_CODE("1130")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4f))"\n"
"\t"STUB_ASM_CODE("1133")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4fv))"\n"
"\t"STUB_ASM_CODE("1134")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4i))"\n"
"\t"STUB_ASM_CODE("1135")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4iv))"\n"
"\t"STUB_ASM_CODE("1136")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4ui))"\n"
"\t"STUB_ASM_CODE("1137")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniform4uiv))"\n"
"\t"STUB_ASM_CODE("1138")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix2fv))"\n"
"\t"STUB_ASM_CODE("1140")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix2x3fv))"\n"
"\t"STUB_ASM_CODE("1142")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix2x4fv))"\n"
"\t"STUB_ASM_CODE("1144")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix3fv))"\n"
"\t"STUB_ASM_CODE("1146")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix3x2fv))"\n"
"\t"STUB_ASM_CODE("1148")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix3x4fv))"\n"
"\t"STUB_ASM_CODE("1150")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix4fv))"\n"
"\t"STUB_ASM_CODE("1152")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix4x2fv))"\n"
"\t"STUB_ASM_CODE("1154")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ProgramUniformMatrix4x3fv))"\n"
"\t"STUB_ASM_CODE("1156")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(UnlockArraysEXT))"\n"
"\t"STUB_ASM_CODE("1157")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(UseProgramStages))"\n"
"\t"STUB_ASM_CODE("1158")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ValidateProgramPipeline))"\n"
"\t"STUB_ASM_CODE("1159")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DebugMessageCallback))"\n"
"\t"STUB_ASM_CODE("1161")"\n"

".globl "GLAPI_PREFIX_STR(DebugMessageCallbackARB)"\n"
".set "GLAPI_PREFIX_STR(DebugMessageCallbackARB)", "GLAPI_PREFIX_STR(DebugMessageCallback)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DebugMessageControl))"\n"
"\t"STUB_ASM_CODE("1162")"\n"

".globl "GLAPI_PREFIX_STR(DebugMessageControlARB)"\n"
".set "GLAPI_PREFIX_STR(DebugMessageControlARB)", "GLAPI_PREFIX_STR(DebugMessageControl)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DebugMessageInsert))"\n"
"\t"STUB_ASM_CODE("1163")"\n"

".globl "GLAPI_PREFIX_STR(DebugMessageInsertARB)"\n"
".set "GLAPI_PREFIX_STR(DebugMessageInsertARB)", "GLAPI_PREFIX_STR(DebugMessageInsert)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetDebugMessageLog))"\n"
"\t"STUB_ASM_CODE("1164")"\n"

".globl "GLAPI_PREFIX_STR(GetDebugMessageLogARB)"\n"
".set "GLAPI_PREFIX_STR(GetDebugMessageLogARB)", "GLAPI_PREFIX_STR(GetDebugMessageLog)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetObjectLabel))"\n"
"\t"STUB_ASM_CODE("1165")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetObjectPtrLabel))"\n"
"\t"STUB_ASM_CODE("1166")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ObjectLabel))"\n"
"\t"STUB_ASM_CODE("1167")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ObjectPtrLabel))"\n"
"\t"STUB_ASM_CODE("1168")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PopDebugGroup))"\n"
"\t"STUB_ASM_CODE("1169")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PushDebugGroup))"\n"
"\t"STUB_ASM_CODE("1170")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(SecondaryColor3fEXT))"\n"
"\t"STUB_ASM_CODE("1171")"\n"

".globl "GLAPI_PREFIX_STR(SecondaryColor3f)"\n"
".set "GLAPI_PREFIX_STR(SecondaryColor3f)", "GLAPI_PREFIX_STR(SecondaryColor3fEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(SecondaryColor3fvEXT))"\n"
"\t"STUB_ASM_CODE("1172")"\n"

".globl "GLAPI_PREFIX_STR(SecondaryColor3fv)"\n"
".set "GLAPI_PREFIX_STR(SecondaryColor3fv)", "GLAPI_PREFIX_STR(SecondaryColor3fvEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiDrawElementsEXT))"\n"
"\t"STUB_ASM_CODE("1173")"\n"

".globl "GLAPI_PREFIX_STR(MultiDrawElements)"\n"
".set "GLAPI_PREFIX_STR(MultiDrawElements)", "GLAPI_PREFIX_STR(MultiDrawElementsEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(FogCoordfEXT))"\n"
"\t"STUB_ASM_CODE("1174")"\n"

".globl "GLAPI_PREFIX_STR(FogCoordf)"\n"
".set "GLAPI_PREFIX_STR(FogCoordf)", "GLAPI_PREFIX_STR(FogCoordfEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(FogCoordfvEXT))"\n"
"\t"STUB_ASM_CODE("1175")"\n"

".globl "GLAPI_PREFIX_STR(FogCoordfv)"\n"
".set "GLAPI_PREFIX_STR(FogCoordfv)", "GLAPI_PREFIX_STR(FogCoordfvEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PrimitiveRestartNV))"\n"
"\t"STUB_ASM_CODE("1269")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindFramebufferEXT))"\n"
"\t"STUB_ASM_CODE("1274")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BindRenderbufferEXT))"\n"
"\t"STUB_ASM_CODE("1275")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI1iEXT))"\n"
"\t"STUB_ASM_CODE("1279")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI1i)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI1i)", "GLAPI_PREFIX_STR(VertexAttribI1iEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI1uiEXT))"\n"
"\t"STUB_ASM_CODE("1280")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI1ui)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI1ui)", "GLAPI_PREFIX_STR(VertexAttribI1uiEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI2iEXT))"\n"
"\t"STUB_ASM_CODE("1281")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI2i)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI2i)", "GLAPI_PREFIX_STR(VertexAttribI2iEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI2ivEXT))"\n"
"\t"STUB_ASM_CODE("1282")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI2iv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI2iv)", "GLAPI_PREFIX_STR(VertexAttribI2ivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI2uiEXT))"\n"
"\t"STUB_ASM_CODE("1283")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI2ui)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI2ui)", "GLAPI_PREFIX_STR(VertexAttribI2uiEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI2uivEXT))"\n"
"\t"STUB_ASM_CODE("1284")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI2uiv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI2uiv)", "GLAPI_PREFIX_STR(VertexAttribI2uivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI3iEXT))"\n"
"\t"STUB_ASM_CODE("1285")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI3i)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI3i)", "GLAPI_PREFIX_STR(VertexAttribI3iEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI3ivEXT))"\n"
"\t"STUB_ASM_CODE("1286")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI3iv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI3iv)", "GLAPI_PREFIX_STR(VertexAttribI3ivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI3uiEXT))"\n"
"\t"STUB_ASM_CODE("1287")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI3ui)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI3ui)", "GLAPI_PREFIX_STR(VertexAttribI3uiEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI3uivEXT))"\n"
"\t"STUB_ASM_CODE("1288")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI3uiv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI3uiv)", "GLAPI_PREFIX_STR(VertexAttribI3uivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI4iEXT))"\n"
"\t"STUB_ASM_CODE("1289")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI4i)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI4i)", "GLAPI_PREFIX_STR(VertexAttribI4iEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI4ivEXT))"\n"
"\t"STUB_ASM_CODE("1290")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI4iv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI4iv)", "GLAPI_PREFIX_STR(VertexAttribI4ivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI4uiEXT))"\n"
"\t"STUB_ASM_CODE("1291")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI4ui)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI4ui)", "GLAPI_PREFIX_STR(VertexAttribI4uiEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(VertexAttribI4uivEXT))"\n"
"\t"STUB_ASM_CODE("1292")"\n"

".globl "GLAPI_PREFIX_STR(VertexAttribI4uiv)"\n"
".set "GLAPI_PREFIX_STR(VertexAttribI4uiv)", "GLAPI_PREFIX_STR(VertexAttribI4uivEXT)"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearColorIiEXT))"\n"
"\t"STUB_ASM_CODE("1293")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearColorIuiEXT))"\n"
"\t"STUB_ASM_CODE("1294")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TextureBarrierNV))"\n"
"\t"STUB_ASM_CODE("1313")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(AlphaFuncx))"\n"
"\t"STUB_ASM_CODE("1375")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearColorx))"\n"
"\t"STUB_ASM_CODE("1376")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClearDepthx))"\n"
"\t"STUB_ASM_CODE("1377")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Color4x))"\n"
"\t"STUB_ASM_CODE("1378")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(DepthRangex))"\n"
"\t"STUB_ASM_CODE("1379")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Fogx))"\n"
"\t"STUB_ASM_CODE("1380")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Fogxv))"\n"
"\t"STUB_ASM_CODE("1381")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Frustumf))"\n"
"\t"STUB_ASM_CODE("1382")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Frustumx))"\n"
"\t"STUB_ASM_CODE("1383")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(LightModelx))"\n"
"\t"STUB_ASM_CODE("1384")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(LightModelxv))"\n"
"\t"STUB_ASM_CODE("1385")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Lightx))"\n"
"\t"STUB_ASM_CODE("1386")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Lightxv))"\n"
"\t"STUB_ASM_CODE("1387")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(LineWidthx))"\n"
"\t"STUB_ASM_CODE("1388")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(LoadMatrixx))"\n"
"\t"STUB_ASM_CODE("1389")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Materialx))"\n"
"\t"STUB_ASM_CODE("1390")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Materialxv))"\n"
"\t"STUB_ASM_CODE("1391")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultMatrixx))"\n"
"\t"STUB_ASM_CODE("1392")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(MultiTexCoord4x))"\n"
"\t"STUB_ASM_CODE("1393")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Normal3x))"\n"
"\t"STUB_ASM_CODE("1394")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Orthof))"\n"
"\t"STUB_ASM_CODE("1395")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Orthox))"\n"
"\t"STUB_ASM_CODE("1396")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PointSizex))"\n"
"\t"STUB_ASM_CODE("1397")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PolygonOffsetx))"\n"
"\t"STUB_ASM_CODE("1398")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Rotatex))"\n"
"\t"STUB_ASM_CODE("1399")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(SampleCoveragex))"\n"
"\t"STUB_ASM_CODE("1400")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Scalex))"\n"
"\t"STUB_ASM_CODE("1401")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexEnvx))"\n"
"\t"STUB_ASM_CODE("1402")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexEnvxv))"\n"
"\t"STUB_ASM_CODE("1403")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexParameterx))"\n"
"\t"STUB_ASM_CODE("1404")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(Translatex))"\n"
"\t"STUB_ASM_CODE("1405")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClipPlanef))"\n"
"\t"STUB_ASM_CODE("1406")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(ClipPlanex))"\n"
"\t"STUB_ASM_CODE("1407")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetClipPlanef))"\n"
"\t"STUB_ASM_CODE("1408")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetClipPlanex))"\n"
"\t"STUB_ASM_CODE("1409")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetFixedv))"\n"
"\t"STUB_ASM_CODE("1410")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetLightxv))"\n"
"\t"STUB_ASM_CODE("1411")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetMaterialxv))"\n"
"\t"STUB_ASM_CODE("1412")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetTexEnvxv))"\n"
"\t"STUB_ASM_CODE("1413")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(GetTexParameterxv))"\n"
"\t"STUB_ASM_CODE("1414")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PointParameterx))"\n"
"\t"STUB_ASM_CODE("1415")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PointParameterxv))"\n"
"\t"STUB_ASM_CODE("1416")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(TexParameterxv))"\n"
"\t"STUB_ASM_CODE("1417")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(BlendBarrier))"\n"
"\t"STUB_ASM_CODE("1418")"\n"

STUB_ASM_ENTRY(GLAPI_PREFIX_STR(PrimitiveBoundingBox))"\n"
"\t"STUB_ASM_CODE("1419")"\n"

);
#undef MAPI_TMP_STUB_ASM_GCC_NO_HIDDEN
#endif /* MAPI_TMP_STUB_ASM_GCC_NO_HIDDEN */

#ifdef _WIN32
#define NOGDI
#endif

#include "../main/glheader.h"
#include "../main/context.h"
#include "../main/dispatch.h"
#include "../main/dlist.h"
#include "../main/eval.h"
#include "vbo_noop.h"
#include "vbo_attrib.h"

static void GLAPIENTRY
_mesa_noop_Materialfv(GLenum face, GLenum pname, const GLfloat * params)
{
}

static void GLAPIENTRY
_mesa_noop_EvalCoord1f(GLfloat a)
{
}

static void GLAPIENTRY
_mesa_noop_EvalCoord1fv(const GLfloat * v)
{
}

static void GLAPIENTRY
_mesa_noop_EvalCoord2f(GLfloat a, GLfloat b)
{
}

static void GLAPIENTRY
_mesa_noop_EvalCoord2fv(const GLfloat * v)
{
}

static void GLAPIENTRY
_mesa_noop_EvalPoint1(GLint a)
{
}

static void GLAPIENTRY
_mesa_noop_EvalPoint2(GLint a, GLint b)
{
}

static void GLAPIENTRY
_mesa_noop_ArrayElement(GLint elem)
{
}


static void GLAPIENTRY
_mesa_noop_Begin(GLenum mode)
{
}

static void GLAPIENTRY
_mesa_noop_End(void)
{
}

static void GLAPIENTRY
_mesa_noop_PrimitiveRestartNV(void)
{
}

/**
 * If index=0, does glVertexAttrib*() alias glVertex() to emit a vertex?
 * It depends on a few things, including whether we're inside or outside
 * of glBegin/glEnd.
 */
static inline bool
is_vertex_position(const struct gl_context *ctx, GLuint index)
{
    return false; /* it doesn't matter for noop */
}

#define ATTR_UNION(A, N, T, C, V0, V1, V2, V3) do { (void)ctx; (void)(A); } while(0)
#define ERROR(err) _mesa_error(ctx, err, __func__)
#define TAG(x) _mesa_noop_##x

#include "vbo_attrib_tmp.h"


/**
 * Build a vertexformat of functions that are no-ops.
 * These are used in out-of-memory situations when we have no VBO
 * to put the vertex data into.
 */
void
_mesa_noop_vtxfmt_init(struct gl_context *ctx, GLvertexformat * vfmt)
{
#define NAME_AE(x) _mesa_noop_##x
#define NAME_CALLLIST(x) _mesa_##x
#define NAME(x) _mesa_noop_##x
#define NAME_ES(x) _mesa_noop_##x##ARB

#include "vbo_init_tmp.h"
}


/**
 * Is the given dispatch table using the no-op functions?
 */
GLboolean
_mesa_using_noop_vtxfmt(const struct _glapi_table *dispatch)
{
    return GET_Begin((struct _glapi_table *) dispatch) == _mesa_noop_Begin;
}

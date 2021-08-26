#include "conservativeraster.h"
#include "context.h"
#include "enums.h"

static ALWAYS_INLINE void
conservative_raster_parameter(GLenum pname, GLfloat param,
                              bool no_error, const char *func)
{
    GET_CURRENT_CONTEXT(ctx);

    if (!no_error && !ctx->Extensions.NV_conservative_raster_dilate &&
        !ctx->Extensions.NV_conservative_raster_pre_snap_triangles) {
        _mesa_error(ctx, GL_INVALID_OPERATION, "%s not supported", func);
        return;
    }

    if (MESA_VERBOSE & VERBOSE_API)
        _mesa_debug(ctx, "%s(%s, %g)\n",
                    func, _mesa_enum_to_string(pname), param);

    ASSERT_OUTSIDE_BEGIN_END(ctx);

    switch (pname) {
        case GL_CONSERVATIVE_RASTER_DILATE_NV:
            if (!no_error && !ctx->Extensions.NV_conservative_raster_dilate)
                goto invalid_pname_enum;

            if (!no_error && param<0.0) {
                _mesa_error(ctx, GL_INVALID_VALUE, "%s(param=%g)", func, param);
                return;
            }

            FLUSH_VERTICES(ctx, 0, 0);
            ctx->NewDriverState |=
                    ctx->DriverFlags.NewNvConservativeRasterizationParams;

            ctx->ConservativeRasterDilate =
                    CLAMP(param,
                          ctx->Const.ConservativeRasterDilateRange[0],
                          ctx->Const.ConservativeRasterDilateRange[1]);
            break;
        case GL_CONSERVATIVE_RASTER_MODE_NV:
            if (!no_error && !ctx->Extensions.NV_conservative_raster_pre_snap_triangles)
                goto invalid_pname_enum;

            if (!no_error && param != GL_CONSERVATIVE_RASTER_MODE_POST_SNAP_NV &&
                param != GL_CONSERVATIVE_RASTER_MODE_PRE_SNAP_TRIANGLES_NV) {
                _mesa_error(ctx, GL_INVALID_ENUM,
                            "%s(pname=%s)", func, _mesa_enum_to_string(param));
                return;
            }

            FLUSH_VERTICES(ctx, 0, 0);
            ctx->NewDriverState |=
                    ctx->DriverFlags.NewNvConservativeRasterizationParams;

            ctx->ConservativeRasterMode = param;
            break;
        default:
            goto invalid_pname_enum;
            break;
    }

    return;
    invalid_pname_enum:
    if (!no_error)
        _mesa_error(ctx, GL_INVALID_ENUM, "%s(pname=%s)",
                    func, _mesa_enum_to_string(pname));
}

void GLAPIENTRY
_mesa_ConservativeRasterParameteriNV_no_error(GLenum pname, GLint param)
{
    conservative_raster_parameter(pname, param, true,
                                  "glConservativeRasterParameteriNV");
}

void GLAPIENTRY
_mesa_ConservativeRasterParameteriNV(GLenum pname, GLint param)
{
    conservative_raster_parameter(pname, param, false,
                                  "glConservativeRasterParameteriNV");
}

void GLAPIENTRY
_mesa_ConservativeRasterParameterfNV_no_error(GLenum pname, GLfloat param)
{
    conservative_raster_parameter(pname, param, true,
                                  "glConservativeRasterParameterfNV");
}

void GLAPIENTRY
_mesa_ConservativeRasterParameterfNV(GLenum pname, GLfloat param)
{
    conservative_raster_parameter(pname, param, false,
                                  "glConservativeRasterParameterfNV");
}

void
_mesa_init_conservative_raster(struct gl_context *ctx)
{
    ctx->ConservativeRasterDilate = 0.0;
    ctx->ConservativeRasterMode = GL_CONSERVATIVE_RASTER_MODE_POST_SNAP_NV;
}

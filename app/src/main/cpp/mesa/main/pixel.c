#include "../include/c99_math.h"
#include "glheader.h"
#include "bufferobj.h"
#include "context.h"
#include "macros.h"
#include "pixel.h"
#include "pbo.h"
#include "mtypes.h"


/**********************************************************************/
/*****                    glPixelZoom                             *****/
/**********************************************************************/

void GLAPIENTRY
_mesa_PixelZoom( GLfloat xfactor, GLfloat yfactor )
{
    GET_CURRENT_CONTEXT(ctx);

    if (ctx->Pixel.ZoomX == xfactor &&
        ctx->Pixel.ZoomY == yfactor)
        return;

    FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
    ctx->Pixel.ZoomX = xfactor;
    ctx->Pixel.ZoomY = yfactor;
}



/**********************************************************************/
/*****                         glPixelMap                         *****/
/**********************************************************************/

/**
 * Return pointer to a pixelmap by name.
 */
static struct gl_pixelmap *
get_pixelmap(struct gl_context *ctx, GLenum map)
{
    switch (map) {
        case GL_PIXEL_MAP_I_TO_I:
            return &ctx->PixelMaps.ItoI;
        case GL_PIXEL_MAP_S_TO_S:
            return &ctx->PixelMaps.StoS;
        case GL_PIXEL_MAP_I_TO_R:
            return &ctx->PixelMaps.ItoR;
        case GL_PIXEL_MAP_I_TO_G:
            return &ctx->PixelMaps.ItoG;
        case GL_PIXEL_MAP_I_TO_B:
            return &ctx->PixelMaps.ItoB;
        case GL_PIXEL_MAP_I_TO_A:
            return &ctx->PixelMaps.ItoA;
        case GL_PIXEL_MAP_R_TO_R:
            return &ctx->PixelMaps.RtoR;
        case GL_PIXEL_MAP_G_TO_G:
            return &ctx->PixelMaps.GtoG;
        case GL_PIXEL_MAP_B_TO_B:
            return &ctx->PixelMaps.BtoB;
        case GL_PIXEL_MAP_A_TO_A:
            return &ctx->PixelMaps.AtoA;
        default:
            return NULL;
    }
}


/**
 * Helper routine used by the other _mesa_PixelMap() functions.
 */
static void
store_pixelmap(struct gl_context *ctx, GLenum map, GLsizei mapsize,
               const GLfloat *values)
{
    GLint i;
    struct gl_pixelmap *pm = get_pixelmap(ctx, map);
    if (!pm) {
        _mesa_error(ctx, GL_INVALID_ENUM, "glPixelMap(map)");
        return;
    }

    switch (map) {
        case GL_PIXEL_MAP_S_TO_S:
            /* special case */
            ctx->PixelMaps.StoS.Size = mapsize;
            for (i = 0; i < mapsize; i++) {
                ctx->PixelMaps.StoS.Map[i] = roundf(values[i]);
            }
            break;
        case GL_PIXEL_MAP_I_TO_I:
            /* special case */
            ctx->PixelMaps.ItoI.Size = mapsize;
            for (i = 0; i < mapsize; i++) {
                ctx->PixelMaps.ItoI.Map[i] = values[i];
            }
            break;
        default:
            /* general case */
            pm->Size = mapsize;
            for (i = 0; i < mapsize; i++) {
                GLfloat val = CLAMP(values[i], 0.0F, 1.0F);
                pm->Map[i] = val;
            }
    }
}


/**
 * Convenience wrapper for _mesa_validate_pbo_access() for gl[Get]PixelMap().
 */
static GLboolean
validate_pbo_access(struct gl_context *ctx,
                    struct gl_pixelstore_attrib *pack, GLsizei mapsize,
                    GLenum format, GLenum type, GLsizei clientMemSize,
                    const GLvoid *ptr)
{
    GLboolean ok;

    /* Note, need to use DefaultPacking and Unpack's buffer object */
    _mesa_reference_buffer_object(ctx,
                                  &ctx->DefaultPacking.BufferObj,
                                  pack->BufferObj);

    ok = _mesa_validate_pbo_access(1, &ctx->DefaultPacking, mapsize, 1, 1,
                                   format, type, clientMemSize, ptr);

    /* restore */
    _mesa_reference_buffer_object(ctx,
                                  &ctx->DefaultPacking.BufferObj, NULL);

    if (!ok) {
        if (pack->BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "gl[Get]PixelMap*v(out of bounds PBO access)");
        } else {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glGetnPixelMap*vARB(out of bounds access:"
                        " bufSize (%d) is too small)", clientMemSize);
        }
    }
    return ok;
}


void GLAPIENTRY
_mesa_PixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values )
{
    GET_CURRENT_CONTEXT(ctx);

    /* XXX someday, test against ctx->Const.MaxPixelMapTableSize */
    if (mapsize < 1 || mapsize > MAX_PIXEL_MAP_TABLE) {
        _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapfv(mapsize)" );
        return;
    }

    if (map >= GL_PIXEL_MAP_S_TO_S && map <= GL_PIXEL_MAP_I_TO_A) {
        /* test that mapsize is a power of two */
        if (!util_is_power_of_two_or_zero(mapsize)) {
            _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapfv(mapsize)" );
            return;
        }
    }

    FLUSH_VERTICES(ctx, _NEW_PIXEL, 0);

    if (!validate_pbo_access(ctx, &ctx->Unpack, mapsize, GL_INTENSITY,
                             GL_FLOAT, INT_MAX, values)) {
        return;
    }

    values = (const GLfloat *) _mesa_map_pbo_source(ctx, &ctx->Unpack, values);
    if (!values) {
        if (ctx->Unpack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glPixelMapfv(PBO is mapped)");
        }
        return;
    }

    store_pixelmap(ctx, map, mapsize, values);

    _mesa_unmap_pbo_source(ctx, &ctx->Unpack);
}


void GLAPIENTRY
_mesa_PixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values )
{
    GLfloat fvalues[MAX_PIXEL_MAP_TABLE];
    GET_CURRENT_CONTEXT(ctx);

    if (mapsize < 1 || mapsize > MAX_PIXEL_MAP_TABLE) {
        _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapuiv(mapsize)" );
        return;
    }

    if (map >= GL_PIXEL_MAP_S_TO_S && map <= GL_PIXEL_MAP_I_TO_A) {
        /* test that mapsize is a power of two */
        if (!util_is_power_of_two_or_zero(mapsize)) {
            _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapuiv(mapsize)" );
            return;
        }
    }

    FLUSH_VERTICES(ctx, _NEW_PIXEL, 0);

    if (!validate_pbo_access(ctx, &ctx->Unpack, mapsize, GL_INTENSITY,
                             GL_UNSIGNED_INT, INT_MAX, values)) {
        return;
    }

    values = (const GLuint *) _mesa_map_pbo_source(ctx, &ctx->Unpack, values);
    if (!values) {
        if (ctx->Unpack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glPixelMapuiv(PBO is mapped)");
        }
        return;
    }

    /* convert to floats */
    if (map == GL_PIXEL_MAP_I_TO_I || map == GL_PIXEL_MAP_S_TO_S) {
        GLint i;
        for (i = 0; i < mapsize; i++) {
            fvalues[i] = (GLfloat) values[i];
        }
    }
    else {
        GLint i;
        for (i = 0; i < mapsize; i++) {
            fvalues[i] = UINT_TO_FLOAT( values[i] );
        }
    }

    _mesa_unmap_pbo_source(ctx, &ctx->Unpack);

    store_pixelmap(ctx, map, mapsize, fvalues);
}


void GLAPIENTRY
_mesa_PixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values )
{
    GLfloat fvalues[MAX_PIXEL_MAP_TABLE];
    GET_CURRENT_CONTEXT(ctx);

    if (mapsize < 1 || mapsize > MAX_PIXEL_MAP_TABLE) {
        _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapusv(mapsize)" );
        return;
    }

    if (map >= GL_PIXEL_MAP_S_TO_S && map <= GL_PIXEL_MAP_I_TO_A) {
        /* test that mapsize is a power of two */
        if (!util_is_power_of_two_or_zero(mapsize)) {
            _mesa_error( ctx, GL_INVALID_VALUE, "glPixelMapusv(mapsize)" );
            return;
        }
    }

    FLUSH_VERTICES(ctx, _NEW_PIXEL, 0);

    if (!validate_pbo_access(ctx, &ctx->Unpack, mapsize, GL_INTENSITY,
                             GL_UNSIGNED_SHORT, INT_MAX, values)) {
        return;
    }

    values = (const GLushort *) _mesa_map_pbo_source(ctx, &ctx->Unpack, values);
    if (!values) {
        if (ctx->Unpack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glPixelMapusv(PBO is mapped)");
        }
        return;
    }

    /* convert to floats */
    if (map == GL_PIXEL_MAP_I_TO_I || map == GL_PIXEL_MAP_S_TO_S) {
        GLint i;
        for (i = 0; i < mapsize; i++) {
            fvalues[i] = (GLfloat) values[i];
        }
    }
    else {
        GLint i;
        for (i = 0; i < mapsize; i++) {
            fvalues[i] = USHORT_TO_FLOAT( values[i] );
        }
    }

    _mesa_unmap_pbo_source(ctx, &ctx->Unpack);

    store_pixelmap(ctx, map, mapsize, fvalues);
}


void GLAPIENTRY
_mesa_GetnPixelMapfvARB( GLenum map, GLsizei bufSize, GLfloat *values )
{
    GET_CURRENT_CONTEXT(ctx);
    GLint mapsize, i;
    const struct gl_pixelmap *pm;

    pm = get_pixelmap(ctx, map);
    if (!pm) {
        _mesa_error(ctx, GL_INVALID_ENUM, "glGetPixelMapfv(map)");
        return;
    }

    mapsize = pm->Size;

    if (!validate_pbo_access(ctx, &ctx->Pack, mapsize, GL_INTENSITY,
                             GL_FLOAT, bufSize, values)) {
        return;
    }

    values = (GLfloat *) _mesa_map_pbo_dest(ctx, &ctx->Pack, values);
    if (!values) {
        if (ctx->Pack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glGetPixelMapfv(PBO is mapped)");
        }
        return;
    }

    if (map == GL_PIXEL_MAP_S_TO_S) {
        /* special case */
        for (i = 0; i < mapsize; i++) {
            values[i] = (GLfloat) ctx->PixelMaps.StoS.Map[i];
        }
    }
    else {
        memcpy(values, pm->Map, mapsize * sizeof(GLfloat));
    }

    _mesa_unmap_pbo_dest(ctx, &ctx->Pack);
}


void GLAPIENTRY
_mesa_GetPixelMapfv( GLenum map, GLfloat *values )
{
    _mesa_GetnPixelMapfvARB(map, INT_MAX, values);
}

void GLAPIENTRY
_mesa_GetnPixelMapuivARB( GLenum map, GLsizei bufSize, GLuint *values )
{
    GET_CURRENT_CONTEXT(ctx);
    GLint mapsize, i;
    const struct gl_pixelmap *pm;

    pm = get_pixelmap(ctx, map);
    if (!pm) {
        _mesa_error(ctx, GL_INVALID_ENUM, "glGetPixelMapuiv(map)");
        return;
    }

    mapsize = pm->Size;

    if (!validate_pbo_access(ctx, &ctx->Pack, mapsize, GL_INTENSITY,
                             GL_UNSIGNED_INT, bufSize, values)) {
        return;
    }

    values = (GLuint *) _mesa_map_pbo_dest(ctx, &ctx->Pack, values);
    if (!values) {
        if (ctx->Pack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glGetPixelMapuiv(PBO is mapped)");
        }
        return;
    }

    if (map == GL_PIXEL_MAP_S_TO_S) {
        /* special case */
        memcpy(values, ctx->PixelMaps.StoS.Map, mapsize * sizeof(GLint));
    }
    else {
        for (i = 0; i < mapsize; i++) {
            values[i] = FLOAT_TO_UINT( pm->Map[i] );
        }
    }

    _mesa_unmap_pbo_dest(ctx, &ctx->Pack);
}


void GLAPIENTRY
_mesa_GetPixelMapuiv( GLenum map, GLuint *values )
{
    _mesa_GetnPixelMapuivARB(map, INT_MAX, values);
}

void GLAPIENTRY
_mesa_GetnPixelMapusvARB( GLenum map, GLsizei bufSize, GLushort *values )
{
    GET_CURRENT_CONTEXT(ctx);
    GLint mapsize, i;
    const struct gl_pixelmap *pm;

    pm = get_pixelmap(ctx, map);
    if (!pm) {
        _mesa_error(ctx, GL_INVALID_ENUM, "glGetPixelMapusv(map)");
        return;
    }

    mapsize = pm->Size;

    if (!validate_pbo_access(ctx, &ctx->Pack, mapsize, GL_INTENSITY,
                             GL_UNSIGNED_SHORT, bufSize, values)) {
        return;
    }

    values = (GLushort *) _mesa_map_pbo_dest(ctx, &ctx->Pack, values);
    if (!values) {
        if (ctx->Pack.BufferObj) {
            _mesa_error(ctx, GL_INVALID_OPERATION,
                        "glGetPixelMapusv(PBO is mapped)");
        }
        return;
    }

    switch (map) {
        /* special cases */
        case GL_PIXEL_MAP_I_TO_I:
            for (i = 0; i < mapsize; i++) {
                values[i] = (GLushort) CLAMP(ctx->PixelMaps.ItoI.Map[i], 0.0F, 65535.0F);
            }
            break;
        case GL_PIXEL_MAP_S_TO_S:
            for (i = 0; i < mapsize; i++) {
                values[i] = (GLushort) CLAMP(ctx->PixelMaps.StoS.Map[i], 0.0F, 65535.0F);
            }
            break;
        default:
            for (i = 0; i < mapsize; i++) {
                CLAMPED_FLOAT_TO_USHORT(values[i], pm->Map[i] );
            }
    }

    _mesa_unmap_pbo_dest(ctx, &ctx->Pack);
}


void GLAPIENTRY
_mesa_GetPixelMapusv( GLenum map, GLushort *values )
{
    _mesa_GetnPixelMapusvARB(map, INT_MAX, values);
}


/**********************************************************************/
/*****                       glPixelTransfer                      *****/
/**********************************************************************/


/*
 * Implements glPixelTransfer[fi] whether called immediately or from a
 * display list.
 */
void GLAPIENTRY
_mesa_PixelTransferf( GLenum pname, GLfloat param )
{
    GET_CURRENT_CONTEXT(ctx);

    switch (pname) {
        case GL_MAP_COLOR:
            if (ctx->Pixel.MapColorFlag == (param ? GL_TRUE : GL_FALSE))
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.MapColorFlag = param ? GL_TRUE : GL_FALSE;
            break;
        case GL_MAP_STENCIL:
            if (ctx->Pixel.MapStencilFlag == (param ? GL_TRUE : GL_FALSE))
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.MapStencilFlag = param ? GL_TRUE : GL_FALSE;
            break;
        case GL_INDEX_SHIFT:
            if (ctx->Pixel.IndexShift == (GLint) param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.IndexShift = (GLint) param;
            break;
        case GL_INDEX_OFFSET:
            if (ctx->Pixel.IndexOffset == (GLint) param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.IndexOffset = (GLint) param;
            break;
        case GL_RED_SCALE:
            if (ctx->Pixel.RedScale == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.RedScale = param;
            break;
        case GL_RED_BIAS:
            if (ctx->Pixel.RedBias == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.RedBias = param;
            break;
        case GL_GREEN_SCALE:
            if (ctx->Pixel.GreenScale == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.GreenScale = param;
            break;
        case GL_GREEN_BIAS:
            if (ctx->Pixel.GreenBias == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.GreenBias = param;
            break;
        case GL_BLUE_SCALE:
            if (ctx->Pixel.BlueScale == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.BlueScale = param;
            break;
        case GL_BLUE_BIAS:
            if (ctx->Pixel.BlueBias == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.BlueBias = param;
            break;
        case GL_ALPHA_SCALE:
            if (ctx->Pixel.AlphaScale == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.AlphaScale = param;
            break;
        case GL_ALPHA_BIAS:
            if (ctx->Pixel.AlphaBias == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.AlphaBias = param;
            break;
        case GL_DEPTH_SCALE:
            if (ctx->Pixel.DepthScale == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.DepthScale = param;
            break;
        case GL_DEPTH_BIAS:
            if (ctx->Pixel.DepthBias == param)
                return;
            FLUSH_VERTICES(ctx, _NEW_PIXEL, GL_PIXEL_MODE_BIT);
            ctx->Pixel.DepthBias = param;
            break;
        default:
            _mesa_error( ctx, GL_INVALID_ENUM, "glPixelTransfer(pname)" );
            return;
    }
}


void GLAPIENTRY
_mesa_PixelTransferi( GLenum pname, GLint param )
{
    _mesa_PixelTransferf( pname, (GLfloat) param );
}



/**********************************************************************/
/*****                    State Management                        *****/
/**********************************************************************/


/**
 * Update mesa pixel transfer derived state to indicate which operations are
 * enabled.
 */
void
_mesa_update_pixel( struct gl_context *ctx )
{
    GLuint mask = 0;

    if (ctx->Pixel.RedScale   != 1.0F || ctx->Pixel.RedBias   != 0.0F ||
        ctx->Pixel.GreenScale != 1.0F || ctx->Pixel.GreenBias != 0.0F ||
        ctx->Pixel.BlueScale  != 1.0F || ctx->Pixel.BlueBias  != 0.0F ||
        ctx->Pixel.AlphaScale != 1.0F || ctx->Pixel.AlphaBias != 0.0F)
        mask |= IMAGE_SCALE_BIAS_BIT;

    if (ctx->Pixel.IndexShift || ctx->Pixel.IndexOffset)
        mask |= IMAGE_SHIFT_OFFSET_BIT;

    if (ctx->Pixel.MapColorFlag)
        mask |= IMAGE_MAP_COLOR_BIT;

    ctx->_ImageTransferState = mask;
}


/**********************************************************************/
/*****                      Initialization                        *****/
/**********************************************************************/

static void
init_pixelmap(struct gl_pixelmap *map)
{
    map->Size = 1;
    map->Map[0] = 0.0;
}


/**
 * Initialize the context's PIXEL attribute group.
 */
void
_mesa_init_pixel( struct gl_context *ctx )
{
    /* Pixel group */
    ctx->Pixel.RedBias = 0.0;
    ctx->Pixel.RedScale = 1.0;
    ctx->Pixel.GreenBias = 0.0;
    ctx->Pixel.GreenScale = 1.0;
    ctx->Pixel.BlueBias = 0.0;
    ctx->Pixel.BlueScale = 1.0;
    ctx->Pixel.AlphaBias = 0.0;
    ctx->Pixel.AlphaScale = 1.0;
    ctx->Pixel.DepthBias = 0.0;
    ctx->Pixel.DepthScale = 1.0;
    ctx->Pixel.IndexOffset = 0;
    ctx->Pixel.IndexShift = 0;
    ctx->Pixel.ZoomX = 1.0;
    ctx->Pixel.ZoomY = 1.0;
    ctx->Pixel.MapColorFlag = GL_FALSE;
    ctx->Pixel.MapStencilFlag = GL_FALSE;
    init_pixelmap(&ctx->PixelMaps.StoS);
    init_pixelmap(&ctx->PixelMaps.ItoI);
    init_pixelmap(&ctx->PixelMaps.ItoR);
    init_pixelmap(&ctx->PixelMaps.ItoG);
    init_pixelmap(&ctx->PixelMaps.ItoB);
    init_pixelmap(&ctx->PixelMaps.ItoA);
    init_pixelmap(&ctx->PixelMaps.RtoR);
    init_pixelmap(&ctx->PixelMaps.GtoG);
    init_pixelmap(&ctx->PixelMaps.BtoB);
    init_pixelmap(&ctx->PixelMaps.AtoA);

    if (ctx->Visual.doubleBufferMode) {
        ctx->Pixel.ReadBuffer = GL_BACK;
    }
    else {
        ctx->Pixel.ReadBuffer = GL_FRONT;
    }

    /* Miscellaneous */
    ctx->_ImageTransferState = 0;
}

#ifndef TEXSTATE_H
#define TEXSTATE_H


#include "enums.h"
#include "macros.h"
#include "mtypes.h"


static inline struct gl_texture_unit *
_mesa_get_tex_unit(struct gl_context *ctx, GLuint unit)
{
    assert(unit < ARRAY_SIZE(ctx->Texture.Unit));
    return &(ctx->Texture.Unit[unit]);
}

/**
 * Return pointer to current texture unit.
 * This the texture unit set by glActiveTexture(), not glClientActiveTexture().
 */
static inline struct gl_texture_unit *
_mesa_get_current_tex_unit(struct gl_context *ctx)
{
    return _mesa_get_tex_unit(ctx, ctx->Texture.CurrentUnit);
}


/**
 * Return pointer to current fixed-func texture unit.
 * This the texture unit set by glActiveTexture(), not glClientActiveTexture().
 * \return NULL if the current unit is not a fixed-func texture unit
 */
static inline struct gl_fixedfunc_texture_unit *
_mesa_get_fixedfunc_tex_unit(struct gl_context *ctx, GLuint unit)
{
    if (unit >= ARRAY_SIZE(ctx->Texture.FixedFuncUnit))
        return NULL;

    return &ctx->Texture.FixedFuncUnit[unit];
}


static inline GLuint
_mesa_max_tex_unit(struct gl_context *ctx)
{
    /* See OpenGL spec for glActiveTexture: */
    return MAX2(ctx->Const.MaxCombinedTextureImageUnits,
                ctx->Const.MaxTextureCoordUnits);
}


extern void
_mesa_copy_texture_state( const struct gl_context *src, struct gl_context *dst );

extern void
_mesa_print_texunit_state( struct gl_context *ctx, GLuint unit );



/**
 * \name Called from API
 */
/*@{*/

extern void GLAPIENTRY
_mesa_ActiveTexture_no_error( GLenum target );

extern void GLAPIENTRY
_mesa_ActiveTexture( GLenum target );

extern void GLAPIENTRY
_mesa_ClientActiveTexture( GLenum target );

/*@}*/


/**
 * \name Initialization, state maintenance
 */
/*@{*/

extern GLbitfield
_mesa_update_texture_matrices(struct gl_context *ctx);

extern GLbitfield
_mesa_update_texture_state(struct gl_context *ctx);

extern GLboolean
_mesa_init_texture( struct gl_context *ctx );

extern void
_mesa_free_texture_data( struct gl_context *ctx );

extern void
_mesa_update_default_objects_texture(struct gl_context *ctx);

/*@}*/

#endif

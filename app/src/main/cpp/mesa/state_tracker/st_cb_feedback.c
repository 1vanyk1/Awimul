#include "../main/context.h"
#include "../main/feedback.h"
#include "../main/varray.h"

#include "../util/u_memory.h"

#include "../vbo/vbo.h"

#include "st_context.h"
#include "st_draw.h"
#include "st_cb_feedback.h"
#include "st_program.h"
#include "st_util.h"

#include "../include/pipe/p_context.h"
#include "../include/pipe/p_defines.h"

#include "../gallium/draw/draw_context.h"
#include "../gallium/draw/draw_pipe.h"


/**
 * This is actually used for both feedback and selection.
 */
struct feedback_stage
{
    struct draw_stage stage;   /**< Base class */
    struct gl_context *ctx;            /**< Rendering context */
    GLboolean reset_stipple_counter;
};


/**********************************************************************
 * GL Feedback functions
 **********************************************************************/

static inline struct feedback_stage *
feedback_stage( struct draw_stage *stage )
{
    return (struct feedback_stage *)stage;
}


static void
feedback_vertex(struct gl_context *ctx, const struct draw_context *draw,
                const struct vertex_header *v)
{
    const struct st_context *st = st_context(ctx);
    struct st_vertex_program *stvp = (struct st_vertex_program *)st->vp;
    GLfloat win[4];
    const GLfloat *color, *texcoord;
    ubyte slot;

    win[0] = v->data[0][0];
    if (st_fb_orientation(ctx->DrawBuffer) == Y_0_TOP)
        win[1] = ctx->DrawBuffer->Height - v->data[0][1];
    else
        win[1] = v->data[0][1];
    win[2] = v->data[0][2];
    win[3] = 1.0F / v->data[0][3];

    /* XXX
     * When we compute vertex layout, save info about position of the
     * color and texcoord attribs to use here.
     */

    slot = stvp->result_to_output[VARYING_SLOT_COL0];
    if (slot != 0xff)
        color = v->data[slot];
    else
        color = ctx->Current.Attrib[VERT_ATTRIB_COLOR0];

    slot = stvp->result_to_output[VARYING_SLOT_TEX0];
    if (slot != 0xff)
        texcoord = v->data[slot];
    else
        texcoord = ctx->Current.Attrib[VERT_ATTRIB_TEX0];

    _mesa_feedback_vertex(ctx, win, color, texcoord);
}


static void
feedback_tri( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    struct draw_context *draw = stage->draw;
    _mesa_feedback_token(fs->ctx, (GLfloat) GL_POLYGON_TOKEN);
    _mesa_feedback_token(fs->ctx, (GLfloat) 3); /* three vertices */
    feedback_vertex(fs->ctx, draw, prim->v[0]);
    feedback_vertex(fs->ctx, draw, prim->v[1]);
    feedback_vertex(fs->ctx, draw, prim->v[2]);
}


static void
feedback_line( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    struct draw_context *draw = stage->draw;
    if (fs->reset_stipple_counter) {
        _mesa_feedback_token(fs->ctx, (GLfloat) GL_LINE_RESET_TOKEN);
        fs->reset_stipple_counter = GL_FALSE;
    }
    else {
        _mesa_feedback_token(fs->ctx, (GLfloat) GL_LINE_TOKEN);
    }
    feedback_vertex(fs->ctx, draw, prim->v[0]);
    feedback_vertex(fs->ctx, draw, prim->v[1]);
}


static void
feedback_point( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    struct draw_context *draw = stage->draw;
    _mesa_feedback_token(fs->ctx, (GLfloat) GL_POINT_TOKEN);
    feedback_vertex(fs->ctx, draw, prim->v[0]);
}


static void
feedback_flush( struct draw_stage *stage, unsigned flags )
{
    /* no-op */
}


static void
feedback_reset_stipple_counter( struct draw_stage *stage )
{
    struct feedback_stage *fs = feedback_stage(stage);
    fs->reset_stipple_counter = GL_TRUE;
}


static void
feedback_destroy( struct draw_stage *stage )
{
    FREE(stage);
}

/**
 * Create GL feedback drawing stage.
 */
static struct draw_stage *
draw_glfeedback_stage(struct gl_context *ctx, struct draw_context *draw)
{
    struct feedback_stage *fs = CALLOC_STRUCT(feedback_stage);

    fs->stage.draw = draw;
    fs->stage.next = NULL;
    fs->stage.point = feedback_point;
    fs->stage.line = feedback_line;
    fs->stage.tri = feedback_tri;
    fs->stage.flush = feedback_flush;
    fs->stage.reset_stipple_counter = feedback_reset_stipple_counter;
    fs->stage.destroy = feedback_destroy;
    fs->ctx = ctx;

    return &fs->stage;
}



/**********************************************************************
 * GL Selection functions
 **********************************************************************/

static void
select_tri( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    _mesa_update_hitflag( fs->ctx, prim->v[0]->data[0][2] );
    _mesa_update_hitflag( fs->ctx, prim->v[1]->data[0][2] );
    _mesa_update_hitflag( fs->ctx, prim->v[2]->data[0][2] );
}

static void
select_line( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    _mesa_update_hitflag( fs->ctx, prim->v[0]->data[0][2] );
    _mesa_update_hitflag( fs->ctx, prim->v[1]->data[0][2] );
}


static void
select_point( struct draw_stage *stage, struct prim_header *prim )
{
    struct feedback_stage *fs = feedback_stage(stage);
    _mesa_update_hitflag( fs->ctx, prim->v[0]->data[0][2] );
}


static void
select_flush( struct draw_stage *stage, unsigned flags )
{
    /* no-op */
}


static void
select_reset_stipple_counter( struct draw_stage *stage )
{
    /* no-op */
}

static void
select_destroy( struct draw_stage *stage )
{
    FREE(stage);
}


/**
 * Create GL selection mode drawing stage.
 */
static struct draw_stage *
draw_glselect_stage(struct gl_context *ctx, struct draw_context *draw)
{
    struct feedback_stage *fs = CALLOC_STRUCT(feedback_stage);

    fs->stage.draw = draw;
    fs->stage.next = NULL;
    fs->stage.point = select_point;
    fs->stage.line = select_line;
    fs->stage.tri = select_tri;
    fs->stage.flush = select_flush;
    fs->stage.reset_stipple_counter = select_reset_stipple_counter;
    fs->stage.destroy = select_destroy;
    fs->ctx = ctx;

    return &fs->stage;
}


void
st_RenderMode(struct gl_context *ctx, GLenum newMode )
{
    struct st_context *st = st_context(ctx);
    struct draw_context *draw = st_get_draw_context(st);

    if (!st->draw)
        return;

    if (newMode == GL_RENDER) {
        /* restore normal VBO draw function */
        st_init_draw_functions(st->screen, &ctx->Driver);
    }
    else if (newMode == GL_SELECT) {
        if (!st->selection_stage)
            st->selection_stage = draw_glselect_stage(ctx, draw);
        draw_set_rasterize_stage(draw, st->selection_stage);
        /* Plug in new vbo draw function */
        ctx->Driver.DrawGallium = _mesa_draw_gallium_fallback;
        ctx->Driver.DrawGalliumMultiMode = _mesa_draw_gallium_multimode_fallback;
    }
    else {
        struct gl_program *vp = st->ctx->VertexProgram._Current;

        if (!st->feedback_stage)
            st->feedback_stage = draw_glfeedback_stage(ctx, draw);
        draw_set_rasterize_stage(draw, st->feedback_stage);
        /* Plug in new vbo draw function */
        ctx->Driver.DrawGallium = _mesa_draw_gallium_fallback;
        ctx->Driver.DrawGalliumMultiMode = _mesa_draw_gallium_multimode_fallback;
        /* need to generate/use a vertex program that emits pos/color/tex */
        if (vp)
            st->dirty |= ST_NEW_VERTEX_PROGRAM(st, st_program(vp));
    }
}

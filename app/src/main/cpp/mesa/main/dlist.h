#ifndef DLIST_H
#define DLIST_H

#include <stdio.h>

struct gl_context;

/**
 * Describes the location and size of a glBitmap image in a texture atlas.
 */
struct gl_bitmap_glyph
{
    unsigned short x, y, w, h;  /**< position and size in the texture */
    float xorig, yorig;         /**< bitmap origin */
    float xmove, ymove;         /**< rasterpos move */
};


/**
 * Describes a set of glBitmap display lists which live in a texture atlas.
 * The idea is when we see a code sequence of glListBase(b), glCallLists(n)
 * we're probably drawing bitmap font glyphs.  We try to put all the bitmap
 * glyphs into one texture map then render the glCallLists as a textured
 * quadstrip.
 */
struct gl_bitmap_atlas
{
    GLint Id;
    bool complete;     /**< Is the atlas ready to use? */
    bool incomplete;   /**< Did we fail to construct this atlas? */

    unsigned numBitmaps;
    unsigned texWidth, texHeight;
    struct gl_texture_object *texObj;
    struct gl_texture_image *texImage;

    unsigned glyphHeight;

    struct gl_bitmap_glyph *glyphs;
};

void
_mesa_delete_bitmap_atlas(struct gl_context *ctx,
                          struct gl_bitmap_atlas *atlas);


GLboolean GLAPIENTRY
_mesa_IsList(GLuint list);

void GLAPIENTRY
_mesa_DeleteLists(GLuint list, GLsizei range);

GLuint GLAPIENTRY
_mesa_GenLists(GLsizei range);

void GLAPIENTRY
_mesa_NewList(GLuint name, GLenum mode);

void GLAPIENTRY
_mesa_EndList(void);

void GLAPIENTRY
_mesa_CallList(GLuint list);

void GLAPIENTRY
_mesa_CallLists(GLsizei n, GLenum type, const GLvoid *lists);

void GLAPIENTRY
_mesa_ListBase(GLuint base);

struct gl_display_list *
_mesa_lookup_list(struct gl_context *ctx, GLuint list);

void
_mesa_compile_error(struct gl_context *ctx, GLenum error, const char *s);

void *
_mesa_dlist_alloc(struct gl_context *ctx, GLuint opcode, GLuint sz);

void *
_mesa_dlist_alloc_aligned(struct gl_context *ctx, GLuint opcode, GLuint bytes);

void *
_mesa_dlist_alloc_vertex_list(struct gl_context *ctx);

void
_mesa_delete_list(struct gl_context *ctx, struct gl_display_list *dlist);

void
_mesa_initialize_save_table(const struct gl_context *);

void
_mesa_install_dlist_vtxfmt(struct _glapi_table *disp,
                           const GLvertexformat *vfmt);

void
_mesa_init_display_list(struct gl_context * ctx);

bool
_mesa_get_list(struct gl_context *ctx, GLuint list,
               struct gl_display_list **dlist);

#endif /* DLIST_H */

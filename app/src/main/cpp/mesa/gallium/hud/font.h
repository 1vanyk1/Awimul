#ifndef FONT_H
#define FONT_H

#include "../../include/pipe/p_compiler.h"

struct pipe_resource;
struct pipe_context;

enum util_font_name {
    UTIL_FONT_FIXED_8X13
};

/* The font is stored in a RECT texture. There are 256 glyphs
 * drawn in a 16x16 matrix. The texture coordinates of a glyph
 * within the matrix should be calculated as follows:
 *
 *    x1 = (glyph % 16) * glyph_width;
 *    y1 = (glyph / 16) * glyph_height;
 *    x2 = x1 + glyph_width;
 *    y2 = y1 + glyph_height;
 */
struct util_font {
    struct pipe_resource *texture;
    unsigned glyph_width;
    unsigned glyph_height;
};

boolean
util_font_create(struct pipe_context *pipe, enum util_font_name name,
                 struct util_font *out_font);

#endif

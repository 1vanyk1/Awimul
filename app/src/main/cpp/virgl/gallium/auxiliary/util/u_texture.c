#include "../../include/pipe/p_defines.h"

#include "../../../mesa/util/u_debug.h"
#include "u_texture.h"

void util_map_texcoords2d_onto_cubemap(unsigned face,
                                       const float *in_st, unsigned in_stride,
                                       float *out_str, unsigned out_stride,
                                       boolean allow_scale)
{
    int i;
    float rx, ry, rz;

    /* loop over quad verts */
    for (i = 0; i < 4; i++) {
        /* Compute sc = +/-scale and tc = +/-scale.
         * Not +/-1 to avoid cube face selection ambiguity near the edges,
         * though that can still sometimes happen with this scale factor...
         *
         * XXX: Yep, there is no safe scale factor that will prevent sampling
         * the neighbouring face when stretching out.  A more reliable solution
         * would be to clamp (sc, tc) against +/- 1.0-1.0/mipsize, in the shader.
         *
         * Also, this is not necessary when minifying, or 1:1 blits.
         */
        const float scale = allow_scale ? 0.9999f : 1.0f;
        const float sc = (2 * in_st[0] - 1) * scale;
        const float tc = (2 * in_st[1] - 1) * scale;

        switch (face) {
            case PIPE_TEX_FACE_POS_X:
                rx = 1;
                ry = -tc;
                rz = -sc;
                break;
            case PIPE_TEX_FACE_NEG_X:
                rx = -1;
                ry = -tc;
                rz = sc;
                break;
            case PIPE_TEX_FACE_POS_Y:
                rx = sc;
                ry = 1;
                rz = tc;
                break;
            case PIPE_TEX_FACE_NEG_Y:
                rx = sc;
                ry = -1;
                rz = -tc;
                break;
            case PIPE_TEX_FACE_POS_Z:
                rx = sc;
                ry = -tc;
                rz = 1;
                break;
            case PIPE_TEX_FACE_NEG_Z:
                rx = -sc;
                ry = -tc;
                rz = -1;
                break;
            default:
                rx = ry = rz = 0;
                assert(0);
        }

        out_str[0] = rx; /*s*/
        out_str[1] = ry; /*t*/
        out_str[2] = rz; /*r*/

        in_st += in_stride;
        out_str += out_stride;
    }
}

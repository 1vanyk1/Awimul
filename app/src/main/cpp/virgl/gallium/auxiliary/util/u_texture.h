#ifndef U_TEXTURE_H
#define U_TEXTURE_H

#include "../../../mesa/pipe/p_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert 2D texture coordinates of 4 vertices into cubemap coordinates
 * in the given face.
 * Coordinates must be in the range [0,1].
 *
 * \param face          Cubemap face.
 * \param in_st         4 pairs of 2D texture coordinates to convert.
 * \param in_stride     Stride of in_st in floats.
 * \param out_str       STR cubemap texture coordinates to compute.
 * \param out_stride    Stride of out_str in floats.
 */
void util_map_texcoords2d_onto_cubemap(unsigned face,
                                       const float *in_st, unsigned in_stride,
                                       float *out_str, unsigned out_stride,
                                       boolean allow_scale);


#ifdef __cplusplus
}
#endif

#endif

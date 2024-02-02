#include <stdbool.h>
#include "../main/image.h"
#include "../main/pbo.h"

#include "st_nir.h"
#include "st_format.h"
#include "st_pbo.h"
#include "st_texture.h"
#include "../compiler/nir/nir_builder.h"
#include "../compiler/nir/nir_format_convert.h"
#include "../compiler/glsl/gl_nir.h"
#include "../util/u_sampler.h"

#define BGR_FORMAT(NAME) \
    {{ \
     [0] = PIPE_FORMAT_##NAME##_SNORM, \
     [1] = PIPE_FORMAT_##NAME##_SINT, \
    }, \
    { \
     [0] = PIPE_FORMAT_##NAME##_UNORM, \
     [1] = PIPE_FORMAT_##NAME##_UINT, \
    }}

#define FORMAT(NAME, NAME16, NAME32) \
   {{ \
    [1] = PIPE_FORMAT_##NAME##_SNORM, \
    [2] = PIPE_FORMAT_##NAME16##_SNORM, \
    [4] = PIPE_FORMAT_##NAME32##_SNORM, \
   }, \
   { \
    [1] = PIPE_FORMAT_##NAME##_UNORM, \
    [2] = PIPE_FORMAT_##NAME16##_UNORM, \
    [4] = PIPE_FORMAT_##NAME32##_UNORM, \
   }}

/* don't try these at home */
static enum pipe_format
get_convert_format(struct gl_context *ctx,
                   enum pipe_format src_format,
                   GLenum format, GLenum type,
                   bool *need_bgra_swizzle)
{
    struct st_context *st = st_context(ctx);
    GLint bpp = _mesa_bytes_per_pixel(format, type);
    if (_mesa_is_depth_format(format) ||
        format == GL_GREEN_INTEGER ||
        format == GL_BLUE_INTEGER) {
        switch (bpp) {
            case 1:
                return _mesa_is_type_unsigned(type) ? PIPE_FORMAT_R8_UINT : PIPE_FORMAT_R8_SINT;
            case 2:
                return _mesa_is_type_unsigned(type) ? PIPE_FORMAT_R16_UINT : PIPE_FORMAT_R16_SINT;
            case 4:
                return _mesa_is_type_unsigned(type) ? PIPE_FORMAT_R32_UINT : PIPE_FORMAT_R32_SINT;
        }
    }
    mesa_format mformat = _mesa_tex_format_from_format_and_type(ctx, format, type);
    enum pipe_format pformat = st_mesa_format_to_pipe_format(st, mformat);
    if (!pformat) {
        GLint dst_components = _mesa_components_in_format(format);
        bpp /= dst_components;
        if (format == GL_BGR || format == GL_BGRA) {
            pformat = st_pbo_get_dst_format(ctx, PIPE_TEXTURE_2D, src_format, false, format == GL_BGR ? GL_RGB : GL_RGBA, type, 0);
            if (!pformat)
                pformat = get_convert_format(ctx, src_format, format == GL_BGR ? GL_RGB : GL_RGBA, type, need_bgra_swizzle);
            assert(pformat);
            *need_bgra_swizzle = true;
        } else if (format == GL_BGR_INTEGER || format == GL_BGRA_INTEGER) {
            pformat = st_pbo_get_dst_format(ctx, PIPE_TEXTURE_2D, src_format, false, format == GL_BGR_INTEGER ? GL_RGB_INTEGER : GL_RGBA_INTEGER, type, 0);
            if (!pformat)
                pformat = get_convert_format(ctx, src_format, format == GL_BGR_INTEGER ? GL_RGB_INTEGER : GL_RGBA_INTEGER, type, need_bgra_swizzle);
            assert(pformat);
            *need_bgra_swizzle = true;
        } else {
            /* [signed,unsigned][bpp] */
            enum pipe_format rgb[5][2][5] = {
                    [1] = FORMAT(R8, R16, R32),
                    [2] = FORMAT(R8G8, R16G16, R32G32),
                    [3] = FORMAT(R8G8B8, R16G16B16, R32G32B32),
                    [4] = FORMAT(R8G8B8A8, R16G16B16A16, R32G32B32A32),
            };
            pformat = rgb[dst_components][_mesa_is_type_unsigned(type)][bpp];
        }
        assert(util_format_get_nr_components(pformat) == dst_components);
    }
    assert(pformat);
    return pformat;
}
#undef BGR_FORMAT
#undef FORMAT


struct pbo_shader_data {
    nir_ssa_def *offset;
    nir_ssa_def *range;
    nir_ssa_def *invert;
    nir_ssa_def *blocksize;
    nir_ssa_def *alignment;
    nir_ssa_def *dst_bit_size;
    nir_ssa_def *channels;
    nir_ssa_def *normalized;
    nir_ssa_def *integer;
    nir_ssa_def *clamp_uint;
    nir_ssa_def *r11g11b10_or_sint;
    nir_ssa_def *r9g9b9e5;
    nir_ssa_def *bits1;
    nir_ssa_def *bits2;
    nir_ssa_def *bits3;
    nir_ssa_def *bits4;
    nir_ssa_def *swap;
    nir_ssa_def *bits; //vec4
};


/* must be under 16bytes / sizeof(vec4) / 128 bits) */
struct pbo_data {
    union {
        struct {
            struct {
                uint16_t x, y;
            };
            struct {
                uint16_t width, height;
            };
            struct {
                uint16_t depth;
                uint8_t invert : 1;
                uint8_t blocksize : 7;

                uint8_t clamp_uint : 1;
                uint8_t r11g11b10_or_sint : 1;
                uint8_t r9g9b9e5 : 1;
                uint8_t swap : 1;
                uint16_t alignment : 2;
                uint8_t dst_bit_size : 2; //8, 16, 32, 64
            };

            struct {
                uint8_t channels : 2;
                uint8_t bits1 : 6;
                uint8_t normalized : 1;
                uint8_t integer : 1;
                uint8_t bits2 : 6;
                uint8_t bits3 : 6;
                uint8_t pad1 : 2;
                uint8_t bits4 : 6;
                uint8_t pad2 : 2;
            };
        };
        float vec[4];
    };
};


#define STRUCT_OFFSET(name) (offsetof(struct pbo_data, name) * 8)

#define STRUCT_BLOCK(offset, ...) \
   do { \
      assert(offset % 8 == 0); \
      nir_ssa_def *block##offset = nir_u2u32(b, nir_extract_bits(b, &ubo_load, 1, (offset), 1, 8)); \
      __VA_ARGS__ \
   } while (0)
#define STRUCT_MEMBER(blockoffset, name, offset, size, op, clamp) \
   do { \
      assert(offset + size <= 8); \
      nir_ssa_def *val = nir_iand_imm(b, block##blockoffset, u_bit_consecutive(offset, size)); \
      if (offset) \
         val = nir_ushr_imm(b, val, offset); \
      sd->name = op; \
      if (clamp) \
         sd->name = nir_umin(b, sd->name, nir_imm_int(b, clamp)); \
   } while (0)
#define STRUCT_MEMBER_SHIFTED_2BIT(blockoffset, name, offset, shift, clamp) \
   STRUCT_MEMBER(blockoffset, name, offset, 2, nir_ishl(b, nir_imm_int(b, shift), val), clamp)

#define STRUCT_MEMBER_BOOL(blockoffset, name, offset) \
   STRUCT_MEMBER(blockoffset, name, offset, 1, nir_ieq_imm(b, val, 1), 0)

/* this function extracts the conversion data from pbo_data using the
 * size annotations for each grouping. data is compacted into bitfields,
 * so bitwise operations must be used to "unpact" everything
 */
static void
init_pbo_shader_data(nir_builder *b, struct pbo_shader_data *sd)
{
    nir_variable *ubo = nir_variable_create(b->shader, nir_var_uniform, glsl_uvec4_type(), "offset");
    nir_ssa_def *ubo_load = nir_load_var(b, ubo);

    sd->offset = nir_umin(b, nir_u2u32(b, nir_extract_bits(b, &ubo_load, 1, STRUCT_OFFSET(x), 2, 16)), nir_imm_int(b, 65535));
    sd->range = nir_umin(b, nir_u2u32(b, nir_extract_bits(b, &ubo_load, 1, STRUCT_OFFSET(width), 3, 16)), nir_imm_int(b, 65535));

    STRUCT_BLOCK(80,
                 STRUCT_MEMBER_BOOL(80, invert, 0);
                         STRUCT_MEMBER(80, blocksize, 1, 7, nir_iadd_imm(b, val, 1), 128);
    );

    STRUCT_BLOCK(88,
                 STRUCT_MEMBER_BOOL(88, clamp_uint, 0);
                         STRUCT_MEMBER_BOOL(88, r11g11b10_or_sint, 1);
                         STRUCT_MEMBER_BOOL(88, r9g9b9e5, 2);
                         STRUCT_MEMBER_BOOL(88, swap, 3);
                         STRUCT_MEMBER_SHIFTED_2BIT(88, alignment, 4, 1, 8);
                         STRUCT_MEMBER_SHIFTED_2BIT(88, dst_bit_size, 6, 8, 64);
    );

    STRUCT_BLOCK(96,
                 STRUCT_MEMBER(96, channels, 0, 2, nir_iadd_imm(b, val, 1), 4);
                         STRUCT_MEMBER(96, bits1, 2, 6, val, 32);
    );

    STRUCT_BLOCK(104,
                 STRUCT_MEMBER_BOOL(104, normalized, 0);
                         STRUCT_MEMBER_BOOL(104, integer, 1);
                         STRUCT_MEMBER(104, bits2, 2, 6, val, 32);
    );


    STRUCT_BLOCK(112,
                 STRUCT_MEMBER(112, bits3, 0, 6, val, 32);
    );

    STRUCT_BLOCK(120,
                 STRUCT_MEMBER(120, bits4, 0, 6, val, 32);
    );
    sd->bits = nir_vec4(b, sd->bits1, sd->bits2, sd->bits3, sd->bits4);

    /* clamp swap in the shader to enable better optimizing */
    /* TODO?
    sd->swap = nir_bcsel(b, nir_ior(b,
                                    nir_ieq_imm(b, sd->blocksize, 8),
                                    nir_bcsel(b,
                                              nir_ieq_imm(b, sd->bits1, 8),
                                              nir_bcsel(b,
                                                        nir_uge(b, sd->channels, nir_imm_int(b, 2)),
                                                        nir_bcsel(b,
                                                                  nir_uge(b, sd->channels, nir_imm_int(b, 3)),
                                                                  nir_bcsel(b,
                                                                            nir_ieq(b, sd->channels, nir_imm_int(b, 4)),
                                                                            nir_ball(b, nir_ieq(b, sd->bits, nir_imm_ivec4(b, 8, 8, 8, 8))),
                                                                            nir_ball(b, nir_ieq(b, nir_channels(b, sd->bits, 7), nir_imm_ivec3(b, 8, 8, 8)))),
                                                                  nir_ball(b, nir_ieq(b, nir_channels(b, sd->bits, 3), nir_imm_ivec2(b, 8, 8)))),
                                                        nir_imm_bool(b, 0)),
                                              nir_imm_bool(b, 0))),
                            nir_imm_bool(b, 0),
                            sd->swap);
      */
}

static unsigned
fill_pbo_data(struct pbo_data *pd, enum pipe_format src_format, enum pipe_format dst_format, bool swap)
{
    unsigned bits[4] = {0};
    bool weird_packed = false;
    const struct util_format_description *dst_desc = util_format_description(dst_format);
    bool is_8bit = true;

    for (unsigned c = 0; c < 4; c++) {
        bits[c] = dst_desc->channel[c].size;
        if (c < dst_desc->nr_channels) {
            weird_packed |= bits[c] != bits[0] || bits[c] % 8 != 0;
            if (bits[c] != 8)
                is_8bit = false;
        }
    }

    if (is_8bit || dst_desc->block.bits == 8)
        swap = false;

    unsigned dst_bit_size = 0;
    if (weird_packed) {
        dst_bit_size = dst_desc->block.bits;
    } else {
        dst_bit_size = dst_desc->block.bits / dst_desc->nr_channels;
    }
    assert(dst_bit_size);
    assert(dst_bit_size <= 64);

    pd->dst_bit_size = dst_bit_size >> 4;
    pd->channels = dst_desc->nr_channels - 1;
    pd->normalized = dst_desc->is_unorm || dst_desc->is_snorm;
    pd->clamp_uint = dst_desc->is_unorm ||
                     (util_format_is_pure_sint(dst_format) &&
                      !util_format_is_pure_sint(src_format) &&
                      !util_format_is_snorm(src_format)) ||
                     util_format_is_pure_uint(dst_format);
    pd->integer = util_format_is_pure_uint(dst_format) || util_format_is_pure_sint(dst_format);
    pd->r11g11b10_or_sint = dst_format == PIPE_FORMAT_R11G11B10_FLOAT || util_format_is_pure_sint(dst_format);
    pd->r9g9b9e5 = dst_format == PIPE_FORMAT_R9G9B9E5_FLOAT;
    pd->bits1 = bits[0];
    pd->bits2 = bits[1];
    pd->bits3 = bits[2];
    pd->bits4 = bits[3];
    pd->swap = swap;

    return weird_packed ? 1 : dst_desc->nr_channels;
}

static nir_ssa_def *
get_buffer_offset(nir_builder *b, nir_ssa_def *coord, struct pbo_shader_data *sd)
{
/* from _mesa_image_offset():
      offset = topOfImage
               + (skippixels + column) * bytes_per_pixel
               + (skiprows + row) * bytes_per_row
               + (skipimages + img) * bytes_per_image;
 */
    nir_ssa_def *bytes_per_row = nir_imul(b, nir_channel(b, sd->range, 0), sd->blocksize);
    bytes_per_row = nir_bcsel(b, nir_ult(b, sd->alignment, nir_imm_int(b, 2)),
                              bytes_per_row,
                              nir_iand(b,
                                       nir_isub(b, nir_iadd(b, bytes_per_row, sd->alignment), nir_imm_int(b, 1)),
                                       nir_inot(b, nir_isub(b, sd->alignment, nir_imm_int(b, 1)))));
    nir_ssa_def *bytes_per_image = nir_imul(b, bytes_per_row, nir_channel(b, sd->range, 1));
    bytes_per_row = nir_bcsel(b, sd->invert,
                              nir_isub(b, nir_imm_int(b, 0), bytes_per_row),
                              bytes_per_row);
    return nir_iadd(b,
                    nir_imul(b, nir_channel(b, coord, 0), sd->blocksize),
                    nir_iadd(b,
                             nir_imul(b, nir_channel(b, coord, 1), bytes_per_row),
                             nir_imul(b, nir_channel(b, coord, 2), bytes_per_image)));
}

static inline void
write_ssbo(nir_builder *b, nir_ssa_def *pixel, nir_ssa_def *buffer_offset)
{
    nir_store_ssbo(b, pixel, nir_imm_zero(b, 1, 32), buffer_offset,
            .align_mul = pixel->bit_size / 8,
    .write_mask = (1 << pixel->num_components) - 1);
}

static void
write_conversion(nir_builder *b, nir_ssa_def *pixel, nir_ssa_def *buffer_offset, struct pbo_shader_data *sd)
{
    nir_push_if(b, nir_ilt(b, sd->dst_bit_size, nir_imm_int(b, 32)));
    nir_push_if(b, nir_ieq_imm(b, sd->dst_bit_size, 16));
    write_ssbo(b, nir_u2u16(b, pixel), buffer_offset);
    nir_push_else(b, NULL);
    write_ssbo(b, nir_u2u8(b, pixel), buffer_offset);
    nir_pop_if(b, NULL);
    nir_push_else(b, NULL);
    write_ssbo(b, pixel, buffer_offset);
    nir_pop_if(b, NULL);
}

static nir_ssa_def *
swap2(nir_builder *b, nir_ssa_def *src)
{
    /* dst[i] = (src[i] >> 8) | ((src[i] << 8) & 0xff00); */
    return nir_ior(b,
                   nir_ushr_imm(b, src, 8),
                   nir_iand_imm(b, nir_ishl(b, src, nir_imm_int(b, 8)), 0xff00));
}

static nir_ssa_def *
swap4(nir_builder *b, nir_ssa_def *src)
{
    /* a = (b >> 24) | ((b >> 8) & 0xff00) | ((b << 8) & 0xff0000) | ((b << 24) & 0xff000000); */
    return nir_ior(b,
            /* (b >> 24) */
                   nir_ushr_imm(b, src, 24),
                   nir_ior(b,
                           /* ((b >> 8) & 0xff00) */
                           nir_iand(b, nir_ushr_imm(b, src, 8), nir_imm_int(b, 0xff00)),
                           nir_ior(b,
                                   /* ((b << 8) & 0xff0000) */
                                   nir_iand(b, nir_ishl(b, src, nir_imm_int(b, 8)), nir_imm_int(b, 0xff0000)),
                                   /* ((b << 24) & 0xff000000) */
                                   nir_iand(b, nir_ishl(b, src, nir_imm_int(b, 24)), nir_imm_int(b, 0xff000000)))));
}

/* explode the cf to handle channel counts in the shader */
static void
grab_components(nir_builder *b, nir_ssa_def *pixel, nir_ssa_def *buffer_offset, struct pbo_shader_data *sd, bool weird_packed)
{
    if (weird_packed) {
        nir_push_if(b, nir_ieq_imm(b, sd->bits1, 32));
        write_conversion(b, nir_channels(b, pixel, 3), buffer_offset, sd);
        nir_push_else(b, NULL);
        write_conversion(b, nir_channel(b, pixel, 0), buffer_offset, sd);
        nir_pop_if(b, NULL);
    } else {
        nir_push_if(b, nir_ieq_imm(b, sd->channels, 1));
        write_conversion(b, nir_channel(b, pixel, 0), buffer_offset, sd);
        nir_push_else(b, NULL);
        nir_push_if(b, nir_ieq_imm(b, sd->channels, 2));
        write_conversion(b, nir_channels(b, pixel, (1 << 2) - 1), buffer_offset, sd);
        nir_push_else(b, NULL);
        nir_push_if(b, nir_ieq_imm(b, sd->channels, 3));
        write_conversion(b, nir_channels(b, pixel, (1 << 3) - 1), buffer_offset, sd);
        nir_push_else(b, NULL);
        write_conversion(b, nir_channels(b, pixel, (1 << 4) - 1), buffer_offset, sd);
        nir_pop_if(b, NULL);
        nir_pop_if(b, NULL);
        nir_pop_if(b, NULL);
    }
}

/* if byteswap is enabled, handle that and then write the components */
static void
handle_swap(nir_builder *b, nir_ssa_def *pixel, nir_ssa_def *buffer_offset,
            struct pbo_shader_data *sd, unsigned num_components, bool weird_packed)
{
    nir_push_if(b, sd->swap); {
        nir_push_if(b, nir_ieq_imm(b, nir_udiv_imm(b, sd->blocksize, num_components), 2)); {
            /* this is a single high/low swap per component */
            nir_ssa_def *components[4];
            for (unsigned i = 0; i < 4; i++)
                components[i] = swap2(b, nir_channel(b, pixel, i));
            nir_ssa_def *v = nir_vec(b, components, 4);
            grab_components(b, v, buffer_offset, sd, weird_packed);
        } nir_push_else(b, NULL); {
            /* this is a pair of high/low swaps for each half of the component */
            nir_ssa_def *components[4];
            for (unsigned i = 0; i < 4; i++)
                components[i] = swap4(b, nir_channel(b, pixel, i));
            nir_ssa_def *v = nir_vec(b, components, 4);
            grab_components(b, v, buffer_offset, sd, weird_packed);
        } nir_pop_if(b, NULL);
    } nir_push_else(b, NULL); {
        /* swap disabled */
        grab_components(b, pixel, buffer_offset, sd, weird_packed);
    } nir_pop_if(b, NULL);
}

static nir_ssa_def *
check_for_weird_packing(nir_builder *b, struct pbo_shader_data *sd, unsigned component)
{
    nir_ssa_def *c = nir_channel(b, sd->bits, component - 1);

    return nir_bcsel(b,
                     nir_ige(b, sd->channels, nir_imm_int(b, component)),
                     nir_ior(b,
                             nir_ine(b, c, sd->bits1),
                             nir_ine(b, nir_imod(b, c, nir_imm_int(b, 8)), nir_imm_int(b, 0))),
                     nir_imm_bool(b, 0));
}

/* convenience function for clamping signed integers */
static inline nir_ssa_def *
nir_imin_imax(nir_builder *build, nir_ssa_def *src, nir_ssa_def *clamp_to_min, nir_ssa_def *clamp_to_max)
{
    return nir_imax(build, nir_imin(build, src, clamp_to_min), clamp_to_max);
}

static inline nir_ssa_def *
nir_format_float_to_unorm_with_factor(nir_builder *b, nir_ssa_def *f, nir_ssa_def *factor)
{
    /* Clamp to the range [0, 1] */
    f = nir_fsat(b, f);

    return nir_f2u32(b, nir_fround_even(b, nir_fmul(b, f, factor)));
}

static inline nir_ssa_def *
nir_format_float_to_snorm_with_factor(nir_builder *b, nir_ssa_def *f, nir_ssa_def *factor)
{
    /* Clamp to the range [-1, 1] */
    f = nir_fmin(b, nir_fmax(b, f, nir_imm_float(b, -1)), nir_imm_float(b, 1));

    return nir_f2i32(b, nir_fround_even(b, nir_fmul(b, f, factor)));
}

static nir_ssa_def *
clamp_and_mask(nir_builder *b, nir_ssa_def *src, nir_ssa_def *channels)
{
    nir_ssa_def *one = nir_imm_ivec4(b, 1, 0, 0, 0);
    nir_ssa_def *two = nir_imm_ivec4(b, 1, 1, 0, 0);
    nir_ssa_def *three = nir_imm_ivec4(b, 1, 1, 1, 0);
    nir_ssa_def *four = nir_imm_ivec4(b, 1, 1, 1, 1);
    /* avoid underflow by clamping to channel count */
    src = nir_bcsel(b,
                    nir_ieq(b, channels, one),
                    nir_isub(b, src, one),
                    nir_bcsel(b,
                              nir_ieq_imm(b, channels, 2),
                              nir_isub(b, src, two),
                              nir_bcsel(b,
                                        nir_ieq_imm(b, channels, 3),
                                        nir_isub(b, src, three),
                                        nir_isub(b, src, four))));

    return nir_mask(b, src, 32);
}

static void
convert_swap_write(nir_builder *b, nir_ssa_def *pixel, nir_ssa_def *buffer_offset,
                   unsigned num_components,
                   struct pbo_shader_data *sd)
{

    nir_ssa_def *weird_packed = nir_ior(b,
                                        nir_ior(b,
                                                check_for_weird_packing(b, sd, 4),
                                                check_for_weird_packing(b, sd, 3)),
                                        check_for_weird_packing(b, sd, 2));
    if (num_components == 1) {
        nir_push_if(b, weird_packed);
        nir_push_if(b, sd->r11g11b10_or_sint);
        handle_swap(b, nir_pad_vec4(b, nir_format_pack_11f11f10f(b, pixel)), buffer_offset, sd, 1, true);
        nir_push_else(b, NULL);
        nir_push_if(b, sd->r9g9b9e5);
        handle_swap(b, nir_pad_vec4(b, nir_format_pack_r9g9b9e5(b, pixel)), buffer_offset, sd, 1, true);
        nir_push_else(b, NULL);
        nir_push_if(b, nir_ieq_imm(b, sd->bits1, 32)); { //PIPE_FORMAT_Z32_FLOAT_S8X24_UINT
            nir_ssa_def *pack[2];
            pack[0] = nir_format_pack_uint_unmasked_ssa(b, nir_channel(b, pixel, 0), nir_channel(b, sd->bits, 0));
            pack[1] = nir_format_pack_uint_unmasked_ssa(b, nir_channels(b, pixel, 6), nir_channels(b, sd->bits, 6));
            handle_swap(b, nir_pad_vec4(b, nir_vec2(b, pack[0], pack[1])), buffer_offset, sd, 2, true);
        } nir_push_else(b, NULL);
        handle_swap(b, nir_pad_vec4(b, nir_format_pack_uint_unmasked_ssa(b, pixel, sd->bits)), buffer_offset, sd, 1, true);
        nir_pop_if(b, NULL);
        nir_pop_if(b, NULL);
        nir_pop_if(b, NULL);
        nir_push_else(b, NULL);
        handle_swap(b, pixel, buffer_offset, sd, num_components, false);
        nir_pop_if(b, NULL);
    } else {
        nir_push_if(b, weird_packed);
        handle_swap(b, pixel, buffer_offset, sd, num_components, true);
        nir_push_else(b, NULL);
        handle_swap(b, pixel, buffer_offset, sd, num_components, false);
        nir_pop_if(b, NULL);
    }
}

static void
do_shader_conversion(nir_builder *b, nir_ssa_def *pixel,
                     unsigned num_components,
                     nir_ssa_def *coord, struct pbo_shader_data *sd)
{
    nir_ssa_def *buffer_offset = get_buffer_offset(b, coord, sd);

    nir_ssa_def *signed_bit_mask = clamp_and_mask(b, sd->bits, sd->channels);

#define CONVERT_SWAP_WRITE(PIXEL) \
   convert_swap_write(b, PIXEL, buffer_offset, num_components, sd);
    nir_push_if(b, sd->normalized);
    nir_push_if(b, sd->clamp_uint); //unorm
    CONVERT_SWAP_WRITE(nir_format_float_to_unorm_with_factor(b, pixel, nir_u2f32(b, nir_mask(b, sd->bits, 32))));
    nir_push_else(b, NULL);
    CONVERT_SWAP_WRITE(nir_format_float_to_snorm_with_factor(b, pixel, nir_u2f32(b, signed_bit_mask)));
    nir_pop_if(b, NULL);
    nir_push_else(b, NULL);
    nir_push_if(b, sd->integer);
    nir_push_if(b, sd->r11g11b10_or_sint); //sint
    nir_push_if(b, sd->clamp_uint); //uint -> sint
    CONVERT_SWAP_WRITE(nir_umin(b, pixel, signed_bit_mask));
    nir_push_else(b, NULL);
    CONVERT_SWAP_WRITE(nir_imin_imax(b, pixel, signed_bit_mask, nir_isub(b, nir_ineg(b, signed_bit_mask), nir_imm_int(b, 1))));
    nir_pop_if(b, NULL);
    nir_push_else(b, NULL);
    nir_push_if(b, sd->clamp_uint); //uint
    /* nir_format_clamp_uint */
    CONVERT_SWAP_WRITE(nir_umin(b, pixel, nir_mask(b, sd->bits, 32)));
    nir_pop_if(b, NULL);
    nir_pop_if(b, NULL);
    nir_push_else(b, NULL);
    nir_push_if(b, nir_ieq_imm(b, sd->bits1, 16)); //half
    CONVERT_SWAP_WRITE(nir_format_float_to_half(b, pixel));
    nir_push_else(b, NULL);
    CONVERT_SWAP_WRITE(pixel);
    nir_pop_if(b, NULL);
    nir_pop_if(b, NULL);
}

static void *
create_conversion_shader(struct st_context *st, enum pipe_texture_target target, unsigned num_components)
{
    const nir_shader_compiler_options *options = st_get_nir_compiler_options(st, MESA_SHADER_COMPUTE);
    nir_builder b = nir_builder_init_simple_shader(MESA_SHADER_COMPUTE, options, "%s", "convert");
    b.shader->info.workgroup_size[0] = target != PIPE_TEXTURE_1D ? 8 : 64;
    b.shader->info.workgroup_size[1] = target != PIPE_TEXTURE_1D ? 8 : 1;

    b.shader->info.workgroup_size[2] = 1;
    b.shader->info.textures_used[0] = 1;
    b.shader->info.num_ssbos = 1;
    b.shader->num_uniforms = 2;
    nir_variable_create(b.shader, nir_var_mem_ssbo, glsl_array_type(glsl_float_type(), 0, 4), "ssbo");
    nir_variable *sampler = nir_variable_create(b.shader, nir_var_uniform, st_pbo_sampler_type_for_target(target, ST_PBO_CONVERT_FLOAT), "sampler");
    unsigned coord_components = glsl_get_sampler_coordinate_components(sampler->type);
    sampler->data.explicit_binding = 1;

    struct pbo_shader_data sd;
    init_pbo_shader_data(&b, &sd);

    nir_ssa_def *bsize = nir_imm_ivec4(&b,
                                       b.shader->info.workgroup_size[0],
                                       b.shader->info.workgroup_size[1],
                                       b.shader->info.workgroup_size[2],
                                       0);
    nir_ssa_def *wid = nir_load_workgroup_id(&b, 32);
    nir_ssa_def *iid = nir_load_local_invocation_id(&b);
    nir_ssa_def *tile = nir_imul(&b, wid, bsize);
    nir_ssa_def *global_id = nir_iadd(&b, tile, iid);
    nir_ssa_def *start = nir_iadd(&b, global_id, sd.offset);

    nir_ssa_def *coord = nir_channels(&b, start, (1<<coord_components)-1);
    nir_ssa_def *max = nir_iadd(&b, sd.offset, sd.range);
    nir_push_if(&b, nir_ball(&b, nir_ilt(&b, coord, nir_channels(&b, max, (1<<coord_components)-1))));
    nir_tex_instr *txf = nir_tex_instr_create(b.shader, 3);
    txf->is_array = glsl_sampler_type_is_array(sampler->type);
    txf->op = nir_texop_txf;
    txf->sampler_dim = glsl_get_sampler_dim(sampler->type);
    txf->dest_type = nir_type_float32;
    txf->coord_components = coord_components;
    txf->texture_index = 0;
    txf->sampler_index = 0;
    txf->src[0].src_type = nir_tex_src_coord;
    txf->src[0].src = nir_src_for_ssa(coord);
    txf->src[1].src_type = nir_tex_src_lod;
    txf->src[1].src = nir_src_for_ssa(nir_imm_int(&b, 0));
    txf->src[2].src_type = nir_tex_src_texture_deref;
    nir_deref_instr *sampler_deref = nir_build_deref_var(&b, sampler);
    txf->src[2].src = nir_src_for_ssa(&sampler_deref->dest.ssa);

    nir_ssa_dest_init(&txf->instr, &txf->dest, 4, 32, NULL);
    nir_builder_instr_insert(&b, &txf->instr);

    /* pass the grid offset as the coord to get the zero-indexed buffer offset */
    do_shader_conversion(&b, &txf->dest.ssa, num_components, global_id, &sd);

    nir_pop_if(&b, NULL);

    nir_validate_shader(b.shader, NULL);
    st_nir_opts(b.shader);
    return st_nir_finish_builtin_shader(st, b.shader);
}

static void
invert_swizzle(uint8_t *out, const uint8_t *in)
{
    /* First, default to all zeroes to prevent uninitialized junk */
    for (unsigned c = 0; c < 4; ++c)
        out[c] = PIPE_SWIZZLE_0;

    /* Now "do" what the swizzle says */
    for (unsigned c = 0; c < 4; ++c) {
        unsigned char i = in[c];

        /* Who cares? */
        assert(PIPE_SWIZZLE_X == 0);
        if (i > PIPE_SWIZZLE_W)
            continue;
        /* Invert */
        unsigned idx = i - PIPE_SWIZZLE_X;
        out[idx] = PIPE_SWIZZLE_X + c;
    }
}

static uint32_t
compute_shader_key(enum pipe_texture_target target, unsigned num_components)
{
    uint8_t key_target[] = {
            [PIPE_BUFFER] = UINT8_MAX,
            [PIPE_TEXTURE_1D] = 1,
            [PIPE_TEXTURE_2D] = 2,
            [PIPE_TEXTURE_3D] = 3,
            [PIPE_TEXTURE_CUBE] = 4,
            [PIPE_TEXTURE_RECT] = UINT8_MAX,
            [PIPE_TEXTURE_1D_ARRAY] = 5,
            [PIPE_TEXTURE_2D_ARRAY] = 6,
            [PIPE_TEXTURE_CUBE_ARRAY] = UINT8_MAX,
    };
    assert(target < ARRAY_SIZE(key_target));
    assert(key_target[target] != UINT8_MAX);
    return key_target[target] | (num_components << 3);
}

static unsigned
get_dim_from_target(enum pipe_texture_target target)
{
    switch (target) {
        case PIPE_TEXTURE_1D:
            return 1;
        case PIPE_TEXTURE_2D_ARRAY:
        case PIPE_TEXTURE_3D:
            return 3;
        default:
            return 2;
    }
}

static enum pipe_texture_target
get_target_from_texture(struct pipe_resource *src)
{
    enum pipe_texture_target view_target;
    switch (src->target) {
        case PIPE_TEXTURE_RECT:
            view_target = PIPE_TEXTURE_2D;
            break;
        case PIPE_TEXTURE_CUBE:
        case PIPE_TEXTURE_CUBE_ARRAY:
            view_target = PIPE_TEXTURE_2D_ARRAY;
            break;
        default:
            view_target = src->target;
            break;
    }
    return view_target;
}

/* force swizzling behavior for sampling */
enum swizzle_clamp {
    /* force component selection for named format */
    SWIZZLE_CLAMP_LUMINANCE = 1,
    SWIZZLE_CLAMP_ALPHA = 2,
    SWIZZLE_CLAMP_LUMINANCE_ALPHA = 3,
    SWIZZLE_CLAMP_INTENSITY = 4,
    SWIZZLE_CLAMP_RGBX = 5,

    /* select only 1 component */
    SWIZZLE_CLAMP_GREEN = 8,
    SWIZZLE_CLAMP_BLUE = 16,

    /* reverse ordering for format emulation */
    SWIZZLE_CLAMP_BGRA = 32,
};

static struct pipe_resource *
download_texture_compute(struct st_context *st,
                         const struct gl_pixelstore_attrib *pack,
                         GLint xoffset, GLint yoffset, GLint zoffset,
                         GLsizei width, GLsizei height, GLint depth,
                         unsigned level, unsigned layer,
                         GLenum format, GLenum type,
                         enum pipe_format src_format,
                         enum pipe_texture_target view_target,
                         struct pipe_resource *src,
                         enum pipe_format dst_format,
                         enum swizzle_clamp swizzle_clamp)
{
    struct pipe_context *pipe = st->pipe;
    struct pipe_screen *screen = st->screen;
    struct pipe_resource *dst = NULL;
    unsigned dim = get_dim_from_target(view_target);

    /* clamp 3d offsets based on slice */
    if (view_target == PIPE_TEXTURE_3D)
        zoffset += layer;

    unsigned num_components = 0;
    /* Upload constants */
    {
        struct pipe_constant_buffer cb;
        assert(view_target != PIPE_TEXTURE_1D_ARRAY || !yoffset);
        struct pbo_data pd = {
                .x = xoffset,
                .y = yoffset,
                .width = width, .height = height, .depth = depth,
                .invert = pack->Invert,
                .blocksize = util_format_get_blocksize(dst_format) - 1,
                .alignment = ffs(MAX2(pack->Alignment, 1)) - 1,
        };
        num_components = fill_pbo_data(&pd, src_format, dst_format, pack->SwapBytes == 1);

        cb.buffer = NULL;
        cb.user_buffer = &pd;
        cb.buffer_offset = 0;
        cb.buffer_size = sizeof(pd);

        pipe->set_constant_buffer(pipe, PIPE_SHADER_COMPUTE, 0, false, &cb);
    }

    uint32_t hash_key = compute_shader_key(view_target, num_components);
    assert(hash_key != 0);

    struct hash_entry *he = _mesa_hash_table_search(st->pbo.shaders, (void*)(uintptr_t)hash_key);
    void *cs;
    if (!he) {
        cs = create_conversion_shader(st, view_target, num_components);
        he = _mesa_hash_table_insert(st->pbo.shaders, (void*)(uintptr_t)hash_key, cs);
    }
    cs = he->data;
    assert(cs);
    struct cso_context *cso = st->cso_context;

    cso_save_compute_state(cso, CSO_BIT_COMPUTE_SHADER | CSO_BIT_COMPUTE_SAMPLERS);
    cso_set_compute_shader_handle(cso, cs);

    /* Set up the sampler_view */
    {
        struct pipe_sampler_view templ;
        struct pipe_sampler_view *sampler_view;
        struct pipe_sampler_state sampler = {0};
        const struct pipe_sampler_state *samplers[1] = {&sampler};
        const struct util_format_description *desc = util_format_description(dst_format);

        u_sampler_view_default_template(&templ, src, src_format);
        if (util_format_is_depth_or_stencil(dst_format)) {
            templ.swizzle_r = PIPE_SWIZZLE_X;
            templ.swizzle_g = PIPE_SWIZZLE_X;
            templ.swizzle_b = PIPE_SWIZZLE_X;
            templ.swizzle_a = PIPE_SWIZZLE_X;
        } else {
            uint8_t invswizzle[4];
            const uint8_t *swizzle;

            /* these swizzle output bits require explicit component selection/ordering */
            if (swizzle_clamp & SWIZZLE_CLAMP_GREEN) {
                for (unsigned i = 0; i < 4; i++)
                    invswizzle[i] = PIPE_SWIZZLE_Y;
            } else if (swizzle_clamp & SWIZZLE_CLAMP_BLUE) {
                for (unsigned i = 0; i < 4; i++)
                    invswizzle[i] = PIPE_SWIZZLE_Z;
            } else {
                if (swizzle_clamp & SWIZZLE_CLAMP_BGRA) {
                    if (util_format_get_nr_components(dst_format) == 3)
                        swizzle = util_format_description(PIPE_FORMAT_B8G8R8_UNORM)->swizzle;
                    else
                        swizzle = util_format_description(PIPE_FORMAT_B8G8R8A8_UNORM)->swizzle;
                } else {
                    swizzle = desc->swizzle;
                }
                invert_swizzle(invswizzle, swizzle);
            }
            swizzle_clamp &= ~(SWIZZLE_CLAMP_BGRA | SWIZZLE_CLAMP_GREEN | SWIZZLE_CLAMP_BLUE);

            /* these swizzle input modes clamp unused components to 0 and (sometimes) alpha to 1 */
            switch (swizzle_clamp) {
                case SWIZZLE_CLAMP_LUMINANCE:
                    if (util_format_is_luminance(dst_format))
                        break;
                    for (unsigned i = 0; i < 4; i++) {
                        if (invswizzle[i] != PIPE_SWIZZLE_X)
                            invswizzle[i] = invswizzle[i] == PIPE_SWIZZLE_W ? PIPE_SWIZZLE_1 : PIPE_SWIZZLE_0;
                    }
                    break;
                case SWIZZLE_CLAMP_ALPHA:
                    for (unsigned i = 0; i < 4; i++) {
                        if (invswizzle[i] != PIPE_SWIZZLE_W)
                            invswizzle[i] = PIPE_SWIZZLE_0;
                    }
                    break;
                case SWIZZLE_CLAMP_LUMINANCE_ALPHA:
                    if (util_format_is_luminance_alpha(dst_format))
                        break;
                    for (unsigned i = 0; i < 4; i++) {
                        if (invswizzle[i] != PIPE_SWIZZLE_X && invswizzle[i] != PIPE_SWIZZLE_W)
                            invswizzle[i] = PIPE_SWIZZLE_0;
                    }
                    break;
                case SWIZZLE_CLAMP_INTENSITY:
                    for (unsigned i = 0; i < 4; i++) {
                        if (invswizzle[i] == PIPE_SWIZZLE_W)
                            invswizzle[i] = PIPE_SWIZZLE_1;
                        else if (invswizzle[i] != PIPE_SWIZZLE_X)
                            invswizzle[i] = PIPE_SWIZZLE_0;
                    }
                    break;
                case SWIZZLE_CLAMP_RGBX:
                    for (unsigned i = 0; i < 4; i++) {
                        if (invswizzle[i] == PIPE_SWIZZLE_W)
                            invswizzle[i] = PIPE_SWIZZLE_1;
                    }
                    break;
                default: break;
            }
            templ.swizzle_r = invswizzle[0];
            templ.swizzle_g = invswizzle[1];
            templ.swizzle_b = invswizzle[2];
            templ.swizzle_a = invswizzle[3];
        }
        templ.target = view_target;
        templ.u.tex.first_level = level;
        templ.u.tex.last_level = level;

        /* array textures expect to have array index provided */
        if (view_target != PIPE_TEXTURE_3D && src->array_size) {
            templ.u.tex.first_layer = layer;
            if (view_target == PIPE_TEXTURE_1D_ARRAY) {
                templ.u.tex.first_layer += yoffset;
                templ.u.tex.last_layer = templ.u.tex.first_layer + height - 1;
            } else {
                templ.u.tex.first_layer += zoffset;
                templ.u.tex.last_layer = templ.u.tex.first_layer + depth - 1;
            }
        }

        sampler_view = pipe->create_sampler_view(pipe, src, &templ);
        if (sampler_view == NULL)
            goto fail;

        pipe->set_sampler_views(pipe, PIPE_SHADER_COMPUTE, 0, 1, 0, false,
                                &sampler_view);
        st->state.num_sampler_views[PIPE_SHADER_COMPUTE] =
                MAX2(st->state.num_sampler_views[PIPE_SHADER_COMPUTE], 1);

        pipe_sampler_view_reference(&sampler_view, NULL);

        cso_set_samplers(cso, PIPE_SHADER_COMPUTE, 1, samplers);
    }

    /* Set up destination buffer */
    unsigned img_stride = _mesa_image_image_stride(pack, width, height, format, type);
    unsigned buffer_size = (depth + (dim == 3 ? pack->SkipImages : 0)) * img_stride;
    {
        dst = pipe_buffer_create(screen, PIPE_BIND_SHADER_BUFFER, PIPE_USAGE_STAGING, buffer_size);
        if (!dst)
            goto fail;

        struct pipe_shader_buffer buffer;
        memset(&buffer, 0, sizeof(buffer));
        buffer.buffer = dst;
        buffer.buffer_size = buffer_size;

        pipe->set_shader_buffers(pipe, PIPE_SHADER_COMPUTE, 0, 1, &buffer, 0x1);
    }

    struct pipe_grid_info info = { 0 };
    info.block[0] = src->target != PIPE_TEXTURE_1D ? 8 : 64;
    info.block[1] = src->target != PIPE_TEXTURE_1D ? 8 : 1;
    info.last_block[0] = width % info.block[0];
    info.last_block[1] = height % info.block[1];
    info.block[2] = 1;
    info.grid[0] = DIV_ROUND_UP(width, info.block[0]);
    info.grid[1] = DIV_ROUND_UP(height, info.block[1]);
    info.grid[2] = depth;

    pipe->launch_grid(pipe, &info);

    fail:
    cso_restore_compute_state(cso);

    /* Unbind all because st/mesa won't do it if the current shader doesn't
     * use them.
     */
    pipe->set_sampler_views(pipe, PIPE_SHADER_COMPUTE, 0, 0, false,
                            st->state.num_sampler_views[PIPE_SHADER_COMPUTE],
                            NULL);
    st->state.num_sampler_views[PIPE_SHADER_COMPUTE] = 0;
    pipe->set_shader_buffers(pipe, PIPE_SHADER_COMPUTE, 0, 1, NULL, 0);

    st->dirty |= ST_NEW_CS_CONSTANTS |
                 ST_NEW_CS_SSBOS |
                 ST_NEW_CS_SAMPLER_VIEWS;

    return dst;
}

static void
copy_converted_buffer(struct gl_context * ctx,
                      struct gl_pixelstore_attrib *pack,
                      enum pipe_texture_target view_target,
                      struct pipe_resource *dst, enum pipe_format dst_format,
                      GLint xoffset, GLint yoffset, GLint zoffset,
                      GLsizei width, GLsizei height, GLint depth,
                      GLenum format, GLenum type, void *pixels)
{
    struct pipe_transfer *xfer;
    struct st_context *st = st_context(ctx);
    unsigned dim = get_dim_from_target(view_target);
    uint8_t *map = pipe_buffer_map(st->pipe, dst, PIPE_MAP_READ | PIPE_MAP_ONCE, &xfer);
    if (!map)
        return;

    pixels = _mesa_map_pbo_dest(ctx, pack, pixels);
    /* compute shader doesn't handle these to cut down on uniform size */
    if (pack->RowLength ||
        pack->SkipPixels ||
        pack->SkipRows ||
        pack->ImageHeight ||
        pack->SkipImages) {

        if (view_target == PIPE_TEXTURE_1D_ARRAY) {
            depth = height;
            height = 1;
            zoffset = yoffset;
            yoffset = 0;
        }
        struct gl_pixelstore_attrib packing = *pack;
        memset(&packing.RowLength, 0, offsetof(struct gl_pixelstore_attrib, SwapBytes) - offsetof(struct gl_pixelstore_attrib, RowLength));
        for (unsigned z = 0; z < depth; z++) {
            for (unsigned y = 0; y < height; y++) {
                GLubyte *dst = _mesa_image_address(dim, pack, pixels,
                                                   width, height, format, type,
                                                   z, y, 0);
                GLubyte *srcpx = _mesa_image_address(dim, &packing, map,
                                                     width, height, format, type,
                                                     z, y, 0);
                memcpy(dst, srcpx, util_format_get_stride(dst_format, width));
            }
        }
    } else {
        /* direct copy for all other cases */
        memcpy(pixels, map, dst->width0);
    }

    _mesa_unmap_pbo_dest(ctx, pack);
    pipe_buffer_unmap(st->pipe, xfer);
}

bool
st_GetTexSubImage_shader(struct gl_context * ctx,
                         GLint xoffset, GLint yoffset, GLint zoffset,
                         GLsizei width, GLsizei height, GLint depth,
                         GLenum format, GLenum type, void * pixels,
                         struct gl_texture_image *texImage)
{
    struct st_context *st = st_context(ctx);
    struct pipe_screen *screen = st->screen;
    struct st_texture_object *stObj = st_texture_object(texImage->TexObject);
    struct pipe_resource *src = stObj->pt;
    struct pipe_resource *dst = NULL;
    enum pipe_format dst_format, src_format;
    unsigned level = texImage->Level + texImage->TexObject->Attrib.MinLevel;
    unsigned layer = texImage->Face + texImage->TexObject->Attrib.MinLayer;
    enum pipe_texture_target view_target;

    assert(!_mesa_is_format_etc2(texImage->TexFormat) &&
           !_mesa_is_format_astc_2d(texImage->TexFormat) &&
           texImage->TexFormat != MESA_FORMAT_ETC1_RGB8);

    /* See if the texture format already matches the format and type,
     * in which case the memcpy-based fast path will be used. */
    if (_mesa_format_matches_format_and_type(texImage->TexFormat, format,
                                             type, ctx->Pack.SwapBytes, NULL)) {
        return false;
    }
    enum swizzle_clamp swizzle_clamp = 0;
    src_format = st_pbo_get_src_format(screen, stObj->surface_based ? stObj->surface_format : src->format, src);
    if (src_format == PIPE_FORMAT_NONE)
        return false;

    if (texImage->_BaseFormat != _mesa_get_format_base_format(texImage->TexFormat)) {
        /* special handling for drivers that don't support these formats natively */
        if (texImage->_BaseFormat == GL_LUMINANCE)
            swizzle_clamp = SWIZZLE_CLAMP_LUMINANCE;
        else if (texImage->_BaseFormat == GL_LUMINANCE_ALPHA)
            swizzle_clamp = SWIZZLE_CLAMP_LUMINANCE_ALPHA;
        else if (texImage->_BaseFormat == GL_ALPHA)
            swizzle_clamp = SWIZZLE_CLAMP_ALPHA;
        else if (texImage->_BaseFormat == GL_INTENSITY)
            swizzle_clamp = SWIZZLE_CLAMP_INTENSITY;
        else if (texImage->_BaseFormat == GL_RGB)
            swizzle_clamp = SWIZZLE_CLAMP_RGBX;
    }

    dst_format = st_pbo_get_dst_format(ctx, PIPE_BUFFER, src_format, false, format, type, 0);

    if (dst_format == PIPE_FORMAT_NONE) {
        bool need_bgra_swizzle = false;
        dst_format = get_convert_format(ctx, src_format, format, type, &need_bgra_swizzle);
        if (dst_format == PIPE_FORMAT_NONE)
            return false;
        /* special swizzling for component selection */
        if (need_bgra_swizzle)
            swizzle_clamp |= SWIZZLE_CLAMP_BGRA;
        else if (format == GL_GREEN_INTEGER)
            swizzle_clamp |= SWIZZLE_CLAMP_GREEN;
        else if (format == GL_BLUE_INTEGER)
            swizzle_clamp |= SWIZZLE_CLAMP_BLUE;
    }

    /* check with the driver to see if memcpy is likely to be faster */
    if (!screen->is_compute_copy_faster(screen, src_format, dst_format, width, height, depth, true))
        return false;

    view_target = get_target_from_texture(src);
    /* I don't know why this works
     * only for the texture rects
     * but that's how it is
     */
    if ((src->target != PIPE_TEXTURE_RECT &&
         /* this would need multiple samplerviews */
         ((util_format_is_depth_and_stencil(src_format) && util_format_is_depth_and_stencil(dst_format)) ||
          /* these format just doesn't work and science can't explain why */
          dst_format == PIPE_FORMAT_Z32_FLOAT)) ||
        /* L8 -> L32_FLOAT is another thinker */
        (!util_format_is_float(src_format) && dst_format == PIPE_FORMAT_L32_FLOAT))
        return false;

    dst = download_texture_compute(st, &ctx->Pack, xoffset, yoffset, zoffset, width, height, depth,
                                   level, layer, format, type, src_format, view_target, src, dst_format,
                                   swizzle_clamp);

    copy_converted_buffer(ctx, &ctx->Pack, view_target, dst, dst_format, xoffset, yoffset, zoffset,
                          width, height, depth, format, type, pixels);

    pipe_resource_reference(&dst, NULL);

    return true;
}


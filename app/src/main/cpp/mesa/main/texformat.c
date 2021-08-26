#include "context.h"
#include "enums.h"
#include "mtypes.h"
#include "texcompress.h"
#include "texformat.h"
#include "glformats.h"

#define RETURN_IF_SUPPORTED(f) do {		\
   if (ctx->TextureFormatSupported[f])		\
      return f;					\
} while (0)

/**
 * Choose an appropriate texture format given the format, type and
 * internalFormat parameters passed to glTexImage().
 *
 * \param ctx  the GL context.
 * \param target  a texture target (GL_TEXTURE_x)
 * \param internalFormat  user's prefered internal texture format.
 * \param format  incoming image pixel format.
 * \param type  incoming image data type.
 *
 * \return the closest mesa_format for the given format/type arguments
 *
 * This is called via dd_function_table::ChooseTextureFormat.  Hardware
 * drivers may override this function with a specialized version.
 */
mesa_format
_mesa_choose_tex_format(struct gl_context *ctx, GLenum target,
                        GLint internalFormat, GLenum format, GLenum type)
{
    (void) format;

    switch (internalFormat) {
        /* shallow RGBA formats */
        case 4:
        case GL_RGBA:
            if (type == GL_UNSIGNED_SHORT_4_4_4_4_REV) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B4G4R4A4_UNORM);
            } else if (type == GL_UNSIGNED_SHORT_1_5_5_5_REV) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B5G5R5A1_UNORM);
            } else if (type == GL_UNSIGNED_SHORT_5_5_5_1) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B5G5R5A1_UNORM);
            } else if (type == GL_UNSIGNED_INT_2_10_10_10_REV) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B10G10R10A2_UNORM);
            }
            FALLTHROUGH;

        case GL_RGBA8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;
        case GL_RGB5_A1:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B5G5R5A1_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A1B5G5R5_UNORM);
            break;
        case GL_RGBA2:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A4R4G4B4_UNORM); /* just to test another format*/
            RETURN_IF_SUPPORTED(MESA_FORMAT_B4G4R4A4_UNORM);
            break;
        case GL_RGBA4:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B4G4R4A4_UNORM);
            break;

            /* deep RGBA formats */
        case GL_RGB10_A2:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R10G10B10A2_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B10G10R10A2_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;
        case GL_RGBA12:
        case GL_RGBA16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;

            /* shallow RGB formats */
        case 3:
        case GL_RGB:
            if (type == GL_UNSIGNED_INT_2_10_10_10_REV) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B10G10R10A2_UNORM);
            }
            if (type == GL_UNSIGNED_SHORT_5_6_5) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_B5G6R5_UNORM);
            }
            FALLTHROUGH;
        case GL_RGB8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8X8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_UNORM);

            RETURN_IF_SUPPORTED(MESA_FORMAT_BGR_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8X8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;
        case GL_R3_G3_B2:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B2G3R3_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B5G6R5_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R5G6B5_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_BGR_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8X8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;
        case GL_RGB4:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R5G6B5_UNORM); /* just to test another format */
            RETURN_IF_SUPPORTED(MESA_FORMAT_B5G6R5_UNORM);
            break;
        case GL_RGB5:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B5G6R5_UNORM);
            break;

            /* deep RGB formats */
        case GL_RGB10:
        case GL_RGB12:
        case GL_RGB16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8X8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;

            /* Alpha formats */
        case GL_ALPHA:
        case GL_ALPHA4:
        case GL_ALPHA8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;

        case GL_ALPHA12:
        case GL_ALPHA16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;

            /* Luminance formats */
        case 1:
        case GL_LUMINANCE:
        case GL_LUMINANCE4:
        case GL_LUMINANCE8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UNORM8);
            break;

        case GL_LUMINANCE12:
        case GL_LUMINANCE16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UNORM8);
            break;

            /* Luminance/Alpha formats */
        case GL_LUMINANCE4_ALPHA4:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L4A4_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;

        case 2:
        case GL_LUMINANCE_ALPHA:
        case GL_LUMINANCE6_ALPHA2:
        case GL_LUMINANCE8_ALPHA8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;

        case GL_LUMINANCE12_ALPHA4:
        case GL_LUMINANCE12_ALPHA12:
        case GL_LUMINANCE16_ALPHA16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;

        case GL_INTENSITY:
        case GL_INTENSITY4:
        case GL_INTENSITY8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UNORM8);
            break;

        case GL_INTENSITY12:
        case GL_INTENSITY16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UNORM8);
            break;

        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32:
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z_UNORM32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z24_UNORM_X8_UINT);
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z24_UNORM_S8_UINT);
            break;
        case GL_DEPTH_COMPONENT16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z_UNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z24_UNORM_X8_UINT);
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z24_UNORM_S8_UINT);
            break;

        case GL_COMPRESSED_ALPHA_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;
        case GL_COMPRESSED_LUMINANCE_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UNORM8);
            break;
        case GL_COMPRESSED_LUMINANCE_ALPHA_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UNORM8);
            break;
        case GL_COMPRESSED_INTENSITY_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UNORM8);
            break;
        case GL_COMPRESSED_RGB_ARB:
            /* We don't use texture compression for 1D and 1D array textures.
             * For 1D textures, compressions doesn't buy us much.
             * For 1D ARRAY textures, there's complicated issues with updating
             * sub-regions on non-block boundaries with glCopyTexSubImage, among
             * other issues.  FWIW, the GL_EXT_texture_array extension prohibits
             * 1D ARRAY textures in S3TC format.
             */
            if (target != GL_TEXTURE_1D && target != GL_TEXTURE_1D_ARRAY) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_DXT1);
            }
            RETURN_IF_SUPPORTED(MESA_FORMAT_BGR_UNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8X8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;
        case GL_COMPRESSED_RGBA_ARB:
            /* We don't use texture compression for 1D and 1D array textures. */
            if (target != GL_TEXTURE_1D && target != GL_TEXTURE_1D_ARRAY) {
                RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_DXT3); /* Not rgba_dxt1, see spec */
            }
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;

        case GL_RGB565:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B5G6R5_UNORM);
            break;

        case GL_YCBCR_MESA:
            if (type == GL_UNSIGNED_SHORT_8_8_MESA)
                RETURN_IF_SUPPORTED(MESA_FORMAT_YCBCR);
            else
                RETURN_IF_SUPPORTED(MESA_FORMAT_YCBCR_REV);
            break;

        case GL_ALPHA16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_ALPHA32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_LUMINANCE16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_LUMINANCE32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_LUMINANCE_ALPHA16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_LUMINANCE_ALPHA32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_INTENSITY16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_INTENSITY32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_RGB16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_RGB32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_RGBA16F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_RGBA32F_ARB:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;

        case GL_RGB9_E5:
            /* GL_EXT_texture_shared_exponent -- just one format to support */
            assert(ctx->TextureFormatSupported[MESA_FORMAT_R9G9B9E5_FLOAT]);
            return MESA_FORMAT_R9G9B9E5_FLOAT;

        case GL_R11F_G11F_B10F:
            /* GL_EXT_texture_packed_float -- just one format to support */
            assert(ctx->TextureFormatSupported[MESA_FORMAT_R11G11B10_FLOAT]);
            return MESA_FORMAT_R11G11B10_FLOAT;

        case GL_DEPTH_STENCIL_EXT:
        case GL_DEPTH24_STENCIL8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_S8_UINT_Z24_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_Z24_UNORM_S8_UINT);
            break;

        case GL_DEPTH_COMPONENT32F:
            assert(ctx->TextureFormatSupported[MESA_FORMAT_Z_FLOAT32]);
            return MESA_FORMAT_Z_FLOAT32;
        case GL_DEPTH32F_STENCIL8:
            assert(ctx->TextureFormatSupported[MESA_FORMAT_Z32_FLOAT_S8X24_UINT]);
            return MESA_FORMAT_Z32_FLOAT_S8X24_UINT;

        case GL_RED_SNORM:
        case GL_R8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SNORM8);
            break;
        case GL_RG_SNORM:
        case GL_RG8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SNORM8);
            break;
        case GL_RGB_SNORM:
        case GL_RGB8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8X8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_X8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            break;
        case GL_RGBA_SNORM:
        case GL_RGBA8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_ALPHA_SNORM:
        case GL_ALPHA8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_SNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_LUMINANCE_SNORM:
        case GL_LUMINANCE8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_X8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_LUMINANCE_ALPHA_SNORM:
        case GL_LUMINANCE8_ALPHA8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_INTENSITY_SNORM:
        case GL_INTENSITY8_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_SNORM8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_R16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SNORM16);
            break;
        case GL_RG16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SNORM16);
            break;
        case GL_RGB16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_SNORM16);
            FALLTHROUGH;
        case GL_RGBA16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_ALPHA16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_LUMINANCE16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SNORM16);
            FALLTHROUGH;
        case GL_LUMINANCE16_ALPHA16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;
        case GL_INTENSITY16_SNORM:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SNORM16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SNORM);
            break;

        case GL_SRGB_EXT:
        case GL_SRGB8_EXT:
            /* there is no MESA_FORMAT_RGB_SRGB8 */
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8X8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SRGB);

            RETURN_IF_SUPPORTED(MESA_FORMAT_BGR_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);

            RETURN_IF_SUPPORTED(MESA_FORMAT_X8B8G8R8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_SRGB_ALPHA_EXT:
        case GL_SRGB8_ALPHA8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R8G8B8A8_SRGB);

            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_SR8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SRGB8);
            break;
        case GL_SRG8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SRGB8);
            break;
        case GL_SLUMINANCE_EXT:
        case GL_SLUMINANCE8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_SLUMINANCE_ALPHA_EXT:
        case GL_SLUMINANCE8_ALPHA8_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_COMPRESSED_SLUMINANCE_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_COMPRESSED_SLUMINANCE_ALPHA_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_COMPRESSED_SRGB_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_SRGB_DXT1);
            RETURN_IF_SUPPORTED(MESA_FORMAT_BGR_SRGB8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;
        case GL_COMPRESSED_SRGB_ALPHA_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_SRGBA_DXT3); /* Not srgba_dxt1, see spec */
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8B8G8R8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_SRGB);
            RETURN_IF_SUPPORTED(MESA_FORMAT_A8R8G8B8_SRGB);
            break;

        case GL_ALPHA8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_ALPHA16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_ALPHA32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_ALPHA8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_ALPHA16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_ALPHA32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_A_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;
        case GL_LUMINANCE8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_LUMINANCE16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_LUMINANCE32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_LUMINANCE8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_LUMINANCE16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_LUMINANCE32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_L_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;
        case GL_LUMINANCE_ALPHA8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_LUMINANCE_ALPHA16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_LUMINANCE_ALPHA32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_LUMINANCE_ALPHA8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_LUMINANCE_ALPHA16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_LUMINANCE_ALPHA32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_LA_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;
        case GL_INTENSITY8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_INTENSITY16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_INTENSITY32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_INTENSITY8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_INTENSITY16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_INTENSITY32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_I_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;

        case GL_RGB8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_UINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_RGB16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_UINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_RGB32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_UINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_RGB8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_SINT8);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_RGB16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_SINT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_RGB32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGB_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBX_SINT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;
        case GL_RGBA8UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT8);
            break;
        case GL_RGBA16UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT16);
            break;
        case GL_RGBA32UI_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_UINT32);
            break;
        case GL_RGBA8I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT8);
            break;
        case GL_RGBA16I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT16);
            break;
        case GL_RGBA32I_EXT:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_SINT32);
            break;

        case GL_R8:
        case GL_RED:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UNORM8);
            break;

        case GL_COMPRESSED_RED:
            if (target != GL_TEXTURE_1D && target != GL_TEXTURE_1D_ARRAY)
                RETURN_IF_SUPPORTED(MESA_FORMAT_R_RGTC1_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UNORM8);
            break;

        case GL_R16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UNORM16);
            break;

        case GL_RG:
        case GL_RG8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UNORM8);
            break;

        case GL_COMPRESSED_RG:
            if (target != GL_TEXTURE_1D && target != GL_TEXTURE_1D_ARRAY)
                RETURN_IF_SUPPORTED(MESA_FORMAT_RG_RGTC2_UNORM);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UNORM8);
            break;

        case GL_RG16:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UNORM16);
            break;

        case GL_R16F:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_R32F:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;
        case GL_RG16F:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            break;
        case GL_RG32F:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT32);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_FLOAT16);
            RETURN_IF_SUPPORTED(MESA_FORMAT_RGBA_FLOAT16);
            break;

        case GL_R8UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UINT8);
            break;
        case GL_RG8UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UINT8);
            break;
        case GL_R16UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UINT16);
            break;
        case GL_RG16UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UINT16);
            break;
        case GL_R32UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_UINT32);
            break;
        case GL_RG32UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_UINT32);
            break;
        case GL_R8I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SINT8);
            break;
        case GL_RG8I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SINT8);
            break;
        case GL_R16I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SINT16);
            break;
        case GL_RG16I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SINT16);
            break;
        case GL_R32I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_R_SINT32);
            break;
        case GL_RG32I:
            RETURN_IF_SUPPORTED(MESA_FORMAT_RG_SINT32);
            break;

        case GL_RGB10_A2UI:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B10G10R10A2_UINT);
            RETURN_IF_SUPPORTED(MESA_FORMAT_R10G10B10A2_UINT);
            break;

        case GL_BGRA:
            RETURN_IF_SUPPORTED(MESA_FORMAT_B8G8R8A8_UNORM);
            break;

        case GL_STENCIL_INDEX:
        case GL_STENCIL_INDEX8:
            RETURN_IF_SUPPORTED(MESA_FORMAT_S_UINT8);
            break;

        default:
            /* For non-generic compressed format we assert two things:
             *
             * 1. The format has already been validated against the set of available
             *    extensions.
             *
             * 2. The driver only enables the extension if it supports all of the
             *    formats that are part of that extension.
             */
            if (_mesa_is_compressed_format(ctx, internalFormat))
                return _mesa_glenum_to_compressed_format(internalFormat);
    }

    _mesa_problem(ctx, "unexpected format %s in _mesa_choose_tex_format()",
                  _mesa_enum_to_string(internalFormat));
    return MESA_FORMAT_NONE;
}

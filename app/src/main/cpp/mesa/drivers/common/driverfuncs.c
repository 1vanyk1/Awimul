#include "../../main/glheader.h"
#include "../../main/accum.h"
#include "../../main/arrayobj.h"
#include "../../main/context.h"
#include "../../main/draw.h"
#include "../../main/formatquery.h"
#include "../../main/framebuffer.h"
#include "../../main/mipmap.h"
#include "../../main/queryobj.h"
#include "../../main/readpix.h"
#include "../../main/rastpos.h"
#include "../../main/renderbuffer.h"
#include "../../main/shaderobj.h"
#include "../../main/texcompress.h"
#include "../../main/texformat.h"
#include "../../main/texgetimage.h"
#include "../../main/teximage.h"
#include "../../main/texobj.h"
#include "../../main/texstorage.h"
#include "../../main/texstore.h"
#include "../../main/bufferobj.h"
#include "../../main/fbobject.h"
#include "../../main/samplerobj.h"
#include "../../main/syncobj.h"
#include "../../main/barrier.h"
#include "../../main/transformfeedback.h"
#include "../../main/externalobjects.h"

#include "../../program/program.h"
#include "../../tnl/tnl.h"
#include "../../swrast/swrast.h"
#include "../../swrast/s_renderbuffer.h"

#include "driverfuncs.h"
#include "meta.h"



/**
 * Plug in default functions for all pointers in the dd_function_table
 * structure.
 * Device drivers should call this function and then plug in any
 * functions which it wants to override.
 * Some functions (pointers) MUST be implemented by all drivers (REQUIRED).
 *
 * \param table the dd_function_table to initialize
 */
void
_mesa_init_driver_functions(struct dd_function_table *driver)
{
    memset(driver, 0, sizeof(*driver));

    driver->GetString = NULL;  /* REQUIRED! */
    driver->UpdateState = NULL;  /* REQUIRED! */

    driver->Finish = NULL;
    driver->Flush = NULL;

    /* framebuffer/image functions */
    driver->Clear = _swrast_Clear;
    driver->RasterPos = _mesa_RasterPos;
    driver->DrawPixels = _swrast_DrawPixels;
    driver->ReadPixels = _mesa_readpixels;
    driver->CopyPixels = _swrast_CopyPixels;
    driver->Bitmap = _swrast_Bitmap;

    /* Texture functions */
    driver->ChooseTextureFormat = _mesa_choose_tex_format;
    driver->QueryInternalFormat = _mesa_query_internal_format_default;
    driver->TexImage = _mesa_store_teximage;
    driver->TexSubImage = _mesa_store_texsubimage;
    driver->GetTexSubImage = _mesa_meta_GetTexSubImage;
    driver->ClearTexSubImage = _mesa_meta_ClearTexSubImage;
    driver->CopyTexSubImage = _mesa_meta_CopyTexSubImage;
    driver->GenerateMipmap = _mesa_meta_GenerateMipmap;
    driver->TestProxyTexImage = _mesa_test_proxy_teximage;
    driver->CompressedTexImage = _mesa_store_compressed_teximage;
    driver->CompressedTexSubImage = _mesa_store_compressed_texsubimage;
    driver->BindTexture = NULL;
    driver->NewTextureObject = _mesa_new_texture_object;
    driver->DeleteTexture = _mesa_delete_texture_object;
    driver->NewTextureImage = _swrast_new_texture_image;
    driver->DeleteTextureImage = _swrast_delete_texture_image;
    driver->AllocTextureImageBuffer = _swrast_alloc_texture_image_buffer;
    driver->FreeTextureImageBuffer = _swrast_free_texture_image_buffer;
    driver->MapTextureImage = _swrast_map_teximage;
    driver->UnmapTextureImage = _swrast_unmap_teximage;
    driver->DrawTex = _mesa_meta_DrawTex;

    /* Vertex/fragment programs */
    driver->NewProgram = _mesa_new_program;
    driver->DeleteProgram = _mesa_delete_program;

    /* ATI_fragment_shader */
    driver->NewATIfs = NULL;

    /* Draw functions */
    driver->Draw = NULL;
    driver->DrawGallium = _mesa_draw_gallium_fallback;
    driver->DrawGalliumComplex = _mesa_draw_gallium_complex_fallback;
    driver->DrawIndirect = NULL;
    driver->DrawTransformFeedback = NULL;

    /* simple state commands */
    driver->AlphaFunc = NULL;
    driver->BlendColor = NULL;
    driver->BlendEquationSeparate = NULL;
    driver->BlendFuncSeparate = NULL;
    driver->ClipPlane = NULL;
    driver->ColorMask = NULL;
    driver->ColorMaterial = NULL;
    driver->CullFace = NULL;
    driver->DrawBuffer = NULL;
    driver->FrontFace = NULL;
    driver->DepthFunc = NULL;
    driver->DepthMask = NULL;
    driver->DepthRange = NULL;
    driver->Enable = NULL;
    driver->Fogfv = NULL;
    driver->Lightfv = NULL;
    driver->LightModelfv = NULL;
    driver->LineStipple = NULL;
    driver->LineWidth = NULL;
    driver->LogicOpcode = NULL;
    driver->PointParameterfv = NULL;
    driver->PointSize = NULL;
    driver->PolygonMode = NULL;
    driver->PolygonOffset = NULL;
    driver->PolygonStipple = NULL;
    driver->ReadBuffer = NULL;
    driver->RenderMode = NULL;
    driver->Scissor = NULL;
    driver->ShadeModel = NULL;
    driver->StencilFuncSeparate = NULL;
    driver->StencilOpSeparate = NULL;
    driver->StencilMaskSeparate = NULL;
    driver->TexGen = NULL;
    driver->TexEnv = NULL;
    driver->TexParameter = NULL;
    driver->Viewport = NULL;

    /* buffer objects */
    _mesa_init_buffer_object_functions(driver);

    /* query objects */
    _mesa_init_query_object_functions(driver);

    _mesa_init_sync_object_functions(driver);

    /* memory objects */
    _mesa_init_memory_object_functions(driver);

    driver->NewFramebuffer = _mesa_new_framebuffer;
    driver->NewRenderbuffer = _swrast_new_soft_renderbuffer;
    driver->MapRenderbuffer = _swrast_map_soft_renderbuffer;
    driver->UnmapRenderbuffer = _swrast_unmap_soft_renderbuffer;
    driver->RenderTexture = _swrast_render_texture;
    driver->FinishRenderTexture = _swrast_finish_render_texture;
    driver->FramebufferRenderbuffer = _mesa_FramebufferRenderbuffer_sw;
    driver->ValidateFramebuffer = _mesa_validate_framebuffer;

    driver->BlitFramebuffer = _swrast_BlitFramebuffer;
    driver->DiscardFramebuffer = NULL;

    _mesa_init_barrier_functions(driver);
    _mesa_init_shader_object_functions(driver);
    _mesa_init_transform_feedback_functions(driver);
    _mesa_init_sampler_object_functions(driver);

    /* T&L stuff */
    driver->CurrentExecPrimitive = 0;
    driver->CurrentSavePrimitive = 0;
    driver->NeedFlush = 0;
    driver->SaveNeedFlush = 0;

    driver->ProgramStringNotify = _tnl_program_string;
    driver->LightingSpaceChange = NULL;

    /* GL_ARB_texture_storage */
    driver->AllocTextureStorage = _mesa_AllocTextureStorage_sw;

    /* GL_ARB_texture_view */
    driver->TextureView = NULL;

    /* GL_ARB_texture_multisample */
    driver->GetSamplePosition = NULL;

    /* Multithreading */
    driver->SetBackgroundContext = NULL;
}


/**
 * Call the ctx->Driver.* state functions with current values to initialize
 * driver state.
 * Only the Intel drivers use this so far.
 */
void
_mesa_init_driver_state(struct gl_context *ctx)
{
    ctx->Driver.AlphaFunc(ctx, ctx->Color.AlphaFunc, ctx->Color.AlphaRef);

    ctx->Driver.BlendColor(ctx, ctx->Color.BlendColor);

    ctx->Driver.BlendEquationSeparate(ctx,
                                      ctx->Color.Blend[0].EquationRGB,
                                      ctx->Color.Blend[0].EquationA);

    ctx->Driver.BlendFuncSeparate(ctx,
                                  ctx->Color.Blend[0].SrcRGB,
                                  ctx->Color.Blend[0].DstRGB,
                                  ctx->Color.Blend[0].SrcA,
                                  ctx->Color.Blend[0].DstA);

    ctx->Driver.ColorMask(ctx,
                          GET_COLORMASK_BIT(ctx->Color.ColorMask, 0, 0),
                          GET_COLORMASK_BIT(ctx->Color.ColorMask, 0, 1),
                          GET_COLORMASK_BIT(ctx->Color.ColorMask, 0, 2),
                          GET_COLORMASK_BIT(ctx->Color.ColorMask, 0, 3));

    ctx->Driver.CullFace(ctx, ctx->Polygon.CullFaceMode);
    ctx->Driver.DepthFunc(ctx, ctx->Depth.Func);
    ctx->Driver.DepthMask(ctx, ctx->Depth.Mask);

    ctx->Driver.Enable(ctx, GL_ALPHA_TEST, ctx->Color.AlphaEnabled);
    ctx->Driver.Enable(ctx, GL_BLEND, ctx->Color.BlendEnabled);
    ctx->Driver.Enable(ctx, GL_COLOR_LOGIC_OP, ctx->Color.ColorLogicOpEnabled);
    ctx->Driver.Enable(ctx, GL_COLOR_SUM, ctx->Fog.ColorSumEnabled);
    ctx->Driver.Enable(ctx, GL_CULL_FACE, ctx->Polygon.CullFlag);
    ctx->Driver.Enable(ctx, GL_DEPTH_TEST, ctx->Depth.Test);
    ctx->Driver.Enable(ctx, GL_DITHER, ctx->Color.DitherFlag);
    ctx->Driver.Enable(ctx, GL_FOG, ctx->Fog.Enabled);
    ctx->Driver.Enable(ctx, GL_LIGHTING, ctx->Light.Enabled);
    ctx->Driver.Enable(ctx, GL_LINE_SMOOTH, ctx->Line.SmoothFlag);
    ctx->Driver.Enable(ctx, GL_POLYGON_STIPPLE, ctx->Polygon.StippleFlag);
    ctx->Driver.Enable(ctx, GL_SCISSOR_TEST, ctx->Scissor.EnableFlags);
    ctx->Driver.Enable(ctx, GL_STENCIL_TEST, ctx->Stencil.Enabled);
    ctx->Driver.Enable(ctx, GL_TEXTURE_1D, GL_FALSE);
    ctx->Driver.Enable(ctx, GL_TEXTURE_2D, GL_FALSE);
    ctx->Driver.Enable(ctx, GL_TEXTURE_RECTANGLE_NV, GL_FALSE);
    ctx->Driver.Enable(ctx, GL_TEXTURE_3D, GL_FALSE);
    ctx->Driver.Enable(ctx, GL_TEXTURE_CUBE_MAP, GL_FALSE);

    ctx->Driver.Fogfv(ctx, GL_FOG_COLOR, ctx->Fog.Color);
    {
        GLfloat mode = (GLfloat) ctx->Fog.Mode;
        ctx->Driver.Fogfv(ctx, GL_FOG_MODE, &mode);
    }
    ctx->Driver.Fogfv(ctx, GL_FOG_DENSITY, &ctx->Fog.Density);
    ctx->Driver.Fogfv(ctx, GL_FOG_START, &ctx->Fog.Start);
    ctx->Driver.Fogfv(ctx, GL_FOG_END, &ctx->Fog.End);

    ctx->Driver.FrontFace(ctx, ctx->Polygon.FrontFace);

    {
        GLfloat f = (GLfloat) ctx->Light.Model.ColorControl;
        ctx->Driver.LightModelfv(ctx, GL_LIGHT_MODEL_COLOR_CONTROL, &f);
    }

    ctx->Driver.LineWidth(ctx, ctx->Line.Width);
    ctx->Driver.LogicOpcode(ctx, ctx->Color._LogicOp);
    ctx->Driver.PointSize(ctx, ctx->Point.Size);
    ctx->Driver.PolygonStipple(ctx, (const GLubyte *) ctx->PolygonStipple);
    ctx->Driver.Scissor(ctx);
    ctx->Driver.ShadeModel(ctx, ctx->Light.ShadeModel);
    ctx->Driver.StencilFuncSeparate(ctx, GL_FRONT,
                                    ctx->Stencil.Function[0],
                                    ctx->Stencil.Ref[0],
                                    ctx->Stencil.ValueMask[0]);
    ctx->Driver.StencilFuncSeparate(ctx, GL_BACK,
                                    ctx->Stencil.Function[1],
                                    ctx->Stencil.Ref[1],
                                    ctx->Stencil.ValueMask[1]);
    ctx->Driver.StencilMaskSeparate(ctx, GL_FRONT, ctx->Stencil.WriteMask[0]);
    ctx->Driver.StencilMaskSeparate(ctx, GL_BACK, ctx->Stencil.WriteMask[1]);
    ctx->Driver.StencilOpSeparate(ctx, GL_FRONT,
                                  ctx->Stencil.FailFunc[0],
                                  ctx->Stencil.ZFailFunc[0],
                                  ctx->Stencil.ZPassFunc[0]);
    ctx->Driver.StencilOpSeparate(ctx, GL_BACK,
                                  ctx->Stencil.FailFunc[1],
                                  ctx->Stencil.ZFailFunc[1],
                                  ctx->Stencil.ZPassFunc[1]);


    ctx->Driver.DrawBuffer(ctx);
}

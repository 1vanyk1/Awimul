#ifndef GLX_EXTENSION_STRING_H
#define GLX_EXTENSION_STRING_H

enum {
/*   GLX_ARB_get_proc_address is implemented on the client. */
    ARB_context_flush_control_bit = 0,
    ARB_create_context_bit,
    ARB_create_context_no_error_bit,
    ARB_create_context_profile_bit,
    ARB_create_context_robustness_bit,
    ARB_fbconfig_float_bit,
    ARB_framebuffer_sRGB_bit,
    ARB_multisample_bit,
    EXT_create_context_es_profile_bit,
    EXT_create_context_es2_profile_bit,
    EXT_fbconfig_packed_float_bit,
    EXT_get_drawable_type_bit,
    EXT_import_context_bit,
    EXT_libglvnd_bit,
    EXT_no_config_context_bit,
    EXT_stereo_tree_bit,
    EXT_texture_from_pixmap_bit,
    EXT_visual_info_bit,
    EXT_visual_rating_bit,
    MESA_copy_sub_buffer_bit,
    OML_swap_method_bit,
    SGI_make_current_read_bit,
    SGI_swap_control_bit,
    SGI_video_sync_bit,
    SGIS_multisample_bit,
    SGIX_fbconfig_bit,
    SGIX_pbuffer_bit,
    SGIX_visual_select_group_bit,
    INTEL_swap_event_bit,
    __NUM_GLX_EXTS,
};

/* For extensions which have identical ARB and EXT implementation
 * in GLX area, use one enabling bit for both. */
#define EXT_framebuffer_sRGB_bit ARB_framebuffer_sRGB_bit

#define __GLX_EXT_BYTES ((__NUM_GLX_EXTS + 7) / 8)

extern int __glXGetExtensionString(const unsigned char *enable_bits,
                                   char *buffer);
extern void __glXEnableExtension(unsigned char *enable_bits, const char *ext);
extern void __glXInitExtensionEnableBits(unsigned char *enable_bits);

#endif                          /* GLX_EXTENSION_STRING_H */

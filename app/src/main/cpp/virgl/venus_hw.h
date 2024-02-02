#ifndef VENUS_HW_H
#define VENUS_HW_H

#include <stdint.h>

#ifdef VIRGL_RENDERER_UNSTABLE_APIS
struct virgl_renderer_capset_venus {
   uint32_t wire_format_version;
   uint32_t vk_xml_version;
   uint32_t vk_ext_command_serialization_spec_version;
   uint32_t vk_mesa_venus_protocol_spec_version;
};
#endif

#endif /* VENUS_HW_H */

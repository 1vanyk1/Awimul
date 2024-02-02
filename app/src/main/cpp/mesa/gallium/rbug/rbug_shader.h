#ifndef _RBUG_PROTO_SHADER_H_
#define _RBUG_PROTO_SHADER_H_

#include "rbug_proto.h"
#include "rbug_core.h"

struct rbug_proto_shader_list
{
    struct rbug_header header;
    rbug_context_t context;
};

struct rbug_proto_shader_info
{
    struct rbug_header header;
    rbug_context_t context;
    rbug_shader_t shader;
};

struct rbug_proto_shader_disable
{
    struct rbug_header header;
    rbug_context_t context;
    rbug_shader_t shader;
    uint8_t disable;
};

struct rbug_proto_shader_replace
{
    struct rbug_header header;
    rbug_context_t context;
    rbug_shader_t shader;
    uint32_t *tokens;
    uint32_t tokens_len;
};

struct rbug_proto_shader_list_reply
{
    struct rbug_header header;
    uint32_t serial;
    rbug_shader_t *shaders;
    uint32_t shaders_len;
};

struct rbug_proto_shader_info_reply
{
    struct rbug_header header;
    uint32_t serial;
    uint32_t *original;
    uint32_t original_len;
    uint32_t *replaced;
    uint32_t replaced_len;
    uint8_t disabled;
};

int rbug_send_shader_list(struct rbug_connection *__con,
                          rbug_context_t context,
                          uint32_t *__serial);

int rbug_send_shader_info(struct rbug_connection *__con,
                          rbug_context_t context,
                          rbug_shader_t shader,
                          uint32_t *__serial);

int rbug_send_shader_disable(struct rbug_connection *__con,
                             rbug_context_t context,
                             rbug_shader_t shader,
                             uint8_t disable,
                             uint32_t *__serial);

int rbug_send_shader_replace(struct rbug_connection *__con,
                             rbug_context_t context,
                             rbug_shader_t shader,
                             uint32_t *tokens,
                             uint32_t tokens_len,
                             uint32_t *__serial);

int rbug_send_shader_list_reply(struct rbug_connection *__con,
                                uint32_t serial,
                                rbug_shader_t *shaders,
                                uint32_t shaders_len,
                                uint32_t *__serial);

int rbug_send_shader_info_reply(struct rbug_connection *__con,
                                uint32_t serial,
                                uint32_t *original,
                                uint32_t original_len,
                                uint32_t *replaced,
                                uint32_t replaced_len,
                                uint8_t disabled,
                                uint32_t *__serial);

struct rbug_proto_shader_list * rbug_demarshal_shader_list(struct rbug_proto_header *header);

struct rbug_proto_shader_info * rbug_demarshal_shader_info(struct rbug_proto_header *header);

struct rbug_proto_shader_disable * rbug_demarshal_shader_disable(struct rbug_proto_header *header);

struct rbug_proto_shader_replace * rbug_demarshal_shader_replace(struct rbug_proto_header *header);

struct rbug_proto_shader_list_reply * rbug_demarshal_shader_list_reply(struct rbug_proto_header *header);

struct rbug_proto_shader_info_reply * rbug_demarshal_shader_info_reply(struct rbug_proto_header *header);

#endif

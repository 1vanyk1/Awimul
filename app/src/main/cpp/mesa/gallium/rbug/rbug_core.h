#ifndef _RBUG_PROTO_CORE_H_
#define _RBUG_PROTO_CORE_H_

#include "rbug_proto.h"

typedef uint64_t rbug_shader_t;
typedef uint64_t rbug_context_t;
typedef uint64_t rbug_texture_t;

struct rbug_proto_noop
{
    struct rbug_header header;
};

struct rbug_proto_ping
{
    struct rbug_header header;
};

struct rbug_proto_error
{
    struct rbug_header header;
    uint32_t error;
};

struct rbug_proto_ping_reply
{
    struct rbug_header header;
    uint32_t serial;
};

struct rbug_proto_error_reply
{
    struct rbug_header header;
    uint32_t serial;
    uint32_t error;
};

int rbug_send_noop(struct rbug_connection *__con,
                   uint32_t *__serial);

int rbug_send_ping(struct rbug_connection *__con,
                   uint32_t *__serial);

int rbug_send_error(struct rbug_connection *__con,
                    uint32_t error,
                    uint32_t *__serial);

int rbug_send_ping_reply(struct rbug_connection *__con,
                         uint32_t serial,
                         uint32_t *__serial);

int rbug_send_error_reply(struct rbug_connection *__con,
                          uint32_t serial,
                          uint32_t error,
                          uint32_t *__serial);

struct rbug_proto_noop * rbug_demarshal_noop(struct rbug_proto_header *header);

struct rbug_proto_ping * rbug_demarshal_ping(struct rbug_proto_header *header);

struct rbug_proto_error * rbug_demarshal_error(struct rbug_proto_header *header);

struct rbug_proto_ping_reply * rbug_demarshal_ping_reply(struct rbug_proto_header *header);

struct rbug_proto_error_reply * rbug_demarshal_error_reply(struct rbug_proto_header *header);

#endif

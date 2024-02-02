#ifndef _RBUG_PROTO_TEXTURE_H_
#define _RBUG_PROTO_TEXTURE_H_

#include "rbug_proto.h"
#include "rbug_core.h"

struct rbug_proto_texture_list
{
    struct rbug_header header;
};

struct rbug_proto_texture_info
{
    struct rbug_header header;
    rbug_texture_t texture;
};

struct rbug_proto_texture_write
{
    struct rbug_header header;
    rbug_texture_t texture;
    uint32_t face;
    uint32_t level;
    uint32_t zslice;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint8_t *data;
    uint32_t data_len;
    uint32_t stride;
};

struct rbug_proto_texture_read
{
    struct rbug_header header;
    rbug_texture_t texture;
    uint32_t face;
    uint32_t level;
    uint32_t zslice;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

struct rbug_proto_texture_list_reply
{
    struct rbug_header header;
    uint32_t serial;
    rbug_texture_t *textures;
    uint32_t textures_len;
};

struct rbug_proto_texture_info_reply
{
    struct rbug_header header;
    uint32_t serial;
    uint32_t target;
    uint32_t format;
    uint32_t *width;
    uint32_t width_len;
    uint32_t *height;
    uint32_t height_len;
    uint32_t *depth;
    uint32_t depth_len;
    uint32_t blockw;
    uint32_t blockh;
    uint32_t blocksize;
    uint32_t last_level;
    uint32_t nr_samples;
    uint32_t tex_usage;
};

struct rbug_proto_texture_read_reply
{
    struct rbug_header header;
    uint32_t serial;
    uint32_t format;
    uint32_t blockw;
    uint32_t blockh;
    uint32_t blocksize;
    uint8_t *data;
    uint32_t data_len;
    uint32_t stride;
};

int rbug_send_texture_list(struct rbug_connection *__con,
                           uint32_t *__serial);

int rbug_send_texture_info(struct rbug_connection *__con,
                           rbug_texture_t texture,
                           uint32_t *__serial);

int rbug_send_texture_write(struct rbug_connection *__con,
                            rbug_texture_t texture,
                            uint32_t face,
                            uint32_t level,
                            uint32_t zslice,
                            uint32_t x,
                            uint32_t y,
                            uint32_t w,
                            uint32_t h,
                            uint8_t *data,
                            uint32_t data_len,
                            uint32_t stride,
                            uint32_t *__serial);

int rbug_send_texture_read(struct rbug_connection *__con,
                           rbug_texture_t texture,
                           uint32_t face,
                           uint32_t level,
                           uint32_t zslice,
                           uint32_t x,
                           uint32_t y,
                           uint32_t w,
                           uint32_t h,
                           uint32_t *__serial);

int rbug_send_texture_list_reply(struct rbug_connection *__con,
                                 uint32_t serial,
                                 rbug_texture_t *textures,
                                 uint32_t textures_len,
                                 uint32_t *__serial);

int rbug_send_texture_info_reply(struct rbug_connection *__con,
                                 uint32_t serial,
                                 uint32_t target,
                                 uint32_t format,
                                 uint32_t *width,
                                 uint32_t width_len,
                                 uint16_t *height,
                                 uint32_t height_len,
                                 uint16_t *depth,
                                 uint32_t depth_len,
                                 uint32_t blockw,
                                 uint32_t blockh,
                                 uint32_t blocksize,
                                 uint32_t last_level,
                                 uint32_t nr_samples,
                                 uint32_t tex_usage,
                                 uint32_t *__serial);

int rbug_send_texture_read_reply(struct rbug_connection *__con,
                                 uint32_t serial,
                                 uint32_t format,
                                 uint32_t blockw,
                                 uint32_t blockh,
                                 uint32_t blocksize,
                                 uint8_t *data,
                                 uint32_t data_len,
                                 uint32_t stride,
                                 uint32_t *__serial);

struct rbug_proto_texture_list * rbug_demarshal_texture_list(struct rbug_proto_header *header);

struct rbug_proto_texture_info * rbug_demarshal_texture_info(struct rbug_proto_header *header);

struct rbug_proto_texture_write * rbug_demarshal_texture_write(struct rbug_proto_header *header);

struct rbug_proto_texture_read * rbug_demarshal_texture_read(struct rbug_proto_header *header);

struct rbug_proto_texture_list_reply * rbug_demarshal_texture_list_reply(struct rbug_proto_header *header);

struct rbug_proto_texture_info_reply * rbug_demarshal_texture_info_reply(struct rbug_proto_header *header);

struct rbug_proto_texture_read_reply * rbug_demarshal_texture_read_reply(struct rbug_proto_header *header);

#endif

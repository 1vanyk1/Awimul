#include "rbug_internal.h"
#include "rbug_core.h"

int rbug_send_noop(struct rbug_connection *__con,
                   uint32_t *__serial)
{
    uint32_t __len = 0;
    uint32_t __pos = 0;
    uint8_t *__data = NULL;
    int __ret = 0;

    LEN(8); /* header */

    /* align */
    PAD(__len, 8);

    __data = (uint8_t*)MALLOC(__len);
    if (!__data)
        return -ENOMEM;

    WRITE(4, int32_t, ((int32_t)RBUG_OP_NOOP));
    WRITE(4, uint32_t, ((uint32_t)(__len / 4)));

    /* final pad */
    PAD(__pos, 8);

    if (__pos != __len) {
        __ret = -EINVAL;
    } else {
        rbug_connection_send_start(__con, RBUG_OP_NOOP, __len);
        rbug_connection_write(__con, __data, __len);
        __ret = rbug_connection_send_finish(__con, __serial);
    }

    FREE(__data);
    return __ret;
}

int rbug_send_ping(struct rbug_connection *__con,
                   uint32_t *__serial)
{
    uint32_t __len = 0;
    uint32_t __pos = 0;
    uint8_t *__data = NULL;
    int __ret = 0;

    LEN(8); /* header */

    /* align */
    PAD(__len, 8);

    __data = (uint8_t*)MALLOC(__len);
    if (!__data)
        return -ENOMEM;

    WRITE(4, int32_t, ((int32_t)RBUG_OP_PING));
    WRITE(4, uint32_t, ((uint32_t)(__len / 4)));

    /* final pad */
    PAD(__pos, 8);

    if (__pos != __len) {
        __ret = -EINVAL;
    } else {
        rbug_connection_send_start(__con, RBUG_OP_PING, __len);
        rbug_connection_write(__con, __data, __len);
        __ret = rbug_connection_send_finish(__con, __serial);
    }

    FREE(__data);
    return __ret;
}

int rbug_send_error(struct rbug_connection *__con,
                    uint32_t error,
                    uint32_t *__serial)
{
    uint32_t __len = 0;
    uint32_t __pos = 0;
    uint8_t *__data = NULL;
    int __ret = 0;

    LEN(8); /* header */
    LEN(4); /* error */

    /* align */
    PAD(__len, 8);

    __data = (uint8_t*)MALLOC(__len);
    if (!__data)
        return -ENOMEM;

    WRITE(4, int32_t, ((int32_t)RBUG_OP_ERROR));
    WRITE(4, uint32_t, ((uint32_t)(__len / 4)));
    WRITE(4, uint32_t, error); /* error */

    /* final pad */
    PAD(__pos, 8);

    if (__pos != __len) {
        __ret = -EINVAL;
    } else {
        rbug_connection_send_start(__con, RBUG_OP_ERROR, __len);
        rbug_connection_write(__con, __data, __len);
        __ret = rbug_connection_send_finish(__con, __serial);
    }

    FREE(__data);
    return __ret;
}

int rbug_send_ping_reply(struct rbug_connection *__con,
                         uint32_t serial,
                         uint32_t *__serial)
{
    uint32_t __len = 0;
    uint32_t __pos = 0;
    uint8_t *__data = NULL;
    int __ret = 0;

    LEN(8); /* header */
    LEN(4); /* serial */

    /* align */
    PAD(__len, 8);

    __data = (uint8_t*)MALLOC(__len);
    if (!__data)
        return -ENOMEM;

    WRITE(4, int32_t, ((int32_t)RBUG_OP_PING_REPLY));
    WRITE(4, uint32_t, ((uint32_t)(__len / 4)));
    WRITE(4, uint32_t, serial); /* serial */

    /* final pad */
    PAD(__pos, 8);

    if (__pos != __len) {
        __ret = -EINVAL;
    } else {
        rbug_connection_send_start(__con, RBUG_OP_PING_REPLY, __len);
        rbug_connection_write(__con, __data, __len);
        __ret = rbug_connection_send_finish(__con, __serial);
    }

    FREE(__data);
    return __ret;
}

int rbug_send_error_reply(struct rbug_connection *__con,
                          uint32_t serial,
                          uint32_t error,
                          uint32_t *__serial)
{
    uint32_t __len = 0;
    uint32_t __pos = 0;
    uint8_t *__data = NULL;
    int __ret = 0;

    LEN(8); /* header */
    LEN(4); /* serial */
    LEN(4); /* error */

    /* align */
    PAD(__len, 8);

    __data = (uint8_t*)MALLOC(__len);
    if (!__data)
        return -ENOMEM;

    WRITE(4, int32_t, ((int32_t)RBUG_OP_ERROR_REPLY));
    WRITE(4, uint32_t, ((uint32_t)(__len / 4)));
    WRITE(4, uint32_t, serial); /* serial */
    WRITE(4, uint32_t, error); /* error */

    /* final pad */
    PAD(__pos, 8);

    if (__pos != __len) {
        __ret = -EINVAL;
    } else {
        rbug_connection_send_start(__con, RBUG_OP_ERROR_REPLY, __len);
        rbug_connection_write(__con, __data, __len);
        __ret = rbug_connection_send_finish(__con, __serial);
    }

    FREE(__data);
    return __ret;
}

struct rbug_proto_noop * rbug_demarshal_noop(struct rbug_proto_header *header)
{
    struct rbug_proto_noop *ret;

    if (!header)
        return NULL;
    if (header->opcode != (int32_t)RBUG_OP_NOOP)
        return NULL;

    ret = MALLOC(sizeof(*ret));
    if (!ret)
        return NULL;

    ret->header.__message = header;
    ret->header.opcode = header->opcode;

    return ret;
}

struct rbug_proto_ping * rbug_demarshal_ping(struct rbug_proto_header *header)
{
    struct rbug_proto_ping *ret;

    if (!header)
        return NULL;
    if (header->opcode != (int32_t)RBUG_OP_PING)
        return NULL;

    ret = MALLOC(sizeof(*ret));
    if (!ret)
        return NULL;

    ret->header.__message = header;
    ret->header.opcode = header->opcode;

    return ret;
}

struct rbug_proto_error * rbug_demarshal_error(struct rbug_proto_header *header)
{
    uint32_t len = 0;
    uint32_t pos = 0;
    uint8_t *data =  NULL;
    struct rbug_proto_error *ret;

    if (!header)
        return NULL;
    if (header->opcode != (int32_t)RBUG_OP_ERROR)
        return NULL;

    pos = 0;
    len = header->length * 4;
    data = (uint8_t*)&header[1];
    ret = MALLOC(sizeof(*ret));
    if (!ret)
        return NULL;

    ret->header.__message = header;
    ret->header.opcode = header->opcode;

    READ(4, uint32_t, error); /* error */

    return ret;
}

struct rbug_proto_ping_reply * rbug_demarshal_ping_reply(struct rbug_proto_header *header)
{
    uint32_t len = 0;
    uint32_t pos = 0;
    uint8_t *data =  NULL;
    struct rbug_proto_ping_reply *ret;

    if (!header)
        return NULL;
    if (header->opcode != (int32_t)RBUG_OP_PING_REPLY)
        return NULL;

    pos = 0;
    len = header->length * 4;
    data = (uint8_t*)&header[1];
    ret = MALLOC(sizeof(*ret));
    if (!ret)
        return NULL;

    ret->header.__message = header;
    ret->header.opcode = header->opcode;

    READ(4, uint32_t, serial); /* serial */

    return ret;
}

struct rbug_proto_error_reply * rbug_demarshal_error_reply(struct rbug_proto_header *header)
{
    uint32_t len = 0;
    uint32_t pos = 0;
    uint8_t *data =  NULL;
    struct rbug_proto_error_reply *ret;

    if (!header)
        return NULL;
    if (header->opcode != (int32_t)RBUG_OP_ERROR_REPLY)
        return NULL;

    pos = 0;
    len = header->length * 4;
    data = (uint8_t*)&header[1];
    ret = MALLOC(sizeof(*ret));
    if (!ret)
        return NULL;

    ret->header.__message = header;
    ret->header.opcode = header->opcode;

    READ(4, uint32_t, serial); /* serial */
    READ(4, uint32_t, error); /* error */

    return ret;
}

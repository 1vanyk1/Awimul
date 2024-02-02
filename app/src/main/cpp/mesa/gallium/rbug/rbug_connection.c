#include "rbug.h"
#include "rbug_internal.h"

#include "../../util/u_network.h"

struct rbug_connection
{
    int socket;
    uint32_t send_serial;
    uint32_t recv_serial;
    enum rbug_opcode opcode;
};

/**
 * Create a rbug connection from a socket created with u_socket.
 *
 * Result:
 *    A new allocated connection using socket as communication path
 */
struct rbug_connection *
rbug_from_socket(int socket)
{
    struct rbug_connection *c = CALLOC_STRUCT(rbug_connection);
    c->socket = socket;
    return c;
}

/**
 * Free a connection, also closes socket.
 */
void
rbug_disconnect(struct rbug_connection *c)
{
    u_socket_close(c->socket);
    FREE(c);
}

/**
 * Waits for a message to be fully received.
 * Also returns the serial for the message, serial is not touched for replys.
 *
 * Result:
 *    demarshaled message on success, NULL on connection error
 */
struct rbug_header *
rbug_get_message(struct rbug_connection *c, uint32_t *serial)
{
    struct rbug_proto_header header;
    struct rbug_header *out;
    struct rbug_proto_header *data;
    size_t length = 0;
    size_t read = 0;
    int ret;


    ret = u_socket_peek(c->socket, &header, sizeof(header));
    if (ret <= 0) {
        return NULL;
    }

    length = (size_t)header.length * 4;
    data = MALLOC(length);
    if (!data) {
        return NULL;
    }
    data->opcode = 0;

    do {
        uint8_t *ptr = ((uint8_t*)data) + read;
        ret = u_socket_recv(c->socket, ptr, length - read);

        if (ret <= 0) {
            FREE(data);
            return NULL;
        }

        read += ret;
    } while(read < length);

    out = rbug_demarshal(data);
    if (!out)
        FREE(data);
    else if (serial)
        *serial = c->recv_serial++;
    else
        c->recv_serial++;

    return out;
}

/**
 * Frees a message and associated data.
 */
void
rbug_free_header(struct rbug_header *header)
{
    if (!header)
        return;

    FREE(header->__message);
    FREE(header);
}

/**
 * Internal function used by rbug_send_* functions.
 *
 * Start sending a message.
 */
int
rbug_connection_send_start(struct rbug_connection *c, enum rbug_opcode opcode, uint32_t length)
{
    c->opcode = opcode;
    return 0;
}

/**
 * Internal function used by rbug_send_* functions.
 *
 * Write data to the socket.
 */
int
rbug_connection_write(struct rbug_connection *c, void *to, uint32_t size)
{
    int ret = u_socket_send(c->socket, to, size);
    return ret;
}

/**
 * Internal function used by rbug_send_* functions.
 *
 * Finish writing data to the socket.
 * Ups the send_serial and sets the serial argument if supplied.
 */
int rbug_connection_send_finish(struct rbug_connection *c, uint32_t *serial)
{
    if (c->opcode < 0)
        return 0;
    else if (serial)
        *serial = c->send_serial++;
    else
        c->send_serial++;

    return 0;
}

#ifndef _RBUG_CONNECTION_H_
#define _RBUG_CONNECTION_H_

#include "rbug_proto.h"

struct rbug_connection * rbug_from_socket(int socket);

void rbug_disconnect(struct rbug_connection *c);

struct rbug_header * rbug_get_message(struct rbug_connection *c, uint32_t *serial);

void rbug_free_header(struct rbug_header *header);

struct rbug_header * rbug_demarshal(struct rbug_proto_header *header);

#endif

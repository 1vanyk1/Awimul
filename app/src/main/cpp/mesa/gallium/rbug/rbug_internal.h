#ifndef _RBUG_INTERNAL_H_
#define _RBUG_INTERNAL_H_

#include "rbug_proto.h"

#include "../../util/u_memory.h"
#include "../../util/u_debug.h"
#include <errno.h>

int rbug_connection_send_start(struct rbug_connection *con, enum rbug_opcode opcode, uint32_t length);
int rbug_connection_write(struct rbug_connection *con, void *data, uint32_t size);
int rbug_connection_send_finish(struct rbug_connection *con, uint32_t *c);

/**
 * Only works with multiples of 2
 */
#define PAD(from, to)                       \
do {                                        \
	from = (from + to - 1) & ~(to - 1); \
} while(0)

#define LEN(size)         \
do {                      \
	PAD(__len, size); \
	__len += size;    \
} while(0)

#define LEN_ARRAY(size, name)       \
do {                                \
	LEN(4);                     \
	PAD(__len, size);           \
	__len += size * name##_len; \
} while(0)

#define WRITE(size, type, name)             \
do {                                        \
	PAD(__pos, size);                   \
	*((type *)(&__data[__pos])) = name; \
	__pos += size;                      \
} while(0)

#define WRITE_ARRAY(size, type, name)                    \
do {                                                     \
	WRITE(4, uint32_t, name##_len);                  \
	PAD(__pos, size);                                \
	memcpy(&__data[__pos], name, size * name##_len); \
	__pos += size * name##_len;                      \
} while(0)

#define READ(size, type, name)                      \
do {                                                \
	PAD(pos, size);                             \
	pos += size;                                \
	if (pos > len)                              \
		break;                              \
	ret->name = *((type *)(&data[pos - size])); \
} while(0)

#define READ_ARRAY(size, type, name)                             \
do {                                                             \
	READ(4, uint32_t, name##_len);                           \
	if (pos > len)                                           \
		break;                                           \
	PAD(pos, size);                                          \
	pos += size * ret->name##_len;                           \
	if (pos > len)                                           \
		break;                                           \
	ret->name = (type *)&data[pos - size * ret->name##_len]; \
} while(0)

#endif

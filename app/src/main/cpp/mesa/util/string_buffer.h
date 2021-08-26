#ifndef _STRING_BUFFER_H
#define _STRING_BUFFER_H

#include "ralloc.h"
#include "u_string.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _mesa_string_buffer {
    char *buf;
    uint32_t length;
    uint32_t capacity;
};

struct _mesa_string_buffer *
_mesa_string_buffer_create(void *mem_ctx, uint32_t initial_capacity);

static inline void
_mesa_string_buffer_destroy(struct _mesa_string_buffer *str)
{
    ralloc_free(str);
}

bool
_mesa_string_buffer_append_all(struct _mesa_string_buffer *str,
                               uint32_t num_args, ...);
bool
_mesa_string_buffer_append_len(struct _mesa_string_buffer *str,
                               const char *c, uint32_t len);

static inline bool
_mesa_string_buffer_append_char(struct _mesa_string_buffer *str, char c)
{
    return _mesa_string_buffer_append_len(str, &c, 1);
}

static inline bool
_mesa_string_buffer_append(struct _mesa_string_buffer *str, const char *c)
{
    return _mesa_string_buffer_append_len(str, c, strlen(c));
}

static inline void
_mesa_string_buffer_clear(struct _mesa_string_buffer *str)
{
    str->length = 0;
    str->buf[str->length] = '\0';
}

static inline void
_mesa_string_buffer_crimp_to_fit(struct _mesa_string_buffer *str)
{
    char *crimped =
            (char *) reralloc_array_size(str, str->buf, sizeof(char),
                                         str->capacity);
    if (!crimped)
        return;

    str->capacity = str->length + 1;
    str->buf = crimped;
}

bool
_mesa_string_buffer_vprintf(struct _mesa_string_buffer *str,
                            const char *format, va_list args);

bool
_mesa_string_buffer_printf(struct _mesa_string_buffer *str,
                           const char *format, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _STRING_BUFFER_H */

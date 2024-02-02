#ifndef U_LOG_H
#define U_LOG_H

#include <stdio.h>

#include "u_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

struct u_log_page;
struct u_log_auto_logger;

struct u_log_chunk_type {
    void (*destroy)(void *data);
    void (*print)(void *data, FILE *stream);
};

struct u_log_context {
    struct u_log_page *cur;
    struct u_log_auto_logger *auto_loggers;
    unsigned num_auto_loggers;
};

typedef void (u_auto_log_fn)(void *data, struct u_log_context *ctx);

void
u_log_context_init(struct u_log_context *ctx);

void
u_log_context_destroy(struct u_log_context *ctx);

void
u_log_add_auto_logger(struct u_log_context *ctx, u_auto_log_fn *callback,
                      void *data);

void
u_log_flush(struct u_log_context *ctx);

void
u_log_printf(struct u_log_context *ctx, const char *fmt, ...) _util_printf_format(2,3);

void
u_log_chunk(struct u_log_context *ctx, const struct u_log_chunk_type *type,
            void *data);

void
u_log_new_page_print(struct u_log_context *ctx, FILE *stream);

struct u_log_page *
u_log_new_page(struct u_log_context *ctx);

void
u_log_page_destroy(struct u_log_page *page);

void
u_log_page_print(struct u_log_page *page, FILE *stream);

#ifdef __cplusplus
}
#endif

#endif /* U_LOG_H */

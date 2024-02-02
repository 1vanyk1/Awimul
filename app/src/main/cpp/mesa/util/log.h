#ifndef MESA_LOG_H
#define MESA_LOG_H

#include <stdarg.h>

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MESA_LOG_TAG
#define MESA_LOG_TAG "MESA"
#endif

enum mesa_log_level {
    MESA_LOG_ERROR,
    MESA_LOG_WARN,
    MESA_LOG_INFO,
    MESA_LOG_DEBUG,
};

void PRINTFLIKE(3, 4)
mesa_log(enum mesa_log_level, const char *tag, const char *format, ...);

void
mesa_log_v(enum mesa_log_level, const char *tag, const char *format,
           va_list va);

#define mesa_loge(fmt, ...) mesa_log(MESA_LOG_ERROR, (MESA_LOG_TAG), (fmt), ##__VA_ARGS__)
#define mesa_logw(fmt, ...) mesa_log(MESA_LOG_WARN, (MESA_LOG_TAG), (fmt), ##__VA_ARGS__)
#define mesa_logi(fmt, ...) mesa_log(MESA_LOG_INFO, (MESA_LOG_TAG), (fmt), ##__VA_ARGS__)
#ifdef DEBUG
#define mesa_logd(fmt, ...) mesa_log(MESA_LOG_DEBUG, (MESA_LOG_TAG), (fmt), ##__VA_ARGS__)
#else
#define mesa_logd(fmt, ...) __mesa_log_use_args((fmt), ##__VA_ARGS__)
#endif

#define mesa_loge_v(fmt, va) mesa_log_v(MESA_LOG_ERROR, (MESA_LOG_TAG), (fmt), (va))
#define mesa_logw_v(fmt, va) mesa_log_v(MESA_LOG_WARN, (MESA_LOG_TAG), (fmt), (va))
#define mesa_logi_v(fmt, va) mesa_log_v(MESA_LOG_INFO, (MESA_LOG_TAG), (fmt), (va))
#ifdef DEBUG
#define mesa_logd_v(fmt, va) mesa_log_v(MESA_LOG_DEBUG, (MESA_LOG_TAG), (fmt), (va))
#else
#define mesa_logd_v(fmt, va) __mesa_log_use_args((fmt), (va))
#endif

struct log_stream {
    char *msg;
    const char *tag;
    size_t pos;
    enum mesa_log_level level;
};

struct log_stream *_mesa_log_stream_create(enum mesa_log_level level, char *tag);
#define mesa_log_streame() _mesa_log_stream_create(MESA_LOG_ERROR, (MESA_LOG_TAG))
#define mesa_log_streamw() _mesa_log_stream_create(MESA_LOG_WARN, (MESA_LOG_TAG))
#define mesa_log_streami() _mesa_log_stream_create(MESA_LOG_INFO, (MESA_LOG_TAG))
void mesa_log_stream_destroy(struct log_stream *stream);
void mesa_log_stream_printf(struct log_stream *stream, const char *format, ...);

void _mesa_log_multiline(enum mesa_log_level level, const char *tag, const char *lines);
#define mesa_log_multiline(level, lines) _mesa_log_multiline(level, (MESA_LOG_TAG), lines)

#ifndef DEBUG
/* Suppres -Wunused */
static inline void PRINTFLIKE(1, 2)
__mesa_log_use_args(UNUSED const char *format, ...) { }
#endif

#ifdef __cplusplus
}
#endif

#endif /* MESA_LOG_H */

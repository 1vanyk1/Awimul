#ifndef VIRGL_UTIL_H
#define VIRGL_UTIL_H

#define HAVE_CONFIG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "virglrenderer.h"

#define TRACE_WITH_PERFETTO 1
#define TRACE_WITH_STDERR 2
#define TRACE_WITH_PERCETTO 3

#define BIT(n)                   (UINT32_C(1) << (n))

static inline bool has_bit(uint32_t mask, uint32_t bit)
{
    return !!(mask & bit);
}

static inline bool has_bits(uint32_t mask, uint32_t bits)
{
    return !!((mask & bits) == bits);
}

static inline bool is_only_bit(uint32_t mask, uint32_t bit)
{
    return (mask == bit);
}

unsigned hash_func_u32(void *key);

int compare_func(void *key1, void *key2);

bool has_eventfd(void);
int create_eventfd(unsigned int initval);
int write_eventfd(int fd, uint64_t val);
void flush_eventfd(int fd);

virgl_debug_callback_type virgl_log_set_logger(virgl_debug_callback_type logger);
void virgl_logv(const char *fmt, va_list va);

static inline void virgl_log(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    virgl_logv(fmt, va);
    va_end(va);
}

#ifdef ENABLE_TRACING
void trace_init(void);

#define TRACE_INIT() trace_init()
#define TRACE_FUNC() TRACE_SCOPE(__func__)

#if ENABLE_TRACING == TRACE_WITH_PERCETTO

#include <percetto.h>

#define VIRGL_PERCETTO_CATEGORIES(C, G) \
  C(virgl, "virglrenderer") \
  C(virgls, "virglrenderer detailed events", "slow")

PERCETTO_CATEGORY_DECLARE(VIRGL_PERCETTO_CATEGORIES)

#define TRACE_SCOPE(SCOPE) TRACE_EVENT(virgl, SCOPE)
/* Trace high frequency events (tracing may impact performance). */
#define TRACE_SCOPE_SLOW(SCOPE) TRACE_EVENT(virgls, SCOPE)

#define TRACE_SCOPE_BEGIN(SCOPE) TRACE_EVENT_BEGIN(virgl, SCOPE)
#define TRACE_SCOPE_END(SCOPE) do { TRACE_EVENT_END(virgl); (void)SCOPE; } while (0)

#else

const char *trace_begin(const char *scope);
void trace_end(const char **scope);

#define TRACE_SCOPE(SCOPE) \
   const char *trace_dummy __attribute__((cleanup (trace_end), unused)) = \
   trace_begin(SCOPE)

#define TRACE_SCOPE_SLOW(SCOPE) TRACE_SCOPE(SCOPE)

#define TRACE_SCOPE_BEGIN(SCOPE) trace_begin(SCOPE);
#define TRACE_SCOPE_END(SCOPE)  trace_end(&SCOPE);

#endif /* ENABLE_TRACING == TRACE_WITH_PERCETTO */

#else
#define TRACE_INIT()
#define TRACE_FUNC()
#define TRACE_SCOPE(SCOPE)
#define TRACE_SCOPE_SLOW(SCOPE)
#define TRACE_SCOPE_BEGIN(SCOPE, VAR)
#define TRACE_SCOPE_END(VAR)
#endif /* ENABLE_TRACING */

#endif /* VIRGL_UTIL_H */

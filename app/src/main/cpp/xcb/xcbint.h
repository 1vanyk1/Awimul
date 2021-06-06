#ifndef __XCBINT_H
#define __XCBINT_H

#include "bigreq.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef GCC_HAS_VISIBILITY
#pragma GCC visibility push(hidden)
#endif

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with older compilers. */
#endif

#if __has_attribute(fallthrough)
# define XCB_ALLOW_FALLTHRU __attribute__ ((fallthrough));
#else
# define XCB_ALLOW_FALLTHRU /* FALLTHRU */
#endif

enum workarounds {
    WORKAROUND_NONE,
    WORKAROUND_GLX_GET_FB_CONFIGS_BUG,
    WORKAROUND_EXTERNAL_SOCKET_OWNER
};

enum lazy_reply_tag
{
    LAZY_NONE = 0,
    LAZY_COOKIE,
    LAZY_FORCED
};

#define XCB_PAD(i) (-(i) & 3)

#define XCB_SEQUENCE_COMPARE(a,op,b)    ((int64_t) ((a) - (b)) op 0)

#ifndef offsetof
#define offsetof(type,member) ((size_t) &((type *)0)->member)
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#define container_of(pointer,type,member) ((type *)(((char *)(pointer)) - offsetof(type, member)))

/* xcb_list.c */

typedef void (*xcb_list_free_func_t)(void *);

typedef struct _xcb_map _xcb_map;

_xcb_map *_xcb_map_new(void);
void _xcb_map_delete(_xcb_map *q, xcb_list_free_func_t do_free);
int _xcb_map_put(_xcb_map *q, unsigned int key, void *data);
void *_xcb_map_remove(_xcb_map *q, unsigned int key);


/* xcb_out.c */

#if HAVE_SENDMSG
#define XCB_MAX_PASS_FD 16

typedef struct _xcb_fd {
    int fd[XCB_MAX_PASS_FD];
    int nfd;
    int ifd;
} _xcb_fd;
#endif

typedef struct _xcb_out {
    pthread_cond_t cond;
    int writing;

    pthread_cond_t socket_cond;
    void (*return_socket)(void *closure);
    void *socket_closure;
    int socket_moving;

    char queue[4096];
    int queue_len;

    uint64_t request;
    uint64_t request_written;
    uint64_t total_written;

    pthread_mutex_t reqlenlock;
    enum lazy_reply_tag maximum_request_length_tag;
    union {
        xcb_big_requests_enable_cookie_t cookie;
        uint32_t value;
    } maximum_request_length;
#if HAVE_SENDMSG
    _xcb_fd out_fd;
#endif
} _xcb_out;

int _xcb_out_init(_xcb_out *out);
void _xcb_out_destroy(_xcb_out *out);

int _xcb_out_send(xcb_connection_t *c, struct iovec *vector, int count);
void _xcb_out_send_sync(xcb_connection_t *c);
int _xcb_out_flush_to(xcb_connection_t *c, uint64_t request);


/* xcb_in.c */

typedef struct _xcb_in {
    pthread_cond_t event_cond;
    int reading;

    char queue[4096];
    int queue_len;

    uint64_t request_expected;
    uint64_t request_read;
    uint64_t request_completed;
    uint64_t total_read;
    struct reply_list *current_reply;
    struct reply_list **current_reply_tail;

    _xcb_map *replies;
    struct event_list *events;
    struct event_list **events_tail;
    struct reader_list *readers;
    struct special_list *special_waiters;

    struct pending_reply *pending_replies;
    struct pending_reply **pending_replies_tail;
#if HAVE_SENDMSG
    _xcb_fd in_fd;
#endif
    struct xcb_special_event *special_events;
} _xcb_in;

int _xcb_in_init(_xcb_in *in);
void _xcb_in_destroy(_xcb_in *in);

void _xcb_in_wake_up_next_reader(xcb_connection_t *c);

int _xcb_in_expect_reply(xcb_connection_t *c, uint64_t request, enum workarounds workaround, int flags);
void _xcb_in_replies_done(xcb_connection_t *c);

int _xcb_in_read(xcb_connection_t *c);
int _xcb_in_read_block(xcb_connection_t *c, void *buf, int nread);


/* xcb_xid.c */

typedef struct _xcb_xid {
    pthread_mutex_t lock;
    uint32_t last;
    uint32_t base;
    uint32_t max;
    uint32_t inc;
} _xcb_xid;

int _xcb_xid_init(xcb_connection_t *c);
void _xcb_xid_destroy(xcb_connection_t *c);


/* xcb_ext.c */

typedef struct _xcb_ext {
    pthread_mutex_t lock;
    struct lazyreply *extensions;
    int extensions_size;
} _xcb_ext;

int _xcb_ext_init(xcb_connection_t *c);
void _xcb_ext_destroy(xcb_connection_t *c);


/* xcb_conn.c */

struct xcb_connection_t {
    /* This must be the first field; see _xcb_conn_ret_error(). */
    int has_error;

    /* constant data */
    xcb_setup_t *setup;
    int fd;

    /* I/O data */
    pthread_mutex_t iolock;
    _xcb_in in;
    _xcb_out out;

    /* misc data */
    _xcb_ext ext;
    _xcb_xid xid;
};

void _xcb_conn_shutdown(xcb_connection_t *c, int err);

xcb_connection_t *_xcb_conn_ret_error(int err);

int _xcb_conn_wait(xcb_connection_t *c, pthread_cond_t *cond, struct iovec **vector, int *count);


/* xcb_auth.c */

int _xcb_get_auth_info(int fd, xcb_auth_info_t *info, int display);

#ifdef GCC_HAS_VISIBILITY
#pragma GCC visibility pop
#endif

#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "xcb.h"
#include "xcbext.h"
#include "xcbint.h"

typedef struct lazyreply {
    enum lazy_reply_tag tag;
    union {
        xcb_query_extension_cookie_t cookie;
        xcb_query_extension_reply_t *reply;
    } value;
} lazyreply;

static lazyreply *get_index(xcb_connection_t *c, int idx)
{
    if(idx > c->ext.extensions_size)
    {
        int new_size = idx << 1;
        lazyreply *new_extensions = realloc(c->ext.extensions, sizeof(lazyreply) * new_size);
        if(!new_extensions)
            return 0;
        memset(new_extensions + c->ext.extensions_size, 0, sizeof(lazyreply) * (new_size - c->ext.extensions_size));
        c->ext.extensions = new_extensions;
        c->ext.extensions_size = new_size;
    }
    return c->ext.extensions + idx - 1;
}

static lazyreply *get_lazyreply(xcb_connection_t *c, xcb_extension_t *ext)
{
    static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
    static int next_global_id;

    lazyreply *data;

    pthread_mutex_lock(&global_lock);
    if(!ext->global_id)
        ext->global_id = ++next_global_id;
    pthread_mutex_unlock(&global_lock);

    data = get_index(c, ext->global_id);
    if(data && data->tag == LAZY_NONE)
    {
        /* cache miss: query the server */
        data->tag = LAZY_COOKIE;
        data->value.cookie = xcb_query_extension(c, strlen(ext->name), ext->name);
    }
    return data;
}

/* Public interface */

/* Do not free the returned xcb_query_extension_reply_t - on return, it's aliased
 * from the cache. */
const xcb_query_extension_reply_t *xcb_get_extension_data(xcb_connection_t *c, xcb_extension_t *ext)
{
    lazyreply *data;
    if(c->has_error)
        return 0;

    pthread_mutex_lock(&c->ext.lock);
    data = get_lazyreply(c, ext);
    if(data && data->tag == LAZY_COOKIE)
    {
        data->tag = LAZY_FORCED;
        data->value.reply = xcb_query_extension_reply(c, data->value.cookie, 0);
    }
    pthread_mutex_unlock(&c->ext.lock);

    return data ? data->value.reply : 0;
}

void xcb_prefetch_extension_data(xcb_connection_t *c, xcb_extension_t *ext)
{
    if(c->has_error)
        return;
    pthread_mutex_lock(&c->ext.lock);
    get_lazyreply(c, ext);
    pthread_mutex_unlock(&c->ext.lock);
}

/* Private interface */

int _xcb_ext_init(xcb_connection_t *c)
{
    if(pthread_mutex_init(&c->ext.lock, 0))
        return 0;
    return 1;
}

void _xcb_ext_destroy(xcb_connection_t *c)
{
    pthread_mutex_destroy(&c->ext.lock);
    while(c->ext.extensions_size-- > 0)
        if(c->ext.extensions[c->ext.extensions_size].tag == LAZY_FORCED)
            free(c->ext.extensions[c->ext.extensions_size].value.reply);
    free(c->ext.extensions);
}

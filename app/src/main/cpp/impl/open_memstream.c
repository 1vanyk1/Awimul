#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "open_memstream.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#ifdef DEBUG
#include <stdint.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* XXX: There is no FPOS_MAX.  This assumes fpos_t is an off_t. */
#define	FPOS_MAX	LLONG_MAX

struct memstream;

static int
memstream_grow(struct memstream *ms, fpos_t newoff)
{
    char *buf;
    ssize_t newsize;

    if (newoff < 0 || newoff >= SSIZE_MAX)
        newsize = SSIZE_MAX - 1;
    else
        newsize = newoff;
    if (newsize > ms->len) {
        buf = realloc(*ms->bufp, newsize + 1);
        if (buf != NULL) {
#ifdef DEBUG
            fprintf(stderr, "MS: %p growing from %zd to %zd\n",
			    ms, ms->len, newsize);
#endif
            memset(buf + ms->len + 1, 0, newsize - ms->len);
            *ms->bufp = buf;
            ms->len = newsize;
            return (1);
        }
        return (0);
    }
    return (1);
}

static void
memstream_update(struct memstream *ms)
{

    assert(ms->len >= 0 && ms->offset >= 0);
    *ms->sizep = ms->len < ms->offset ? ms->len : ms->offset;
}

static int
memstream_write(void *cookie, const char *buf, int len)
{
    struct memstream *ms;
    ssize_t tocopy;

    ms = cookie;
    if (!memstream_grow(ms, ms->offset + len))
        return (-1);
    tocopy = ms->len - ms->offset;
    if (len < tocopy)
        tocopy = len;
    memcpy(*ms->bufp + ms->offset, buf, tocopy);
    ms->offset += tocopy;
    memstream_update(ms);
#ifdef DEBUG
    fprintf(stderr, "MS: write(%p, %d) = %zd\n", ms, len, tocopy);
#endif
    return (tocopy);
}

static fpos_t
memstream_seek(void *cookie, fpos_t pos, int whence)
{
    struct memstream *ms;
#ifdef DEBUG
    fpos_t old;
#endif

    ms = cookie;
#ifdef DEBUG
    old = ms->offset;
#endif
    switch (whence) {
        case SEEK_SET:
            /* _fseeko() checks for negative offsets. */
            assert(pos >= 0);
            ms->offset = pos;
            break;
        case SEEK_CUR:
            /* This is only called by _ftello(). */
            assert(pos == 0);
            break;
        case SEEK_END:
            if (pos < 0) {
                if (pos + ms->len < 0) {
#ifdef DEBUG
                    fprintf(stderr,
				    "MS: bad SEEK_END: pos %jd, len %zd\n",
				    (intmax_t)pos, ms->len);
#endif
                    errno = EINVAL;
                    return (-1);
                }
            } else {
                if (FPOS_MAX - ms->len < pos) {
#ifdef DEBUG
                    fprintf(stderr,
				    "MS: bad SEEK_END: pos %jd, len %zd\n",
				    (intmax_t)pos, ms->len);
#endif
                    errno = EOVERFLOW;
                    return (-1);
                }
            }
            ms->offset = ms->len + pos;
            break;
    }
    memstream_update(ms);
#ifdef DEBUG
    fprintf(stderr, "MS: seek(%p, %jd, %d) %jd -> %jd\n", ms, (intmax_t)pos,
	    whence, (intmax_t)old, (intmax_t)ms->offset);
#endif
    return (ms->offset);
}

static int
memstream_close(void *cookie)
{

    free(cookie);
    return (0);
}

FILE *
open_memstream(char **bufp, size_t *sizep)
{
    struct memstream *ms;
    int save_errno;
    FILE *fp;

    if (bufp == NULL || sizep == NULL) {
        errno = EINVAL;
        return (NULL);
    }
    *bufp = calloc(1, 1);
    if (*bufp == NULL)
        return (NULL);
    ms = malloc(sizeof(*ms));
    if (ms == NULL) {
        save_errno = errno;
        free(*bufp);
        *bufp = NULL;
        errno = save_errno;
        return (NULL);
    }
    ms->bufp = bufp;
    ms->sizep = sizep;
    ms->len = 0;
    ms->offset = 0;
    memstream_update(ms);
    fp = funopen(ms, NULL, memstream_write, memstream_seek,
                 memstream_close);
    if (fp == NULL) {
        save_errno = errno;
        free(ms);
        free(*bufp);
        *bufp = NULL;
        errno = save_errno;
        return (NULL);
    }
    fwide(fp, -1);
    return (fp);
}

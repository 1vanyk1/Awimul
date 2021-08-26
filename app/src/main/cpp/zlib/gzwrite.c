#include "gzguts.h"

/* Local functions */
static int gz_init(gz_state *);
static int gz_comp(gz_state *, int);
static int gz_zero(gz_state *, off_t);

/* Initialize state for writing a gzip file.  Mark initialization by setting
   state->size to non-zero.  Return -1 on failure or 0 on success. */
static int gz_init(gz_state *state)
{
    int ret;
    z_stream *strm = &state->strm;

    /* allocate input buffer */
    state->in = (unsigned char *)malloc(state->want);
    if (state->in == NULL) {
        gz_error(state, Z_MEM_ERROR, "out of memory");
        return -1;
    }

    /* only need output buffer and deflate state if compressing */
    if (!state->direct) {
        /* allocate output buffer */
        state->out = (unsigned char *)malloc(state->want);
        if (state->out == NULL) {
            free(state->in);
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }

        /* allocate deflate memory, set up for gzip compression */
        strm->zalloc = NULL;
        strm->zfree = NULL;
        strm->opaque = NULL;
        ret = deflateInit2(strm, state->level, Z_DEFLATED,
                           MAX_WBITS + 16, DEF_MEM_LEVEL, state->strategy);
        if (ret != Z_OK) {
            free(state->out);
            free(state->in);
            gz_error(state, Z_MEM_ERROR, "out of memory");
            return -1;
        }
        strm->next_in = NULL;
    }

    /* mark state as initialized */
    state->size = state->want;

    /* initialize write buffer if compressing */
    if (!state->direct) {
        strm->avail_out = state->size;
        strm->next_out = state->out;
        state->x.next = strm->next_out;
    }
    return 0;
}

/* Compress whatever is at avail_in and next_in and write to the output file.
   Return -1 if there is an error writing to the output file, otherwise 0.
   flush is assumed to be a valid deflate() flush value.  If flush is Z_FINISH,
   then the deflate() state is reset to start a new gzip stream.  If gz->direct
   is true, then simply write to the output file without compressing, and
   ignore flush. */
static int gz_comp(gz_state *state,
                   int flush)
{
    int ret, got;
    unsigned int have;
    z_stream *strm = &state->strm;

    /* allocate memory if this is the first time through */
    if (state->size == 0 && gz_init(state) == -1)
        return -1;

    /* write directly if requested */
    if (state->direct) {
        got = write(state->fd, strm->next_in, strm->avail_in);
        if (got < 0 || (unsigned int)got != strm->avail_in) {
            gz_error(state, Z_ERRNO, strerror(errno));
            return -1;
        }
        strm->avail_in = 0;
        return 0;
    }

    /* run deflate() on provided input until it produces no more output */
    ret = Z_OK;
    do {
        /* write out current buffer contents if full, or if flushing, but if
           doing Z_FINISH then don't write until we get to Z_STREAM_END */
        if (strm->avail_out == 0 || (flush != Z_NO_FLUSH &&
                                     (flush != Z_FINISH || ret == Z_STREAM_END))) {
            have = (unsigned int)(strm->next_out - state->x.next);
            if (have && ((got = write(state->fd, state->x.next, have)) < 0 ||
                         (unsigned int)got != have)) {
                gz_error(state, Z_ERRNO, strerror(errno));
                return -1;
            }
            if (strm->avail_out == 0) {
                strm->avail_out = state->size;
                strm->next_out = state->out;
            }
            state->x.next = strm->next_out;
        }

        /* compress */
        have = strm->avail_out;
        ret = deflate(strm, flush);
        if (ret == Z_STREAM_ERROR) {
            gz_error(state, Z_STREAM_ERROR,
                     "internal error: deflate stream corrupt");
            return -1;
        }
        have -= strm->avail_out;
    } while (have);

    /* if that completed a deflate stream, allow another to start */
    if (flush == Z_FINISH)
        deflateReset(strm);

    /* all done, no errors */
    return 0;
}

/* Compress len zeros to output.  Return -1 on error, 0 on success. */
static int gz_zero(gz_state *state,
                   off_t len)
{
    int first;
    unsigned int n;
    z_stream *strm = &state->strm;

    /* consume whatever's left in the input buffer */
    if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
        return -1;

    /* compress len zeros (len guaranteed > 0) */
    first = 1;
    while (len) {
        n = GT_OFF(state->size) || (off_t)state->size > len ?
            (unsigned int)len : state->size;
        if (first) {
            memset(state->in, 0, n);
            first = 0;
        }
        strm->avail_in = n;
        strm->next_in = state->in;
        state->x.pos += n;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return -1;
        len -= n;
    }
    return 0;
}

/* -- see zlib.h -- */
int ZEXPORT gzwrite(gzFile file,
                    const void *buf,
                    unsigned int len)
{
    unsigned put = len;
    gz_state *state;
    z_stream *strm;

    /* get internal structure */
    if (file == NULL)
        return 0;
    state = (gz_state *)file;
    strm = &state->strm;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* since an int is returned, make sure len fits in one, otherwise return
       with an error (this avoids the flaw in the interface) */
    if ( (unsigned int) INT_MAX < len ) {
        gz_error(state, Z_DATA_ERROR, "requested length does not fit in int");
        return 0;
    }

    /* if len is zero, avoid unnecessary operations */
    if (len == 0)
        return 0;

    /* allocate memory if this is the first time through */
    if (state->size == 0 && gz_init(state) == -1)
        return 0;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return 0;
    }

    /* for small len, copy to input buffer, otherwise compress directly */
    if (len < state->size) {
        /* copy to input buffer, compress when full */
        do {
            unsigned int have, copy;

            if (strm->avail_in == 0)
                strm->next_in = state->in;
            have = (unsigned int)((strm->next_in + strm->avail_in) - state->in);
            copy = state->size - have;
            if (copy > len)
                copy = len;
            memcpy(state->in + have, buf, copy);
            strm->avail_in += copy;
            state->x.pos += copy;
            buf = (const char *)buf + copy;
            len -= copy;
            if (len && gz_comp(state, Z_NO_FLUSH) == -1)
                return 0;
        } while (len);
    }
    else {
        /* consume whatever's left in the input buffer */
        if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
            return 0;

        /* directly compress user buffer to file */
        strm->avail_in = len;
        strm->next_in = (const unsigned char *)buf;
        state->x.pos += len;
        if (gz_comp(state, Z_NO_FLUSH) == -1)
            return 0;
    }

    /* input was all buffered or compressed (put will fit in int) */
    return (int)put;
}

/* -- see zlib.h -- */
int ZEXPORT gzputc(gzFile file,
                   int c)
{
    unsigned int have;
    unsigned char buf[1];
    gz_state *state;
    z_stream *strm;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_state *)file;
    strm = &state->strm;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return -1;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return -1;
    }

    /* try writing to input buffer for speed (state->size == 0 if buffer not
       initialized) */
    if (state->size) {
        if (strm->avail_in == 0)
            strm->next_in = state->in;
        have = (unsigned)((strm->next_in + strm->avail_in) - state->in);
        if (have < state->size) {
            state->in[have] = c;
            strm->avail_in++;
            state->x.pos++;
            return c & 0xff;
        }
    }

    /* no room in buffer or not initialized, use gz_write() */
    buf[0] = c;
    if (gzwrite(file, buf, 1) != 1)
        return -1;
    return c & 0xff;
}

/* -- see zlib.h -- */
int ZEXPORT gzputs(gzFile file,
                   const char *str)
{
    int ret;
    unsigned int len;

    /* write string */
    /* TODO: This doesn't work for strings longer than UINT_MAX! */
    len = (unsigned int)strlen(str);
    ret = gzwrite(file, str, len);
    return ret == 0 && len != 0 ? -1 : ret;
}

#include <stdarg.h>

/* -- see zlib.h -- */
/* TODO: This function deserves an audit. */
int ZEXPORTVA gzvprintf(gzFile file, const char *format, va_list va)
{
    int size, len;
    gz_state *state;
    z_stream *strm;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_state *)file;
    strm = &state->strm;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return 0;

    /* make sure we have some buffer space */
    if (state->size == 0 && gz_init(state) == -1)
        return 0;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return 0;
    }

    /* consume whatever's left in the input buffer */
    if (strm->avail_in && gz_comp(state, Z_NO_FLUSH) == -1)
        return 0;

    /* do the printf() into the input buffer, put length in len */
    size = (int)(state->size);
    state->in[size - 1] = 0;
    /* TODO: Potential truncation! */
    len = vsnprintf((char *)(state->in), size, format, va);

    /* check that printf() results fit in buffer */
    if (len <= 0 || len >= (int)size || state->in[size - 1] != 0)
        return 0;

    /* update buffer and position, defer compression until needed */
    strm->avail_in = (unsigned int)len;
    strm->next_in = state->in;
    state->x.pos += len;
    return len;
}

int ZEXPORTVA gzprintf(gzFile file, const char *format, ...)
{
    va_list va;
    int ret;

    va_start(va, format);
    ret = gzvprintf(file, format, va);
    va_end(va);
    return ret;
}

/* -- see zlib.h -- */
int ZEXPORT gzflush(gzFile file,
                    int flush)
{
    gz_state *state;

    /* get internal structure */
    if (file == NULL)
        return -1;
    state = (gz_state *)file;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* check flush parameter */
    if (flush < 0 || flush > Z_FINISH)
        return Z_STREAM_ERROR;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return -1;
    }

    /* compress remaining data with requested flush */
    gz_comp(state, flush);
    return state->err;
}

/* -- see zlib.h -- */
int ZEXPORT gzsetparams(gzFile file,
                        int level,
                        int strategy)
{
    gz_state *state;
    z_stream *strm;

    /* get internal structure */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_state *)file;
    strm = &state->strm;

    /* check that we're writing and that there's no error */
    if (state->mode != GZ_WRITE || state->err != Z_OK)
        return Z_STREAM_ERROR;

    /* if no change is requested, then do nothing */
    if (level == state->level && strategy == state->strategy)
        return Z_OK;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            return -1;
    }

    /* change compression parameters for subsequent input */
    if (state->size) {
        /* flush previous input with previous parameters before changing */
        if (strm->avail_in && gz_comp(state, Z_PARTIAL_FLUSH) == -1)
            return state->err;
        deflateParams(strm, level, strategy);
    }
    state->level = level;
    state->strategy = strategy;
    return Z_OK;
}

/* -- see zlib.h -- */
int ZEXPORT gzclose_w(gzFile file)
{
    int ret = Z_OK;
    gz_state *state;

    /* get internal structure */
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_state *)file;

    /* check that we're writing */
    if (state->mode != GZ_WRITE)
        return Z_STREAM_ERROR;

    /* check for seek request */
    if (state->seek) {
        state->seek = 0;
        if (gz_zero(state, state->skip) == -1)
            ret = state->err;
    }

    /* flush, free memory, and close file */
    if (gz_comp(state, Z_FINISH) == -1)
        ret = state->err;
    if (state->size) {
        if (!state->direct) {
            (void)deflateEnd(&(state->strm));
            free(state->out);
        }
        free(state->in);
    }
    gz_error(state, Z_OK, NULL);
    free(state->path);
    if (close(state->fd) == -1)
        ret = Z_ERRNO;
    free(state);
    return ret;
}

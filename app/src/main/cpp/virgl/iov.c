#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vrend_iov.h"

size_t vrend_get_iovec_size(const struct iovec *iov, int iovlen) {
    size_t size = 0;

    while (iovlen > 0) {
        size += iov->iov_len;
        iov++;
        iovlen--;
    }

    return size;
}

size_t vrend_read_from_iovec(const struct iovec *iov, int iovlen,
                             size_t offset,
                             char *buf, size_t count)
{
    size_t read = 0;
    size_t len;

    while (count > 0 && iovlen > 0) {
        if (iov->iov_len > offset) {
            len = iov->iov_len - offset;

            if (count < len) len = count;

            memcpy(buf, (char*)iov->iov_base + offset, len);
            read += len;

            buf += len;
            count -= len;
            offset = 0;
        } else {
            offset -= iov->iov_len;
        }

        iov++;
        iovlen--;
    }
    assert(offset == 0);
    return read;
}

size_t vrend_write_to_iovec(const struct iovec *iov, int iovlen,
                            size_t offset, const char *buf, size_t count)
{
    size_t written = 0;
    size_t len;

    while (count > 0 && iovlen > 0) {
        if (iov->iov_len > offset) {
            len = iov->iov_len - offset;

            if (count < len) len = count;

            memcpy((char*)iov->iov_base + offset, buf, len);
            written += len;

            offset = 0;
            buf += len;
            count -= len;
        } else {
            offset -= iov->iov_len;
        }
        iov++;
        iovlen--;
    }
    assert(offset == 0);
    return written;
}

size_t vrend_read_from_iovec_cb(const struct iovec *iov, int iovlen,
                                size_t offset, size_t count,
                                iov_cb iocb, void *cookie)
{
    size_t read = 0;
    size_t len;

    while (count > 0 && iovlen > 0) {
        if (iov->iov_len > offset) {
            len = iov->iov_len - offset;

            if (count < len) len = count;

            (*iocb)(cookie, read, (char*)iov->iov_base + offset, len);
            read += len;

            count -= len;
            offset = 0;
        } else {
            offset -= iov->iov_len;
        }
        iov++;
        iovlen--;
    }
    assert(offset == 0);
    return read;


}

/**
 * Copy data from one iovec to another iovec.
 *
 * TODO: Implement iovec copy without copy to intermediate buffer.
 *
 * \param src_iov    The source iov.
 * \param src_iovlen The number of memory regions in the source iov.
 * \param src_offset The byte offset in the source iov to start reading from.
 * \param dst_iov    The destination iov.
 * \param dst_iovlen The number of memory regions in the destination iov.
 * \param dst_offset The byte offset in the destination iov to start writing to.
 * \param count      The number of bytes to copy
 * \param buf        If not NULL, a pointer to a buffer of at least count size
 *                   to use a temporary storage for the copy operation.
 * \return           -1 on failure, 0 on success
 */
int vrend_copy_iovec(const struct iovec *src_iov, int src_iovlen, size_t src_offset,
                     const struct iovec *dst_iov, int dst_iovlen, size_t dst_offset,
                     size_t count, char *buf)
{
    int ret = 0;
    bool needs_free;
    size_t nread;
    size_t nwritten;

    if (src_iov == NULL || dst_iov == NULL)
        return -1;

    if (src_iov == dst_iov && src_offset == dst_offset)
        return 0;

    if (!buf) {
        buf = malloc(count);
        needs_free = true;
    } else {
        needs_free = false;
    }

    if (!buf)
        return -1;

    nread = vrend_read_from_iovec(src_iov, src_iovlen, src_offset, buf, count);
    if (nread != count) {
        ret = -1;
        goto out;
    }

    nwritten = vrend_write_to_iovec(dst_iov, dst_iovlen, dst_offset, buf, count);
    if (nwritten != count) {
        ret = -1;
        goto out;
    }

    out:
    if (needs_free)
        free(buf);

    return ret;
}

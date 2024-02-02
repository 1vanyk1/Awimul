#ifndef VREND_IOV_H
#define VREND_IOV_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#else
struct iovec {
    void *iov_base;
    size_t iov_len;
};
#endif

struct vrend_transfer_info {
    int level;
    uint32_t stride;
    uint32_t layer_stride;
    unsigned int iovec_cnt;
    const struct iovec *iovec;
    uint64_t offset;
    struct pipe_box *box;
    bool synchronized;
};

typedef void (*iov_cb)(void *cookie, unsigned int doff, void *src, int len);

size_t vrend_get_iovec_size(const struct iovec *iov, int iovlen);
size_t vrend_read_from_iovec(const struct iovec *iov, int iov_cnt,
                             size_t offset, char *buf, size_t bytes);
size_t vrend_write_to_iovec(const struct iovec *iov, int iov_cnt,
                            size_t offset, const char *buf, size_t bytes);

size_t vrend_read_from_iovec_cb(const struct iovec *iov, int iov_cnt,
                                size_t offset, size_t bytes, iov_cb iocb, void *cookie);

int vrend_copy_iovec(const struct iovec *src_iov, int src_iovlen, size_t src_offset,
                     const struct iovec *dst_iov, int dst_iovlen, size_t dst_offset,
                     size_t count, char *buf);

#endif

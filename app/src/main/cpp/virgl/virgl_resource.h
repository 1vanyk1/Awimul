#ifndef VIRGL_RESOURCE_H
#define VIRGL_RESOURCE_H

#include <stdint.h>

struct iovec;
struct pipe_resource;

enum virgl_resource_fd_type {
    VIRGL_RESOURCE_FD_DMABUF,
    VIRGL_RESOURCE_FD_OPAQUE,

    VIRGL_RESOURCE_FD_INVALID = -1,
};

/**
 * A global cross-context resource.  A virgl_resource is not directly usable
 * by renderer contexts, but must be attached and imported into renderer
 * contexts to create context objects first.  For example, it can be attached
 * and imported into a vrend_decode_ctx to create a vrend_resource.
 *
 * It is also possible to create a virgl_resource from a context object.
 *
 * The underlying storage of a virgl_resource is provided by a pipe_resource
 * and/or a fd.  When it is provided by a pipe_resource, the virgl_resource is
 * said to be typed because pipe_resource also provides the type information.
 *
 * Conventional resources are always typed.  Blob resources by definition do
 * not have nor need type information, but those created from vrend_decode_ctx
 * objects are typed.  That should be considered a convenience rather than
 * something to be relied upon.  Contexts must not assume that every resource is
 * typed when interop is expected.
 */
struct virgl_resource {
    uint32_t res_id;

    struct pipe_resource *pipe_resource;

    enum virgl_resource_fd_type fd_type;
    int fd;

    const struct iovec *iov;
    int iov_count;

    uint32_t map_info;

    uint64_t map_size;
    void *mapped;

    void *private_data;
};

struct virgl_resource_pipe_callbacks {
    void *data;

    void (*unref)(struct pipe_resource *pres, void *data);

    void (*attach_iov)(struct pipe_resource *pres,
                       const struct iovec *iov,
                       int iov_count,
                       void *data);
    void (*detach_iov)(struct pipe_resource *pres, void *data);

    enum virgl_resource_fd_type (*export_fd)(struct pipe_resource *pres,
                                             int *fd,
                                             void *data);
};

int
virgl_resource_table_init(const struct virgl_resource_pipe_callbacks *callbacks);

void
virgl_resource_table_cleanup(void);

void
virgl_resource_table_reset(void);

struct virgl_resource *
virgl_resource_create_from_pipe(uint32_t res_id,
                                struct pipe_resource *pres,
                                const struct iovec *iov,
                                int iov_count);

struct virgl_resource *
virgl_resource_create_from_fd(uint32_t res_id,
                              enum virgl_resource_fd_type fd_type,
                              int fd,
                              const struct iovec *iov,
                              int iov_count);

struct virgl_resource *
virgl_resource_create_from_iov(uint32_t res_id,
                               const struct iovec *iov,
                               int iov_count);

void
virgl_resource_remove(uint32_t res_id);

struct virgl_resource *
virgl_resource_lookup(uint32_t res_id);

int
virgl_resource_attach_iov(struct virgl_resource *res,
                          const struct iovec *iov,
                          int iov_count);

void
virgl_resource_detach_iov(struct virgl_resource *res);

enum virgl_resource_fd_type
virgl_resource_export_fd(struct virgl_resource *res, int *fd);

#endif /* VIRGL_RESOURCE_H */

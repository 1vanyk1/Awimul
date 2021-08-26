#ifndef U_UPLOAD_MGR_H
#define U_UPLOAD_MGR_H

#include "../include/pipe/p_compiler.h"
#include "../include/pipe/p_defines.h"

struct pipe_context;
struct pipe_resource;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the upload manager.
 *
 * \param pipe          Pipe driver.
 * \param default_size  Minimum size of the upload buffer, in bytes.
 * \param bind          Bitmask of PIPE_BIND_* flags.
 * \param usage         PIPE_USAGE_*
 * \param flags         bitmask of PIPE_RESOURCE_FLAG_* flags.
 */
struct u_upload_mgr *
u_upload_create(struct pipe_context *pipe, unsigned default_size,
                unsigned bind, enum pipe_resource_usage usage, unsigned flags);

/**
 * Create the default uploader for pipe_context. Only pipe_context::screen
 * needs to be set for this to succeed.
 */
struct u_upload_mgr *
u_upload_create_default(struct pipe_context *pipe);

/**
 * Create an uploader with identical parameters as another one, but using
 * the given pipe_context instead.
 */
struct u_upload_mgr *
u_upload_clone(struct pipe_context *pipe, struct u_upload_mgr *upload);

/** Whether to avoid persistent mappings where available */
void
u_upload_disable_persistent(struct u_upload_mgr *upload);

/**
 * Destroy the upload manager.
 */
void u_upload_destroy( struct u_upload_mgr *upload );

/**
 * Unmap upload buffer
 *
 * \param upload           Upload manager
 *
 * This must usually be called prior to firing the command stream
 * which references the upload buffer, as many memory managers either
 * don't like firing a mapped buffer or cause subsequent maps of a
 * fired buffer to wait.
 */
void u_upload_unmap( struct u_upload_mgr *upload );

/**
 * Sub-allocate new memory from the upload buffer.
 *
 * \param upload           Upload manager
 * \param min_out_offset   Minimum offset that should be returned in out_offset.
 * \param size             Size of the allocation.
 * \param alignment        Alignment of the suballocation within the buffer
 * \param out_offset       Pointer to where the new buffer offset will be returned.
 * \param outbuf           Pointer to where the upload buffer will be returned.
 * \param ptr              Pointer to the allocated memory that is returned.
 */
void u_upload_alloc(struct u_upload_mgr *upload,
                    unsigned min_out_offset,
                    unsigned size,
                    unsigned alignment,
                    unsigned *out_offset,
                    struct pipe_resource **outbuf,
                    void **ptr);


/**
 * Allocate and write data to the upload buffer.
 *
 * Same as u_upload_alloc, but in addition to that, it copies "data"
 * to the pointer returned from u_upload_alloc.
 */
void u_upload_data(struct u_upload_mgr *upload,
                   unsigned min_out_offset,
                   unsigned size,
                   unsigned alignment,
                   const void *data,
                   unsigned *out_offset,
                   struct pipe_resource **outbuf);

#ifdef __cplusplus
} // extern "C" {
#endif

#endif

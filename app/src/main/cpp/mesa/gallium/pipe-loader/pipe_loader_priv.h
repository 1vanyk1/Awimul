#ifndef PIPE_LOADER_PRIV_H
#define PIPE_LOADER_PRIV_H

#include "pipe_loader.h"

struct pipe_loader_ops {
    struct pipe_screen *(*create_screen)(struct pipe_loader_device *dev,
                                         const struct pipe_screen_config *config, bool sw_vk);

    const struct driOptionDescription *(*get_driconf)(struct pipe_loader_device *dev,
                                                      unsigned *count);

    void (*release)(struct pipe_loader_device **dev);
};

/**
 * Open the pipe driver module that contains the specified driver.
 */
struct util_dl_library *
pipe_loader_find_module(const char *driver_name,
                        const char *library_paths);

/**
 * Free the base device structure.
 *
 * Implementations of pipe_loader_ops::release must call this.
 *
 * (*dev)->driver_name must be freed by the caller if it was allocated on the
 * heap.
 */
void
pipe_loader_base_release(struct pipe_loader_device **dev);

#endif /* PIPE_LOADER_PRIV_H */

#ifndef OPENCL_INTEROP_H
#define OPENCL_INTEROP_H

/* dlsym these without the "_t" suffix. You should get the correct symbols
 * if the OpenCL driver is loaded.
 */

typedef bool (*opencl_dri_event_add_ref_t)(void *cl_event);
typedef bool (*opencl_dri_event_release_t)(void *cl_event);
typedef bool (*opencl_dri_event_wait_t)(void *cl_event, uint64_t timeout);
typedef struct pipe_fence_handle *(*opencl_dri_event_get_fence_t)(void *cl_event);

#endif /* OPENCL_INTEROP_H */

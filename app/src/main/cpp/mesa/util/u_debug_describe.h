#ifndef U_DEBUG_DESCRIBE_H_
#define U_DEBUG_DESCRIBE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct pipe_reference;
struct pipe_resource;
struct pipe_surface;
struct pipe_sampler_view;
struct pipe_image_view;

/* a 256-byte buffer is necessary and sufficient */
void debug_describe_reference(char* buf, const struct pipe_reference*ptr);
void debug_describe_resource(char* buf, const struct pipe_resource *ptr);
void debug_describe_surface(char* buf, const struct pipe_surface *ptr);
void debug_describe_sampler_view(char* buf, const struct pipe_sampler_view *ptr);
void debug_describe_image_view(char* buf, const struct pipe_image_view *ptr);
void debug_describe_so_target(char* buf,
                              const struct pipe_stream_output_target *ptr);

#ifdef __cplusplus
}
#endif

#endif /* U_DEBUG_DESCRIBE_H_ */

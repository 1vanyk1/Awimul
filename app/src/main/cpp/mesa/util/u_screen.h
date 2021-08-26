struct pipe_screen;
enum pipe_cap;

#ifdef __cplusplus
extern "C" {
#endif

int
u_pipe_screen_get_param_defaults(struct pipe_screen *pscreen,
                                 enum pipe_cap param);

#ifdef __cplusplus
};
#endif

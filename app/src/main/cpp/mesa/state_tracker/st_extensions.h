#ifndef ST_EXTENSIONS_H
#define ST_EXTENSIONS_H


struct st_context;
struct pipe_screen;

extern void st_init_limits(struct pipe_screen *screen,
                           struct gl_constants *c,
                           struct gl_extensions *extensions);

extern void st_init_extensions(struct pipe_screen *screen,
                               struct gl_constants *consts,
                               struct gl_extensions *extensions,
                               struct st_config_options *options,
                               gl_api api);


#endif /* ST_EXTENSIONS_H */

#ifndef REMAP_H
#define REMAP_H


struct gl_function_pool_remap {
    int pool_index;
    int remap_index;
};

extern int
        driDispatchRemapTable[];

extern void
_mesa_init_remap_table(void);


#endif /* REMAP_H */

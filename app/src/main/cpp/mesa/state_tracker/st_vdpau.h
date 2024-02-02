#ifndef ST_VDPAU_H
#define ST_VDPAU_H

struct dd_function_table;

#ifdef HAVE_ST_VDPAU
extern void
st_init_vdpau_functions(struct dd_function_table *functions);
#else
static inline void
st_init_vdpau_functions(struct dd_function_table *functions) {}
#endif

#endif /* ST_VDPAU_H */

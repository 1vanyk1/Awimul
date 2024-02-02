#ifndef ST_ATOM_CONSTBUF_H
#define ST_ATOM_CONSTBUF_H

#include "../compiler/shader_enums.h"

struct gl_program_parameter_list;
struct st_context;


void st_upload_constants(struct st_context *st, struct gl_program *prog, gl_shader_stage stage);


#endif /* ST_ATOM_CONSTBUF_H */

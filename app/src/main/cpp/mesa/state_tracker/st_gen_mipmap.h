#ifndef ST_GEN_MIPMAP_H
#define ST_GEN_MIPMAP_H


#include "../main/glheader.h"

struct gl_context;
struct gl_texture_object;


extern void
st_generate_mipmap(struct gl_context *ctx, GLenum target,
                   struct gl_texture_object *texObj);


#endif /* ST_GEN_MIPMAP_H */

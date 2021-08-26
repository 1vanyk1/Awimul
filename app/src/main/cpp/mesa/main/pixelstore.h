#ifndef PIXELSTORE_H
#define PIXELSTORE_H


#include "glheader.h"

struct gl_context;


extern void GLAPIENTRY
_mesa_PixelStorei( GLenum pname, GLint param );


extern void GLAPIENTRY
_mesa_PixelStoref( GLenum pname, GLfloat param );


extern void GLAPIENTRY
_mesa_PixelStorei_no_error(GLenum pname, GLint param);


extern void GLAPIENTRY
_mesa_PixelStoref_no_error(GLenum pname, GLfloat param);


extern void
_mesa_init_pixelstore( struct gl_context *ctx );


extern bool
_mesa_compressed_pixel_storage_error_check(
        struct gl_context *ctx,
        GLint dimensions,
        const struct gl_pixelstore_attrib *packing,
        const char *caller);


#endif

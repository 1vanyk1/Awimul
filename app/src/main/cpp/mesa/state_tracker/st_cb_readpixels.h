#ifndef ST_CB_READPIXELS_H
#define ST_CB_READPIXELS_H

#include "../main/glheader.h"

void
st_ReadPixels(struct gl_context *ctx, GLint x, GLint y,
              GLsizei width, GLsizei height,
              GLenum format, GLenum type,
              const struct gl_pixelstore_attrib *pack,
              void *pixels);

#endif /* ST_CB_READPIXELS_H */

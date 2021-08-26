#include "gzguts.h"

/* gzclose() is in a separate file so that it is linked in only if it is used.
   That way the other gzclose functions can be used instead to avoid linking in
   unneeded compression or decompression routines. */
int ZEXPORT gzclose(gzFile file)
{
    gz_state *state;

    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_state *)file;

    return state->mode == GZ_READ ? gzclose_r(file) : gzclose_w(file);
}

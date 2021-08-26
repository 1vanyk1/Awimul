#ifndef AWIMUL_OPEN_MEMSTREAM_H
#define AWIMUL_OPEN_MEMSTREAM_H

#include <stdio.h>
#include <stdlib.h>

struct memstream {
    char **bufp;
    size_t *sizep;
    ssize_t len;
    fpos_t offset;
};

static int memstream_grow(struct memstream *ms, fpos_t newoff);

static void memstream_update(struct memstream *ms);

static int memstream_write(void *cookie, const char *buf, int len);

static fpos_t memstream_seek(void *cookie, fpos_t pos, int whence);

static int memstream_close(void *cookie);

FILE * open_memstream(char **bufp, size_t *sizep);

FILE * open_memstream(char **bufp, size_t *sizep);

#endif //AWIMUL_OPEN_MEMSTREAM_H

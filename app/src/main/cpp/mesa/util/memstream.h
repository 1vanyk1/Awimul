#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include <stdbool.h>
#include <stdio.h>
#ifdef _WIN32
#include "../../windows/windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct u_memstream
{
    FILE *f;
#ifdef _WIN32
    char **bufp;
   size_t *sizep;
   char temp[MAX_PATH];
#endif
};

extern bool
u_memstream_open(struct u_memstream *mem, char **bufp, size_t *sizep);

extern void
u_memstream_close(struct u_memstream *mem);

static inline FILE *
u_memstream_get(const struct u_memstream *mem)
{
    return mem->f;
}

#ifdef __cplusplus
}
#endif

#endif

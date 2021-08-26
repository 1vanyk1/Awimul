#ifndef MISCSTRUCT_H
#define MISCSTRUCT_H 1

#include "misc.h"
#include "xprotostr.h"
#include "../../pixman/pixman.h"

typedef xPoint DDXPointRec;

typedef struct pixman_box16 BoxRec;

typedef union _DevUnion {
    void *ptr;
    long val;
    unsigned long uval;
    void *(*fptr) (void);
} DevUnion;

#endif                          /* MISCSTRUCT_H */
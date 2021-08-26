#ifndef MISTRUCT_H
#define MISTRUCT_H

#include "mi.h"
#include "../headers/regionstr.h"

/* information about dashes */
typedef struct _miDash {
    DDXPointRec pt;
    int e1, e2;                 /* keep these, so we don't have to do it again */
    int e;                      /* bresenham error term for this point on line */
    int which;
    int newLine;                /* 0 if part of same original line as previous dash */
} miDashRec;

#endif                          /* MISTRUCT_H */

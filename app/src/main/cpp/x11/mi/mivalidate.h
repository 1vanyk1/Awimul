#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef MIVALIDATE_H
#define MIVALIDATE_H

#include "../headers/regionstr.h"

typedef union _Validate {
    struct BeforeValidate {
        DDXPointRec oldAbsCorner;       /* old window position */
        RegionPtr borderVisible;        /* visible region of border, */
        /* non-null when size changes */
        Bool resized;           /* unclipped winSize has changed */
    } before;
    struct AfterValidate {
        RegionRec exposed;      /* exposed regions, absolute pos */
        RegionRec borderExposed;
    } after;
} ValidateRec;

#endif                          /* MIVALIDATE_H */


#ifndef __MIFPOLY_H__
#define __MIFPOLY_H__

#include "../headers/xfuncproto.h"

static _X_INLINE int
ICEIL(double x)
{
    int _cTmp = x;

    return ((x == _cTmp) || (x < 0.0)) ? _cTmp : _cTmp + 1;
}

#endif                          /* __MIFPOLY_H__ */

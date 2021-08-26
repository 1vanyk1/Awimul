#ifndef VALIDATE_H
#define VALIDATE_H

#include "miscstruct.h"
#include "regionstr.h"

typedef enum { VTOther, VTStack, VTMove, VTUnmap, VTMap, VTBroken } VTKind;

/* union _Validate is now device dependent; see mivalidate.h for an example */
typedef union _Validate *ValidatePtr;

#define UnmapValData ((ValidatePtr)1)

#endif                          /* VALIDATE_H */
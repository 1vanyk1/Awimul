#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/regionstr.h"
#include "mistruct.h"

void
miStepDash(int dist,            /* distance to step */
           int *pDashIndex,     /* current dash */
           unsigned char *pDash,        /* dash list */
           int numInDashList,   /* total length of dash list */
           int *pDashOffset     /* offset into current dash */
)
{
    int dashIndex, dashOffset;
    int totallen;
    int i;

    dashIndex = *pDashIndex;
    dashOffset = *pDashOffset;
    if (dist < pDash[dashIndex] - dashOffset) {
        *pDashOffset = dashOffset + dist;
        return;
    }
    dist -= pDash[dashIndex] - dashOffset;
    if (++dashIndex == numInDashList)
        dashIndex = 0;
    totallen = 0;
    for (i = 0; i < numInDashList; i++)
        totallen += pDash[i];
    if (totallen <= dist)
        dist = dist % totallen;
    while (dist >= pDash[dashIndex]) {
        dist -= pDash[dashIndex];
        if (++dashIndex == numInDashList)
            dashIndex = 0;
    }
    *pDashIndex = dashIndex;
    *pDashOffset = dist;
}

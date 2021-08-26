#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "../headers/x.h"
#include "../headers/xprotostr.h"
#include "../headers/regionstr.h"
#include "../headers/gcstruct.h"
#include "../headers/pixmap.h"
#include "mi.h"

void
miPolyRectangle(DrawablePtr pDraw, GCPtr pGC, int nrects, xRectangle *pRects)
{
    int i;
    xRectangle *pR = pRects;
    DDXPointRec rect[5];
    int bound_tmp;

#define MINBOUND(dst,eqn)	bound_tmp = eqn; \
				if (bound_tmp < -32768) \
				    bound_tmp = -32768; \
				dst = bound_tmp;

#define MAXBOUND(dst,eqn)	bound_tmp = eqn; \
				if (bound_tmp > 32767) \
				    bound_tmp = 32767; \
				dst = bound_tmp;

#define MAXUBOUND(dst,eqn)	bound_tmp = eqn; \
				if (bound_tmp > 65535) \
				    bound_tmp = 65535; \
				dst = bound_tmp;

    if (pGC->lineStyle == LineSolid && pGC->joinStyle == JoinMiter &&
        pGC->lineWidth != 0) {
        xRectangle *tmp, *t;
        int ntmp;
        int offset1, offset2, offset3;
        int x, y, width, height;

        ntmp = (nrects << 2);
        offset2 = pGC->lineWidth;
        offset1 = offset2 >> 1;
        offset3 = offset2 - offset1;
        tmp = xallocarray(ntmp, sizeof(xRectangle));
        if (!tmp)
            return;
        t = tmp;
        for (i = 0; i < nrects; i++) {
            x = pR->x;
            y = pR->y;
            width = pR->width;
            height = pR->height;
            pR++;
            if (width == 0 && height == 0) {
                rect[0].x = x;
                rect[0].y = y;
                rect[1].x = x;
                rect[1].y = y;
                (*pGC->ops->Polylines) (pDraw, pGC, CoordModeOrigin, 2, rect);
            }
            else if (height < offset2 || width < offset1) {
                if (height == 0) {
                    t->x = x;
                    t->width = width;
                }
                else {
                    MINBOUND(t->x, x - offset1)
                    MAXUBOUND(t->width, width + offset2)
                }
                if (width == 0) {
                    t->y = y;
                    t->height = height;
                }
                else {
                    MINBOUND(t->y, y - offset1)
                    MAXUBOUND(t->height, height + offset2)
                }
                t++;
            }
            else {
                MINBOUND(t->x, x - offset1)
                MINBOUND(t->y, y - offset1)
                MAXUBOUND(t->width, width + offset2)
                t->height = offset2;
                t++;
                MINBOUND(t->x, x - offset1)
                MAXBOUND(t->y, y + offset3);
                t->width = offset2;
                t->height = height - offset2;
                t++;
                MAXBOUND(t->x, x + width - offset1);
                MAXBOUND(t->y, y + offset3)
                t->width = offset2;
                t->height = height - offset2;
                t++;
                MINBOUND(t->x, x - offset1)
                MAXBOUND(t->y, y + height - offset1)
                MAXUBOUND(t->width, width + offset2)
                t->height = offset2;
                t++;
            }
        }
        (*pGC->ops->PolyFillRect) (pDraw, pGC, t - tmp, tmp);
        free((void *) tmp);
    }
    else {

        for (i = 0; i < nrects; i++) {
            rect[0].x = pR->x;
            rect[0].y = pR->y;

            MAXBOUND(rect[1].x, pR->x + (int) pR->width)
            rect[1].y = rect[0].y;

            rect[2].x = rect[1].x;
            MAXBOUND(rect[2].y, pR->y + (int) pR->height);

            rect[3].x = rect[0].x;
            rect[3].y = rect[2].y;

            rect[4].x = rect[0].x;
            rect[4].y = rect[0].y;

            (*pGC->ops->Polylines) (pDraw, pGC, CoordModeOrigin, 5, rect);
            pR++;
        }
    }
}

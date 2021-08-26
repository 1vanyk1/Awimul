#ifndef _SHMINT_H_
#define _SHMINT_H_

#include "../headers/extensions/shmproto.h"

#include "../headers/screenint.h"
#include "../headers/pixmap.h"
#include "../headers/gc.h"

#define XSHM_PUT_IMAGE_ARGS \
    DrawablePtr		/* dst */, \
    GCPtr		/* pGC */, \
    int			/* depth */, \
    unsigned int	/* format */, \
    int			/* w */, \
    int			/* h */, \
    int			/* sx */, \
    int			/* sy */, \
    int			/* sw */, \
    int			/* sh */, \
    int			/* dx */, \
    int			/* dy */, \
    char *                      /* data */

#define XSHM_CREATE_PIXMAP_ARGS \
    ScreenPtr	/* pScreen */, \
    int		/* width */, \
    int		/* height */, \
    int		/* depth */, \
    char *                      /* addr */

typedef struct _ShmFuncs {
    PixmapPtr (*CreatePixmap) (XSHM_CREATE_PIXMAP_ARGS);
    void (*PutImage) (XSHM_PUT_IMAGE_ARGS);
} ShmFuncs, *ShmFuncsPtr;

#if XTRANS_SEND_FDS
#define SHM_FD_PASSING  1
#endif

typedef struct _ShmDesc {
    struct _ShmDesc *next;
    int shmid;
    int refcnt;
    char *addr;
    Bool writable;
    unsigned long size;
#ifdef SHM_FD_PASSING
    Bool is_fd;
    struct busfault *busfault;
    XID resource;
#endif
} ShmDescRec, *ShmDescPtr;

#ifdef SHM_FD_PASSING
#define SHMDESC_IS_FD(shmdesc)  ((shmdesc)->is_fd)
#else
#define SHMDESC_IS_FD(shmdesc)  (0)
#endif

extern _X_EXPORT void
ShmRegisterFuncs(ScreenPtr pScreen, ShmFuncsPtr funcs);

extern _X_EXPORT void
ShmRegisterFbFuncs(ScreenPtr pScreen);

extern _X_EXPORT RESTYPE ShmSegType;
extern _X_EXPORT int ShmCompletionCode;
extern _X_EXPORT int BadShmSegCode;

#endif                          /* _SHMINT_H_ */

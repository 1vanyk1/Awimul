#ifndef CLOSESTR_H
#define CLOSESTR_H

#include "xproto.h"
#include "closure.h"
#include "dix.h"
#include "misc.h"
#include "gcstruct.h"

/* closure structures */

/* OpenFont */

typedef struct _OFclosure {
    ClientPtr client;
    short current_fpe;
    short num_fpes;
    FontPathElementPtr *fpe_list;
    Mask flags;

/* XXX -- get these from request buffer instead? */
    const char *origFontName;
    int origFontNameLen;
    XID fontid;
    char *fontname;
    int fnamelen;
    FontPtr non_cachable_font;
} OFclosureRec;

/* ListFontsWithInfo */

#define XLFDMAXFONTNAMELEN	256
typedef struct _LFWIstate {
    char pattern[XLFDMAXFONTNAMELEN];
    int patlen;
    int current_fpe;
    int max_names;
    Bool list_started;
    void *private;
} LFWIstateRec, *LFWIstatePtr;

typedef struct _LFWIclosure {
    ClientPtr client;
    int num_fpes;
    FontPathElementPtr *fpe_list;
    xListFontsWithInfoReply *reply;
    int length;
    LFWIstateRec current;
    LFWIstateRec saved;
    int savedNumFonts;
    Bool haveSaved;
    char *savedName;
} LFWIclosureRec;

/* ListFonts */

typedef struct _LFclosure {
    ClientPtr client;
    int num_fpes;
    FontPathElementPtr *fpe_list;
    FontNamesPtr names;
    LFWIstateRec current;
    LFWIstateRec saved;
    Bool haveSaved;
    char *savedName;
    int savedNameLen;
} LFclosureRec;

/* PolyText */

typedef struct _PTclosure {
    ClientPtr client;
    DrawablePtr pDraw;
    GC *pGC;
    unsigned char *pElt;
    unsigned char *endReq;
    unsigned char *data;
    int xorg;
    int yorg;
    CARD8 reqType;
    XID did;
    int err;
} PTclosureRec;

/* ImageText */

typedef struct _ITclosure {
    ClientPtr client;
    DrawablePtr pDraw;
    GC *pGC;
    BYTE nChars;
    unsigned char *data;
    int xorg;
    int yorg;
    CARD8 reqType;
    XID did;
} ITclosureRec;
#endif                          /* CLOSESTR_H */

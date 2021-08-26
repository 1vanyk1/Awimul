#ifndef DIXFONT_H
#define DIXFONT_H 1

#include "dix.h"
#include "fonts/font.h"
#include "closure.h"
#include "fonts/fontstruct.h"
#include "fonts/fontproto.h"
#include "fonts/fontutil.h"

#define NullDIXFontProp ((DIXFontPropPtr)0)

typedef struct _DIXFontProp *DIXFontPropPtr;

extern _X_EXPORT Bool SetDefaultFont(const char * /*defaultfontname */ );

extern _X_EXPORT int OpenFont(ClientPtr /*client */ ,
                              XID /*fid */ ,
                              Mask /*flags */ ,
                              unsigned /*lenfname */ ,
                              const char * /*pfontname */ );

extern _X_EXPORT int CloseFont(void *pfont,
                               XID fid);

typedef struct _xQueryFontReply *xQueryFontReplyPtr;

extern _X_EXPORT void QueryFont(FontPtr /*pFont */ ,
                                xQueryFontReplyPtr /*pReply */ ,
                                int /*nProtoCCIStructs */ );

extern _X_EXPORT int ListFonts(ClientPtr /*client */ ,
                               unsigned char * /*pattern */ ,
                               unsigned int /*length */ ,
                               unsigned int /*max_names */ );

extern _X_EXPORT int PolyText(ClientPtr /*client */ ,
                              DrawablePtr /*pDraw */ ,
                              GCPtr /*pGC */ ,
                              unsigned char * /*pElt */ ,
                              unsigned char * /*endReq */ ,
                              int /*xorg */ ,
                              int /*yorg */ ,
                              int /*reqType */ ,
                              XID /*did */ );

extern _X_EXPORT int ImageText(ClientPtr /*client */ ,
                               DrawablePtr /*pDraw */ ,
                               GCPtr /*pGC */ ,
                               int /*nChars */ ,
                               unsigned char * /*data */ ,
                               int /*xorg */ ,
                               int /*yorg */ ,
                               int /*reqType */ ,
                               XID /*did */ );

extern _X_EXPORT int SetFontPath(ClientPtr /*client */ ,
                                 int /*npaths */ ,
                                 unsigned char * /*paths */ );

extern _X_EXPORT int SetDefaultFontPath(const char * /*path */ );

extern _X_EXPORT int GetFontPath(ClientPtr client,
                                 int *count,
                                 int *length, unsigned char **result);

extern _X_EXPORT void DeleteClientFontStuff(ClientPtr /*client */ );

/* Quartz support on Mac OS X pulls in the QuickDraw
   framework whose InitFonts function conflicts here. */
#ifdef __APPLE__
#define InitFonts Darwin_X_InitFonts
#endif
extern _X_EXPORT void InitFonts(void);

extern _X_EXPORT void FreeFonts(void);

extern _X_EXPORT FontPtr find_old_font(XID /*id */ );

#define GetGlyphs dixGetGlyphs
extern _X_EXPORT void dixGetGlyphs(FontPtr /*font */ ,
                                   unsigned long /*count */ ,
                                   unsigned char * /*chars */ ,
                                   FontEncoding /*fontEncoding */ ,
                                   unsigned long * /*glyphcount */ ,
                                   CharInfoPtr * /*glyphs */ );

extern _X_EXPORT void register_fpe_functions(void);

#endif                          /* DIXFONT_H */
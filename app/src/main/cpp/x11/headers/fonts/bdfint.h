#ifndef BDFINT_H
#define BDFINT_H

#include "fntfil.h"
#include "font.h"
#include "fntfilio.h"
#include "../xproto.h"

#define bdfIsPrefix(buf,str)	(!strncmp((char *)buf,str,strlen(str)))
#define	bdfStrEqual(s1,s2)	(!strcmp(s1,s2))

#define	BDF_GENPROPS	6
#define NullProperty	((FontPropPtr)0)

/*
 * This structure holds some properties we need to generate if they aren't
 * specified in the BDF file and some other values read from the file
 * that we'll need to calculate them.  We need to keep track of whether
 * or not we've read them.
 */
typedef struct BDFSTAT {
    int         linenum;
    char       *fileName;
    char        fontName[MAXFONTNAMELEN];
    float       pointSize;
    int         resolution_x;
    int         resolution_y;
    int         digitCount;
    int         digitWidths;
    int         exHeight;

    FontPropPtr fontProp;
    FontPropPtr pointSizeProp;
    FontPropPtr resolutionXProp;
    FontPropPtr resolutionYProp;
    FontPropPtr resolutionProp;
    FontPropPtr xHeightProp;
    FontPropPtr weightProp;
    FontPropPtr quadWidthProp;
    BOOL        haveFontAscent;
    BOOL        haveFontDescent;
    BOOL        haveDefaultCh;
}           bdfFileState;

extern void bdfError ( const char * message, ... ) _X_ATTRIBUTE_PRINTF(1, 2);
extern void bdfWarning ( const char *message, ... ) _X_ATTRIBUTE_PRINTF(1, 2);
extern unsigned char * bdfGetLine ( FontFilePtr file, unsigned char *buf,
                                    int len );
extern Atom bdfForceMakeAtom ( const char *str, int *size );
extern Atom bdfGetPropertyValue ( char *s );
extern int bdfIsInteger ( char *str );
extern unsigned char bdfHexByte ( unsigned char *s );
extern Bool bdfSpecialProperty ( FontPtr pFont, FontPropPtr prop,
                                 char isString, bdfFileState *bdfState );
extern int bdfReadFont( FontPtr pFont, FontFilePtr file,
                        int bit, int byte, int glyph, int scan );
extern int bdfReadFontInfo( FontInfoPtr pFontInfo, FontFilePtr file );

extern void FontCharInkMetrics ( FontPtr pFont, CharInfoPtr pCI,
                                 xCharInfo *pInk );
extern void FontCharReshape ( FontPtr pFont, CharInfoPtr pSrc,
                              CharInfoPtr pDst );

#endif				/* BDFINT_H */

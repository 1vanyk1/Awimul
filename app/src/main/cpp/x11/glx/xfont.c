#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "glxutil.h"
#include "unpack.h"
#include "indirect_dispatch.h"
#include "../../GL/gl.h"
#include "../headers/pixmapstr.h"
#include "../headers/windowstr.h"
#include "../headers/dixfontstr.h"

/*
** Make a single GL bitmap from a single X glyph
*/
static int
__glXMakeBitmapFromGlyph(FontPtr font, CharInfoPtr pci)
{
    int i, j;
    int widthPadded;            /* width of glyph in bytes, as padded by X */
    int allocBytes;             /* bytes to allocate to store bitmap */
    int w;                      /* width of glyph in bits */
    int h;                      /* height of glyph */
    register unsigned char *pglyph;
    register unsigned char *p;
    unsigned char *allocbuf;

#define __GL_CHAR_BUF_SIZE 2048
    unsigned char buf[__GL_CHAR_BUF_SIZE];

    w = GLYPHWIDTHPIXELS(pci);
    h = GLYPHHEIGHTPIXELS(pci);
    widthPadded = GLYPHWIDTHBYTESPADDED(pci);

    /*
     ** Use the local buf if possible, otherwise malloc.
     */
    allocBytes = widthPadded * h;
    if (allocBytes <= __GL_CHAR_BUF_SIZE) {
        p = buf;
        allocbuf = 0;
    }
    else {
        p = (unsigned char *) malloc(allocBytes);
        if (!p)
            return BadAlloc;
        allocbuf = p;
    }

    /*
     ** We have to reverse the picture, top to bottom
     */

    pglyph = FONTGLYPHBITS(FONTGLYPHS(font), pci) + (h - 1) * widthPadded;
    for (j = 0; j < h; j++) {
        for (i = 0; i < widthPadded; i++) {
            p[i] = pglyph[i];
        }
        pglyph -= widthPadded;
        p += widthPadded;
    }
    glBitmap(w, h, -pci->metrics.leftSideBearing, pci->metrics.descent,
             pci->metrics.characterWidth, 0, allocbuf ? allocbuf : buf);

    free(allocbuf);
    return Success;
#undef __GL_CHAR_BUF_SIZE
}

/*
** Create a GL bitmap for each character in the X font.  The bitmap is stored
** in a display list.
*/

static int
MakeBitmapsFromFont(FontPtr pFont, int first, int count, int list_base)
{
    unsigned long i, nglyphs;
    CARD8 chs[2];               /* the font index we are going after */
    CharInfoPtr pci;
    int rv;                     /* return value */
    int encoding = (FONTLASTROW(pFont) == 0) ? Linear16Bit : TwoD16Bit;

    glPixelStorei(GL_UNPACK_SWAP_BYTES, FALSE);
    glPixelStorei(GL_UNPACK_LSB_FIRST, BITMAP_BIT_ORDER == LSBFirst);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, GLYPHPADBYTES);
    for (i = 0; i < count; i++) {
        chs[0] = (first + i) >> 8;      /* high byte is first byte */
        chs[1] = first + i;

        (*pFont->get_glyphs) (pFont, 1, chs, (FontEncoding) encoding,
                              &nglyphs, &pci);

        /*
         ** Define a display list containing just a glBitmap() call.
         */
        glNewList(list_base + i, GL_COMPILE);
        if (nglyphs) {
            rv = __glXMakeBitmapFromGlyph(pFont, pci);
            if (rv) {
                return rv;
            }
        }
        glEndList();
    }
    return Success;
}

/************************************************************************/

int
__glXDisp_UseXFont(__GLXclientState * cl, GLbyte * pc)
{
    ClientPtr client = cl->client;
    xGLXUseXFontReq *req;
    FontPtr pFont;
    GLuint currentListIndex;
    __GLXcontext *cx;
    int error;

    req = (xGLXUseXFontReq *) pc;
    cx = __glXForceCurrent(cl, req->contextTag, &error);
    if (!cx) {
        return error;
    }

    glGetIntegerv(GL_LIST_INDEX, (GLint *) &currentListIndex);
    if (currentListIndex != 0) {
        /*
         ** A display list is currently being made.  It is an error
         ** to try to make a font during another lists construction.
         */
        client->errorValue = cx->id;
        return __glXError(GLXBadContextState);
    }

    /*
     ** Font can actually be either the ID of a font or the ID of a GC
     ** containing a font.
     */

    error = dixLookupFontable(&pFont, req->font, client, DixReadAccess);
    if (error != Success)
        return error;

    return MakeBitmapsFromFont(pFont, req->first, req->count, req->listBase);
}

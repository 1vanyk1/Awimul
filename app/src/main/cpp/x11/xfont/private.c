#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "replace.h"
#include "../headers/fonts/fontmisc.h"
#include "../headers/fonts/fontstruct.h"

static int _FontPrivateAllocateIndex = 0;

int
xfont2_allocate_font_private_index (void)
{
    return _FontPrivateAllocateIndex++;
}

FontPtr
CreateFontRec (void)
{
    FontPtr pFont;
    int size;

    size = sizeof(FontRec) + (sizeof(pointer) * _FontPrivateAllocateIndex);

    pFont = malloc(size);

    if(pFont) {
        bzero((char*)pFont, size);
        pFont->maxPrivate = _FontPrivateAllocateIndex - 1;
        if(_FontPrivateAllocateIndex)
            pFont->devPrivates = (pointer)(&pFont[1]);
    }

    return pFont;
}

void
DestroyFontRec (FontPtr pFont)
{
    if (pFont->devPrivates && pFont->devPrivates != (pointer)(&pFont[1]))
        free(pFont->devPrivates);
    free(pFont);
}

void
ResetFontPrivateIndex (void)
{
    _FontPrivateAllocateIndex = 0;
}

Bool
xfont2_font_set_private(FontPtr pFont, int n, pointer ptr)
{
    pointer *new;

    if (n > pFont->maxPrivate) {
        if (pFont->devPrivates && pFont->devPrivates != (pointer)(&pFont[1])) {
            new = reallocarray (pFont->devPrivates, (n + 1), sizeof (pointer));
            if (!new)
                return FALSE;
        } else {
            /* omg realloc */
            new = reallocarray (NULL, (n + 1), sizeof (pointer));
            if (!new)
                return FALSE;
            if (pFont->devPrivates)
                memcpy (new, pFont->devPrivates, (pFont->maxPrivate + 1) * sizeof (pointer));
        }
        pFont->devPrivates = new;
        /* zero out new, uninitialized privates */
        while(++pFont->maxPrivate < n)
            pFont->devPrivates[pFont->maxPrivate] = (pointer)0;
    }
    pFont->devPrivates[n] = ptr;
    return TRUE;
}


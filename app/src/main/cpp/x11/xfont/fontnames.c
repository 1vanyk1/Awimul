#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "replace.h"
#include "../headers/fonts/fontmisc.h"
#include "../headers/fonts/fontstruct.h"

void
xfont2_free_font_names(FontNamesPtr pFN)
{
    int         i;

    if (!pFN)
        return;
    for (i = 0; i < pFN->nnames; i++) {
        free(pFN->names[i]);
    }
    free(pFN->names);
    free(pFN->length);
    free(pFN);
}

FontNamesPtr
xfont2_make_font_names_record(unsigned size)
{
    FontNamesPtr pFN;

    pFN = malloc(sizeof(FontNamesRec));
    if (pFN) {
        pFN->nnames = 0;
        pFN->size = size;
        if (size)
        {
            pFN->length = mallocarray(size, sizeof(int));
            pFN->names = mallocarray(size, sizeof(char *));
            if (!pFN->length || !pFN->names) {
                free(pFN->length);
                free(pFN->names);
                free(pFN);
                pFN = (FontNamesPtr) 0;
            }
        }
        else
        {
            pFN->length = 0;
            pFN->names = 0;
        }
    }
    return pFN;
}

int
xfont2_add_font_names_name(FontNamesPtr names,
                           char *name,
                           int length)
{
    int         index = names->nnames;
    char       *nelt;

    nelt = malloc(length + 1);
    if (!nelt)
        return AllocError;
    if (index >= names->size) {
        int         size = names->size << 1;
        int        *nlength;
        char      **nnames;

        if (size == 0)
            size = 8;
        nlength = reallocarray(names->length, size, sizeof(int));
        nnames = reallocarray(names->names, size, sizeof(char *));
        if (nlength && nnames) {
            names->size = size;
            names->length = nlength;
            names->names = nnames;
        } else {
            free(nelt);
            free(nlength);
            free(nnames);
            return AllocError;
        }
    }
    names->length[index] = length;
    names->names[index] = nelt;
    strncpy(nelt, name, length);
    nelt[length] = '\0';
    names->nnames++;
    return Successful;
}

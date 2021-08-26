#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/xosdefs.h"
#include <stdlib.h>
#include "../headers/fonts/fontmisc.h"

#define XK_LATIN1
#include "../headers/keysymdef.h"

extern void BuiltinRegisterFpeFunctions(void);

void
register_fpe_functions (void)
{
    BuiltinRegisterFpeFunctions();
    FontFileRegisterFpeFunctions();
#ifdef XFONT_FC
    fs_register_fpe_functions();
#endif
}

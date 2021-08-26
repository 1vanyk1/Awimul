#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "libxfontint.h"
#include "../headers/fonts/fntfilst.h"
#include "builtin.h"

static int  font_file_type;

static const char builtin_fonts[] = "built-ins";

static int
BuiltinNameCheck (const char *name)
{
    return (strcmp (name, builtin_fonts) == 0);
}

static int
BuiltinInitFPE (FontPathElementPtr fpe)
{
    int			status;
    FontDirectoryPtr	dir;

    status = BuiltinReadDirectory (fpe->name, &dir);

    if (status == Successful)
        fpe->private = (pointer) dir;
    return status;
}

/* ARGSUSED */
static int
BuiltinResetFPE (FontPathElementPtr fpe)
{
    /* builtins can't change! */
    return Successful;
}

static int
BuiltinFreeFPE (FontPathElementPtr fpe)
{
    FontFileFreeDir ((FontDirectoryPtr) fpe->private);
    return Successful;
}

static const xfont2_fpe_funcs_rec builtin_fpe_funcs = {
        .version = XFONT2_FPE_FUNCS_VERSION,
        .name_check = BuiltinNameCheck,
        .init_fpe = BuiltinInitFPE,
        .free_fpe = BuiltinFreeFPE,
        .reset_fpe = BuiltinResetFPE,
        .open_font = FontFileOpenFont,
        .close_font = FontFileCloseFont,
        .list_fonts = FontFileListFonts,
        .start_list_fonts_with_info = FontFileStartListFontsWithInfo,
        .list_next_font_with_info = FontFileListNextFontWithInfo,
        .wakeup_fpe = 0,
        .client_died = 0,
        .load_glyphs = 0,
        .start_list_fonts_and_aliases = 0,
        .list_next_font_or_alias = 0,
        .set_path_hook = 0
};

void
BuiltinRegisterFpeFunctions(void)
{
    BuiltinRegisterFontFileFunctions ();

    font_file_type = register_fpe_funcs(&builtin_fpe_funcs);
}

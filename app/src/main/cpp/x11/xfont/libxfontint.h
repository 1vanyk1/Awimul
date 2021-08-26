#ifndef _LIBXFONTINT_H_
#define _LIBXFONTINT_H_

#include "../headers/fonts/FSproto.h"

#define client_auth_generation __libxfont__client_auth_generation
#define ClientSignal __libxfont__ClientSignal
#define DeleteFontClientID __libxfont__DeleteFontClientID
#define ErrorF __libxfont__ErrorF
#define find_old_font __libxfont__find_old_font
#define GetClientResolutions __libxfont__GetClientResolutions
#define GetDefaultPointSize __libxfont__GetDefaultPointSize
#define GetNewFontClientID __libxfont__GetNewFontClientID
#define GetTimeInMillis  __libxfont__GetTimeInMillis
#define init_fs_handlers __libxfont__init_fs_handlers
#define remove_fs_handlers __libxfont__remove_fs_handlers
#define __GetServerClient __libxfont____GetServerClient
#define set_font_authorizations __libxfont__set_font_authorizations
#define StoreFontClientFont __libxfont__StoreFontClientFont
#define MakeAtom __libxfont__MakeAtom
#define ValidAtom __libxfont__ValidAtom
#define NameForAtom __libxfont__NameForAtom

#define add_fs_fd __libxfont_add_fs_fd
#define remove_fs_fd __libxfont_remove_fs_fd
#define adjust_fs_wait_for_delay __libxfont_adjust_fs_wait_for_delay

#include	"../headers/fonts/FS.h"
#include	"../headers/fonts/FSproto.h"
#include	"../headers/x.h"
#include	"../headers/xos.h"
#include	"../headers/fonts/fontmisc.h"
#include	"../headers/fonts/fontstruct.h"
#include	"../headers/fonts/fontutil.h"
#include	"../headers/fonts/fontproto.h"
#include	<errno.h>
#include	<limits.h>
#include	<stdint.h>

#include "../headers/fonts/libxfont2.h"

#ifndef LIBXFONT_SKIP_ERRORF
void
ErrorF(const char *f, ...)  _X_ATTRIBUTE_PRINTF(1,2);
#endif

FontPtr
find_old_font(FSID id);

unsigned long
GetTimeInMillis (void);

int
register_fpe_funcs(const xfont2_fpe_funcs_rec *funcs);

void *
__GetServerClient(void);

int
set_font_authorizations(char **authorizations, int *authlen, ClientPtr client);

unsigned long
__GetServerGeneration (void);

int add_fs_fd(int fd, FontFdHandlerProcPtr handler, void *data);

void remove_fs_fd(int fd);

void adjust_fs_wait_for_delay(void *wt, unsigned long newdelay);

int
init_fs_handlers2(FontPathElementPtr fpe,
                  FontBlockHandlerProcPtr block_handler);

void
remove_fs_handlers2(FontPathElementPtr fpe,
                    FontBlockHandlerProcPtr blockHandler,
                    Bool all);

Atom
__libxfont_internal__MakeAtom(const char *string, unsigned len, int makeit);

int
__libxfont_internal__ValidAtom(Atom atom);

const char *
__libxfont_internal__NameForAtom(Atom atom);

int
CheckFSFormat(fsBitmapFormat format,
              fsBitmapFormatMask fmask,
              int *bit_order,
              int *byte_order,
              int *scan,
              int *glyph,
              int *image);

int
        FontCouldBeTerminal(FontInfoPtr);

void
        FontComputeInfoAccelerators(FontInfoPtr);

int
add_range (fsRange *newrange, int *nranges, fsRange **range,
           Bool charset_subset);

#endif /* _LIBXFONTINT_H_ */

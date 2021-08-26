#ifndef _FONTPROTO_H
#define _FONTPROTO_H

#include "../xfuncproto.h"
#include "fontstruct.h"

/* Externally provided functions required by libXfont */

extern _X_EXPORT int RegisterFPEFunctions (
        NameCheckFunc name_func,
        InitFpeFunc init_func,
        FreeFpeFunc free_func,
        ResetFpeFunc reset_func,
        OpenFontFunc open_func,
        CloseFontFunc close_func,
        ListFontsFunc list_func,
        StartLfwiFunc start_lfwi_func,
        NextLfwiFunc next_lfwi_func,
        WakeupFpeFunc wakeup_func,
        ClientDiedFunc client_died,
        LoadGlyphsFunc load_glyphs,
        StartLaFunc start_list_alias_func,
        NextLaFunc next_list_alias_func,
        SetPathFunc set_path_func);

extern _X_EXPORT int GetDefaultPointSize ( void );

extern _X_EXPORT int init_fs_handlers ( FontPathElementPtr fpe,
                                        BlockHandlerProcPtr block_handler);
extern _X_EXPORT void remove_fs_handlers ( FontPathElementPtr fpe,
                                           BlockHandlerProcPtr block_handler,
                                           Bool all );

extern _X_EXPORT int client_auth_generation ( ClientPtr client );

#ifndef ___CLIENTSIGNAL_DEFINED___
#define ___CLIENTSIGNAL_DEFINED___
extern Bool ClientSignal ( ClientPtr client );
#endif /* ___CLIENTSIGNAL_DEFINED___ */

extern _X_EXPORT void DeleteFontClientID ( Font id );
extern _X_EXPORT Font GetNewFontClientID ( void );
extern _X_EXPORT int StoreFontClientFont ( FontPtr pfont, Font id );
extern _X_EXPORT void FontFileRegisterFpeFunctions ( void );
extern _X_EXPORT void FontFileCheckRegisterFpeFunctions ( void );

extern Bool XpClientIsBitmapClient ( ClientPtr client );
extern Bool XpClientIsPrintClient( ClientPtr client, FontPathElementPtr fpe );
extern void PrinterFontRegisterFpeFunctions ( void );

extern void fs_register_fpe_functions ( void );
extern void check_fs_register_fpe_functions ( void );

/* util/private.c */
extern FontPtr  CreateFontRec (void);
extern void  DestroyFontRec (FontPtr font);
extern Bool     _FontSetNewPrivate (FontPtr        /* pFont */,
                                    int            /* n */,
                                    void *         /* ptr */);
extern int      AllocateFontPrivateIndex (void);
extern void ResetFontPrivateIndex (void);

/* Type1/t1funcs.c */
extern void Type1RegisterFontFileFunctions(void);
extern void CIDRegisterFontFileFunctions(void);

/* Speedo/spfuncs.c */
extern void SpeedoRegisterFontFileFunctions(void);

/* FreeType/ftfuncs.c */
extern void FreeTypeRegisterFontFileFunctions(void);

#endif
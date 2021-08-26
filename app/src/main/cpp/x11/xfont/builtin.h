#include "../headers/xdefs.h"
#include "../headers/fonts/font.h"
#include "../headers/fonts/fontxlfd.h"
#include "../headers/fonts/fntfil.h"
#include "../headers/fonts/fntfilio.h"
#include "../headers/fonts/fntfilst.h"

typedef struct _BuiltinFile {
    const char  *name;
    int		len;
    const char  *bits;
} BuiltinFileRec, *BuiltinFilePtr;

typedef struct _BuiltinDir {
    const char	*file_name;
    char	*font_name;
} BuiltinDirRec, *BuiltinDirPtr;

typedef struct _BuiltinAlias {
    char	*alias_name;
    char	*font_name;
} BuiltinAliasRec, *BuiltinAliasPtr;

extern const BuiltinFileRec	builtin_files[];
extern const int		builtin_files_count;

extern const BuiltinDirRec	builtin_dir[];
extern const int		builtin_dir_count;

extern const BuiltinAliasRec	builtin_alias[];
extern const int		builtin_alias_count;

extern FontFilePtr	BuiltinFileOpen (const char *);
extern int		BuiltinFileClose (BufFilePtr, int);
extern int BuiltinReadDirectory (const char *, FontDirectoryPtr *);
extern void BuiltinRegisterFontFileFunctions (void);

extern void BuiltinRegisterFpeFunctions (void);

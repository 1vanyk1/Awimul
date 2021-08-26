#ifndef _FONTMISC_H_
#define _FONTMISC_H_

#include "../xfuncs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../xdefs.h"


#ifndef LSBFirst
#define LSBFirst	0
#define MSBFirst	1
#endif

#ifndef None
#define None	0l
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

extern Atom MakeAtom ( const char *string, unsigned len, int makeit );
extern int ValidAtom ( Atom atom );
extern const char *NameForAtom (Atom atom);

#define lowbit(x) ((x) & (~(x) + 1))

#undef assert
#define assert(x)	((void)0)

extern void
BitOrderInvert(
        register unsigned char *,
        register int
);

extern void
TwoByteSwap(
        register unsigned char *,
        register int
);

extern void
FourByteSwap(
        register unsigned char *,
        register int
);

extern int
RepadBitmap (
        char*,
        char*,
        unsigned,
        unsigned,
        int,
        int
);

extern void CopyISOLatin1Lowered(
        char * /*dest*/,
        const char * /*source*/,
        int /*length*/
);

extern void register_fpe_functions(void);

#endif /* _FONTMISC_H_ */

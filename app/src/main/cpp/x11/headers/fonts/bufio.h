#ifndef ___BUFIO_H___
#define ___BUFIO_H___ 1

#include "../xfuncproto.h"

#define BUFFILESIZE	8192
#define BUFFILEEOF	-1

typedef unsigned char BufChar;
typedef struct _buffile *BufFilePtr;

typedef struct _buffile {
    BufChar *bufp;
    int	    left;
    int     eof;
    BufChar buffer[BUFFILESIZE];
    int	    (*input)( BufFilePtr /* f */);
    int     (*output)( int /* c */, BufFilePtr /* f */);
    int	    (*skip)( BufFilePtr /* f */, int /* count */);
    int	    (*close)( BufFilePtr /* f */, int /* doClose */);
    char    *private;
} BufFileRec;

extern BufFilePtr BufFileCreate (
        char*,
        int (*)(BufFilePtr),
        int (*)(int, BufFilePtr),
        int (*)(BufFilePtr, int),
        int (*)(BufFilePtr, int));
extern BufFilePtr BufFileOpenRead ( int );
extern BufFilePtr BufFileOpenWrite ( int );
extern BufFilePtr BufFilePushCompressed ( BufFilePtr );
extern BufFilePtr BufFilePushZIP ( BufFilePtr );
#ifdef X_BZIP2_FONT_COMPRESSION
extern BufFilePtr BufFilePushBZIP2 ( BufFilePtr );
#endif
extern int BufFileClose ( BufFilePtr, int );
extern int BufFileRead ( BufFilePtr, char*, int );
extern int BufFileWrite ( BufFilePtr, const char*, int );

#define BufFileGet(f)	((f)->left-- ? *(f)->bufp++ : ((f)->eof = (*(f)->input) (f)))
#define BufFilePut(c,f)	(--(f)->left ? *(f)->bufp++ = ((unsigned char)(c)) : (*(f)->output) ((unsigned char)(c),f))
#define BufFileSkip(f,c)    ((f)->eof = (*(f)->skip) (f, c))

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif /* ___BUFIO_H___ */


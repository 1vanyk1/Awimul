#ifndef _FNTFILIO_H_
#define _FNTFILIO_H_

#include "bufio.h"

typedef BufFilePtr  FontFilePtr;

#define FontFileGetc(f)	    BufFileGet(f)
#define FontFilePutc(c,f)   BufFilePut(c,f)
#define FontFileRead(f,b,n) BufFileRead(f,b,n)
#define FontFileWrite(f,b,n)	BufFileWrite(f,b,n)
#define FontFileSkip(f,n)   (BufFileSkip (f, n) != BUFFILEEOF)
#define FontFileSeek(f,n)   (BufFileSeek (f,n,0) != BUFFILEEOF)

#define FontFileEOF	BUFFILEEOF

extern FontFilePtr FontFileOpen ( const char *name );
extern int FontFileClose ( FontFilePtr f );
extern FontFilePtr FontFileOpenWrite ( const char *name );
extern FontFilePtr FontFileOpenWriteFd ( int fd );
extern FontFilePtr FontFileOpenFd ( int fd );

#endif /* _FNTFILIO_H_ */

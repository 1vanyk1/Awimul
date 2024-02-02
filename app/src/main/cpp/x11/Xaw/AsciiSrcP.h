#ifndef _XawAsciiSrcP_h
#define _XawAsciiSrcP_h

#include "TextSrcP.h"
#include "AsciiSrc.h"

#ifdef L_tmpnam
#define TMPSIZ L_tmpnam
#else
#ifdef PATH_MAX
#define TMPSIZ PATH_MAX
#else
#define TMPSIZ 1024		/* bytes to allocate for tmpnam */
#endif
#endif

typedef struct _Piece {		/* Piece of the text file of BUFSIZ allocated
				   characters */
    char *text;			/* The text in this buffer */
    XawTextPosition used;	/* The number of characters of this buffer
				   that have been used */
    struct _Piece *prev, *next;	/* linked list pointers */
} Piece;

typedef struct _AsciiSrcClassPart {
    XtPointer extension;
} AsciiSrcClassPart;

/* Full class record */
typedef struct _AsciiSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	text_src_class;
    AsciiSrcClassPart	ascii_src_class;
} AsciiSrcClassRec;

extern AsciiSrcClassRec asciiSrcClassRec;

/* New fields for the AsciiSrc object */
typedef struct _AsciiSrcPart {
    /* resources */
    char *string;		/* either the string, or the
				   file name, depending upon the type */
    XawAsciiType type;		/* either string or disk */
    XawTextPosition piece_size;	/* Size of text buffer for each piece */
    Boolean data_compression;	/* compress to minimum memory automatically
				   on save? */
#ifdef OLDXAW
    XtCallbackList callback;
#endif
    Boolean use_string_in_place;/* Use the string passed in place */
    int ascii_length;		/* length field for ascii string emulation */

#ifdef ASCII_DISK
    String filename;		/* name of file for Compatability */
#endif /* ASCII_DISK */

    /* private */
    Boolean is_tempfile;	/* Is this a temporary file? */
#ifdef OLDXAW
    Boolean changes;
#endif
    Boolean allocated_string;	/* Have I allocated the
				   string in ascii_src->string? */
    XawTextPosition length;	/* length of file */
    Piece *first_piece;		/* first piece of the text */
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} AsciiSrcPart;

/* instance record */
typedef struct _AsciiSrcRec {
    ObjectPart    object;
    TextSrcPart   text_src;
    AsciiSrcPart  ascii_src;
} AsciiSrcRec;

#endif /* _XawAsciiSrcP_h */

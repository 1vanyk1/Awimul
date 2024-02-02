#ifndef _XawMultiSrcP_h
#define _XawMultiSrcP_h

#include "../headers/xfuncproto.h"
#include "TextSrcP.h"
#include "MultiSrc.h"

#ifdef L_tmpnam
#define TMPSIZ L_tmpnam
#else
#ifdef PATH_MAX
#define TMPSIZ PATH_MAX
#else
#define TMPSIZ 1024		/* bytes to allocate for tmpnam */
#endif
#endif

typedef struct _MultiPiece {	/* Piece of the text file of BUFSIZ allocated
				   characters */
    wchar_t* text;		/* The text in this buffer */
    XawTextPosition used;	/* The number of characters of this buffer
				   that have been used */
    struct _MultiPiece *prev, *next;	/* linked list pointers */
} MultiPiece;

/* New fields for the MultiSrc object class */
typedef struct _MultiSrcClassPart {
    XtPointer extension;
} MultiSrcClassPart;

/* Full class record */
typedef struct _MultiSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	text_src_class;
    MultiSrcClassPart	multi_src_class;
} MultiSrcClassRec;

extern MultiSrcClassRec multiSrcClassRec;

/* New fields for the MultiSrc object */
typedef struct _MultiSrcPart {
    /* resources */
    XIC ic;			/* for X Input Method */
    XtPointer string;		/* either the string, or the file name, depend-
				   ing upon the `type'.  ALWAYS IN MB FORMAT */
    XawAsciiType type;		/* either string or disk */
    XawTextPosition piece_size;	/* Size of text buffer for each piece */
    Boolean data_compression;	/* compress to minimum memory automatically
				   on save? */
#ifdef OLDXAW
    XtCallbackList callback;
#endif
    Boolean use_string_in_place;/* Use the string passed in place */
    int multi_length;		/* length field for multi string emulation */

    /* private */

    Boolean is_tempfile;	  /* Is this a temporary file? */
#ifdef OLDXAW
    Boolean changes;
#endif
    Boolean allocated_string;	/* Have I allocated the
				   string in multi_src->string? */
    XawTextPosition length;	/* length of file - IN CHARACTERS, NOT BYTES */
    MultiPiece *first_piece;	/* first piece of the text */
#ifndef OLDXAW
    XtPointer pad[4];	/* for future use and keep binary compatability */
#endif
} MultiSrcPart;

/* Full instance record */
typedef struct _MultiSrcRec {
    ObjectPart    object;
    TextSrcPart	text_src;
    MultiSrcPart	multi_src;
} MultiSrcRec;

_XFUNCPROTOBEGIN

void _XawMultiSourceFreeString
        (
                Widget		w
        );

_XFUNCPROTOEND

#endif /* _XawMultiSrcP_h */

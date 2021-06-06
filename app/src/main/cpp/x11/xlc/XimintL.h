#ifndef _XIMINTL_H
#define _XIMINTL_H

#define	COMPOSE_FILE	"Compose"

/*
 * Data Structures for Local Processing
 */
typedef INT32  DTIndex;
typedef INT32  DTCharIndex;
typedef BITS32 DTModifier;

typedef struct _DefTree {
    DTIndex          next;
    DTIndex          succession;	/* successive Key Sequence */
    /* Key definitions */
    DTModifier       modifier_mask;
    DTModifier       modifier;
    KeySym           keysym;		/* leaf only */
    DTCharIndex      mb;
    DTCharIndex      wc;		/* make from mb */
    DTCharIndex      utf8;		/* make from mb */
    KeySym           ks;
} DefTree;

typedef struct _DefTreeBase {
    DefTree         *tree;
    char            *mb;
    wchar_t         *wc;
    char            *utf8;
    DTIndex          treeused, treesize;
    DTCharIndex      mbused,   mbsize;
    DTCharIndex      wcused,   wcsize;
    DTCharIndex      utf8used, utf8size;
} DefTreeBase;

typedef struct _XimLocalPrivateRec {
    /* The first fields are identical with XimCommonPrivateRec. */
    XlcConv		 ctom_conv;
    XlcConv		 ctow_conv;
    XlcConv		 ctoutf8_conv;
    XlcConv		 cstomb_conv;
    XlcConv		 cstowc_conv;
    XlcConv		 cstoutf8_conv;
    XlcConv		 ucstoc_conv;
    XlcConv		 ucstoutf8_conv;

    XIC		 current_ic;
    DefTreeBase	 base;
    DTIndex          top;
} XimLocalPrivateRec;

typedef struct _XicThaiPart {
    int		comp_state;
    KeySym		keysym;
    int		input_mode;
} XicThaiPart;

typedef struct _XicLocalPrivateRec {
    long			 value_mask;
    DefTreeBase              base;
    DTIndex			 context;
    DTIndex			 composed;
    XicThaiPart		 thai;

    XIMResourceList		 ic_resources;
    unsigned int		 ic_num_resources;

    unsigned char	 	 brl_pressed, brl_committing, brl_committed;
    Time		 	 brl_release_start;
} XicLocalPrivateRec;
#endif /* _XIMINTL_H */
typedef enum {
    HEAD,       /* i: waiting for magic header */
    FLAGS,      /* i: waiting for method and flags (gzip) */
    TIME,       /* i: waiting for modification time (gzip) */
    OS,         /* i: waiting for extra flags and operating system (gzip) */
    EXLEN,      /* i: waiting for extra length (gzip) */
    EXTRA,      /* i: waiting for extra bytes (gzip) */
    NAME,       /* i: waiting for end of file name (gzip) */
    COMMENT,    /* i: waiting for end of comment (gzip) */
    HCRC,       /* i: waiting for header crc (gzip) */
    DICTID,     /* i: waiting for dictionary check value */
    DICT,       /* waiting for inflateSetDictionary() call */
    TYPE,       /* i: waiting for type bits, including last-flag bit */
    TYPEDO,     /* i: same, but skip check to exit inflate on new block */
    STORED,     /* i: waiting for stored size (length and complement) */
    COPY_,      /* i/o: same as COPY below, but only first time in */
    COPY,       /* i/o: waiting for input or output to copy stored block */
    TABLE,      /* i: waiting for dynamic block table lengths */
    LENLENS,    /* i: waiting for code length code lengths */
    CODELENS,   /* i: waiting for length/lit and distance code lengths */
    LEN_,       /* i: same as LEN below, but only first time in */
    LEN,        /* i: waiting for length/lit/eob code */
    LENEXT,     /* i: waiting for length extra bits */
    DIST,       /* i: waiting for distance code */
    DISTEXT,    /* i: waiting for distance extra bits */
    MATCH,      /* o: waiting for output space to copy string */
    LIT,        /* o: waiting for output space to write literal */
    CHECK,      /* i: waiting for 32-bit check value */
    LENGTH,     /* i: waiting for 32-bit length (gzip) */
    DONE,       /* finished check, done -- remain here until reset */
    BAD,        /* got a data error -- remain here until reset */
    MEM,        /* got an inflate() memory error -- remain here until reset */
    SYNC        /* looking for synchronization bytes to restart inflate() */
} inflate_mode;

/*
    State transitions between above modes -

    (most modes can go to BAD or MEM on error -- not shown for clarity)

    Process header:
        HEAD -> (gzip) or (zlib) or (raw)
        (gzip) -> FLAGS -> TIME -> OS -> EXLEN -> EXTRA -> NAME -> COMMENT ->
                  HCRC -> TYPE
        (zlib) -> DICTID or TYPE
        DICTID -> DICT -> TYPE
        (raw) -> TYPEDO
    Read deflate blocks:
            TYPE -> TYPEDO -> STORED or TABLE or LEN_ or CHECK
            STORED -> COPY_ -> COPY -> TYPE
            TABLE -> LENLENS -> CODELENS -> LEN_
            LEN_ -> LEN
    Read deflate codes in fixed or dynamic block:
                LEN -> LENEXT or LIT or TYPE
                LENEXT -> DIST -> DISTEXT -> MATCH -> LEN
                LIT -> LEN
    Process trailer:
        CHECK -> LENGTH -> DONE
 */

/* state maintained between inflate() calls.  Approximately 10K bytes. */
struct inflate_state {
    inflate_mode mode;          /* current inflate mode */
    int last;                   /* true if processing last block */
    int wrap;                   /* bit 0 true for zlib, bit 1 true for gzip */
    int havedict;               /* true if dictionary provided */
    int flags;                  /* gzip header method and flags (0 if zlib) */
    unsigned int dmax;          /* zlib header max distance */
    unsigned long check;        /* protected copy of check value */
    unsigned long total;        /* protected copy of output count */
    gz_header *head;            /* where to save gzip header information */
    /* sliding window */
    unsigned int wbits;         /* log base 2 of requested window size */
    unsigned int wsize;         /* window size or zero if not using window */
    unsigned int whave;         /* valid bytes in the window */
    unsigned int wnext;         /* window write index */
    unsigned char *window;      /* allocated sliding window, if needed */
    /* bit accumulator */
    unsigned long hold;         /* input bit accumulator */
    unsigned int bits;          /* number of bits in "in" */
    /* for string and stored block copying */
    unsigned int length;        /* literal or length of data to copy */
    unsigned int offset;        /* distance back to copy string from */
    /* for table and code decoding */
    unsigned int extra;         /* extra bits needed */
    /* fixed and dynamic code tables */
    const code *lencode;        /* starting table for length/literal codes */
    const code *distcode;       /* starting table for distance codes */
    unsigned int lenbits;       /* index bits for lencode */
    unsigned int distbits;      /* index bits for distcode */
    /* dynamic table building */
    unsigned int ncode;         /* number of code length code lengths */
    unsigned int nlen;          /* number of length code lengths */
    unsigned int ndist;         /* number of distance code lengths */
    unsigned int have;          /* number of code lengths in lens[] */
    code *next;             /* next available space in codes[] */
    unsigned short lens[320];   /* temporary storage for code lengths */
    unsigned short work[288];   /* work area for code table building */
    code codes[ENOUGH];         /* space for code tables */
    int sane;                   /* if false, allow invalid distance too far */
    int back;                   /* bits back of last unprocessed length/lit */
    unsigned int was;           /* initial length of match */
};

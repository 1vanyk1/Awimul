#ifndef EXPR_H
#define EXPR_H 1

typedef union _ExprResult
{
    char *str;
    int ival;
    unsigned uval;
    XkbKeyNameRec keyName;
} ExprResult;

typedef Bool(*IdentLookupFunc) (XPointer /* priv */ ,
                                Atom /* elem */ ,
                                Atom /* field */ ,
                                unsigned /* type */ ,
                                ExprResult *    /* val_rtrn */
);

extern char *exprTypeText(unsigned      /* type */
);

extern int ExprResolveLhs(ExprDef * /* expr */ ,
                          ExprResult * /* elem_rtrn */ ,
                          ExprResult * /* field_rtrn */ ,
                          ExprDef **    /* index_rtrn */
);

typedef struct _LookupPriv
{
    XPointer priv;
    IdentLookupFunc chain;
    XPointer chainPriv;
} LookupPriv;

typedef struct _LookupEntry
{
    const char *name;
    unsigned result;
} LookupEntry;

typedef struct _LookupTable
{
    char *element;
    LookupEntry *entries;
    struct _LookupTable *nextElement;
} LookupTable;


extern char *exprOpText(unsigned        /* type */
);

extern int RadioLookup(XPointer /* priv */ ,
                       Atom /* elem */ ,
                       Atom /* field */ ,
                       unsigned /* type */ ,
                       ExprResult *     /* val_rtrn */
);

extern int SimpleLookup(XPointer /* priv */ ,
                        Atom /* elem */ ,
                        Atom /* field */ ,
                        unsigned /* type */ ,
                        ExprResult *    /* val_rtrn */
);

extern int TableLookup(XPointer /* priv */ ,
                       Atom /* elem */ ,
                       Atom /* field */ ,
                       unsigned /* type */ ,
                       ExprResult *     /* val_rtrn */
);

extern int LookupModIndex(XPointer /* priv */ ,
                          Atom /* elem */ ,
                          Atom /* field */ ,
                          unsigned /* type */ ,
                          ExprResult *  /* val_rtrn */
);

extern int LookupModMask(XPointer /* priv */ ,
                         Atom /* elem */ ,
                         Atom /* field */ ,
                         unsigned /* type */ ,
                         ExprResult *   /* val_rtrn */
);

extern int ExprResolveModIndex(ExprDef * /* expr */ ,
                               ExprResult * /* val_rtrn */ ,
                               IdentLookupFunc /* lookup */ ,
                               XPointer /* lookupPriv */
);

extern int ExprResolveModMask(ExprDef * /* expr */ ,
                              ExprResult * /* val_rtrn */ ,
                              IdentLookupFunc /* lookup */ ,
                              XPointer  /* priv */
);

extern int ExprResolveBoolean(ExprDef * /* expr */ ,
                              ExprResult * /* val_rtrn */ ,
                              IdentLookupFunc /* lookup */ ,
                              XPointer  /* lookupPriv */
);

extern int ExprResolveInteger(ExprDef * /* expr */ ,
                              ExprResult * /* val_rtrn */ ,
                              IdentLookupFunc /* lookup */ ,
                              XPointer  /* lookupPriv */
);

extern int ExprResolveFloat(ExprDef * /* expr */ ,
                            ExprResult * /* val_rtrn */ ,
                            IdentLookupFunc /* lookup */ ,
                            XPointer    /* lookupPriv */
);

extern int ExprResolveString(ExprDef * /* expr */ ,
                             ExprResult * /* val_rtrn */ ,
                             IdentLookupFunc /* lookup */ ,
                             XPointer   /* lookupPriv */
);

extern int ExprResolveKeyName(ExprDef * /* expr */ ,
                              ExprResult * /* val_rtrn */ ,
                              IdentLookupFunc /* lookup */ ,
                              XPointer  /* lookupPriv */
);

extern int ExprResolveEnum(ExprDef * /* expr */ ,
                           ExprResult * /* val_rtrn */ ,
                           LookupEntry *        /* values */
);

extern int ExprResolveMask(ExprDef * /* expr */ ,
                           ExprResult * /* val_rtrn */ ,
                           IdentLookupFunc /* lookup */ ,
                           XPointer     /* lookupPriv */
);

extern int ExprResolveKeySym(ExprDef * /* expr */ ,
                             ExprResult * /* val_rtrn */ ,
                             IdentLookupFunc /* lookup */ ,
                             XPointer   /* lookupPriv */
);

#endif /* EXPR_H */

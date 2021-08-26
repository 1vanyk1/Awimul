#ifndef VMOD_H
#define VMOD_H 1

typedef struct _VModInfo
{
    XkbDescPtr xkb;
    unsigned defined;
    unsigned available;
    unsigned newlyDefined;
    int errorCount;
} VModInfo;

extern void InitVModInfo(VModInfo * /* info */ ,
                         XkbDescPtr     /* xkb */
);

extern void ClearVModInfo(VModInfo * /* info */ ,
                          XkbDescPtr    /* xkb */
);

extern Bool HandleVModDef(VModDef * /* stmt */ ,
                          unsigned /* mergeMode */ ,
                          VModInfo *    /* info */
);

extern Bool ApplyVModDefs(VModInfo * /* info */ ,
                          XkbDescPtr    /* xkb */
);

extern int LookupVModIndex(XPointer /* priv */ ,
                           Atom /* elem */ ,
                           Atom /* field */ ,
                           unsigned /* type */ ,
                           ExprResult * /* val_rtrn */
);

extern int LookupVModMask(XPointer /* priv */ ,
                          Atom /* elem */ ,
                          Atom /* field */ ,
                          unsigned /* type */ ,
                          ExprResult *  /* val_rtrn */
);

extern int FindKeypadVMod(XkbDescPtr    /* xkb */
);

extern Bool ResolveVirtualModifier(ExprDef * /* def */ ,
                                   ExprResult * /* value_rtrn */ ,
                                   VModInfo *   /* info */
);

#endif /* VMOD_H */

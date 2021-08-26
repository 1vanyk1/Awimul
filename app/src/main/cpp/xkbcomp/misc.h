#ifndef XKBCOMP_MISC_H
#define XKBCOMP_MISC_H 1

typedef struct _CommonInfo
{
    unsigned short defined;
    unsigned char fileID;
    unsigned char merge;
    struct _CommonInfo *next;
} CommonInfo;

extern Bool UseNewField(unsigned /* field */ ,
                        CommonInfo * /* oldDefs */ ,
                        CommonInfo * /* newDefs */ ,
                        unsigned *      /* pCollide */
);

extern Bool MergeNewField(unsigned /* field */ ,
                          CommonInfo * /* oldDefs */ ,
                          CommonInfo * /* newDefs */ ,
                          unsigned *    /* pCollide */
);

extern XPointer ClearCommonInfo(CommonInfo *    /* cmn */
);

extern XPointer AddCommonInfo(CommonInfo * /* old */ ,
                              CommonInfo *      /* new */
);

extern int ReportNotArray(const char * /* type */ ,
                          const char * /* field */ ,
                          const char *  /* name */
);

extern int ReportShouldBeArray(const char * /* type */ ,
                               const char * /* field */ ,
                               char *   /* name */
);

extern int ReportBadType(const char * /* type */ ,
                         const char * /* field */ ,
                         const char * /* name */ ,
                         const char *   /* wanted */
);

extern int ReportBadIndexType(char * /* type */ ,
                              char * /* field */ ,
                              char * /* name */ ,
                              char *    /* wanted */
);

extern int ReportBadField(const char * /* type */ ,
                          const char * /* field */ ,
                          const char *  /* name */
);

extern int ReportMultipleDefs(char * /* type */ ,
                              char * /* field */ ,
                              char *    /* which */
);

extern Bool ProcessIncludeFile(IncludeStmt * /* stmt */ ,
                               unsigned /* file_type */ ,
                               XkbFile ** /* file_rtrn */ ,
                               unsigned *       /* merge_rtrn */
);

extern Status ComputeKbdDefaults(XkbDescPtr     /* xkb */
);

extern Bool FindNamedKey(XkbDescPtr /* xkb */ ,
                         unsigned long /* name */ ,
                         unsigned int * /* kc_rtrn */ ,
                         Bool /* use_aliases */ ,
                         Bool /* create */ ,
                         int    /* start_from */
);

extern Bool FindKeyNameForAlias(XkbDescPtr /* xkb */ ,
                                unsigned long /* lname */ ,
                                unsigned long * /* real_name */
);

#endif /* XKBCOMP_MISC_H */

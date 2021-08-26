#ifndef XKBPARSE_H
#define	XKBPARSE_H 1

#ifndef DEBUG_VAR
#define	DEBUG_VAR	parseDebug
#endif

#include "xkbcomp.h"

extern char scanBuf[1024];
extern int scanInt;
extern int lineNum;

extern XkbFile *rtrnValue;

#ifdef DEBUG
#define	d(str)		fprintf(stderr,"%s\n",str);
#define d1(str,a)	fprintf(stderr,str,a);
#define d2(str,a,b)	fprintf(stderr,str,a,b);
#else
#define	d(str)
#define	d1(str,a)
#define d2(str,a,b)
#endif


extern ParseCommon *AppendStmt(ParseCommon * /* to */ ,
                               ParseCommon *    /* append */
);

extern ExprDef *ExprCreate(unsigned /* op */ ,
                           unsigned     /* type */
);

extern ExprDef *ExprCreateUnary(unsigned /* op */ ,
                                unsigned /* type */ ,
                                ExprDef *       /* child */
);

extern ExprDef *ExprCreateBinary(unsigned /* op */ ,
                                 ExprDef * /* left */ ,
                                 ExprDef *      /* right */
);

extern KeycodeDef *KeycodeCreate(char * /* name */ ,
                                 ExprDef *      /* value */
);

extern KeyAliasDef *KeyAliasCreate(char * /* alias */ ,
                                   char *       /* real */
);

extern VModDef *VModCreate(Atom /* name */ ,
                           ExprDef *    /* value */
);

extern VarDef *VarCreate(ExprDef * /* name */ ,
                         ExprDef *      /* value */
);

extern VarDef *BoolVarCreate(Atom /* nameToken */ ,
                             unsigned   /* set */
);

extern InterpDef *InterpCreate(const char * /* sym_str */ ,
                               ExprDef *        /* match */
);

extern KeyTypeDef *KeyTypeCreate(Atom /* name */ ,
                                 VarDef *       /* body */
);

extern SymbolsDef *SymbolsCreate(char * /* keyName */ ,
                                 ExprDef *      /* symbols */
);

extern GroupCompatDef *GroupCompatCreate(int /* group */ ,
                                         ExprDef *      /* def */
);

extern ModMapDef *ModMapCreate(Atom /* modifier */ ,
                               ExprDef *        /* keys */
);

extern IndicatorMapDef *IndicatorMapCreate(Atom /* name */ ,
                                           VarDef *     /* body */
);

extern IndicatorNameDef *IndicatorNameCreate(int /* ndx */ ,
                                             ExprDef * /* name */ ,
                                             Bool       /* virtual */
);

extern ExprDef *ActionCreate(Atom /* name */ ,
                             ExprDef *  /* args */
);

extern ExprDef *CreateKeysymList(char * /* sym */
);

extern ShapeDef *ShapeDeclCreate(Atom /* name */ ,
                                 OutlineDef *   /* outlines */
);

extern OutlineDef *OutlineCreate(Atom /* field */ ,
                                 ExprDef *      /* points */
);

extern KeyDef *KeyDeclCreate(char * /* name */ ,
                             ExprDef *  /* expr */
);

extern KeyDef *KeyDeclMerge(KeyDef * /* into */ ,
                            KeyDef *    /* from */
);

extern RowDef *RowDeclCreate(KeyDef *   /* keys */
);

extern SectionDef *SectionDeclCreate(Atom /* name */ ,
                                     RowDef *   /* rows */
);

extern OverlayKeyDef *OverlayKeyCreate(char * /* under */ ,
                                       char *   /* over  */
);

extern OverlayDef *OverlayDeclCreate(Atom /* name */ ,
                                     OverlayKeyDef *    /* rows */
);

extern DoodadDef *DoodadCreate(unsigned /* type */ ,
                               Atom /* name */ ,
                               VarDef * /* body */
);

extern ExprDef *AppendKeysymList(ExprDef * /* list */ ,
                                 char * /* sym */
);

extern int LookupKeysym(const char * /* str */ ,
                        KeySym *        /* sym_rtrn */
);

extern IncludeStmt *IncludeCreate(char * /* str */ ,
                                  unsigned      /* merge */
);

extern unsigned StmtSetMerge(ParseCommon * /* stmt */ ,
                             unsigned   /* merge */
);

#ifdef DEBUG
extern void PrintStmtAddrs(ParseCommon *        /* stmt */
    );
#endif

extern int XKBParseFile(FILE * /* file */ ,
                        XkbFile **      /* pRtrn */
);

extern XkbFile *CreateXKBFile(int /* type */ ,
                              char * /* name */ ,
                              ParseCommon * /* defs */ ,
                              unsigned  /* flags */
);

extern void yyerror2(const char *        /* s */
);

extern int yywrap(void);

extern int yylex(void);
extern int yyparse2(void);
extern void scan_set_file(FILE *file);

extern int setScanState(char * /* file */ ,
                        int     /* line */
);

#endif /* XKBPARSE_H */

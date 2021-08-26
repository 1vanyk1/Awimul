#ifndef ALIAS_H
#define ALIAS_H 1

typedef struct _AliasInfo
{
    CommonInfo def;
    char alias[XkbKeyNameLength + 1];
    char real[XkbKeyNameLength + 1];
} AliasInfo;

extern int HandleAliasDef(KeyAliasDef * /* def   */ ,
                          unsigned /* merge */ ,
                          unsigned /* file_id */ ,
                          AliasInfo **  /* info  */
);

extern void ClearAliases(AliasInfo **   /* info */
);

extern Bool MergeAliases(AliasInfo ** /* into */ ,
                         AliasInfo ** /* merge */ ,
                         unsigned       /* how_merge */
);

extern int ApplyAliases(XkbDescPtr /* xkb */ ,
                        Bool /* toGeom */ ,
                        AliasInfo **    /* info */
);

#endif /* ALIAS_H */

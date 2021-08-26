#ifndef _XKBPATH_H_
#define _XKBPATH_H_ 1

extern Bool XkbInitIncludePath(void);

extern void XkbClearIncludePath(void);

extern void XkbAddDefaultDirectoriesToPath(void);

extern Bool XkbAddDirectoryToPath(const char *  /* dir */
);

extern char *XkbDirectoryForInclude(unsigned    /* type */
);

extern FILE *XkbFindFileInPath(char * /* name */ ,
                               unsigned /* type */ ,
                               char **  /* pathRtrn */
);

extern void *XkbAddFileToCache(char * /* name */ ,
                               unsigned /* type */ ,
                               char * /* path */ ,
                               void *   /* data */
);

extern void *XkbFindFileInCache(char * /* name */ ,
                                unsigned /* type */ ,
                                char ** /* pathRtrn */
);

extern Bool XkbParseIncludeMap(char ** /* str_inout */ ,
                               char ** /* file_rtrn */ ,
                               char ** /* map_rtrn */ ,
                               char * /* nextop_rtrn */ ,
                               char **  /* extra_data */
);

#endif /* _XKBPATH_H_ */

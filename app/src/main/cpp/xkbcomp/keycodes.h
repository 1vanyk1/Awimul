#ifndef KEYCODES_H
#define KEYCODES_H 1

#define	KeyNameToLong(n)	((((unsigned long)n[0])<<24)|(((unsigned long)n[1])<<16)|(((unsigned long)n[2])<<8)|n[3])

extern char *longText(unsigned long /* val */ ,
                      unsigned  /* format */
);

extern void LongToKeyName(unsigned long /* val */ ,
                          char *        /* name_rtrn */
);

#endif /* KEYCODES_H */

#ifndef UTILS_H
#define	UTILS_H 1

#include 	<stdio.h>
#include	"../x11/headers/xos.h"
#include	"../x11/headers/xfuncproto.h"
#include	"../x11/headers/xfuncs.h"

#include <stddef.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef NUL
#define	NUL	'\0'
#endif

/***====================================================================***/

#ifndef OPAQUE_DEFINED
typedef void *Opaque;
#endif
#ifndef NullOpaque
#define	NullOpaque	((Opaque)NULL)
#endif

#ifndef BOOLEAN_DEFINED
typedef char Boolean;
#endif

#ifndef True
#define	True	((Boolean)1)
#define	False	((Boolean)0)
#endif /* ndef True */
#define	booleanText(b)	((b)?"True":"False")

#ifndef COMPARISON_DEFINED
typedef int Comparison;

#define	Greater		((Comparison)1)
#define	Equal		((Comparison)0)
#define	Less		((Comparison)-1)
#define	CannotCompare	((Comparison)-37)
#define	comparisonText(c)	((c)?((c)<0?"Less":"Greater"):"Equal")
#endif

/***====================================================================***/

extern Opaque uAlloc(unsigned   /* size */
);
extern Opaque uCalloc(unsigned /* n */ ,
                      unsigned  /* size */
);
extern Opaque uRealloc(Opaque /* old */ ,
                       unsigned /* newSize */
);
extern Opaque uRecalloc(Opaque /* old */ ,
                        unsigned /* nOld */ ,
                        unsigned /* nNew */ ,
                        unsigned        /* newSize */
);
extern void uFree(Opaque        /* ptr */
);

#define	uTypedAlloc(t)		((t *)uAlloc((unsigned)sizeof(t)))
#define	uTypedCalloc(n,t)	((t *)uCalloc((unsigned)n,(unsigned)sizeof(t)))
#define	uTypedRealloc(pO,n,t)	((t *)uRealloc((Opaque)pO,((unsigned)n)*sizeof(t)))
#define	uTypedRecalloc(pO,o,n,t) ((t *)uRecalloc((Opaque)pO,((unsigned)o),((unsigned)n),sizeof(t)))
#if (defined mdHasAlloca) && (mdHasAlloca)
#define	uTmpAlloc(n)	((Opaque)alloca((unsigned)n))
#define	uTmpFree(p)
#else
#define	uTmpAlloc(n)	uAlloc(n)
#define	uTmpFree(p)	uFree(p)
#endif

/***====================================================================***/

extern Boolean uSetErrorFile(char *     /* name */
);

#define INFO 			ALOGI

extern void
uInformation(const char * /* s */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

#define ACTION			ALOGI

extern void uAction(const char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

#define WARN			ALOGW

extern void uWarning(const char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

#define ERROR			ALOGE

extern void uError(const char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

#define FATAL			ALOGE

extern void uFatalError(const char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2) _X_NORETURN;

/* WSGO stands for "Weird Stuff Going On" */
#define WSGO			uInternalError

extern void uInternalError(const char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

extern void uSetPreErrorMessage(char *     /* msg */
);

extern void uSetPostErrorMessage(char *    /* msg */
);

extern void uSetErrorPrefix(char * /* void */
);

extern void uFinishUp(void);


/***====================================================================***/

#define	NullString	((char *)NULL)

#define	uStringText(s)		((s)==NullString?"<NullString>":(s))
#define	uStringEqual(s1,s2)	(uStringCompare(s1,s2)==Equal)
#define	uStringPrefix(p,s)	(strncmp(p,s,strlen(p))==0)
#define	uStringCompare(s1,s2)	(((s1)==NullString||(s2)==NullString)?\
                                 (s1)!=(s2):strcmp(s1,s2))
#define	uStrCaseEqual(s1,s2)	(uStrCaseCmp(s1,s2)==0)
#ifdef HAVE_STRCASECMP
#include <strings.h>
#define	uStrCaseCmp(s1,s2)	(strcasecmp(s1,s2))
#define	uStrCasePrefix(p,s)	(strncasecmp(p,s,strlen(p))==0)
#else
extern int uStrCaseCmp(const char * /* s1 */ ,
                       const char *        /* s2 */
);
extern int uStrCasePrefix(const char * /* p */ ,
                          char *   /* str */
);
#endif
#ifdef HAVE_STRDUP
#include <string.h>
#define	uStringDup(s1)		((s1) ? strdup(s1) : NULL)
#else
extern char *uStringDup(const char *       /* s1 */
);
#endif

/***====================================================================***/

#ifndef DEBUG_VAR
#define	DEBUG_VAR	debugFlags
#endif

extern
unsigned int DEBUG_VAR;

extern void uDebug(char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

extern void uDebugNOI(     /* no indent */
        char * /* s  */ , ...
) _X_ATTRIBUTE_PRINTF(1, 2);

extern Boolean uSetDebugFile(char *name);

extern FILE *uDebugFile;
extern int uDebugIndentLevel;
extern int uDebugIndentSize;
#define	uDebugIndent(l)		(uDebugIndentLevel+=(l))
#define	uDebugOutdent(l)	(uDebugIndentLevel-=(l))
#ifdef DEBUG
#define	uDEBUG(f,s)		{ if (DEBUG_VAR&(f)) uDebug(s);}
#define	uDEBUG1(f,s,a)		{ if (DEBUG_VAR&(f)) uDebug(s,a);}
#define	uDEBUG2(f,s,a,b)	{ if (DEBUG_VAR&(f)) uDebug(s,a,b);}
#define	uDEBUG3(f,s,a,b,c)	{ if (DEBUG_VAR&(f)) uDebug(s,a,b,c);}
#define	uDEBUG4(f,s,a,b,c,d)	{ if (DEBUG_VAR&(f)) uDebug(s,a,b,c,d);}
#define	uDEBUG5(f,s,a,b,c,d,e)	{ if (DEBUG_VAR&(f)) uDebug(s,a,b,c,d,e);}
#define	uDEBUG_NOI(f,s)		{ if (DEBUG_VAR&(f)) uDebug(s);}
#define	uDEBUG_NOI1(f,s,a)	{ if (DEBUG_VAR&(f)) uDebugNOI(s,a);}
#define	uDEBUG_NOI2(f,s,a,b)	{ if (DEBUG_VAR&(f)) uDebugNOI(s,a,b);}
#define	uDEBUG_NOI3(f,s,a,b,c)	{ if (DEBUG_VAR&(f)) uDebugNOI(s,a,b,c);}
#define	uDEBUG_NOI4(f,s,a,b,c,d) { if (DEBUG_VAR&(f)) uDebugNOI(s,a,b,c,d);}
#define	uDEBUG_NOI5(f,s,a,b,c,d,e) { if (DEBUG_VAR&(f)) uDebugNOI(s,a,b,c,d,e);}
#else
#define	uDEBUG(f,s)
#define	uDEBUG1(f,s,a)
#define	uDEBUG2(f,s,a,b)
#define	uDEBUG3(f,s,a,b,c)
#define	uDEBUG4(f,s,a,b,c,d)
#define	uDEBUG5(f,s,a,b,c,d,e)
#define	uDEBUG_NOI(f,s)
#define	uDEBUG_NOI1(f,s,a)
#define	uDEBUG_NOI2(f,s,a,b)
#define	uDEBUG_NOI3(f,s,a,b,c)
#define	uDEBUG_NOI4(f,s,a,b,c,d)
#define	uDEBUG_NOI5(f,s,a,b,c,d,e)
#endif

#endif /* UTILS_H */

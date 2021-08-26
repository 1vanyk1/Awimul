#ifndef _XKBFILE_H_2
#define	_XKBFILE_H_2 1

#include "xkbstr.h"
#include "xlocale.h"
#include "x.h"
#include "xdefs.h"
#include <stdio.h>

/***====================================================================***/

#define	XkbXKMFile	0
#define	XkbCFile	1
#define	XkbXKBFile	2
#define	XkbMessage	3

#define	XkbMapDefined		(1<<0)
#define	XkbStateDefined		(1<<1)

typedef void (*XkbFileAddOnFunc2) (FILE * /* file */ ,
                                  XkbDescPtr /* result */ ,
                                  Bool /* topLevel */ ,
                                  Bool /* showImplicit */ ,
                                  int /* fileSection */ ,
                                  void *        /* priv */
);

/***====================================================================***/

#define	_XkbSuccess			0
#define	_XkbErrMissingNames		1
#define	_XkbErrMissingTypes		2
#define	_XkbErrMissingReqTypes		3
#define	_XkbErrMissingSymbols		4
#define	_XkbErrMissingVMods		5
#define	_XkbErrMissingIndicators	6
#define	_XkbErrMissingCompatMap		7
#define	_XkbErrMissingSymInterps	8
#define	_XkbErrMissingGeometry		9
#define	_XkbErrIllegalDoodad		10
#define	_XkbErrIllegalTOCType		11
#define	_XkbErrIllegalContents		12
#define	_XkbErrEmptyFile		13
#define	_XkbErrFileNotFound		14
#define	_XkbErrFileCannotOpen		15
#define	_XkbErrBadValue			16
#define	_XkbErrBadMatch			17
#define	_XkbErrBadTypeName		18
#define	_XkbErrBadTypeWidth		19
#define	_XkbErrBadFileType		20
#define	_XkbErrBadFileVersion		21
#define	_XkbErrBadFileFormat		22
#define	_XkbErrBadAlloc			23
#define	_XkbErrBadLength		24
#define	_XkbErrXReqFailure		25
#define	_XkbErrBadImplementation	26

/***====================================================================***/

_XFUNCPROTOBEGIN

extern _X_EXPORT char *XkbIndentText2(unsigned   /* size */
);

extern _X_EXPORT char *XkbAtomText2(Atom /* atm */ ,
                                   unsigned     /* format */
);

extern _X_EXPORT char *XkbKeysymText2(KeySym /* sym */ ,
                                     unsigned   /* format */
);

extern _X_EXPORT char *XkbStringText2(char * /* str */ ,
                                     unsigned   /* format */
);

extern _X_EXPORT char *XkbKeyNameText2(char * /* name */ ,
                                      unsigned  /* format */
);

extern _X_EXPORT char *XkbModIndexText2(unsigned /* ndx */ ,
                                       unsigned /* format */
);

extern _X_EXPORT char *XkbModMaskText2(unsigned /* mask */ ,
                                      unsigned  /* format */
);

extern _X_EXPORT char *XkbVModIndexText2(XkbDescPtr /* xkb */ ,
                                        unsigned /* ndx */ ,
                                        unsigned        /* format */
);

extern _X_EXPORT char *XkbVModMaskText2(XkbDescPtr /* xkb */ ,
                                       unsigned /* modMask */ ,
                                       unsigned /* mask */ ,
                                       unsigned /* format */
);

extern _X_EXPORT char *XkbConfigText2(unsigned /* config */ ,
                                     unsigned   /* format */
);

extern _X_EXPORT const char *XkbSIMatchText2(unsigned /* type */ ,
                                            unsigned    /* format */
);

extern _X_EXPORT char *XkbIMWhichStateMaskText2(unsigned /* use_which */ ,
                                               unsigned /* format */
);

extern _X_EXPORT char *XkbControlsMaskText2(unsigned /* ctrls */ ,
                                           unsigned     /* format */
);

extern _X_EXPORT char *XkbGeomFPText2(int /* val */ ,
                                     unsigned   /* format */
);

extern _X_EXPORT char *XkbDoodadTypeText2(unsigned /* type */ ,
                                         unsigned       /* format */
);

extern _X_EXPORT const char *XkbActionTypeText2(unsigned /* type */ ,
                                               unsigned /* format */
);

extern _X_EXPORT char *XkbActionText2(XkbDescPtr /* xkb */ ,
                                     XkbAction * /* action */ ,
                                     unsigned   /* format */
);

extern _X_EXPORT char *XkbBehaviorText2(XkbDescPtr /* xkb */ ,
                                       XkbBehavior * /* behavior */ ,
                                       unsigned /* format */
);

/***====================================================================***/

#define	_XkbKSLower	(1<<0)
#define	_XkbKSUpper	(1<<1)

#define	XkbKSIsLower(k)		(_XkbKSCheckCase(k)&_XkbKSLower)
#define	XkbKSIsUpper(k)		(_XkbKSCheckCase(k)&_XkbKSUpper)
#define XkbKSIsKeypad(k)	(((k)>=XK_KP_Space)&&((k)<=XK_KP_Equal))
#define	XkbKSIsDeadKey(k)	\
		(((k)>=XK_dead_grave)&&((k)<=XK_dead_semivoiced_sound))

extern _X_EXPORT unsigned _XkbKSCheckCase(KeySym        /* sym */
);

extern _X_EXPORT int XkbFindKeycodeByName(XkbDescPtr /* xkb */ ,
                                          char * /* name */ ,
                                          Bool  /* use_aliases */
);

/***====================================================================***/

extern _X_EXPORT Atom XkbInternAtom2(char * /* name */ ,
                                    Bool        /* onlyIfExists */
);

/***====================================================================***/

#ifdef _XKBGEOM_H_

#define	XkbDW_Unknown	0
#define	XkbDW_Doodad	1
#define	XkbDW_Section	2

typedef struct _XkbDrawable {
    int type;
    int priority;
    union {
        XkbDoodadPtr doodad;
        XkbSectionPtr section;
    } u;
    struct _XkbDrawable *next;
} XkbDrawableRec, *XkbDrawablePtr;

#endif

/***====================================================================***/

extern _X_EXPORT unsigned XkbConvertGetByNameComponents(Bool /* toXkm */ ,
                                                        unsigned        /* orig */
);

extern _X_EXPORT Bool XkbNameMatchesPattern(char * /* name */ ,
                                            char *      /* pattern */
);

/***====================================================================***/

extern _X_EXPORT Bool XkbWriteXKBKeycodes2(FILE * /* file */ ,
XkbDescPtr /* result */ ,
Bool /* topLevel */ ,
Bool /* showImplicit */ ,
XkbFileAddOnFunc2 /* addOn */ ,
void *        /* priv */
);

extern _X_EXPORT Bool XkbWriteXKBKeyTypes2(FILE * /* file */ ,
XkbDescPtr /* result */ ,
Bool /* topLevel */ ,
Bool /* showImplicit */ ,
XkbFileAddOnFunc2 /* addOn */ ,
void *        /* priv */
);

extern _X_EXPORT Bool XkbWriteXKBCompatMap2(FILE * /* file */ ,
XkbDescPtr /* result */ ,
Bool /* topLevel */ ,
Bool /* showImplicit */ ,
XkbFileAddOnFunc2 /* addOn */ ,
void *       /* priv */
);

extern _X_EXPORT Bool XkbWriteXKBSymbols2(FILE * /* file */ ,
XkbDescPtr /* result */ ,
Bool /* topLevel */ ,
Bool /* showImplicit */ ,
XkbFileAddOnFunc2 /* addOn */ ,
void * /* priv */
);

extern _X_EXPORT Bool XkbWriteXKBGeometry2(FILE * /* file */ ,
XkbDescPtr /* result */ ,
Bool /* topLevel */ ,
Bool /* showImplicit */ ,
XkbFileAddOnFunc2 /* addOn */ ,
void *        /* priv */
);

extern _X_EXPORT Bool XkbWriteXKBKeymapForNames(FILE * /* file */ ,
XkbComponentNamesPtr /* names */
,
XkbDescPtr /* xkb */ ,
unsigned /* want */ ,
unsigned        /* need */
);

/***====================================================================***/

extern _X_EXPORT Bool XkmProbe2(FILE *   /* file */
);

extern _X_EXPORT unsigned XkmReadFile2(FILE * /* file */ ,
                                      unsigned /* need */ ,
                                      unsigned /* want */ ,
                                      XkbDescPtr *      /* result */
);

_XFUNCPROTOEND
#endif                          /* _XKBFILE_H_2 */

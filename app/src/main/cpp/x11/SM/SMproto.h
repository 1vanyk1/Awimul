#ifndef _SMPROTO_H_
#define _SMPROTO_H_

#include "../headers/xmd.h"

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* n	ARRAY8		previousId */
} smRegisterClientMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* n	ARRAY8		clientId */
} smRegisterClientReplyMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused1[2];
    CARD32	length;
    CARD8	saveType;
    CARD8	shutdown;
    CARD8	interactStyle;
    CARD8	fast;
    CARD8	unused2[4];
} smSaveYourselfMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused1[2];
    CARD32	length;
    CARD8	saveType;
    CARD8	shutdown;
    CARD8	interactStyle;
    CARD8	fast;
    CARD8	global;
    CARD8	unused2[3];
} smSaveYourselfRequestMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	dialogType;
    CARD8	unused;
    CARD32	length;
} smInteractRequestMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smInteractMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	cancelShutdown;
    CARD8	unused;
    CARD32	length;
} smInteractDoneMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8       success;
    CARD8	unused;
    CARD32	length;
} smSaveYourselfDoneMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smDieMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smShutdownCancelledMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* b	LISTofARRAY8	reasons */
} smCloseConnectionMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* a	LISTofPROPERTY	properties */
} smSetPropertiesMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* a	LISTofARRAY8	property names */
} smDeletePropertiesMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smGetPropertiesMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
    /* a	LISTofPROPERTY	properties */
} smPropertiesReplyMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smSaveYourselfPhase2RequestMsg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smSaveYourselfPhase2Msg;

typedef struct {
    CARD8	majorOpcode;
    CARD8	minorOpcode;
    CARD8	unused[2];
    CARD32	length;
} smSaveCompleteMsg;


/*
 * SIZEOF values.  These better be multiples of 8.
 */

#define sz_smRegisterClientMsg 			8
#define sz_smRegisterClientReplyMsg 		8
#define sz_smSaveYourselfMsg 			16
#define sz_smSaveYourselfRequestMsg		16
#define sz_smInteractRequestMsg 		8
#define sz_smInteractMsg 			8
#define sz_smInteractDoneMsg 			8
#define sz_smSaveYourselfDoneMsg 		8
#define sz_smDieMsg 				8
#define sz_smShutdownCancelledMsg 		8
#define sz_smCloseConnectionMsg 		8
#define sz_smSetPropertiesMsg 			8
#define sz_smDeletePropertiesMsg 		8
#define sz_smGetPropertiesMsg 			8
#define sz_smPropertiesReplyMsg 		8
#define sz_smSaveYourselfPhase2RequestMsg	8
#define sz_smSaveYourselfPhase2Msg 		8
#define sz_smSaveCompleteMsg 			8

#endif /* _SMPROTO_H_ */

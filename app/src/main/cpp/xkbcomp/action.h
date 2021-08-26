#ifndef ACTION_H
#define ACTION_H 1

#define	F_ClearLocks	0
#define	F_LatchToLock	1
#define	F_GenKeyEvent	2
#define	F_Report	3
#define	F_Default	4
#define	F_Affect	5
#define	F_Increment	6
#define	F_Modifiers	7
#define	F_Group		8
#define	F_X		9
#define	F_Y		10
#define	F_Accel		11
#define	F_Button	12
#define	F_Value		13
#define	F_Controls	14
#define	F_Type		15
#define	F_Count		16
#define	F_Screen	17
#define	F_Same		18
#define	F_Data		19
#define	F_Device	20
#define	F_Keycode	21
#define	F_ModsToClear	22
#define	F_LastField	F_ModsToClear
#define	F_NumFields	(F_LastField+1)

#define	PrivateAction	(XkbSA_LastAction+1)

typedef struct _ActionInfo
{
    unsigned action;
    unsigned field;
    ExprDef *array_ndx;
    ExprDef *value;
    struct _ActionInfo *next;
} ActionInfo;

extern int HandleActionDef(ExprDef * /* def */ ,
                           XkbDescPtr /* xkb */ ,
                           XkbAnyAction * /* action */ ,
                           unsigned /* mergeMode */ ,
                           ActionInfo * /* info */
);

extern int SetActionField(XkbDescPtr /* xkb */ ,
                          const char * /* elem */ ,
                          const char * /* field */ ,
                          ExprDef * /* index */ ,
                          ExprDef * /* value */ ,
                          ActionInfo ** /* info_rtrn */
);

extern void ActionsInit(void);

extern LookupEntry ctrlNames[];

#endif /* ACTION_H */

#ifndef XKM_H
#define	XKM_H 1

#define	XkmFileVersion		15

#define	XkmIllegalFile		-1
#define	XkmSemanticsFile	20
#define	XkmLayoutFile		21
#define	XkmKeymapFile		22
#define	XkmGeometryFile		23

#define	XkmTypesIndex		0
#define	XkmCompatMapIndex	1
#define	XkmSymbolsIndex		2
#define	XkmIndicatorsIndex	3
#define	XkmKeyNamesIndex	4
#define	XkmGeometryIndex	5
#define	XkmVirtualModsIndex	6
#define	XkmLastIndex		XkmVirtualModsIndex

#define	XkmTypesMask		(1<<0)
#define	XkmCompatMapMask	(1<<1)
#define	XkmSymbolsMask		(1<<2)
#define	XkmIndicatorsMask	(1<<3)
#define	XkmKeyNamesMask		(1<<4)
#define	XkmGeometryMask		(1<<5)
#define	XkmVirtualModsMask	(1<<6)
#define	XkmLegalIndexMask	(0x7f)
#define	XkmAllIndicesMask	(0x7f)

#define	XkmSemanticsRequired	(XkmCompatMapMask)
#define	XkmSemanticsOptional	(XkmTypesMask|XkmVirtualModsMask|XkmIndicatorsMask)
#define	XkmSemanticsLegal	(XkmSemanticsRequired|XkmSemanticsOptional)
#define	XkmLayoutRequired	(XkmKeyNamesMask|XkmSymbolsMask|XkmTypesMask)
#define	XkmLayoutOptional	(XkmVirtualModsMask|XkmGeometryMask)
#define	XkmLayoutLegal		(XkmLayoutRequired|XkmLayoutOptional)
#define	XkmKeymapRequired	(XkmSemanticsRequired|XkmLayoutRequired)
#define	XkmKeymapOptional	((XkmSemanticsOptional|XkmLayoutOptional)&(~XkmKeymapRequired))
#define	XkmKeymapLegal		(XkmKeymapRequired|XkmKeymapOptional)

#define	XkmLegalSection(m)	(((m)&(~XkmKeymapLegal))==0)
#define	XkmSingleSection(m)	(XkmLegalSection(m)&&(((m)&(~(m)+1))==(m)))

#endif /* XKM_H */

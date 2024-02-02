#ifndef _XawMultiSrc_h
#define _XawMultiSrc_h

#include "TextSrc.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 dataCompression     DataCompression	Boolean		True
 length		     Length		int		(internal)
 pieceSize	     PieceSize		int		BUFSIZ
 string		     String		String		NULL
 type		     Type		XawAsciiType	XawAsciiString
 useStringInPlace    UseStringInPlace	Boolean		False

*/

extern WidgetClass multiSrcObjectClass;

typedef struct _MultiSrcClassRec *MultiSrcObjectClass;
typedef struct _MultiSrcRec      *MultiSrcObject;

#define MultiSourceObjectClass MultiSrcObjectClass
#define MultiSourceObject      MultiSrcObject

#define XtCDataCompression "DataCompression"
#define XtCPieceSize "PieceSize"
#define XtCType "Type"
#define XtCUseStringInPlace "UseStringInPlace"

#define XtNdataCompression "dataCompression"
#define XtNpieceSize "pieceSize"
#define XtNtype "type"
#define XtNuseStringInPlace "useStringInPlace"

#define XtRMultiType "MultiType"

#define XtEstring "string"
#define XtEfile "file"

/************************************************************
 *
 * THESE ROUTINES ARE NOT PUBLIC: Source should call
 *
 * the AsciiSrc API which currently forwards requests here.
 *
 * future versions (like theres going to be an R7 Xaw!) may
 *
 * eliminate this file or at least these functions entirely.
 *
 ************************************************************/

_XFUNCPROTOBEGIN

void XawMultiSourceFreeString
        (
                Widget			w
        );

Bool _XawMultiSave
        (
                Widget			w
        );

Bool _XawMultiSaveAsFile
        (
                Widget			w,
                _Xconst char		*name
        );

_XFUNCPROTOEND

#endif /* _XawMultiSrc_h  */

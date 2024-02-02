#ifndef _XawAsciiSrc_h
#define _XawAsciiSrc_h

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

extern WidgetClass asciiSrcObjectClass;

typedef struct _AsciiSrcClassRec *AsciiSrcObjectClass;
typedef struct _AsciiSrcRec      *AsciiSrcObject;

#define AsciiSourceObjectClass AsciiSrcObjectClass
#define AsciiSourceObject      AsciiSrcObject

/*
 * Resource Definitions
 */
#define XtCDataCompression "DataCompression"
#define XtCPieceSize "PieceSize"
#define XtCType "Type"
#define XtCUseStringInPlace "UseStringInPlace"

#define XtNdataCompression "dataCompression"
#define XtNpieceSize "pieceSize"
#define XtNtype "type"
#define XtNuseStringInPlace "useStringInPlace"

#define XtRAsciiType "AsciiType"

#define XtEstring "string"
#define XtEfile "file"

typedef enum {
    XawAsciiFile,
    XawAsciiString
} XawAsciiType;

/*
 * Public routines
 */

_XFUNCPROTOBEGIN

/*
 * Function:
 *	XawAsciiSourceFreeString
 *
 * Parameters:
 *	w - AsciiSrc object
 *
 * Description:
 *	  Frees the string returned by a get values call
 *		     on the string when the source is of type string.
 */
void XawAsciiSourceFreeString
        (
                Widget		w
        );

/*
 * Function:
 *	XawAsciiSave
 *
 * Arguments:
 *	w - asciiSrc Object.
 *
 * Description:
 *	Saves all the pieces into a file or string as required.
 *
 * Returns:
 *	True if the save was successful
 */
Bool XawAsciiSave
        (
                Widget		w
        );

/*
 * Function:
 *	XawAsciiSaveAsFile
 *
 * Parameters:
 *	w    - asciiSrc object
 *	name - name of the file to save this file into
 *
 * Description:
 *	Save the current buffer as a file.
 *
 * Returns:
 *	True if the save was successful
 */
Bool XawAsciiSaveAsFile
        (
                Widget		w,
                _Xconst char	*name
        );

/*
 * Function:
 *	XawAsciiSourceChanged
 *
 * Parameters:
 *	w - asciiSource object
 *
 * Description:
 *	Returns true if the source has changed since last saved.
 *
 * Returns:
 *	a Boolean (see description)
 */
Bool XawAsciiSourceChanged
        (
                Widget		w
        );

_XFUNCPROTOEND

#endif /* _XawAsciiSrc_h */

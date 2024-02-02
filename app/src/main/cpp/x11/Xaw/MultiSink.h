#ifndef _XawMultiSink_h
#define _XawMultiSink_h

/*
 * MultiSink Object
 */

#include "TextSink.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 echo                Output             Boolean         True
 displayNonprinting  Output             Boolean         True
 fontSet             FontSet            XFontSet        XtDefaultFontSet

*/

#define XtCOutput "Output"

#define XtNdisplayNonprinting "displayNonprinting"
#define XtNecho "echo"

#ifndef XtNfontSet		/*Sheeran, Omron KK, 93/03/04*/
#define XtNfontSet		"fontSet"
#endif

#ifndef XtCFontSet		/*Sheeran, Omron KK, 93/03/04*/
#define XtCFontSet		"FontSet"
#endif

/* Class record constants */
extern WidgetClass multiSinkObjectClass;

typedef struct _MultiSinkClassRec *MultiSinkObjectClass;
typedef struct _MultiSinkRec      *MultiSinkObject;

#endif /* _XawMultiSink_h */

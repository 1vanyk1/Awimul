#ifndef _XawAsciiSink_h
#define _XawAsciiSink_h

/*
 * AsciiSink Object
 */

#include "TextSink.h"

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 echo                Output             Boolean         True
 displayNonprinting  Output             Boolean         True

*/

#define XtCOutput "Output"

#define XtNdisplayNonprinting "displayNonprinting"
#define XtNecho "echo"

/* Class record constants */
extern WidgetClass asciiSinkObjectClass;

typedef struct _AsciiSinkClassRec *AsciiSinkObjectClass;
typedef struct _AsciiSinkRec      *AsciiSinkObject;

#endif /* _XawAsciiSink_h */

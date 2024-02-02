#include "IntrinsicI.h"

_XFUNCPROTOBEGIN

/* Representation types */

extern	XrmQuark  _XtQString;

/*
 * Resource conversions
 */

typedef struct _ConverterRec **ConverterTable;

extern void _XtAddDefaultConverters(
        ConverterTable	/* table */
);

extern void _XtSetDefaultConverterTable(
        ConverterTable* 		/* table */
);

extern void _XtFreeConverterTable(
        ConverterTable 		/* table */
);

extern void _XtTableAddConverter(
        ConverterTable		/* table */,
        XrmRepresentation    	/* from_type */,
        XrmRepresentation    	/* to_type */,
        XtTypeConverter      	/* converter */,
        XtConvertArgRec const *     /* convert_args */,
        Cardinal             	/* num_args */,
        _XtBoolean              	/* new_style */,
        XtCacheType	    		/* cache_type */,
        XtDestructor         	/* destructor */,
        _XtBoolean			/* global */
);

extern Boolean _XtConvert(
        Widget			/* widget */,
        XrmRepresentation    	/* from_type */,
        XrmValuePtr			/* from */,
        XrmRepresentation		/* to_type */,
        XrmValuePtr			/* to */,
        XtCacheRef*			/* cache_ref_return */
);

void _XtConvertInitialize(void);

#ifdef DEBUG
void _XtConverterCacheStats(void);
#endif

_XFUNCPROTOEND
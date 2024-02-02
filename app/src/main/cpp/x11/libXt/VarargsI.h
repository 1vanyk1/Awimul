#ifndef _VarargsI_h_
#define _VarargsI_h_

#include <stdarg.h>

/* private routines */

_XFUNCPROTOBEGIN

extern void _XtCountVaList(
        va_list /*var*/, int* /*total_count*/, int* /*typed_count*/
);

extern void _XtVaToArgList(
        Widget /*widget*/, va_list /*var*/, int /*max_count*/, ArgList* /*args_return*/, Cardinal* /*num_args_return*/
);

extern void _XtVaToTypedArgList(
        va_list /*var*/, int /*count*/, XtTypedArgList* /*args_return*/, Cardinal* /*num_args_return*/
);

extern XtTypedArgList _XtVaCreateTypedArgList(
        va_list /*var*/, int /*count*/
);

extern void _XtFreeArgList(
        ArgList /*args*/, int /*total_count*/, int /*typed_count*/
);

extern void _XtGetApplicationResources(
        Widget /*w*/, XtPointer /*base*/, XtResourceList /*resources*/, Cardinal /*num_resources*/, ArgList /*args*/, Cardinal /*num_args*/, XtTypedArgList /*typed_args*/, Cardinal /*num_typed_args*/
);

extern void _XtGetSubresources(
        Widget /*w*/, XtPointer /*base*/, const char* /*name*/, const char* /*class*/, XtResourceList /*resources*/, Cardinal /*num_resources*/, ArgList /*args*/, Cardinal /*num_args*/, XtTypedArgList /*typed_args*/, Cardinal /*num_typed_args*/
);

_XFUNCPROTOEND

#endif /* _VarargsI_h_ */
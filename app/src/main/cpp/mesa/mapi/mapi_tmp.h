#ifndef MAPI_ABI_HEADER
//#error "MAPI_ABI_HEADER must be defined"
#endif

/* does not need hidden entries in bridge mode */
#ifdef MAPI_MODE_BRIDGE

#ifdef MAPI_TMP_PUBLIC_ENTRIES
#undef MAPI_TMP_PUBLIC_ENTRIES
#define MAPI_TMP_PUBLIC_ENTRIES_NO_HIDDEN
#endif

#ifdef MAPI_TMP_STUB_ASM_GCC
#undef MAPI_TMP_STUB_ASM_GCC
#define MAPI_TMP_STUB_ASM_GCC_NO_HIDDEN
#endif

#endif /* MAPI_MODE_BRIDGE */

#include "glapi_mapi_tmp.h"
#include "shared_glapi_mapi_tmp.h"

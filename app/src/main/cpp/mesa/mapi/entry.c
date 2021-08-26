#include <stdlib.h>
#include <stdint.h>

#include "entry.h"
#include "u_current.h"
#include "../util/u_endian.h"

#define _U_STRINGIFY(x) #x
#define U_STRINGIFY(x) _U_STRINGIFY(x)

/* define macros for use by assembly dispatchers */
#define ENTRY_CURRENT_TABLE U_STRINGIFY(u_current_table)

/* in bridge mode, mapi is a user of glapi */
#ifdef MAPI_MODE_BRIDGE
#define ENTRY_CURRENT_TABLE_GET "_glapi_get_dispatch"
#else
#define ENTRY_CURRENT_TABLE_GET U_STRINGIFY(u_current_get_table_internal)
#endif

#if defined(USE_X86_ASM) && defined(__GNUC__)
#   ifdef USE_ELF_TLS
#      include "entry_x86_tls.h"
#   else
#      include "entry_x86_tsd.h"
#   endif
#elif defined(USE_X86_64_ASM) && defined(__GNUC__) && defined(USE_ELF_TLS)
#   include "entry_x86-64_tls.h"
#elif defined(USE_PPC64LE_ASM) && defined(__GNUC__) && UTIL_ARCH_LITTLE_ENDIAN
#   ifdef USE_ELF_TLS
#      include "entry_ppc64le_tls.h"
#   else
#      include "entry_ppc64le_tsd.h"
#   endif
#else

static inline const struct _glapi_table *
entry_current_get(void)
{
#ifdef MAPI_MODE_BRIDGE
    return GET_DISPATCH();
#else
    return u_current_get_table_internal();
#endif
}

/* C version of the public entries */
#define MAPI_TMP_DEFINES
#define MAPI_TMP_PUBLIC_DECLARES
#define MAPI_TMP_PUBLIC_ENTRIES
#include "mapi_tmp.h"

#ifndef MAPI_MODE_BRIDGE

void
entry_patch_public(void)
{
}

mapi_func
entry_get_public(int slot)
{
    /* pubic_entries are defined by MAPI_TMP_PUBLIC_ENTRIES */
    return public_entries[slot];
}

mapi_func
entry_generate(int slot)
{
    return NULL;
}

void
entry_patch(mapi_func entry, int slot)
{
}

#endif /* MAPI_MODE_BRIDGE */

#endif /* asm */

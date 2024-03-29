#include <string.h>
#include <stdlib.h>
#include "glapi/glapi.h"
#include "u_current.h"
#include "table.h" /* for MAPI_TABLE_NUM_SLOTS */
#include "stub.h"

/*
 * Global variables, _glapi_get_context, and _glapi_get_dispatch are defined in
 * u_current.c.
 */

//#ifdef USE_ELF_TLS
/* not used, but defined for compatibility */
struct _glapi_table *_glapi_Dispatch;
void *_glapi_Context;
//#endif /* USE_ELF_TLS */

void
_glapi_destroy_multithread(void)
{
    u_current_destroy();
}

void
_glapi_check_multithread(void)
{
    u_current_init();
}

void
_glapi_set_context(void *context)
{
    u_current_set_context((const void *) context);
}

void
_glapi_set_dispatch(struct _glapi_table *dispatch)
{
    u_current_set_table((const struct _glapi_table *) dispatch);
}

///**
// * Return size of dispatch table struct as number of functions (or
// * slots).
// */
//unsigned int
//_glapi_get_dispatch_table_size(void)
//{
//    return MAPI_TABLE_NUM_SLOTS;
//}

/**
 * Fill-in the dispatch stub for the named function.
 *
 * This function is intended to be called by a hardware driver.  When called,
 * a dispatch stub may be created created for the function.  A pointer to this
 * dispatch function will be returned by glXGetProcAddress.
 *
 * \param function_names       Array of pointers to function names that should
 *                             share a common dispatch offset.
 * \param parameter_signature  String representing the types of the parameters
 *                             passed to the named function.  Parameter types
 *                             are converted to characters using the following
 *                             rules:
 *                               - 'i' for \c GLint, \c GLuint, and \c GLenum
 *                               - 'p' for any pointer type
 *                               - 'f' for \c GLfloat and \c GLclampf
 *                               - 'd' for \c GLdouble and \c GLclampd
 *
 * \returns
 * The offset in the dispatch table of the named function.  A pointer to the
 * driver's implementation of the named function should be stored at
 * \c dispatch_table[\c offset].  Return -1 if error/problem.
 *
 * \sa glXGetProcAddress
 *
 * \warning
 * This function can only handle up to 8 names at a time.  As far as I know,
 * the maximum number of names ever associated with an existing GL function is
 * 4 (\c glPointParameterfSGIS, \c glPointParameterfEXT,
 * \c glPointParameterfARB, and \c glPointParameterf), so this should not be
 * too painful of a limitation.
 *
 * \todo
 * Check parameter_signature.
 */
int
_glapi_add_dispatch( const char * const * function_names,
                     const char * parameter_signature )
{
    const struct mapi_stub *function_stubs[8];
    const struct mapi_stub *alias = NULL;
    unsigned i;

    (void) memset(function_stubs, 0, sizeof(function_stubs));

    /* find the missing stubs, and decide the alias */
    for (i = 0; function_names[i] != NULL && i < 8; i++) {
        const char * funcName = function_names[i];
        const struct mapi_stub *stub;
        int slot;

        if (!funcName || funcName[0] != 'g' || funcName[1] != 'l')
            return -1;
        funcName += 2;

        stub = stub_find_public(funcName);
        if (!stub)
            stub = stub_find_dynamic(funcName, 0);

        slot = (stub) ? stub_get_slot(stub) : -1;
        if (slot >= 0) {
            if (alias && stub_get_slot(alias) != slot)
                return -1;
            /* use the first existing stub as the alias */
            if (!alias)
                alias = stub;

            function_stubs[i] = stub;
        }
    }

    /* generate missing stubs */
    for (i = 0; function_names[i] != NULL && i < 8; i++) {
        const char * funcName = function_names[i] + 2;
        struct mapi_stub *stub;

        if (function_stubs[i])
            continue;

        stub = stub_find_dynamic(funcName, 1);
        if (!stub)
            return -1;

        stub_fix_dynamic(stub, alias);
        if (!alias)
            alias = stub;
    }

    return (alias) ? stub_get_slot(alias) : -1;
}

#if defined(ANDROID) && ANDROID_API_LEVEL <= 30
static int is_debug_marker_func(const char *name)
{
   return (!strcmp(name, "InsertEventMarkerEXT") ||
           !strcmp(name, "PushGroupMarkerEXT") ||
           !strcmp(name, "PopGroupMarkerEXT"));
}
#endif

static const struct mapi_stub *
_glapi_get_stub(const char *name, int generate)
{
    const struct mapi_stub *stub;

    if (!name || name[0] != 'g' || name[1] != 'l')
        return NULL;
    name += 2;

    stub = stub_find_public(name);
#if defined(ANDROID) && ANDROID_API_LEVEL <= 30
    /* Android framework till API Level 30 uses function pointers from
    * eglGetProcAddress without checking GL_EXT_debug_marker.
    * Make sure we don't return stub function pointers if we don't
    * support GL_EXT_debug_marker */
   if (!stub && !is_debug_marker_func(name))
#else
    if (!stub)
#endif
        stub = stub_find_dynamic(name, generate);

    return stub;
}

///**
// * Return offset of entrypoint for named function within dispatch table.
// */
//int
//_glapi_get_proc_offset(const char *funcName)
//{
//    const struct mapi_stub *stub = _glapi_get_stub(funcName, 0);
//    return (stub) ? stub_get_slot(stub) : -1;
//}

///**
// * Return pointer to the named function.  If the function name isn't found
// * in the name of static functions, try generating a new API entrypoint on
// * the fly with assembly language.
// */
//_glapi_proc
//_glapi_get_proc_address(const char *funcName)
//{
//    const struct mapi_stub *stub = _glapi_get_stub(funcName, 1);
//    return (stub) ? (_glapi_proc) stub_get_addr(stub) : NULL;
//}

///**
// * Return the name of the function at the given dispatch offset.
// * This is only intended for debugging.
// */
//const char *
//_glapi_get_proc_name(unsigned int offset)
//{
//    const struct mapi_stub *stub = stub_find_by_slot(offset);
//    return stub ? stub_get_name(stub) : NULL;
//}

///** Return pointer to new dispatch table filled with no-op functions */
//struct _glapi_table *
//_glapi_new_nop_table(unsigned num_entries)
//{
//    struct _glapi_table *table;
//
//    if (num_entries > MAPI_TABLE_NUM_SLOTS)
//        num_entries = MAPI_TABLE_NUM_SLOTS;
//
//    table = malloc(num_entries * sizeof(mapi_func));
//    if (table) {
//        memcpy(table, table_noop_array, num_entries * sizeof(mapi_func));
//    }
//    return table;
//}

void
_glapi_set_noop_handler(_glapi_nop_handler_proc func)
{
    table_set_noop_handler(func);
}

/**
 * This is a deprecated function which should not be used anymore.
 * It's only present to satisfy linking with older versions of libGL.
 */
unsigned long
_glthread_GetID(void)
{
    return 0;
}

//void
//_glapi_noop_enable_warnings(unsigned char enable)
//{
//}

//void
//_glapi_set_warning_func(_glapi_proc func)
//{
//}

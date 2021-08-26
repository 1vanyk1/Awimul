#include <stdlib.h>
#include <string.h>
#include "glapi_priv.h"


void
_glapi_noop_enable_warnings(unsigned char enable)
{
}

void
_glapi_set_warning_func(_glapi_proc func)
{
}


/**
 * We'll jump though this function pointer whenever a no-op function
 * is called.
 */
static _glapi_nop_handler_proc nop_handler = NULL;


/**
 * Register the no-op handler call-back function.
 */
void
_glapi_set_nop_handler(_glapi_nop_handler_proc func)
{
    nop_handler = func;
}


/**
 * Called by each of the no-op GL entrypoints.
 */
static void
nop(const char *func)
{
    if (nop_handler)
        nop_handler(func);
}


/**
 * This is called if the user somehow calls an unassigned GL dispatch function.
 */
static GLint
NoOpUnused(void)
{
    nop("unused GL entry point");
    return 0;
}

/*
 * Defines for the glapitemp.h functions.
 */
#define KEYWORD1 static
#define KEYWORD1_ALT static
#define KEYWORD2 GLAPIENTRY
#define NAME(func)  NoOp##func
#define DISPATCH(func, args, msg)  nop(#func);
#define RETURN_DISPATCH(func, args, msg)  nop(#func); return 0


/*
 * Defines for the table of no-op entry points.
 */
#define TABLE_ENTRY(name) (_glapi_proc) NoOp##name
#define DISPATCH_TABLE_NAME __glapi_noop_table
#define UNUSED_TABLE_NAME __unused_noop_functions

#include "glapitemp.h"


/** Return pointer to new dispatch table filled with no-op functions */
struct _glapi_table *
_glapi_new_nop_table(unsigned num_entries)
{
    struct _glapi_table *table = malloc(num_entries * sizeof(_glapi_proc));
    if (table) {
        memcpy(table, __glapi_noop_table,
               num_entries * sizeof(_glapi_proc));
    }
    return table;
}

#ifndef _TABLE_H_
#define _TABLE_H_

#include "../include/c99_compat.h"
#include "entry.h"

#define MAPI_TMP_TABLE
#include "mapi_tmp.h"

#define MAPI_TABLE_NUM_SLOTS (MAPI_TABLE_NUM_STATIC + MAPI_TABLE_NUM_DYNAMIC)
#define MAPI_TABLE_SIZE (MAPI_TABLE_NUM_SLOTS * sizeof(mapi_func))

struct _glapi_table;

extern const mapi_func table_noop_array[];


typedef void (*nop_handler_proc)(const char *name);


void
table_set_noop_handler(nop_handler_proc func);


/**
 * Get the no-op dispatch table.
 */
static inline const struct _glapi_table *
table_get_noop(void)
{
    return (const struct _glapi_table *) table_noop_array;
}

/**
 * Set the function of a slot.
 */
static inline void
table_set_func(struct _glapi_table *tbl, int slot, mapi_func func)
{
    mapi_func *funcs = (mapi_func *) tbl;
    funcs[slot] = func;
}

/**
 * Return the function of a slot.
 */
static inline mapi_func
table_get_func(const struct _glapi_table *tbl, int slot)
{
    const mapi_func *funcs = (const mapi_func *) tbl;
    return funcs[slot];
}

#endif /* _TABLE_H_ */
